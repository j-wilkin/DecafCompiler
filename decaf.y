/* 
decaf.y
Jason Wilkin
Fall 2013
*/
%token Y_Void
%token Y_Int
%token Y_Double
%token Y_Bool
%token Y_String
%token Y_Class
%token Y_Interface
%token Y_Null
%token Y_This
%token Y_Extends
%token Y_Implements
%token Y_For
%token Y_While
%token Y_If
%token Y_Else
%token Y_Return
%token Y_Break
%token Y_New
%token Y_NewArray
%token Y_Print
%token Y_ReadInteger
%token Y_ReadLine
%token Y_BoolConstant
%token Y_StringConstant
%token Y_IntConstant
%token Y_DoubleConstant
%token Y_Identifier
%token Y_TypeIdentifier
%token Y_LessEqual
%token Y_GreaterEqual
%token Y_Equal
%token Y_NotEqual
%token Y_And
%token Y_Or

%{
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "tokentype.h"
#include <stack>
#include "parsetree.h"
  /* we are building parse trees */
#define YYSTYPE ParseTree *

using namespace std;
 extern ParseTree *top;
 extern stack<Token *> opstack;
 extern Token *myTok;
 extern int yylineno;
 extern char* yytext;
 int yylex();
 Token * temp;



 ParseTree *makeExprTree(ParseTree *p1, ParseTree *p2){
    ParseTree *r = new ParseTree("Binop");
    r->addChild(p1);
    r->addChild(new ParseTree(opstack.top()));
    opstack.pop();
    r->addChild(p2);
    return r;

}
 %}
%right '='
%left Y_Or
%left Y_And
%nonassoc Y_Equal Y_NotEqual
%nonassoc '<' Y_LessEqual '>' Y_GreaterEqual
%left '+' '-'
%left '*' '/' '%'
%right '!'
%left '.' '['

%%


Program:
        Decl
                {top = $$ = new ParseTree("Program");
                $$->addChild($1);}

|       Program Decl
                {$1->addChild($2);
                top = $$ = $1;}


Decl:
        VariableDecl
                {$$ = $1;}
|       FunctionDecl
                {$$ = $1;}
|       ClassDecl
                {$$ = $1;}
|       InterfaceDecl
                {$$ = $1;}                


FunctionDecl:
        Type Ident '(' Formals ')' StmtBlock
                {$$ = new ParseTree("FunctionDecl");
                $$->addChild($1);
                $$->addChild($2);
                $$->addChild($4);
                $$->addChild($6);}

|        Y_Void Ident '(' Formals ')' StmtBlock
                {$$ = new ParseTree("FunctionDecl");
                $$->addChild(NULL);
                $$->addChild($2);
                $$->addChild($4);
                $$->addChild($6);}

ClassDecl:
        Y_Class Ident Extends Implements '{' nFields '}'
                {$$ = new ParseTree("ClassDecl");
                $$->addChild($2);
                $$->addChild($3);
                $$->addChild($4);
                $$->addChild($6);}

Extends:
        Y_Extends Ident
                {$$ = $2;}
|      
                {$$ = NULL;}

Implements: 

        Y_Implements Idents
                {$$ = $2;}
|      
                {$$ = NULL;}

Idents:
        Ident
                {$$ = new ParseTree("Implements");
                $$->addChild($1);}

|       Idents ',' Ident
                {$1->addChild($3);
                $$ = $1;}

Field:
        VariableDecl
                {$$ = $1;}
|       FunctionDecl
                {$$ = $1;} 

nFields:
                {$$ = NULL;}
|       Fields
                {$$ = $1;}

Fields:
        Field
                {$$ = new ParseTree("Fields");
                $$->addChild($1);}

|       Fields  Field
                {$1->addChild($2);
                $$ = $1;}

InterfaceDecl:
        Y_Interface Ident '{' nPrototypes '}'
                {$$ = new ParseTree("InterfaceDecl");
                $$->addChild($2);
                $$->addChild($4);}
nPrototypes:
                {$$ = NULL;}
|       Prototypes
                {$$ = $1;}

Prototypes:
        Prototype
                {$$ = new ParseTree("Prototypes");
                $$->addChild($1);}

|       Prototypes Prototype
                {$1->addChild($2);
                $$ = $1;}

Prototype:
        Type Ident '(' Formals ')' ';'
                {$$ = new ParseTree("Prototype");
                $$->addChild($1);
                $$->addChild($2);
                $$->addChild($4);}
|       Y_Void Ident '(' Formals ')' ';'
                {$$ = new ParseTree("Prototype");
                $$->addChild(NULL);
                $$->addChild($2);
                $$->addChild($4);}            


Stmt:
        MatchedStmt
                {$$ = $1;}
|       OpenStmt
                {$$ = $1;}


StmtBlock:
        '{' nVariableDecls nStmts '}'
                {$$ = new ParseTree("StmtBlock");
                $$->addChild($2);
                $$->addChild($3);}



nVariableDecls:
                {$$ = NULL;}
|       VariableDecls
                {$$ = $1;}

Formals:
                {$$ = NULL;}
|       Variables
                {$$ = $1;}

nStmts:
                {$$ = NULL;}
|       Stmts
                {$$ = $1;}


Variable:
        Type Ident
                {$$ = new ParseTree("Variable");
                $$->addChild($1);
                $$->addChild($2);}


Variables:    
        Variable
               {$$ = new ParseTree("Variables");
                $$->addChild($1);}

|       Variables ',' Variable
                {$1->addChild($3);
                $$ = $1;}
 
VariableDecl:
        Variable ';'
                {$$ = new ParseTree("VariableDecl");
                $$->addChild($1);}

                             
 
VariableDecls:

        
        VariableDecl
               {$$ = new ParseTree("VariableDecls");
                $$->addChild($1);}

|       VariableDecls VariableDecl
                {$1->addChild($2);
                $$ = $1;}


Stmts:
        Stmt
                {$$ = new ParseTree("Stmts");
                $$->addChild($1);}

|       Stmts Stmt
                {$1->addChild($2);
                $$ = $1;}





OpenStmt:
        open_if
                {$$ = $1;}
|       open_while
                {$$ = $1;}           
|       open_for
                {$$ = $1;}


MatchedStmt:
        OtherStmt
                {$$ = $1;}
|       matched_if
                {$$ = $1;}
|       matched_while
                {$$ = $1;}           
|       matched_for
                {$$ = $1;}


OtherStmt:

        PrintStmt
                {$$ = $1;}
|       nexpr ';'
                {$$ = $1;}
|       ReturnStmt
                {$$ = $1;}
|       BreakStmt
                {$$ = $1;}              
|       StmtBlock
                {$$ = $1;}


open_if:
        common_if Stmt
                {$1->addChild($2);}
|       common_if MatchedStmt Y_Else OpenStmt
                {$1->addChild($2);
                $1->addChild($4);}

open_while:
        common_while OpenStmt
                {$1->addChild($2);}


open_for:
        common_for OpenStmt
                {$1->addChild($2);}


matched_if:
        common_if MatchedStmt Y_Else MatchedStmt
                {$1->addChild($2);
                $1->addChild($4);}


matched_while:
        common_while MatchedStmt
                {$1->addChild($2);}


matched_for:
        common_for MatchedStmt
                {$1->addChild($2);}


common_if:
        Y_If '(' expr ')'
                {$$ = new ParseTree("If");
                $$->addChild($3);}


common_for:
        Y_For '(' expr ';' expr ';' expr ')'
                {$$ = new ParseTree("For");
                $$->addChild($3);
                $$->addChild($5);
                $$->addChild($7);}
|       Y_For '(' ';' expr ';' ')'
                {$$ = new ParseTree("For");
                $$->addChild($4);}


common_while:
        Y_While '(' expr ')'
                {$$ = new ParseTree("While");
                $$->addChild($3);}



PrintStmt:
        Y_Print '(' Actuals ')' ';'
                {$$ = new ParseTree("PrintStmt");
                $$->addChild($3);}


expr:
    
        Constant
                {$$ = $1;}

|       expr '+' {opstack.push(myTok); } expr
                {$$ = makeExprTree($1, $4);}
    
|       expr '-' {opstack.push(myTok); } expr
                {$$ = makeExprTree($1, $4);}
    
|       expr '/' {opstack.push(myTok); } expr
                {$$ = makeExprTree($1, $4);}
    
|       expr '*' {opstack.push(myTok); } expr
                {$$ = makeExprTree($1, $4);}
|       expr '%' {opstack.push(myTok); } expr
                {$$ = makeExprTree($1, $4);}        
|       expr '<' {opstack.push(myTok); } expr
                {$$ = makeExprTree($1, $4);}
|       expr '>' {opstack.push(myTok); } expr
                {$$ = makeExprTree($1, $4);}
|       expr Y_LessEqual {opstack.push(myTok); } expr
                {$$ = makeExprTree($1, $4);}
|       expr Y_GreaterEqual {opstack.push(myTok); } expr
                {$$ = makeExprTree($1, $4);}

|       expr Y_NotEqual {opstack.push(myTok); } expr
                {$$ = makeExprTree($1, $4);}
|       expr Y_Equal {opstack.push(myTok); } expr
                {$$ = makeExprTree($1, $4);}
|       expr Y_And {opstack.push(myTok); } expr
                {$$ = makeExprTree($1, $4);}
|       expr Y_Or {opstack.push(myTok); } expr
                {$$ = makeExprTree($1, $4);}
|       Y_This
                {$$ = new ParseTree("this");}    

|       '!' expr
                {$$ = new ParseTree("Not");
                $$->addChild($2);}

|       '-' expr
                {$$ = new ParseTree("Negative");
                $$->addChild($2);}

|       Y_New '(' Ident ')'
                {$$ = new ParseTree("New");
                 $$->addChild($3);}
|       Y_NewArray '(' expr ',' newArrayType
                {$$ = new ParseTree("NewArray");
                 $$->addChild($3);
                 $$->addChild($5);}                 
|       Y_ReadInteger '(' ')'
                {$$ = new ParseTree("ReadInteger");}    
|       Y_ReadLine '(' ')'
                {$$ = new ParseTree("ReadLine");}                                   
|       '(' expr ')'
                {$$ = $2;}

|       LValue '=' {opstack.push(myTok); } expr
                {$$ = makeExprTree($1, $4);}

|       LValue
                {$$ = $1;}

|       Call
                {$$ = $1;}      

newArrayType:
        Type ')'
                {$$ = $1;}
|       Ident ')'
                {$$ = new ParseTree("Type");
                $$->addChild($1);}

Constant:

        Y_IntConstant
                {$$ = new ParseTree(myTok);}
|       Y_DoubleConstant
                {$$ = new ParseTree(myTok);}
|       Y_BoolConstant
                {$$ = new ParseTree(myTok);}
|       Y_StringConstant
                {$$ = new ParseTree(myTok);}
|       Y_Null
                {$$ = new ParseTree(myTok);}                                

 
 
Type:

        Y_Int
                {$$ = new ParseTree("Type_Int");}
|       Y_Double
                {$$ = new ParseTree("Type_Double");}
|       Y_Bool
                {$$ = new ParseTree("Type_Bool");}
|       Y_String
                {$$ = new ParseTree("Type_String");}
|       TIdent
                {$$ = new ParseTree("Type_Identifier");
                 $$->addChild($1);} 

|       Type '[' ']'
                {$$ = new ParseTree("Type_Array"); 
                $$->addChild($1);}              


LValue:
        Ident
                {$$ = new ParseTree("LValue");
                $$->addChild(NULL);
                $$->addChild($1);}
   
|       expr '.' Ident
                {$$ = new ParseTree("Dot");
                cout << "TESTING" << endl;
                $$->addChild($1);
                $$->addChild($3);}
 
|       expr '[' expr ']'
                {$$ = new ParseTree("Bracket");
                $$->addChild($1);
                $$->addChild($3);}
 


Ident:
        Y_Identifier
                {$$ = new ParseTree(myTok);}


TIdent:
        Y_TypeIdentifier
                {$$ = new ParseTree(myTok);}



Actuals:
        expr {$$ = new ParseTree("Actuals");
              $$->addChild($1);}
|
        Actuals ',' expr
                {$1->addChild($3);
                $$ = $1;}

 

nexpr:
                {$$ = NULL;}
|       expr
                {$$ = $1;}

 
ReturnStmt:
        Y_Return nexpr ';'
                {$$ = new ParseTree("return");
                if($2)
                    $$->addChild($2);}


BreakStmt:
        Y_Break ';'
                {$$ = new ParseTree("Break");}



Call:
        Ident '(' Actuals ')'
                {$$ = new ParseTree("Call");
                $$->addChild(NULL);
                $$->addChild($1);
                $$->addChild($3);}

|       Ident '(' ')'
                {$$ = new ParseTree("Call");
                $$->addChild(NULL);
                $$->addChild($1);
                $$->addChild(NULL);}

|       expr '.' Ident '(' Actuals ')'
                {$$ = new ParseTree("Call");
                $$->addChild($1);
                $$->addChild($3);
                $$->addChild($5);}    

|       expr '.' Ident '(' ')'
                {$$ = new ParseTree("Call");
                $$->addChild($1);
                $$->addChild($3);
                $$->addChild(NULL);}     



%%

int yyerror(const char * s)
{
  //fprintf(stderr, "%s\n", s);
  fprintf(stderr, "line %d: %s\nToken: %s\n", yylineno, s, yytext); 
  return 0;
}
int mainold() {

  yyparse();
  traverseTree(top, 0, 0);
  return 0;
}
