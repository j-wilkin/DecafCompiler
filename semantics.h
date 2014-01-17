
#include <vector>
#include <iostream>
#include <map>
using namespace std;


struct semantics {
  // pure virtual function:
  virtual string kind() = 0;
  string name;
  // at least one pure virtual function makes the class abstract 
  // (not instantiable.)

};

struct S_type : public semantics {
  virtual string kind() { return "S_type"; }
  string name;
};

struct S_builtin : public semantics {
  virtual string kind() { return "S_builtin"; }
  string name;
};

struct S_variable : public semantics {
  S_type * type;
  int sequence;
  bool isInstance;
  bool isGlobal;
  string name;
  virtual string kind() { return "S_variable"; }
};

struct S_function : public semantics {
  virtual string kind() { return "S_function"; }
  vector<S_variable *> formals;
  bool isGlobal;
  bool isClassMethod;
  S_type * returnType;  // NULL for a void function
  string name;
};

struct S_interface : public semantics {
  virtual string kind() { return "S_interface"; }
  vector<S_function *> functions;
  string name;
};

struct S_class : public semantics {
  virtual string kind() { return "S_class"; }
  S_class * parentClass;  // extends 
  vector<S_interface *> interfaces;
  vector<semantics *> fields;  // have to be S_function or S_variable
  string name;
};


typedef map <string, semantics *> Dictionary;

struct Symtab {
  // A class to represent symbol tables
  // Chained togeher to represent nested scopes.
 public:
  Dictionary dict;
  Symtab(Symtab *p);
  semantics * lookup(string key);
  semantics * local_lookup(string key);
  
  void insert(string key, semantics * item);

  Symtab * parent;  // outer scope
};

Symtab * closescope();
void openscope();


