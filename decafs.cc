/*
decafs.cc
Jason Wilkin
Fall 2013
*/

#include <cassert>
#include <vector>
#include <algorithm>
#include "semantics.h"
#include "parsetree.h"
#include <fstream>
#include <sstream>

S_interface *currentInterface;
S_class *currentClass;
S_function *currentFunction;
int sequenceNum;
extern Symtab * currentScope;
extern int scopesOpened;
extern int scopesClosed;
extern int scopeDepth;
ofstream * currentFile;
ofstream * topClassFile;
Symtab * toplevel = new Symtab(NULL);


// Creates primitive types in the global symbol table.
void makeBuiltins(Symtab * table){
  S_builtin * myInt = new S_builtin;
  myInt->name = "Int";

  S_builtin * myBool = new S_builtin;
  myBool->name = "Bool";

  S_builtin * myDouble = new S_builtin;
  myDouble->name = "Double";

  S_builtin * myString = new S_builtin;
  myString->name = "String";

  table->insert("Int", myInt);
  table->insert("Bool", myBool);
  table->insert("Double", myDouble);
  table->insert("String", myString);

}

// Returns a field with name 'ident' in class 'classs'
semantics * virtualLookup(string ident, S_class * classs){
  if (!classs)
    return NULL;
  for (size_t i=0; i<classs->fields.size(); i++){
    if (classs->fields[i]->name == ident)
      return classs->fields[i];
  }
  return virtualLookup(ident, classs->parentClass);
}

// Class field lookup for variables (to avoid semantic* casting problems with virtualLookup)
S_variable * virtualVariableLookup(string ident, S_class * classs){
  if (!classs)
    return NULL;
  for (size_t i=0; i<classs->fields.size(); i++){
    if (classs->fields[i]->kind() == "S_variable"){
      if (((S_variable *)classs->fields[i])->name == ident){
        return (S_variable *)classs->fields[i];
      }
    }
  }
  return virtualVariableLookup(ident, classs->parentClass);
  
}

// Class field lookup for functions (to avoid semantic* casting problems with virtualLookup)
S_function * virtualFunctionLookup(string ident, semantics * classs){
  if (classs == NULL)
    return NULL;
  if (classs->kind() == "S_class"){
    S_class * myClass = (S_class *)classs;
    if (!myClass)
      return NULL;
    for (size_t i=0; i<myClass->fields.size(); i++){
      if (myClass->fields[i]->kind() == "S_function")
        if (((S_function *)myClass->fields[i])->name == ident)
          return (S_function *)myClass->fields[i];
    }
    return virtualFunctionLookup(ident, myClass->parentClass);
  }
  else if (classs->kind() == "S_interface"){
    S_interface * myInterface = (S_interface *)classs;
    for (size_t i=0; i<myInterface->functions.size(); i++){
      if (myInterface->functions[i]->kind() == "S_function")
        if (((S_function *)myInterface->functions[i])->name == ident)
          return (S_function *)myInterface->functions[i];
    }
  }
  return NULL;

}

// Global symbol table lookup
semantics * global_lookup(string key) { 
  return toplevel->dict[key];
}

// Uses virtual and local lookups and determines which scope to use
semantics * field_lookup(string id, Symtab * scope, S_class * classs){
  semantics * fromClass = virtualLookup(id, classs);
  semantics * fromScope = scope->lookup(id);

  if (!fromClass) return fromScope;
  if (!fromScope) return fromClass;
  if (fromScope == fromClass) return fromScope;

  if (fromScope->kind() == "S_function"){
    S_function * scopeFunc = (S_function *)fromScope;
    if (scopeFunc->isGlobal) return fromClass;

  }
  else if (fromScope->kind() == "S_variable"){
    S_variable * scopeVar = (S_variable *)fromScope;
    if (scopeVar->isGlobal) return fromClass;

  }
  return fromScope;
}

S_class * getParent(S_class * classs){
  return classs->parentClass;
}


// Prints semantic error with optional line number
void semantic_error(string err, int line, bool noLine = false)
{
  cout << "Semantic error: " << err;
  if (noLine)
    cout << endl;
  else
    cout << " on line " << line << endl;
  exit(1);
}


// Convert a tree representing a type to a string.
string get_type_name(ParseTree *typetree)
{
  assert(typetree->description.substr(0,5)=="Type_");
  
  if (typetree->description == "Type_Identifier")
    return typetree->children[0]->token->text;
  else if (typetree->description == "Type_Array")
    return get_type_name(typetree->children[0])+" []";
  else
    return typetree->description.substr(5);
}

// Check for circular class heirarchy
bool isCycleHeirarchy(S_class * classs){
  vector<S_class *> classes;
  S_class * parent = classs->parentClass;
  if (classs->parentClass){
    classes.push_back(classs);
    while (parent != NULL){
      if (std::find(classes.begin(), classes.end(), parent) != classes.end()){
        return true;
      }
      else{
        classes.push_back(parent);
        parent = parent->parentClass;
      }
    }
  }
  return false;
}

// For pass 1.5:
// Collect vector of pairs of class names (subclass, superclass) that extend in pass 1

template <typename A, typename B, typename C>
struct triple {
  A first;
  B second;
  C third;
  triple(A a, B b, C c) : first(a), second(b), third(c) {}

};
typedef triple<string, string, int> Info;
vector<triple<string, string, int> > Subsuprels;

// Strips away ' []' from tree type name
string getPrimitiveTypeName(string typeName){
  std::size_t pos = typeName.find(" ");
  if (pos==std::string::npos)
    return typeName;
  else
    return typeName.substr(0,pos);
}

// tree represents an identifier.  Make sure it's not already defined.
void error_if_defined_locally(ParseTree *tree)
{
  assert(tree->type == TERMINAL and tree->token->type == T_Identifier);
  string var_name = tree->token->text;
  int lineno = tree->token->line;
  if (currentScope->local_lookup(var_name)){
    semantic_error(var_name + " already defined", lineno);
  }
}


// In pass1, Create a S_variable object, and insert in the symbol table.
// This is useful for formal parameters as well as
// variable declarations in side Stmt blocks.
S_variable * makeVariable(ParseTree *tree)
{

  assert(tree->description == "Variable");
  string var_name = tree->children[1]->token->text;
  string type_name = get_type_name(tree->children[0]);
  error_if_defined_locally(tree->children[1]);
  S_variable *entry = new S_variable;
  entry->isInstance = false;
  entry->type = new S_type;
  entry->type->name = type_name;
  if (currentFunction){
    entry->sequence = sequenceNum;
    if (entry->type->name == "Double")
      sequenceNum = sequenceNum + 2;
    else
      sequenceNum++;
  }
  currentScope->insert(var_name, entry);
  return entry;
}


/*

Postconditions of pass1:
   Semantic objects for all declarations created
   Symbol tables for every lexical scope created
   S_class objects contain appropriate S_variable and S_function objects
   S_interface objects contain appropriate S_function objects (for prototypes)

In Pass 1:
  -building initial semantic* objects with names, types, fields (for classes)
  -marking variables as global/local/instance
  -number local variables (for code generation)
  -checking if variables are previously defined

*/

void pass1(ParseTree * tree)
{
  if (!tree) {return;}
  else {cout << tree->description << endl;}
  if (tree->type==TERMINAL) return;

  // must be nonterminal
  else if (tree->description=="StmtBlock") {
    openscope();
    for (size_t i = 0; i < tree->children.size();i++)
      pass1(tree->children[i]);
    tree->symtab = closescope();
  }

  else if (tree->description=="VariableDecl"){
    S_variable * var = makeVariable(tree->children[0]);
    var->name = tree->children[0]->children[1]->token->text;
    if (currentFunction)
      var->isInstance = true;
    if (currentClass){
      currentClass->fields.push_back(var);
    }
    else if (!currentClass and !currentFunction)
      var->isGlobal = true;
  }

  else if (tree->description=="FunctionDecl"){
    error_if_defined_locally(tree->children[1]);
    S_function * func = new S_function;
    currentFunction = func;
    if (!currentClass){
      currentFunction->isGlobal = true;
      sequenceNum = 0;
    }
    else{
      currentFunction->isGlobal = false;
      sequenceNum = 1;
    }
    func->name = tree->children[1]->token->text;
    if (tree->children[0]){
      func->returnType = new S_type;
      func->returnType->name = get_type_name(tree->children[0]);
    }
    else
      func->returnType = NULL;
    
    openscope();
    if (tree->children[2]){
      for (size_t i = 0; i < tree->children[2]->children.size();i++){
        S_variable * formal = makeVariable(tree->children[2]->children[i]);
        formal->isInstance = true;
        func->formals.push_back(formal);
      }
    }
    pass1(tree->children[3]);
    tree->symtab = closescope();
    currentScope->insert(tree->children[1]->token->text, func);
    if (currentInterface)
      currentInterface->functions.push_back(func);
    if (currentClass)
      currentClass->fields.push_back(func);
    currentFunction = NULL;

  }
  
  else if (tree->description=="ClassDecl"){
    error_if_defined_locally(tree->children[0]);
    S_class * newclass = new S_class;
    string className = tree->children[0]->token->text;
    newclass->name = className;
    newclass->parentClass = NULL;
    currentClass = newclass;
    openscope();
    // Fields
    if (tree->children[3])
      for (size_t i=0; i<tree->children[3]->children.size(); i++)
        pass1(tree->children[3]->children[i]);
    if (tree->children[1]){
      string superName = tree->children[1]->token->text;
      int linenum = tree->children[1]->token->line;
      Info rel(className, superName, linenum);
      Subsuprels.push_back(rel);
    }
    pass1(tree->children[2]);
    tree->symtab = closescope();
    currentScope->insert(className, newclass);
    currentClass = NULL;

  }

  else if (tree->description=="InterfaceDecl"){
    error_if_defined_locally(tree->children[0]);
    S_interface * newinterface = new S_interface;
    newinterface->name = tree->children[0]->token->text;
    currentInterface = newinterface;
    openscope();
    if (tree->children[1])
      for (size_t i=0; i<tree->children[1]->children.size(); i++)
        pass1(tree->children[1]->children[i]);
    tree->symtab = closescope();
    currentScope->insert(tree->children[0]->token->text, newinterface);
    currentInterface = NULL;
  }

  else if (tree->description=="Prototype"){
    error_if_defined_locally(tree->children[1]);
    S_function * proto = new S_function;
    currentFunction = proto;
    proto->name = tree->children[1]->token->text;
    if (tree->children[0]){
      proto->returnType = new S_type;
      proto->returnType->name = get_type_name(tree->children[0]);
    }
    openscope();
    if (tree->children[2])
      for (size_t i=0; i<tree->children[2]->children.size(); i++){
        error_if_defined_locally(tree->children[2]->children[i]->children[1]);
        proto->formals.push_back(makeVariable(tree->children[2]->children[i])); 
      }
    tree->symtab = closescope();
    currentScope->insert(tree->children[1]->token->text, proto);
    if (currentInterface)
      currentInterface->functions.push_back(proto);
    if (currentClass)
      currentClass->fields.push_back(proto);
    currentFunction = NULL;
  }

  else{
    for (size_t i = 0; i < tree->children.size();i++)
      pass1(tree->children[i]);
  }
}

// Link classes together (extends), check for circular heirarchies.
void linkClasses(ParseTree * tree){
  for (size_t i=0; i<Subsuprels.size(); i++){
    S_class * subclass = (S_class *)(currentScope->lookup(Subsuprels[i].first));
    semantics * super = currentScope->lookup(Subsuprels[i].second);
    if (super == NULL)
      semantic_error(Subsuprels[i].second + " is never defined", Subsuprels[i].third);
    if (super->kind() != "S_class")
      semantic_error(
        Subsuprels[i].second + " was defined as something other than a class",
        Subsuprels[i].third
      );
    subclass->parentClass = (S_class *)super;
    if (isCycleHeirarchy(subclass))
      semantic_error(subclass->name + " has a circular heirarchy", Subsuprels[i].third);
  }
}

void pass2(ParseTree * tree, Symtab * currScope);

// Pass2 helper functions

// Check variable fields for redefinitions; ensures variable field has valid type.
void checkVarField(
  ParseTree * tree, S_class * parent, bool fieldIsVariable,
  Symtab* & currScope, size_t i
  )
{
  string fieldName;
  if (fieldIsVariable)
    fieldName = ((S_variable *)parent->fields[i])->name;
  else
    fieldName = ((S_function *)parent->fields[i])->name;

  if (fieldName == tree->children[0]->children[1]->token->text)
    semantic_error(
      fieldName + " is previously defined",
      tree->children[0]->children[1]->token->line);
  pass2(tree->children[0], currentScope);

}

// Check function fields for correct redefinitions; ensures function field has valid type.
void checkFuncField(
  ParseTree * tree, S_class * parent, bool fieldIsVariable,
  Symtab* & currScope, size_t i
  )
{
  if (fieldIsVariable){
  S_variable * field = (S_variable *)parent->fields[i];
  if(field->name == tree->children[1]->token->text)
    semantic_error(
      field->name + " is previously defined as a variable",
      tree->children[1]->token->line
    );
  }

  S_function * fieldFunc = (S_function *)parent->fields[i];
  if (fieldFunc->name == tree->children[1]->token->text){
    string funcType = get_type_name(tree->children[0]);
    if(fieldFunc->returnType->name != funcType)
      semantic_error(
        fieldFunc->name + " is not properly redefined (Incorrect return type)",
        tree->children[1]->token->line
      );

    if (tree->children[2] == NULL){
      if (fieldFunc->formals.size() != 0)
        semantic_error(
          fieldFunc->name + " is not properly redefined (Incorrect number of formals)",
          tree->children[1]->token->line
        );
    }

    else{
      if (fieldFunc->formals.size() != tree->children[2]->children.size()){
        semantic_error(
          fieldFunc->name + " is not properly redefined (Incorrect number of formals)",
          tree->children[1]->token->line
        );
      }
        else
          for (size_t k=0; k<fieldFunc->formals.size(); k++){
            string fieldType = get_type_name(tree->children[2]->children[i]->children[0]);
            if (fieldFunc->formals[k]->type->name != fieldType)
              semantic_error(
                fieldFunc->name + " is not properly redefined (Formal types do not match)",
                tree->children[1]->token->line
              );
          }
    }
  }
}


/*

Pass 2 is about checking the type system for basic sanity.
We go though the declarations a second time, ensuring all types have been defined,
all Interfaces are implemented, and so on.

*/
void pass2(ParseTree * tree, Symtab * currScope){
  if (!tree) {return;}
  cout << tree->description << endl;
  if (tree->symtab) {
    currScope = tree->symtab;
  }
  if (tree->description == "Variable"){
    string curTypeName = getPrimitiveTypeName(get_type_name(tree->children[0]));
    semantics *typeLookup = global_lookup(curTypeName);
    if (typeLookup){
      string typeKind = typeLookup->kind();
      if (!(typeKind == "S_class" || typeKind == "S_builtin" || typeKind == "S_interface")){
        semantic_error(curTypeName + " is not a valid type", tree->children[1]->token->line);
      }
    }
    else{
      semantic_error(curTypeName + " has not been defined", tree->children[1]->token->line);
    }
  }

  else if (tree->description == "InterfaceDecl") return;

  else if (tree->description == "ClassDecl"){
    string className = tree->children[0]->token->text;
    semantics * currClass = currScope->lookup(className);
    currentClass = (S_class *)currClass;
    pass2(tree->children[3], currScope);

    if (tree->children[2]){ // Class has interface(s)
      for (size_t i=0; i<tree->children[2]->children.size(); i++){
        ParseTree * myInterface = tree->children[2]->children[i];
        string interfaceName = myInterface->token->text;
        S_interface *interface = (S_interface *)currScope->lookup(interfaceName);
        if (interface){
          if (interface->kind() != "S_interface")
            semantic_error(interfaceName + " is not a valid interface", myInterface->token->line);
        }  
        else
          semantic_error(interfaceName + " has not been defined", myInterface->token->line); 

        // for each prototype P in the interface defined by I, 
        //      look up P in the class hierarchy.
        //      if not defined, or not a function, or has return type or
        //         parameters that do not match exactly,
        //            issue the appropriate semantic error
        for (size_t i=0; i<interface->functions.size(); i++){
          S_function * classProto = NULL;
          S_function * interfaceProto = (S_function *)interface->functions[i];
          classProto = virtualFunctionLookup(interfaceProto->name, currentClass);
          if (classProto != NULL){
            if (classProto->returnType == NULL || interfaceProto->returnType == NULL){
              if (!(classProto->returnType == NULL && interfaceProto->returnType == NULL))
                semantic_error(
                  interfaceName + " is not properly implemented (Return types do not match)",
                  myInterface->token->line
                );
            }
            else if (classProto->returnType->name != interfaceProto->returnType->name)
              semantic_error(
                interfaceName + " is not properly implemented (Return types do not match)",
                myInterface->token->line
              );
            if (classProto->formals.size() != interfaceProto->formals.size()){
              semantic_error(
                interfaceName + " is not properly implemented (Number of formals does not match)",
                myInterface->token->line
              );
            }
            else {
              for (size_t k=0; k<classProto->formals.size(); k++){
                if (classProto->formals[k]->type->name != interfaceProto->formals[k]->type->name){
                  semantic_error(
                    interfaceName + " is not properly implemented (Formal types do not match)",
                    myInterface->token->line
                  );
                }
              }
            }
          }
          else{
            semantic_error(interfaceName + " is not properly implemented (Prototype not found)",
              myInterface->token->line);
          }
        }
        currentClass->interfaces.push_back(interface);
      }
      currentClass = NULL;
    }
  }
  
  else if(
    currentClass && 
    (tree->description == "VariableDecl" || tree->description == "FunctionDecl")
    ){
      S_class * parent = currentClass->parentClass;
      while (parent){
        for (size_t i=0; i<parent->fields.size(); i++){

          bool fieldIsVariable = false;
          if (parent->fields[i]->kind() == "S_variable")
            fieldIsVariable = true;
          
          if (tree->description == "VariableDecl")
            checkVarField(tree, parent, fieldIsVariable, currScope, i);
          else{ // FunctionDecl   
            checkFuncField(tree, parent, fieldIsVariable, currScope, i);
            pass2(tree->children[3], currScope);
          }
        }
      parent = getParent(parent);
    }

    if (tree->description == "VariableDecl")
      pass2(tree->children[0], currScope);
    else
      pass2(tree->children[3], currScope);
  }
  
  else{
    for (size_t i = 0; i < tree->children.size();i++)
      pass2(tree->children[i], currScope);
  }
}


// Ensures tree is of type int or double
void ensureIntOrDouble(ParseTree * tree, int binopLineNo){
  if (tree->children[0]->myType == "Int"){
    if (tree->children[2]->myType == "Int"){
      tree->myType = "Int";
    }
    else
      semantic_error("Types do not match", binopLineNo);
  }
  else if (tree->children[0]->myType == "Double"){
    if (tree->children[2]->myType == "Double"){
      tree->myType = "Double";
    }
    else
      semantic_error("Types do not match", binopLineNo);
  }
  else
    semantic_error("Incorrect type used", binopLineNo);
}

// Returns true iff (ParseTree*) tree is an array type, else false
bool isArray(ParseTree * tree, Symtab* & currScope){
  S_variable * lookup = (S_variable *)currScope->lookup(tree->children[1]->token->text);
  //unsigned foundArray = lookup->type->name.find("[]");
  //cout << "found: " << lookup->type->name << foundArray << endl;
  //if (foundArray!=std::string::npos)
  char ch = *lookup->type->name.rbegin();
  if (ch == ']')
    return true;
  else
    return false;
}

// Returns true iff (string) type is an array, else false
bool typeIsArray(string type){
  char ch = *type.rbegin();
  if (ch == ']')
    return true;
  else
    return false;
}

// Identifier lookup and array check
bool identIsArray(string ident, Symtab* & currScope){
  S_variable * lookup = (S_variable *)currScope->lookup(ident);
  return typeIsArray(lookup->type->name);
}


// Returns true iff two types are compatible, else false
bool compatible(string subtype, string supertype){
  if (subtype == "Void" || supertype == "Void")
    return false;
  if (subtype == supertype)
    return true;

  if (typeIsArray(subtype))
    return false;
  if (typeIsArray(supertype))
    return false;

  semantics * lookup1 = global_lookup(subtype);
  semantics * lookup2 = global_lookup(supertype);
  if (lookup1->kind() == "S_builtin" || lookup2->kind() == "S_builtin")
    if (subtype != supertype)
      return false;

  S_class * subtypeLookup = (S_class *)global_lookup(subtype);
  for (size_t i=0; i<subtypeLookup->interfaces.size(); i++){
    if (subtypeLookup->interfaces[i]->name == supertype)
      return true;
  }

  if (subtypeLookup->parentClass == NULL)
    return false;
  return compatible(subtypeLookup->parentClass->name, supertype);
}

// A special field lookup (with semantic error checking) for variables in pass3
void findFieldType(ParseTree * tree, Symtab* & currScope){
  // look up the class heirarchy for the field var and set tree->myType
  S_class * varClassLookup = (S_class *)global_lookup(tree->children[0]->myType);
  bool foundField = false;
  while (varClassLookup && !foundField){
    for (size_t i = 0; i<varClassLookup->fields.size(); i++){
      // Skip all functions
      if (varClassLookup->fields[i]->kind() == "S_variable"){
        if (currentClass == varClassLookup){
          S_variable * varFieldLookup = virtualVariableLookup(tree->children[1]->token->text, varClassLookup);
          if (varFieldLookup){
            foundField = true;
            tree->myType = varFieldLookup->type->name;
        }
      }
        break; 
      }

      S_variable * fieldLookup = (S_variable *)varClassLookup->fields[i];
      if (fieldLookup->name == tree->children[1]->token->text){
        tree->myType = fieldLookup->type->name;
        foundField = true;
        break;
      }
    }
    varClassLookup = varClassLookup->parentClass;
  }

  if (!foundField)
    semantic_error(tree->children[1]->token->text + " is not a valid field in the current scope",
        tree->children[1]->token->line);

}

// Converts token->type to a simple string
string constantToTypeName(ParseTree * tree){
    if (tree->token->type == T_IntConstant){
      return "Int";
    }
    else if (tree->token->type == T_BoolConstant){
      return "Bool";
    }
    else if (tree->token->type == T_DoubleConstant){
      return "Double";
    }
    else if (tree->token->type == T_StringConstant){
      return "String";
    }
    else if (tree->token->type == T_Null){
      return "Null";
    }
    else if (tree->token->type == T_Identifier)
      return "Ident";
    else
      return "";
}

// Recursively builds array type name (e.g. 'Int [] []')
string arrayType(ParseTree * tree){
  if (tree->children[0]->type == TERMINAL)
    return constantToTypeName(tree->children[0]);
  else
    return arrayType(tree->children[0]) + " []";
}

/*

Algorithm for Pass 3, in which we confirm that all statements
are semantic-error free.  The basic idea is to analyze the code of each function or method,
ascribing a type to every expression (tree->myType), to verify correctness of method calls, and to ensure
that lvalues are valid.

*/

void pass3(ParseTree* & tree, Symtab* & currScope){
  if (!tree)
    return;
  cout << tree->description << endl;
  if (tree->symtab)
    currScope = tree->symtab;

  if (tree->type == TERMINAL){
    tree->myType = constantToTypeName(tree);
  }

  else if (tree->description == "ClassDecl"){
    currentClass = (S_class *)global_lookup(tree->children[0]->token->text);
    for (size_t i = 0; i < tree->children.size();i++)
      pass3(tree->children[i], currScope);
    currentClass = NULL;
  }

  else if (tree->description == "FunctionDecl"){
    currentFunction = (S_function *)(currScope->lookup(tree->children[1]->token->text));
    for (size_t i = 0; i < tree->children.size();i++)
      pass3(tree->children[i], currScope);

    currentFunction = NULL;
  }

  else if (tree->description == "Binop"){ // Handling semantic checking for all binary ops
    pass3(tree->children[0], currScope);
    pass3(tree->children[2], currScope);
    string binopName = tree->children[1]->token->text;
    int binopLineNo = tree->children[1]->token->line;
    string type1 = tree->children[0]->myType;
    string type2 = tree->children[2]->myType;

    string myTypes[5] = {"Array", "Int", "Boolean", "String", "Double"};
    bool isPrimitive = false;
    for (size_t i = 0; i<5; i++){
      if (type1 == myTypes[i] || type2 == myTypes[i]){
        isPrimitive = true;
      }
    }
    if (isPrimitive)
      if (type1 != type2)
        semantic_error("Types do not match", binopLineNo);   


    if (binopName == "==" || binopName == "!="){

      if ((type1 == "String" && type2 == "Null") || (type2 == "String" && type1 == "Null"))
          tree->myType = "Bool";
      else{
        // Null can only be compared to S_class or S_interface
        if (type1 == "Null"){
          semantics * lvalLookup = currScope->lookup(type2);
          if (lvalLookup->kind() == "S_class" || lvalLookup->kind() == "S_interface")
            tree->myType = "Bool";
          else
            semantic_error("Types are not compatible", binopLineNo);
        }

        else if(type2 == "Null"){
          semantics * lvalLookup = currScope->lookup(type1);
          if (lvalLookup->kind() == "S_class" || lvalLookup->kind() == "S_interface")
            tree->myType = "Bool";
          else
            semantic_error("Types are not compatible", binopLineNo);
        }

        else{
            // ensure type1 or type2 is a subtype of or equal to the other
            // ascribe type(tree) = boolean
            if (compatible(type1, type2) ||
                compatible(type2, type1))
              tree->myType = "Bool";
            else
              semantic_error("Types are not compatible", binopLineNo);
        }
      }
    }

    else if(binopName == "="){
      if (compatible(type2, type1))
        tree->myType = "Assign";
      else
        semantic_error("Types are not compatible", binopLineNo);
    }    

    else if (
      binopName == "+" || binopName == "-" || binopName == "*" ||
      binopName == "/" || binopName == "%"){
      ensureIntOrDouble(tree, binopLineNo);
    }
    else if (
      binopName == "<" || binopName == "<=" || binopName == ">" ||
      binopName == ">" || binopName == ">="
      ){
      ensureIntOrDouble(tree, binopLineNo);
      tree->myType = "Bool";
    }
    else if(binopName == "&&" || binopName == "||"){
      if (tree->children[0]->myType == "Bool" && tree->children[2]->myType == "Bool")
        tree->myType = "Bool";
      else
        semantic_error("Incorrect type used", binopLineNo);
    }
  }

  else if (tree->description == "Not"){
    pass3(tree->children[0], currScope);
    if (tree->children[0]->myType != "Bool")
      semantic_error("Not can only be used on a boolean expression", 0);
    else
      tree->myType = "Bool";
  }

  else if (tree->description == "Variable"){
    tree->myType = get_type_name(tree->children[0]);
  }

  else if (tree->description == "LValue"){
    semantics *identLookup = currScope->lookup(tree->children[1]->token->text);
    if (identLookup == NULL && currentClass){
      identLookup = virtualVariableLookup(tree->children[1]->token->text, currentClass);
    }
    if (identLookup == NULL)
      semantic_error(tree->children[1]->token->text + " has not been defined",
        tree->children[1]->token->line);
    
    if (tree->children[0] == NULL){
      tree->myType = ((S_variable *)identLookup)->type->name;
    }
  }

  else if (tree->description == "Type_Int")
    tree->myType = "Int";
  else if (tree->description == "Type_Bool")
    tree->myType = "Bool";
  else if (tree->description == "Type_Double")
    tree->myType = "Double";
  else if (tree->description == "Type_String")
    tree->myType = "String";
  else if (tree->description == "Type_Array")
    tree->myType = arrayType(tree);


  else if (tree->type == TERMINAL){
    tree->myType = constantToTypeName(tree);
  }

  else if(tree->description == "PrintStmt"){
    ParseTree * actuals = tree->children[0];
    for (size_t i=0; i<actuals->children.size(); i++){
      pass3(actuals->children[i], currentScope);  
      if (actuals->children[i]->type != TERMINAL){
        string actualType = actuals->children[i]->myType;

        string myTypes[4] = {"Int", "Bool", "String", "Double"};
        bool isPrimitive = false;
        for (size_t i = 0; i<4; i++){
          if (actualType == myTypes[i]){
            isPrimitive = true;
          }
        }
        if (!isPrimitive)
          semantic_error("Cannot print type " + actualType, 0, true); 
      }
    }
  }

  else if(tree->description == "New"){
    S_class * classLookup = (S_class *)global_lookup(tree->children[0]->token->text);
    if (classLookup)
      tree->myType = classLookup->name;
    else
      semantic_error(tree->children[0]->token->text + " is not a defined class",
        tree->children[0]->token->line);
  }

  else if (tree->description == "NewArray"){
    pass3(tree->children[0], currScope);
    pass3(tree->children[1], currScope);

    if (tree->children[0]->myType != "Int")
      semantic_error(tree->children[0]->token->text + " is not an integer",
          tree->children[0]->token->line);

    if (tree->children[1]->myType != ""){
      S_class * classLookup = (S_class *)global_lookup(tree->children[1]->myType);
      if (!classLookup)
        semantic_error(tree->children[1]->token->text + " is not a class",
            tree->children[0]->token->line);
      else
        tree->myType = tree->children[1]->myType + " []";
    }
    else
      semantic_error("Invalid class",
            tree->children[0]->token->line);   

  }

  else if (tree->description == "Bracket"){
    pass3(tree->children[0], currScope);
    pass3(tree->children[1], currScope);

    if (tree->children[1]->myType != "Int")
      semantic_error(tree->children[1]->token->text + " is not an integer",
          tree->children[1]->token->line);


    ParseTree * arrayVar = tree->children[0];
    int arrayDepth = 1;
    while (arrayVar->description == "Bracket"){
      arrayVar = arrayVar->children[0];
      arrayDepth++;
    }

    if (tree->children[0]->description == "Dot"){  // Need to fix for array fields
      arrayVar = arrayVar->children[0];
    }
    string arrayName = arrayVar->children[1]->token->text;
    if (!isArray(arrayVar, currScope))
      semantic_error(arrayName + " is not an array",
        arrayVar->token->line);
    else{
      S_variable * arrayLookup = (S_variable *)currScope->lookup(arrayName);
      string arrayTypeName = arrayLookup->type->name;
      tree->myType = arrayTypeName.substr(0, arrayTypeName.size()-3*arrayDepth);
    }
  }

  else if (tree->description == "Dot"){
    pass3(tree->children[0], currScope);
    findFieldType(tree, currScope);
  }

  else if (tree->description == "return"){
    if (!currentFunction)
      semantic_error("'return' used outside of a Function", 0, true); 

    if (tree->children.size() == 0 && currentFunction->returnType != NULL)
      semantic_error("No value was returned from non-void function", 0, true); 
    if (tree->children.size() != 0 && currentFunction->returnType == NULL)
      semantic_error("Value was returned from void function", 0, true); 
    if (tree->children.size() > 0){
      pass3(tree->children[0], currScope);
      if (currentFunction->returnType->name != tree->children[0]->myType)
        semantic_error("Incorrect return type used", 0, true); 
      tree->myType = tree->children[0]->myType;
    }
  }

 else if (tree->description == "While"){
    pass3(tree->children[0], currScope);
    if (tree->children[0]->myType != "Bool")
      semantic_error("While statement requires boolean condition", 0, true);
    pass3(tree->children[1], currScope);
  }

 else if (tree->description == "If"){
    pass3(tree->children[0], currScope);
    if (tree->children[0]->myType != "Bool")
      semantic_error("While statement requires boolean condition", 0, true);
    pass3(tree->children[1], currScope);
    if (tree->children[2])
      pass3(tree->children[2], currScope);
  }

 else if (tree->description == "For"){
    ParseTree * boolTree;
    if (tree->children.size() < 4){
      boolTree = tree->children[0];
      pass3(tree->children[1], currScope);
    }
    else{
      boolTree = tree->children[1];
      pass3(tree->children[3], currScope);
      pass3(tree->children[0], currScope);
      pass3(tree->children[2], currScope);

    }
    pass3(boolTree, currScope);
    if (boolTree->myType != "Bool")
      semantic_error("While statement requires boolean condition", 0, true); 
  }

  else if(tree->description == "Negative"){
    pass3(tree->children[0], currScope);
    tree->myType = tree->children[0]->myType;
  }

  else if(tree->description == "StmtBlock"){
    if (tree->children[0])
      for (size_t i=0; i<tree->children[0]->children.size(); i++){
        pass3(tree->children[0]->children[i], currScope);
      }
    if (tree->children[1])
      for (size_t i=0; i<tree->children[1]->children.size(); i++){
        pass3(tree->children[1]->children[i], currScope);
      }
  }

  else if(tree->description == "VariableDecl"){
    pass3(tree->children[0], currScope);
    tree->myType = tree->children[0]->myType;
  }

  else if (tree->description == "this"){
    if (!currentClass)
      semantic_error("'this' used outside of a Class", 0, true); 
  }

  else if (tree->description == "Call"){
    S_function * funcLookup;
    if (tree->children[0] == NULL){
      funcLookup = (S_function *)currScope->lookup(tree->children[1]->token->text);

      if (!funcLookup){
        if (currentFunction->name == tree->children[1]->token->text){ // This is probably not safe
          funcLookup = currentFunction;
        }
        else{
          semantic_error(tree->children[1]->token->text + " is not a defined function",
            tree->children[1]->token->line);
        }
      }
    }
    else{
      pass3(tree->children[0], currScope);
      semantics * varClassLookup = global_lookup(tree->children[0]->myType);
      funcLookup = virtualFunctionLookup(tree->children[1]->token->text, varClassLookup);
      if (!funcLookup){
        semantic_error(tree->children[1]->token->text + " is not a defined function",
          tree->children[1]->token->line);
      }

    }
    if (tree->children[2] == NULL){
      if (funcLookup->formals.size()>0){
        semantic_error(tree->children[1]->token->text + " has incorrect number of formals",
            tree->children[1]->token->line);
      }

    }
    else{ // There are parameters in tree
      if (funcLookup->formals.size() != tree->children[2]->children.size()){
        semantic_error(tree->children[1]->token->text + " has incorrect number of formals",
            tree->children[1]->token->line);
      }
      for (size_t i=0; i<funcLookup->formals.size(); i++){
        string name1;
        if (tree->children[2]->children[i]->type == TERMINAL){
          name1 = constantToTypeName(tree->children[2]->children[i]);
          if (name1 == "Ident"){
            S_variable * identLookup = (S_variable *)currScope->lookup(tree->children[2]->children[i]->token->text);
            if (!identLookup)
              semantic_error(tree->children[2]->children[i]->token->text + "is not defined",
                tree->children[1]->token->line);
            else
              name1 = identLookup->type->name;
          }
        }
        else{
          pass3(tree->children[2]->children[i], currScope);
          name1 = tree->children[2]->children[i]->myType;
        }
        string name2 = funcLookup->formals[i]->type->name;
        if (!compatible(name1, name2))
          semantic_error("Formal types do not match definition",
            tree->children[1]->token->line);
      }
    }
    
    if (funcLookup->returnType)
      tree->myType = funcLookup->returnType->name;
    else
      tree->myType = "Void";

  }


  else{
    for (size_t i = 0; i < tree->children.size();i++)
      pass3(tree->children[i], currScope);
    tree->myType = "";
  }
}


/*

   ~~~~~~~~~~~~~~~~~~~~~~~~ CODE GENERATION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/

extern int yyparse(void);
extern ParseTree * top;
extern FILE * yyin;
int labelNum = 0;
void generate_expr(ParseTree * expr);
string topClassName;
bool inArray = false;
string currClassName;
bool inCall = false;
string negative = "";

string decaf_type_to_jasmin_type(string tname)
{
  string array = "";
  
  
  // suck up the array type stuff...
  while (tname[tname.size()-1]==']') {
    array += "[";
    tname = tname.substr(0, tname.size()-3);
  }
  // convert from decaf builtin 
  // to jasmin/java builtin
  if (tname=="String")
    tname = "Ljava/lang/String;";
  else if (tname=="Int") 
    tname = "I"; 
  else if (tname=="Bool")
    tname = "Z";
  else if (tname=="Double")
    tname = "D"; 
  else
    tname = "L" + tname + ";";
  return array + tname;
}



string jasmin_type_from_tree(ParseTree * typetree)
{
  // typetree is a possibly NULL 
  // parsetree representing a type.
  // return the correct jasmin string for this type.

  if (!typetree)
    return "V";  // void if the tree is NULL

  return decaf_type_to_jasmin_type(get_type_name(typetree));
  
}

string decaf_type(ParseTree * exprTree)
{
  // tree represents an expression  (actual parameter, rvalue, etc)
  // return the decaf type (string) determined
  // during pass 3 of semantic type-checking
  if (exprTree->myType == "Double")
    return "Double";
  else if (exprTree->myType == "Bool")
    return "Bool";
  else if (exprTree->myType == "String")
    return "String";
  else
    return "Int";
}

string jasmin_binop_instruction(string op, string type)
{
  if (type=="Int") {
    if (op=="+")
      return "iadd";
    if (op=="-")
      return "isub";
    if (op=="*")
      return "imul";
    if (op=="/")
      return "idiv";
    if (op=="%")
      return "irem";
    exit(1);
  }
  else if (type=="Double") {
    if (op=="+")
      return "dadd";
    if (op=="-")
      return "dsub";
    if (op=="*")
      return "dmul";
    if (op=="/")
      return "ddiv";
    if (op=="%")
      return "drem";
    exit(1);
  }
  exit(1);
}


string initializeArray(ParseTree * tree){
  if (get_type_name(tree->children[1]) == "String")
    return "anewarray";
  else
    return "newarray";
}

string load_mnemonic(string typestr)
{
  if (typestr == "Double")
    return "dload";
  else if (typestr == "String")
    return "aload";
  else
    return "iload";
}

string load_mnemonic_array(string typestr)
{
  if (typestr == "Double")
    return "daload";
  else if (typestr == "Bool")
    return "baload";
  else if (typestr == "Int")
    return "iaload";
  else
    return "aaload";
}

string store_mnemonic(string typestr)
{
  if (typestr == "Double")
    return "dstore";
  else if (typestr == "Int" || typestr == "Bool")
    return "istore";
  else
    return "astore";
}

string store_mnemonic_array(string typestr)
{
  if (typestr == "Double")
    return "dastore";
  else if (typestr == "Int")
    return "iastore";
  else if (typestr == "Bool")
    return "bastore";
  else
    return "aastore";
}

void generate_variable_load(string varname)
{
  S_variable *var=(S_variable *)currentScope->lookup(varname);
  if (var->isGlobal){
    *currentFile << "   getstatic  " << topClassName << "/"
                 << varname << " "
                 << decaf_type_to_jasmin_type(var->type->name) << endl;
  }
  else{

    if (currentClass && !var->isGlobal)
      *currentFile << "   aload_0" << endl;

    if (identIsArray(varname, currentScope))
      *currentFile << "   aload  " << var->sequence << endl;
    else
      *currentFile << "   " 
                   << load_mnemonic(var->type->name) << " " 
                   << var->sequence << endl;
  }
}


void generate_class_variable_load(string varname)
{
  //S_variable *var=(S_variable *)currentScope->lookup(varname);
  S_variable * var = (S_variable *)field_lookup(varname, currentScope, currentClass);
  *currentFile << "   aload  " << var->sequence << endl
               << "   getfield  " << currentClass->name << "/"
               << varname << " "
               << decaf_type_to_jasmin_type(var->type->name) << endl;



  
}

string create_label(){
  stringstream ss;
  ss << "LABEL" << labelNum;
  labelNum++;
  return ss.str();
}


void generate_bool_expr(ParseTree * expr){
  string boolOp;
  string doubleCmp = "";

  string treeOp = expr->children[1]->token->text;
  string label1 = create_label();
  string label2 = create_label();
  if (expr->children[0]->myType == "Bool" && expr->children[2]->myType == "Bool"){
    string label3 = create_label();
    if (treeOp == "||")
      boolOp = "ifne";
    else if (treeOp =="&&")
      boolOp = "ifeq";
  
    generate_expr(expr->children[0]);
    *currentFile << "   " << boolOp << "  " << label1 << endl;
    generate_expr(expr->children[2]);
    *currentFile << "   ifeq  " << label2 << endl
                 << label1 + ":" << endl
                 << "   iconst_1" << endl
                 << "   goto  " << label3 << endl
                 << label2 + ":" << endl
                 << "   iconst_0" << endl
                 << label3 + ":" << endl;

    return;
  }

  generate_expr(expr->children[0]);
  generate_expr(expr->children[2]);

  if (expr->children[0]->myType == "Int" && expr->children[2]->myType == "Int"){
    if (treeOp == "<")
      boolOp = "if_icmpge";
    else if (treeOp ==">")
      boolOp = "if_icmple";
    else if (treeOp ==">=")
      boolOp = "if_icmplt";
    else if (treeOp =="<=")
      boolOp = "if_icmpgt";
    else if (treeOp =="==")
      boolOp = "if_icmpne";
    else if (treeOp =="!=")
      boolOp = "if_icmpeq";
  }
  else if (expr->children[0]->myType == "Double" && expr->children[2]->myType == "Double"){
    if (treeOp == "<"){
      doubleCmp = "dcmpg";
      boolOp = "ifge";
    }
    else if (treeOp ==">"){
      doubleCmp = "dcmpl";
      boolOp = "ifle";
    }
    else if (treeOp ==">="){
      doubleCmp = "dcmpl";
      boolOp = "iflt";
    }
    else if (treeOp =="<="){
      doubleCmp = "dcmpg";
      boolOp = "ifgt";
    }
    else if (treeOp =="=="){
      doubleCmp = "dcmpl";
      boolOp = "ifne";
    }
    else if (treeOp =="!="){
      doubleCmp = "dcmpl";
      boolOp = "ifeq";
    }
    *currentFile << "   "
                 << doubleCmp << endl; 
  }


  *currentFile << "   "
               << boolOp << "   " << label1 << endl
               << "   " << "iconst_1" << endl
               << "   " << "goto" << "   " << label2 << endl
               << label1 + ":" << endl
               << "   " << "iconst_0" << endl
               << label2 + ":" << endl;


}


void generate_bool_for_expr(ParseTree * expr, string label){

  generate_expr(expr->children[0]);
  generate_expr(expr->children[2]);

  string boolOp;
  string doubleCmp = "";

  string treeOp = expr->children[1]->token->text;

  if (expr->children[0]->myType == "Int" && expr->children[2]->myType == "Int"){
    if (treeOp == "<")
      boolOp = "if_icmpge";
    else if (treeOp ==">")
      boolOp = "if_icmple";
    else if (treeOp ==">=")
      boolOp = "if_icmplt";
    else if (treeOp =="<=")
      boolOp = "if_icmpgt";
    else if (treeOp =="==")
      boolOp = "if_icmpne";
    else if (treeOp =="!=")
      boolOp = "if_icmpeq";
  }
  else if (expr->children[0]->myType == "Double" && expr->children[2]->myType == "Double"){
    if (treeOp == "<"){
      doubleCmp = "dcmpg";
      boolOp = "ifge";
    }
    else if (treeOp ==">"){
      doubleCmp = "dcmpl";
      boolOp = "ifle";
    }
    else if (treeOp ==">="){
      doubleCmp = "dcmpl";
      boolOp = "iflt";
    }
    else if (treeOp =="<="){
      doubleCmp = "dcmpg";
      boolOp = "ifgt";
    }
    else if (treeOp =="=="){
      doubleCmp = "dcmpl";
      boolOp = "ifne";
    }
    else if (treeOp =="!="){
      doubleCmp = "dcmpl";
      boolOp = "ifeq";
    }
    *currentFile << "   "
                 << doubleCmp << endl; 
  }


  *currentFile << "   "
               << boolOp << "   " << label << endl;

}

string typeTree_to_arrayString(ParseTree * expr){
  if (expr->description == "Type_Int")
    return "int";
  else if(expr->description == "Type_String")
    return "java/lang/String";
  else if(expr->description == "Type_Double")
    return "double";
  else
    return "boolean";
}

string jasmin_semantic_formals(vector<S_variable *> formals)
{
  // collect all the jasmin types of the 
  // formals into one string
  string answer = "";
  if (formals.size() > 0){
    for (size_t i=0;i<formals.size();i++) {
      answer += decaf_type_to_jasmin_type(formals[i]->type->name);
    }
  }
  return "(" + answer + ")";
}

void generate_statement(ParseTree * statement);

void generate_expr(ParseTree * expr)
{
  // Post:  generate code that will compute expr and leave 
  // it on the stack.
  
  // terminals:
  cout << "generate_expr: " << expr->description << endl;
  if (expr->type==TERMINAL and expr->token->type==T_IntConstant) {
    if (!inArray && !inCall)
      *currentFile << "   sipush   " << negative << expr->token->text << endl;
    else
      *currentFile << "   bipush   " << negative << expr->token->text << endl;
  }
  else if (expr->type==TERMINAL and expr->token->type==T_DoubleConstant) {
    *currentFile << "   ldc2_w   " << negative << expr->token->text << endl;
  }
    else if (expr->type==TERMINAL and expr->token->type==T_StringConstant) {
    *currentFile << "   ldc   " << expr->token->text << endl;
  }
  else if (expr->type==TERMINAL and expr->token->type==T_BoolConstant) {
    if (expr->token->text == "true")
      *currentFile << "   iconst_1" << endl;
    else
      *currentFile << "   iconst_0" << endl;
  }

  else if(expr->description == "Negative"){
    negative = "-";
    generate_expr(expr->children[0]);
    negative = "";
  }

  else if (expr->description == "Not"){
    string label1 = create_label();
    string label2 = create_label();

    generate_expr(expr->children[0]);
    *currentFile << "   ifne  " << label1 << endl
                 << "   iconst_1" << endl
                 << "   goto  " << label2 << endl
                 << label1 + ":" << endl
                 << "   iconst_0" << endl
                 << label2 + ":" << endl;
  }

  else if (expr->type==NONTERMINAL and expr->myType=="Bool" and expr->description != "LValue") {
    generate_bool_expr(expr);
  }

  else if (expr->type==NONTERMINAL and expr->description=="Binop" ){
    generate_expr(expr->children[0]);
    generate_expr(expr->children[2]);
    *currentFile << "   " 
                 << jasmin_binop_instruction(expr->children[1]->token->text,
                                             decaf_type(expr))
                 << endl;
  }
  else if (expr->type==TERMINAL and expr->token->type==T_Identifier) {
    generate_variable_load(expr->token->text);
  }

  else if(expr->description == "Dot"){
    S_variable * lvalLookup = (S_variable *)currentScope->lookup(expr->children[0]->children[1]->token->text);
    *currentFile << "   aload  " << lvalLookup->sequence << endl;;

    S_class * classLookup = (S_class *)global_lookup(lvalLookup->type->name);
    S_variable * varFieldLookup = virtualVariableLookup(expr->children[1]->token->text, classLookup);
    S_function * funcFieldLookup = virtualFunctionLookup(expr->children[1]->token->text, classLookup);
    if (varFieldLookup){
      *currentFile << "   getfield  " << lvalLookup->type->name
                   << "/" << expr->children[0]->children[1]->token->text
                   << " " << decaf_type_to_jasmin_type(varFieldLookup->type->name) << endl;

    }
    else
      *currentFile << "   invokevirtual  " << lvalLookup->type->name
                   << "/" << lvalLookup->name << " "
                   << jasmin_semantic_formals(funcFieldLookup->formals)
                   << decaf_type_to_jasmin_type(funcFieldLookup->returnType->name) << endl;

  }

  else if (expr->description == "LValue"){
    S_variable * lvalLookup = (S_variable *)field_lookup(expr->children[1]->token->text, currentScope, currentClass);
    
    if (currentClass && !(lvalLookup->isGlobal || lvalLookup->isInstance))
      generate_class_variable_load(expr->children[1]->token->text);
    else
      generate_variable_load(expr->children[1]->token->text);
  }

  else if (expr->description == "NewArray"){
    *currentFile << "   bipush  " << expr->children[0]->token->text << endl
                 << "   " << initializeArray(expr) << "  "
                 << typeTree_to_arrayString(expr->children[1]) << endl;
  }

  else if(expr->description == "Bracket"){
    inArray = true;
    generate_variable_load(expr->children[0]->children[1]->token->text);
    generate_expr(expr->children[1]);
    *currentFile << "   " << load_mnemonic_array(expr->myType) << endl;
    inArray = false;
  }

  else if (expr->description == "Actuals"){
    for (size_t i=0; i<expr->children.size(); i++){
      generate_expr(expr->children[i]);
    }
  }

  else if(expr->description == "Call")
    generate_statement(expr);

  else if(expr->description == "New"){
    *currentFile << "   new  " << expr->children[0]->token->text << endl
                 << "   dup" << endl
                 << "   invokespecial  " << expr->children[0]->token->text << "/<init>()V" << endl;
  }


  else {
    exit(1);
  }
}

void generate_Print_expr(ParseTree * expr)
{
  // generates a print statement
  // for Print(expr).
  *currentFile << "   getstatic java/lang/System/out "
   "Ljava/io/PrintStream;" << endl;
  if (expr->description == "LValue")
    generate_expr(expr->children[1]);
  else
    generate_expr(expr);
  *currentFile << "   invokevirtual java/io/PrintStream/print("
               << decaf_type_to_jasmin_type(decaf_type(expr)) << ")V" << endl;
}


void generate_Print(ParseTree * printstmt) {
  ParseTree *exprs = printstmt->children[0];
  for (size_t i=0;i<exprs->children.size();i++) 
    generate_Print_expr(exprs->children[i]);
  // followed by a println...
  *currentFile << "   getstatic java/lang/System/out "
   "Ljava/io/PrintStream;" << endl;
  *currentFile << "   invokevirtual java/io/PrintStream/println()V" << endl;
}

void generate_global_vars(ParseTree * tree){
  for (size_t i=0; i<tree->children.size(); i++){
    if (tree->children[i]->description == "VariableDecl")
      *currentFile << ".field   public static " << tree->children[i]->children[0]->children[1]->token->text << " "
                   << decaf_type_to_jasmin_type(get_type_name(tree->children[i]->children[0]->children[0])) << endl;
    else
      break;
  }
  *currentFile << endl;
}

void generate_preamble(string className, string superClassName, ParseTree * tree)
{
  // write the jasmin header for a class.
  // superClassName might be the empty string if there is
  // no superclass.
  // 
  *currentFile << ".class public " << className << endl
               << ".super " 
               <<  (superClassName==""?"java/lang/Object":superClassName)
               << endl
               << endl;
  generate_global_vars(tree);
    // constructor!
  *currentFile << ".method                  public <init>()V" << endl;
  *currentFile << ".limit stack          1" << endl;
  *currentFile << ".limit locals         1" << endl;
  *currentFile << "   aload_0" << endl;
  *currentFile << "   invokespecial         java/lang/Object/<init>()V" << endl;
  *currentFile << "   return" << endl;
  *currentFile << ".end method" << endl;
}

void generate_field_headers(ParseTree * fields);

void generate_class_preamble(string className, string superClassName, ParseTree * fields)
{
  // write the jasmin header for a class.
  // superClassName might be the empty string if there is
  // no superclass.
  // 

  string newSuperClassName = superClassName==""?"java/lang/Object":superClassName;

  *currentFile << ".class public " << className << endl
               << ".super " 
               <<  newSuperClassName
               << endl
               << endl;
  generate_field_headers(fields);
    // constructor!
  *currentFile << ".method                  public <init>()V" << endl;
  *currentFile << ".limit stack          1" << endl;
  *currentFile << ".limit locals         1" << endl;
  *currentFile << "   aload_0" << endl;
  *currentFile << "   invokespecial         " << newSuperClassName << "/<init>()V" << endl;
  *currentFile << "   return" << endl;
  *currentFile << ".end method" << endl;
}

void generate_main_java_function(string mainClassName)
{
  *currentFile << ".method                  public static main([Ljava/lang/String;)V" << endl;
  *currentFile << "   .limit stack          0" << endl;
  *currentFile << "   .limit locals         1" << endl;
  *currentFile << "   invokestatic          " << mainClassName << "/main()V" << endl;
  *currentFile << "   return                " << endl;
  *currentFile << ".end method " << endl << endl;
}

string jasmin_formals(ParseTree * formals)
{
  // collect all the jasmin types of the 
  // formals into one string
  string answer = "";
  if (formals){
    for (size_t i=0;i<formals->children.size();i++) {
      ParseTree* var = formals->children[i];
      answer += decaf_type_to_jasmin_type(get_type_name(var->children[0]));
    }
  }
  return "(" + answer + ")";
}



void generate_block(ParseTree * block);
void generate_class(ParseTree * classs);
void generate_return(ParseTree * tree);

void generate_statement(ParseTree * statement)
{
  if (statement->description=="PrintStmt") {
    generate_Print(statement);
  }
  else if (statement->myType=="Assign") {
    // assign a local variable.  FIXME: there is so much more possible.
    // Current assumptions:  It's a simple local variable.
    //*currentFile << "   aload_0" << endl;
    if (currentClass)
      *currentFile << "   aload_0" << endl;
    if (statement->children[0]->description == "LValue"){
      generate_expr(statement->children[2]);
      S_variable * var = 
        (S_variable *)currentScope->lookup(statement->children[0]->children[1]->token->text);

      if (var->isGlobal){
          *currentFile << "   putstatic  " << topClassName << "/"
                       << var->name << " "
                       << decaf_type_to_jasmin_type(var->type->name) << endl;
      
      }
      else{
        if (isArray(statement->children[0], currentScope)){
          *currentFile << "   astore  " << var->sequence <<endl;
        }
        else if (currentClass){
          *currentFile << "   putfield  " << currentClass->name << "/"
                       << statement->children[0]->children[1]->token->text << " "
                       << decaf_type_to_jasmin_type(var->type->name) << endl;
        }
        else {
          *currentFile << "   " << store_mnemonic(var->type->name) 
                       << " " << var->sequence <<endl;
        }
      }
    }
    else if (statement->children[0]->description == "Bracket"){
      inArray = true;
      S_variable * var = 
        (S_variable *)currentScope->lookup(statement->children[0]->children[0]->children[1]->token->text);
      *currentFile << "   aload  " << var->sequence << endl;
                   //<< "   bipush  " << statement->children[0]->children[1]->token->text << endl;
      generate_expr(statement->children[0]->children[1]);
      generate_expr(statement->children[2]);
      *currentFile << "   " << store_mnemonic_array(statement->children[0]->myType) << endl;
      inArray = false;

    }
  }

  else if (statement->description == "return"){
    generate_return(statement);
  }

  else if (statement->description == "Call"){
    string funcRetType;
    inCall = true;
    if (statement->children[0]){
      

      S_class * classLookup;
      if (statement->children[0]->description == "LValue"){
        S_variable * var = (S_variable *)currentScope->lookup(statement->children[0]->children[1]->token->text);
        classLookup = (S_class *)global_lookup(var->type->name);
        *currentFile << "   aload  " << var->sequence << endl;
    }
      else {
        generate_expr(statement->children[0]);
        classLookup = (S_class *)global_lookup(statement->children[0]->myType);

      }
      S_function * funcLookup = virtualFunctionLookup(statement->children[1]->token->text, classLookup);
      if (funcLookup->returnType)
        funcRetType = decaf_type_to_jasmin_type(funcLookup->returnType->name);
      else
        funcRetType = "V";
      

      if (statement->children[2]){
        for (size_t i=0; i<statement->children[2]->children.size(); i++){
          generate_expr(statement->children[2]->children[i]);
        }
      }

      *currentFile << "   invokevirtual  " << classLookup->name << "/" << funcLookup->name
                   << jasmin_semantic_formals(funcLookup->formals) << funcRetType << endl;


    }
    else{  
      S_function * func = (S_function *)currentScope->lookup(statement->children[1]->token->text);
      
      if (func->returnType)
        funcRetType = decaf_type_to_jasmin_type(func->returnType->name);
      else
        funcRetType = "V";


      if (statement->children[2]){
        for (size_t i=0; i<statement->children[2]->children.size(); i++){
          generate_expr(statement->children[2]->children[i]);
        }
      }

      string callType = "invokestatic";
      if (currentClass && !func->isGlobal)
        callType = "invokevirtual";

      *currentFile << "   " << callType << "   " << currClassName << "/" << func->name
                   << jasmin_semantic_formals(func->formals) << funcRetType << endl;
    }
    inCall = false;
  }

  else if (statement->description == "If"){
    generate_expr(statement->children[0]);
    string elseLabel = create_label();
    string doneLabel = create_label();
    *currentFile << "   ifeq  " << elseLabel << endl;
    if (statement->children[1]->description == "StmtBlock")
      generate_block(statement->children[1]);
    else
      generate_statement(statement->children[1]);
    *currentFile << "   goto  " << doneLabel << endl;
    *currentFile << elseLabel + ":" << endl; 

    if (statement->children[2])
      generate_block(statement->children[2]);
    *currentFile << doneLabel + ":" << endl;
  }

  else if (statement->description == "While"){
    string whileLabel1 = create_label();
    string whileLabel2 = create_label();
    *currentFile << whileLabel1 + ":" << endl; 

    generate_expr(statement->children[0]);
    *currentFile << "   ifeq  " << whileLabel2 << endl;
    generate_block(statement->children[1]);
    *currentFile << "   goto  " << whileLabel1 << endl
                 << whileLabel2 + ":" << endl;


                 
  }

  else if (statement->description == "For"){
    string forLabel1 = create_label();
    string forLabel2 = create_label();
    if (statement->children.size() == 4){
      generate_statement(statement->children[0]);

      *currentFile << forLabel1 + ":" << endl; 

      generate_bool_for_expr(statement->children[1], forLabel2);

      generate_block(statement->children[3]);
      generate_statement(statement->children[2]);
    }
    else{
      *currentFile << forLabel1 + ":" << endl; 

      generate_expr(statement->children[0]);
      *currentFile << "   ifeq  " << forLabel2 << endl;
      generate_block(statement->children[1]);
      
    }
    *currentFile << "   goto  " << forLabel1 << endl
                   << forLabel2 + ":" << endl;
  }

else// FIXME
    return;
}



void generate_block(ParseTree * block)
{
  Symtab *oldScope = currentScope;
  currentScope = block->symtab;
  ParseTree * statements = block->children[1];
  for (size_t i=0;i<statements->children.size();i++)
    generate_statement(statements->children[i]);
  currentScope = oldScope;
}


void generate_return(ParseTree * tree){
  if (tree->children.size() > 0){
    generate_expr(tree->children[0]);
    if (tree->myType == "Int" || tree->myType == "Bool" )
      *currentFile << "   ireturn" << endl;
    else if (tree->myType == "String")
      *currentFile << "   areturn" << endl;
    else if (tree->myType == "Double")
      *currentFile << "   dreturn" << endl;
  }
  else{
    *currentFile << "   return" << endl;
  }

}

void generate_function_block(ParseTree * block)
{
  Symtab *oldScope = currentScope;
  currentScope = block->symtab;
  ParseTree * statements = block->children[1];
  bool generatedReturn = false;
  if (statements){
    for (size_t i=0;i<statements->children.size();i++){
      if (statements->children[i]->description == "return"){
        generate_return(statements->children[i]);
        generatedReturn = true;
      }
      else{
        generate_statement(statements->children[i]);
      }
    }
  }
  if (!generatedReturn){
    *currentFile << "   return" << endl;
  }
  currentScope = oldScope;
}

void generate_field_headers(ParseTree * fields){
  if (!fields) return;
  for (size_t i=0; i<fields->children.size(); i++){
    if (fields->children[i]->description == "VariableDecl"){
      *currentFile << ".field   public " << fields->children[i]->children[0]->children[1]->token->text << " "
      << decaf_type_to_jasmin_type(get_type_name(fields->children[i]->children[0]->children[0])) << endl << endl;
    }
  }

}

void generate_function(ParseTree * function)
{
  // generate a jasmin method.
  // it will be static if currentClass is NULL.
  string functionName = function->children[1]->token->text;
  *currentFile << ".method public " << (currentClass==NULL?"static ":" ")
               << functionName
               << jasmin_formals(function->children[2])
               << jasmin_type_from_tree(function->children[0]) << endl;
  *currentFile << "   .limit stack 10" << endl; // FIXME
  *currentFile << "   .limit locals 10" << endl; // FIXME
  generate_function_block(function->children[3]);
  // need to generate a return here.
  // for void functions, it's easy.
  // for others???  Not so sure right now.
  // FIXME!!
  
  *currentFile << ".end method" << endl << endl;    
}

void generate_class(ParseTree * classs){
  currClassName = classs->children[0]->token->text;
  currentFile = new ofstream((currClassName+".j").c_str());
  string superClassName = classs->children[1] ? classs->children[1]->token->text : "";
  generate_class_preamble(currClassName, superClassName, classs->children[3]);
  if (classs->children[3])
    for (size_t i=0; i<classs->children[3]->children.size(); i++){
      if (classs->children[3]->children[i]->description == "FunctionDecl")
        generate_function(classs->children[3]->children[i]);
    }
  currentFile = topClassFile;
  currClassName = topClassName;
  currentClass = NULL;

}



void generate(ParseTree * tree)
{
  if (!tree)
    return;
  if (tree->description=="FunctionDecl")
    generate_function(tree);
  else if (tree->description == "ClassDecl"){
    currentClass = (S_class *)global_lookup(tree->children[0]->token->text);
    generate_class(tree);
  }
  else 
    for (size_t i=0;i<tree->children.size();i++)
      generate(tree->children[i]);
}

int main(int argc, char **argv)
{
  /* Make sure there's a given file name */
  if (argc != 2) {
    cout << "USAGE: " << argv[0] << " FILE" << endl;
    exit(1);
  }       
  yyin = fopen(argv[1], "r");
  /* and that it exists and can be read */
  if (!yyin) {
    cout << argv[1] << ": No such file or file can't be opened for reading."
         << endl;
    exit(1);
  }

  yyparse();
  currentClass = NULL;
  currentInterface = NULL;
  currentFunction = NULL;
  traverseTree(top,0,0);

  // Semantics:

  currentScope = toplevel;
  makeBuiltins(currentScope);
  //openscope();
  cout << "pass1" << endl;
  pass1(top);
  linkClasses(top);
  cout << "pass2" << endl;
  pass2(top, currentScope);
  cout << "pass3" << endl;
  pass3(top, currentScope);
  cout << "pass 3 done" << endl;
  // Code generation

  topClassName = string(argv[1]);
  topClassName = topClassName.substr(0,topClassName.size()-6);  // strip .decaf
  currClassName = topClassName;

  topClassFile = new ofstream((topClassName+".j").c_str());
  currentClass = NULL;
  currentFile = topClassFile;
  cout << "preamble" << endl;

  generate_preamble(topClassName, "", top);
  cout << "generate" << endl;
  generate(top);
  generate_main_java_function(topClassName);

  return 0;
}


