/* decafl.l
Jason Wilkin
9/9/13
*/

%option yylineno

hexdigit  [0-9A-Fa-f]
idC [0-9a-zA-Z_]
idsC [a-zA-Z_]
typeident [[:space:]]*\[[[:space:]]*\]|[[:space:]]+{idsC}{idC}*
keyword void|int|double|bool|string|class|interface|null|this|extends|implements|for|while|if|else|return|break|New|NewArray|Print|ReadInteger|ReadLine
%{
#include "tokentype.h"
#include <iostream>
#include <cstdlib>
#include "y.tab.h"

using namespace std;
Token * myTok;

 /* Multi-line comment tracking */
int multiLineStart;
std::string savedStr;
std::string rawStr;

%}
 /* Start states */
%x string
%x multiLineComment

%%
 /* Punctuation */
\+ {myTok = new Token(T_Plus,yytext,yylineno); return '+';}
\- {myTok = new Token(T_Minus,yytext,yylineno); return '-';}
\* {myTok = new Token(T_Times,yytext,yylineno); return '*';}
\/ {myTok = new Token(T_Div,yytext,yylineno); return '/';}
% {myTok = new Token(T_Mod,yytext,yylineno); return '%';}
\< {myTok = new Token(T_Less,yytext,yylineno); return '<';}
\<= {myTok = new Token(T_LessEqual,yytext,yylineno); return Y_LessEqual;}
\> {myTok = new Token(T_Greater,yytext,yylineno); return '>';}
\>= {myTok = new Token(T_GreaterEqual,yytext,yylineno); return Y_GreaterEqual;}
= {myTok = new Token(T_Assign,yytext,yylineno); return '=';}
== {myTok = new Token(T_Equal,yytext,yylineno); return Y_Equal;}
!= {myTok = new Token(T_NotEqual,yytext,yylineno); return Y_NotEqual;}
&& {myTok = new Token(T_And,yytext,yylineno); return Y_And;}
\|\| {myTok = new Token(T_Or,yytext,yylineno); return Y_Or;}
! {myTok = new Token(T_Not,yytext,yylineno); return '!';}
; {myTok = new Token(T_Semicolon,yytext,yylineno); return ';';}
\, {myTok = new Token(T_Comma,yytext,yylineno); return ',';}
\. {myTok = new Token(T_Dot,yytext,yylineno); return '.';}
\[ {myTok = new Token(T_LBracket,yytext,yylineno); return '[';}
\] {myTok = new Token(T_RBracket,yytext,yylineno); return ']';}
\( {myTok = new Token(T_LParen,yytext,yylineno); return '(';}
\) {myTok = new Token(T_RParen,yytext,yylineno); return ')';}
\{ {myTok = new Token(T_LBrace,yytext,yylineno); return '{';}
\} {myTok = new Token(T_RBrace,yytext,yylineno); return '}';}

 /* Keywords */
void/{typeident}? {myTok = new Token(T_Void,yytext,yylineno); return Y_Void;}
int/{typeident}? {myTok = new Token(T_Int,yytext,yylineno); return Y_Int;}
double/{typeident}? {myTok = new Token(T_Double,yytext,yylineno); return Y_Double;}
bool/{typeident}? {myTok = new Token(T_Bool,yytext,yylineno); return Y_Bool;}
string/{typeident}? {myTok = new Token(T_String,yytext,yylineno); return Y_String;}
class/{typeident}? {myTok = new Token(T_Class,yytext,yylineno); return Y_Class;}
interface/{typeident}? {myTok = new Token(T_Interface,yytext,yylineno); return Y_Interface;}
null/{typeident}? {myTok = new Token(T_Null,yytext,yylineno); return Y_Null;}
this/{typeident}? {myTok = new Token(T_This,yytext,yylineno); return Y_This;}
extends/{typeident}? {myTok = new Token(T_Extends,yytext,yylineno); return Y_Extends;}
implements/{typeident}? {myTok = new Token(T_Implements,yytext,yylineno); return Y_Implements;}
for/{typeident}? {myTok = new Token(T_For,yytext,yylineno); return Y_For;}
while/{typeident}? {myTok = new Token(T_While,yytext,yylineno); return Y_While;}
if/{typeident}? {myTok = new Token(T_If,yytext,yylineno); return Y_If;}
else/{typeident}? {myTok = new Token(T_Else,yytext,yylineno); return Y_Else;}
return/{typeident}? {myTok = new Token(T_Return,yytext,yylineno); return Y_Return;}
break/{typeident}? {myTok = new Token(T_Break,yytext,yylineno); return Y_Break;}
New/{typeident}? {myTok = new Token(T_New,yytext,yylineno); return Y_New;}
NewArray/{typeident}? {myTok = new Token(T_NewArray,yytext,yylineno); return Y_NewArray;}
Print/{typeident}? {myTok = new Token(T_Print,yytext,yylineno); return Y_Print;}
ReadInteger/{typeident}? {myTok = new Token(T_ReadInteger,yytext,yylineno); return Y_ReadInteger;}
ReadLine/{typeident}? {myTok = new Token(T_ReadLine,yytext,yylineno); return Y_ReadLine;}

 /* Constants */
true|false {myTok = new Token(T_BoolConstant,yytext,yylineno); return Y_BoolConstant;}
[0-9]+\.[0-9]*([eE][+-]?[0-9]+)? {myTok = new Token(T_DoubleConstant,yytext,yylineno); return Y_DoubleConstant;}
[0-9]+|0[Xx]{hexdigit}+  {myTok = new Token(T_IntConstant,yytext,yylineno); return Y_IntConstant;}         

 /* String */
\"  BEGIN(string);
<string>[^\n\"]* { rawStr = yytext; savedStr = "\"" + rawStr + "\"";}
<string>\" { BEGIN(0); myTok = new Token(T_StringConstant,savedStr,yylineno); return Y_StringConstant;}
<string>\n { cout << "Syntax Error: Missing \" at end of string constant on line " << yylineno-1 << endl; return -1;}
<string><<EOF>> { cout << "Syntax Error: Missing \" at end of string constant on line " << yylineno << endl; return -1;} 


  /* Line comments */
\/\/.*$
  /* Multiline Comments (tracking where they start for errors) */
\/\*  { BEGIN(multiLineComment); multiLineStart = yylineno;}
<multiLineComment>([^*]|\*[^/])*
<multiLineComment>\*\/ BEGIN(0);
<multiLineComment><<EOF>> { cout << "Syntax Error: Missing */ for block comment beginning on line " << multiLineStart << endl; return -1;} 

 /* Identifiers */
{idsC}{idC}{31,} { cout << "Syntax Error: Identifier " << yytext << " too long on line " << yylineno << endl; return -1;}
{idsC}{idC}*/([[:space:]]+{keyword})?   {myTok = new Token(T_Identifier,yytext,yylineno); return Y_Identifier;}
{idsC}{idC}*/{typeident}   {myTok = new Token(T_Identifier,yytext,yylineno); return Y_TypeIdentifier;}


\n
[ ]
\t
<<EOF>>         return -1;
. { cout << "Stray '" << yytext << "' on line " << yylineno << endl; return -1;}
%%



int yywrap(void) {
    return 1;
}

void bogus(int x, char * y) {
     yyunput(0, NULL);
}  



