#ifndef lint
static const char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif

#include <stdlib.h>
#include <string.h>

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20070509

#define YYEMPTY (-1)
#define yyclearin    (yychar = YYEMPTY)
#define yyerrok      (yyerrflag = 0)
#define YYRECOVERING (yyerrflag != 0)

extern int yyparse(void);

static int yygrowstack(void);
#define YYPREFIX "yy"
#line 42 "decaf.y"
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
 
#line 55 "y.tab.c"
#define Y_Void 257
#define Y_Int 258
#define Y_Double 259
#define Y_Bool 260
#define Y_String 261
#define Y_Class 262
#define Y_Interface 263
#define Y_Null 264
#define Y_This 265
#define Y_Extends 266
#define Y_Implements 267
#define Y_For 268
#define Y_While 269
#define Y_If 270
#define Y_Else 271
#define Y_Return 272
#define Y_Break 273
#define Y_New 274
#define Y_NewArray 275
#define Y_Print 276
#define Y_ReadInteger 277
#define Y_ReadLine 278
#define Y_BoolConstant 279
#define Y_StringConstant 280
#define Y_IntConstant 281
#define Y_DoubleConstant 282
#define Y_Identifier 283
#define Y_TypeIdentifier 284
#define Y_LessEqual 285
#define Y_GreaterEqual 286
#define Y_Equal 287
#define Y_NotEqual 288
#define Y_And 289
#define Y_Or 290
#define YYERRCODE 256
short yylhs[] = {                                        -1,
    0,    0,    1,    1,    1,    1,    3,    3,    4,   10,
   10,   11,   11,   13,   13,   14,   14,   12,   12,   15,
   15,    5,   16,   16,   17,   17,   18,   18,   19,   19,
    9,   22,   22,    8,    8,   23,   23,   27,   25,   25,
    2,   24,   24,   26,   26,   21,   21,   21,   20,   20,
   20,   20,   31,   31,   31,   31,   31,   28,   28,   29,
   30,   32,   33,   34,   39,   41,   41,   40,   35,   42,
   45,   42,   46,   42,   47,   42,   48,   42,   49,   42,
   50,   42,   51,   42,   52,   42,   53,   42,   54,   42,
   55,   42,   56,   42,   57,   42,   42,   42,   42,   42,
   42,   42,   42,   42,   60,   42,   42,   42,   58,   58,
   44,   44,   44,   44,   44,    6,    6,    6,    6,    6,
    6,   59,   59,   59,    7,   62,   43,   43,   36,   36,
   37,   38,   61,   61,   61,   61,
};
short yylen[] = {                                         2,
    1,    2,    1,    1,    1,    1,    6,    6,    7,    2,
    0,    2,    0,    1,    3,    1,    1,    0,    1,    1,
    2,    5,    0,    1,    1,    2,    6,    6,    1,    1,
    4,    0,    1,    0,    1,    0,    1,    2,    1,    3,
    2,    1,    2,    1,    2,    1,    1,    1,    1,    1,
    1,    1,    1,    2,    1,    1,    1,    2,    4,    2,
    2,    4,    2,    2,    4,    8,    6,    4,    5,    1,
    0,    4,    0,    4,    0,    4,    0,    4,    0,    4,
    0,    4,    0,    4,    0,    4,    0,    4,    0,    4,
    0,    4,    0,    4,    0,    4,    1,    2,    2,    4,
    5,    3,    3,    3,    0,    4,    1,    1,    2,    2,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    3,    1,    3,    4,    1,    1,    1,    3,    0,    1,
    3,    2,    4,    3,    6,    5,
};
short yydefred[] = {                                      0,
    0,  116,  117,  118,  119,    0,    0,  126,    0,    1,
    3,    4,    5,    6,    0,    0,  120,  125,    0,    0,
    0,    2,    0,    0,   41,    0,    0,    0,    0,  121,
    0,    0,    0,    0,   39,   10,    0,    0,    0,    0,
    0,    0,   25,    0,   38,    0,    0,   14,    0,    0,
    0,    0,   22,   26,    0,    0,    8,   40,    0,   16,
   17,    0,   20,    0,    0,    0,    7,   42,    0,    0,
   15,    9,   21,    0,    0,  115,   97,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  113,  114,  111,
  112,    0,    0,    0,    0,   57,   44,   29,   30,    0,
    0,   46,   47,   48,   49,   50,   51,   52,   53,    0,
   55,   56,    0,    0,    0,    0,   70,    0,  108,   43,
    0,    0,    0,    0,    0,    0,  132,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   31,   45,   54,   58,
    0,   63,   60,   64,   61,   85,   87,   91,   89,   93,
   95,   81,   83,   71,   73,   77,   75,   79,    0,    0,
  105,   28,   27,    0,    0,    0,    0,  131,    0,    0,
    0,    0,  102,  103,  104,  134,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   68,   65,  100,    0,
    0,    0,  133,   62,   59,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  124,
    0,    0,    0,    0,    0,  101,   69,    0,  136,    0,
   67,    0,  109,  110,  135,    0,   66,
};
short yydgoto[] = {                                       9,
   10,   11,   12,   13,   14,   32,   95,   33,   96,   28,
   38,   62,   49,   63,   64,   41,   42,   43,   97,   98,
   99,   69,  100,   70,   34,  101,   16,  102,  103,  104,
  105,  106,  107,  108,  109,  110,  111,  112,  113,  114,
  115,  116,  172,  117,  187,  188,  190,  189,  191,  185,
  186,  179,  180,  182,  181,  183,  184,  226,  118,  194,
  119,   17,
};
short yysindex[] = {                                     46,
 -244,    0,    0,    0,    0, -244, -244,    0,   46,    0,
    0,    0,    0,    0,  -89,   -2,    0,    0,   18, -204,
  -57,    0,  -18,   42,    0, -133, -244, -177,   81,    0,
 -133,  -89,   59,   57,    0,    0, -244,  -21, -244,  -89,
   -6,   81,    0,   70,    0,   13, -133,    0,   79,  139,
  101,  107,    0,    0,   13, -133,    0,    0, -244,    0,
    0,   12,    0,  139, -133, -133,    0,    0,  557, -133,
    0,    0,    0,  111,  113,    0,    0,  108,  122,  134,
  641,  104,  140,  147,  151,  155,  171,    0,    0,    0,
    0,  641,  641,  641,  172,    0,    0,    0,    0,   54,
  557,    0,    0,    0,    0,    0,    0,    0,    0,  154,
    0,    0,  557,  557,  557,  445,    0,  159,    0,    0,
  158,  169,  588,  641,  641,  170,    0, -244,  641,  641,
  174,  192,  228,  -32,   97,  389,    0,    0,    0,    0,
  -31,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, -244,  641,
    0,    0,    0,  641,  123,  130,  156,    0,  198,  163,
  445,   11,    0,    0,    0,    0,   47,  557,  641,  641,
  641,  641,  641,  641,  641,  641,  641,  641,  641,  641,
  641,  201,  236,  641,  330,  641,    0,    0,    0,  -15,
  196,  641,    0,    0,    0,  189,  189,  518,  518,  492,
  452,  189,  189,  228,  228,  -32,  -32,  -32,  610,    0,
  445,  215,  417,  -38,  216,    0,    0,  445,    0,  105,
    0,  641,    0,    0,    0,  424,    0,
};
short yyrindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -122,
    0,    0,    0,  199,    0,  225,    0,  144,  152,    0,
  225,    0,    0,  231,    0,    0,    0,    0,    0,    0,
    0,  160,    0,    0,    0,    0,    0,    0,  161,  168,
    0,    0,    0,    0,    0,  487,    0,    0,    0,    0,
    0,    0,    0,  175,  225,  225,    0,    0,  -28,  517,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  235,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  -37,    0,    0,    0,    0,    0,
   -8,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  235,  235,  235,  240,    0,    1,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  656,   27,    0,    0,    0,    0,    0,    0,
  537,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  112,    0,    0,    0,    0,    0,    0,  235,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  -26,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   91,  137,  183,  251,  814,
  145,  347,  805,  683,  835,   36,   62,   71,    0,    0,
  178,    0,    0,    0,    0,    0,    0,  117,    0,    0,
    0,    0,    0,    0,    0,    0,    0,
};
short yygindex[] = {                                      0,
  292,  446,  -24,    0,    0,  544,  947,   19,  -14,    0,
    0,    0,    0,  247,    0,    0,    0,  278,  -64,  -85,
 -102,    0,    0,    0,    0,    0,  271,    0,    0,    0,
    0,    0,    0,    0,    0,  250,    0,    0,    0,    0,
    0,  865, -109,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,
};
#define YYTABLESIZE 1147
short yytable[] = {                                     122,
   11,   23,  233,  122,  122,  122,  122,  122,  122,  122,
  123,  143,  145,  159,  123,  123,  123,  123,  123,  123,
  123,  122,  122,  122,  122,   61,  177,  141,  142,  144,
  129,   57,  123,  123,  123,  123,  138,  107,   18,   61,
   67,  107,  107,  107,  107,  107,  107,  107,  140,   44,
  129,  201,   23,  122,  202,  122,   25,   26,  160,  107,
  107,   27,  107,   98,  123,   29,  123,   98,   98,   98,
   98,   98,   78,   98,   30,  205,   78,   78,   78,   78,
   78,   31,   78,   74,   75,   98,   98,  203,   98,   37,
  202,  107,  204,  107,   78,   78,   36,   78,   76,   46,
   47,   50,   76,   76,   76,   76,   76,   80,   76,  230,
   55,   80,   80,   80,   80,   80,   37,   80,   53,   98,
   76,   76,   59,   76,    2,    3,    4,    5,   78,   80,
   80,   86,   80,  158,   86,   56,   72,  175,  156,  154,
   65,  155,  159,  157,   11,  235,   66,  123,  202,   86,
    8,  121,  127,  122,   76,  127,  152,  128,  153,  158,
  128,  124,  127,   80,  156,  154,  158,  155,  159,  157,
  197,  156,  154,  125,  155,  159,  157,   88,  137,  128,
   88,  196,  152,   86,  153,   96,  129,  160,   96,  152,
  130,  153,  158,   18,  131,   88,  198,  156,  154,  158,
  155,  159,  157,   96,  156,  154,  200,  155,  159,  157,
  132,  136,  139,  160,  173,  152,  162,  153,  106,  161,
  160,  106,  152,   92,  153,  158,   92,  163,  168,   88,
  156,  154,  174,  155,  159,  157,  106,   96,  199,  178,
  219,   92,    2,    3,    4,    5,  160,  122,  122,  122,
  122,  122,  122,  160,  227,  231,  234,   38,  123,  123,
  123,  123,  123,  123,  158,   34,   13,   18,    8,  156,
  106,   35,  158,  159,  157,   92,   23,  156,  154,  160,
  155,  159,  157,   12,   24,  107,  107,  107,  107,  107,
  107,   90,   18,  129,   90,  152,   35,  153,  130,   19,
   22,   35,    1,    2,    3,    4,    5,    6,    7,   90,
   73,   98,   98,   98,   98,   98,   98,   58,  160,   54,
   78,   78,   78,   78,   78,   78,  160,    0,  220,    8,
  126,    0,    0,    0,    0,   35,   35,   39,    2,    3,
    4,    5,    0,   90,    0,    0,   76,   76,   76,   76,
   76,   76,    0,    0,    0,   80,   80,   80,   80,   80,
   80,    0,    0,    0,    8,    0,  158,    0,    0,    0,
    0,  156,  154,    0,  155,  159,  157,   86,   86,   86,
   86,  146,  147,  148,  149,  150,  151,   82,  222,  152,
   82,  153,    0,    0,    0,    1,    2,    3,    4,    5,
    0,    0,    0,    0,    0,   82,    0,  146,  147,  148,
  149,  150,  151,    0,  146,  147,  148,  149,  150,  151,
  160,   93,    8,   88,   88,   88,   88,    0,   94,  176,
    0,    0,    0,   92,   96,    0,    0,    0,    0,   82,
  146,  147,  148,  149,  150,  151,    0,  146,  147,  148,
  149,  150,  151,  158,    0,    0,    0,    0,  156,  154,
  158,  155,  159,  157,  237,  156,  154,    0,  155,  159,
  157,   92,   92,    0,    0,  232,  152,    0,  153,    0,
    0,  158,    0,  152,    0,  153,  156,  154,  158,  155,
  159,  157,    0,  156,  154,   60,  155,  159,  157,    0,
    0,   68,    0,    0,  152,    0,  153,  160,    0,   60,
    0,  152,    0,  153,  160,  120,    0,    0,    0,   32,
  146,  147,  148,  149,  150,  151,   32,    0,  158,    0,
    0,   32,    0,  156,  154,  160,  155,  159,  157,   90,
   90,    0,  160,   15,    0,   32,    0,    0,    0,   33,
    0,  152,   15,  153,  158,    0,   33,    0,    0,  156,
  154,   33,  155,  159,  157,    0,    0,    0,    0,   29,
    0,    0,   40,    0,    0,   33,   29,  152,    0,  153,
    0,   29,  160,    0,    0,   40,    0,    0,    0,   93,
    0,    0,    0,   15,    0,   29,   94,    0,    0,    0,
    0,   92,    0,    0,    0,    0,    0,   15,  160,   32,
    0,   32,    0,    0,  146,  147,  148,  149,  150,  151,
   93,    0,    0,    0,    0,    0,    0,   94,    0,    0,
    0,    0,   92,   82,   82,   82,   82,    0,    0,   33,
    0,   33,   93,    0,    0,    0,  164,    0,    0,   94,
  229,    0,   76,   77,   92,    0,    0,    0,    0,   29,
    0,   29,   83,   84,    0,   86,   87,   88,   89,   90,
   91,   18,    0,   93,    0,    0,    0,    0,    0,   56,
   94,    0,    0,    0,    0,   92,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   99,    0,   99,   99,
   99,  146,  147,  148,  149,  150,  151,    0,  146,  147,
  148,  149,  150,  151,   99,   99,    0,   99,    0,    0,
    0,    0,    0,   72,    0,   72,   72,   72,    0,  146,
  147,  148,  149,  150,  151,    0,  146,  147,  148,  149,
  150,   72,   72,  224,   72,    0,    0,    0,   99,    0,
   32,   32,    0,    0,   32,   32,   32,    0,   32,   32,
   32,   32,   32,   32,   32,   32,   32,   32,   32,   32,
    0,    0,    0,    0,    0,   72,  146,  147,  148,  149,
   33,   33,    0,    0,   33,   33,   33,    0,   33,   33,
   33,   33,   33,   33,   33,   33,   33,   33,   33,   33,
   29,   29,  146,  147,   29,   29,   29,    0,   29,   29,
   29,   29,   29,   29,   29,   29,   29,   29,   29,   29,
   76,   77,    0,    0,   78,   79,   80,    0,   81,   82,
   83,   84,   85,   86,   87,   88,   89,   90,   91,   18,
    0,    0,    0,    0,    0,   84,    0,    0,   84,    0,
    0,   76,   77,    0,   94,    0,    0,   94,    0,    0,
    0,   83,   84,   84,   86,   87,   88,   89,   90,   91,
   18,    0,   94,   76,   77,   74,    0,   74,   74,   74,
    0,    0,    0,   83,   84,    0,   86,   87,   88,   89,
   90,   91,   18,   74,   74,    0,   74,   84,    0,    0,
    0,    0,    0,    0,   76,   77,   94,    0,    0,    0,
    0,    0,    0,    0,   83,   84,    0,   86,   87,   88,
   89,   90,   91,   18,    0,    0,    0,   74,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   99,   99,   99,   99,   99,   99,    0,   19,    0,    0,
    0,    0,   20,   21,    0,    0,  133,  134,  135,    0,
    0,   24,    0,    0,    0,    0,    0,   72,   72,   72,
   72,   72,   72,   36,    0,    0,    0,    0,   45,    0,
    0,    0,    0,   48,    0,   51,   52,  165,  166,  167,
    0,    0,    0,  170,  171,    0,    0,    0,    0,    0,
  171,    0,    0,    0,    0,   71,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  193,    0,    0,    0,  195,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  206,  207,  208,  209,  210,  211,  212,
  213,  214,  215,  216,  217,  218,    0,    0,  221,    0,
  223,    0,    0,    0,    0,    0,  228,    0,    0,    0,
    0,    0,    0,    0,  169,    0,    0,    0,    0,    0,
    0,    0,    0,  171,    0,    0,    0,    0,    0,    0,
    0,   84,   84,   84,   84,    0,  236,    0,    0,    0,
    0,    0,   94,   94,    0,  192,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   74,
   74,   74,   74,   74,   74,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  225,
};
short yycheck[] = {                                      37,
  123,   91,   41,   41,   42,   43,   44,   45,   46,   47,
   37,  114,  115,   46,   41,   42,   43,   44,   45,   46,
   47,   59,   60,   61,   62,   50,  136,  113,  114,  115,
   59,   46,   59,   60,   61,   62,  101,   37,  283,   64,
   55,   41,   42,   43,   44,   45,   46,   47,  113,   31,
   59,   41,   91,   91,   44,   93,   59,   40,   91,   59,
   60,  266,   62,   37,   91,  123,   93,   41,   42,   43,
   44,   45,   37,   47,   93,  178,   41,   42,   43,   44,
   45,   40,   47,   65,   66,   59,   60,   41,   62,  267,
   44,   91,  178,   93,   59,   60,  125,   62,   37,   41,
   44,  123,   41,   42,   43,   44,   45,   37,   47,  219,
   41,   41,   42,   43,   44,   45,  125,   47,  125,   93,
   59,   60,   44,   62,  258,  259,  260,  261,   93,   59,
   60,   41,   62,   37,   44,  123,  125,   41,   42,   43,
   40,   45,   46,   47,  267,   41,   40,   40,   44,   59,
  284,   41,   41,   41,   93,   44,   60,   41,   62,   37,
   44,   40,   59,   93,   42,   43,   37,   45,   46,   47,
   41,   42,   43,   40,   45,   46,   47,   41,  125,   40,
   44,   59,   60,   93,   62,   41,   40,   91,   44,   60,
   40,   62,   37,  283,   40,   59,   41,   42,   43,   37,
   45,   46,   47,   59,   42,   43,   44,   45,   46,   47,
   40,   40,   59,   91,   41,   60,   59,   62,   41,   61,
   91,   44,   60,   41,   62,   37,   44,   59,   59,   93,
   42,   43,   41,   45,   46,   47,   59,   93,   41,  271,
   40,   59,  258,  259,  260,  261,   91,  285,  286,  287,
  288,  289,  290,   91,   59,   41,   41,   59,  285,  286,
  287,  288,  289,  290,   37,   41,  123,  283,  284,   42,
   93,   41,   37,   46,   47,   93,  125,   42,   43,   91,
   45,   46,   47,  123,  125,  285,  286,  287,  288,  289,
  290,   41,  125,   59,   44,   60,   26,   62,   59,  125,
    9,   31,  257,  258,  259,  260,  261,  262,  263,   59,
   64,  285,  286,  287,  288,  289,  290,   47,   91,   42,
  285,  286,  287,  288,  289,  290,   91,   -1,   93,  284,
   81,   -1,   -1,   -1,   -1,   65,   66,  257,  258,  259,
  260,  261,   -1,   93,   -1,   -1,  285,  286,  287,  288,
  289,  290,   -1,   -1,   -1,  285,  286,  287,  288,  289,
  290,   -1,   -1,   -1,  284,   -1,   37,   -1,   -1,   -1,
   -1,   42,   43,   -1,   45,   46,   47,  287,  288,  289,
  290,  285,  286,  287,  288,  289,  290,   41,   59,   60,
   44,   62,   -1,   -1,   -1,  257,  258,  259,  260,  261,
   -1,   -1,   -1,   -1,   -1,   59,   -1,  285,  286,  287,
  288,  289,  290,   -1,  285,  286,  287,  288,  289,  290,
   91,   33,  284,  287,  288,  289,  290,   -1,   40,   41,
   -1,   -1,   -1,   45,  290,   -1,   -1,   -1,   -1,   93,
  285,  286,  287,  288,  289,  290,   -1,  285,  286,  287,
  288,  289,  290,   37,   -1,   -1,   -1,   -1,   42,   43,
   37,   45,   46,   47,   41,   42,   43,   -1,   45,   46,
   47,  289,  290,   -1,   -1,   59,   60,   -1,   62,   -1,
   -1,   37,   -1,   60,   -1,   62,   42,   43,   37,   45,
   46,   47,   -1,   42,   43,   50,   45,   46,   47,   -1,
   -1,   56,   -1,   -1,   60,   -1,   62,   91,   -1,   64,
   -1,   60,   -1,   62,   91,   70,   -1,   -1,   -1,   33,
  285,  286,  287,  288,  289,  290,   40,   -1,   37,   -1,
   -1,   45,   -1,   42,   43,   91,   45,   46,   47,  289,
  290,   -1,   91,    0,   -1,   59,   -1,   -1,   -1,   33,
   -1,   60,    9,   62,   37,   -1,   40,   -1,   -1,   42,
   43,   45,   45,   46,   47,   -1,   -1,   -1,   -1,   33,
   -1,   -1,   29,   -1,   -1,   59,   40,   60,   -1,   62,
   -1,   45,   91,   -1,   -1,   42,   -1,   -1,   -1,   33,
   -1,   -1,   -1,   50,   -1,   59,   40,   -1,   -1,   -1,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   64,   91,  123,
   -1,  125,   -1,   -1,  285,  286,  287,  288,  289,  290,
   33,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,
   -1,   -1,   45,  287,  288,  289,  290,   -1,   -1,  123,
   -1,  125,   33,   -1,   -1,   -1,   59,   -1,   -1,   40,
   41,   -1,  264,  265,   45,   -1,   -1,   -1,   -1,  123,
   -1,  125,  274,  275,   -1,  277,  278,  279,  280,  281,
  282,  283,   -1,   33,   -1,   -1,   -1,   -1,   -1,  123,
   40,   -1,   -1,   -1,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,   43,   44,
   45,  285,  286,  287,  288,  289,  290,   -1,  285,  286,
  287,  288,  289,  290,   59,   60,   -1,   62,   -1,   -1,
   -1,   -1,   -1,   41,   -1,   43,   44,   45,   -1,  285,
  286,  287,  288,  289,  290,   -1,  285,  286,  287,  288,
  289,   59,   60,  200,   62,   -1,   -1,   -1,   93,   -1,
  264,  265,   -1,   -1,  268,  269,  270,   -1,  272,  273,
  274,  275,  276,  277,  278,  279,  280,  281,  282,  283,
   -1,   -1,   -1,   -1,   -1,   93,  285,  286,  287,  288,
  264,  265,   -1,   -1,  268,  269,  270,   -1,  272,  273,
  274,  275,  276,  277,  278,  279,  280,  281,  282,  283,
  264,  265,  285,  286,  268,  269,  270,   -1,  272,  273,
  274,  275,  276,  277,  278,  279,  280,  281,  282,  283,
  264,  265,   -1,   -1,  268,  269,  270,   -1,  272,  273,
  274,  275,  276,  277,  278,  279,  280,  281,  282,  283,
   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,
   -1,  264,  265,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,  274,  275,   59,  277,  278,  279,  280,  281,  282,
  283,   -1,   59,  264,  265,   41,   -1,   43,   44,   45,
   -1,   -1,   -1,  274,  275,   -1,  277,  278,  279,  280,
  281,  282,  283,   59,   60,   -1,   62,   93,   -1,   -1,
   -1,   -1,   -1,   -1,  264,  265,   93,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  274,  275,   -1,  277,  278,  279,
  280,  281,  282,  283,   -1,   -1,   -1,   93,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  285,  286,  287,  288,  289,  290,   -1,    1,   -1,   -1,
   -1,   -1,    6,    7,   -1,   -1,   92,   93,   94,   -1,
   -1,   15,   -1,   -1,   -1,   -1,   -1,  285,  286,  287,
  288,  289,  290,   27,   -1,   -1,   -1,   -1,   32,   -1,
   -1,   -1,   -1,   37,   -1,   39,   40,  123,  124,  125,
   -1,   -1,   -1,  129,  130,   -1,   -1,   -1,   -1,   -1,
  136,   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  160,   -1,   -1,   -1,  164,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  179,  180,  181,  182,  183,  184,  185,
  186,  187,  188,  189,  190,  191,   -1,   -1,  194,   -1,
  196,   -1,   -1,   -1,   -1,   -1,  202,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  128,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  219,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  287,  288,  289,  290,   -1,  232,   -1,   -1,   -1,
   -1,   -1,  289,  290,   -1,  159,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  285,
  286,  287,  288,  289,  290,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  200,
};
#define YYFINAL 9
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 290
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"'!'",0,0,0,"'%'",0,0,"'('","')'","'*'","'+'","','","'-'","'.'","'/'",0,0,0,0,0,
0,0,0,0,0,0,"';'","'<'","'='","'>'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,"'['",0,"']'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,"'{'",0,"'}'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"Y_Void","Y_Int","Y_Double","Y_Bool",
"Y_String","Y_Class","Y_Interface","Y_Null","Y_This","Y_Extends","Y_Implements",
"Y_For","Y_While","Y_If","Y_Else","Y_Return","Y_Break","Y_New","Y_NewArray",
"Y_Print","Y_ReadInteger","Y_ReadLine","Y_BoolConstant","Y_StringConstant",
"Y_IntConstant","Y_DoubleConstant","Y_Identifier","Y_TypeIdentifier",
"Y_LessEqual","Y_GreaterEqual","Y_Equal","Y_NotEqual","Y_And","Y_Or",
};
char *yyrule[] = {
"$accept : Program",
"Program : Decl",
"Program : Program Decl",
"Decl : VariableDecl",
"Decl : FunctionDecl",
"Decl : ClassDecl",
"Decl : InterfaceDecl",
"FunctionDecl : Type Ident '(' Formals ')' StmtBlock",
"FunctionDecl : Y_Void Ident '(' Formals ')' StmtBlock",
"ClassDecl : Y_Class Ident Extends Implements '{' nFields '}'",
"Extends : Y_Extends Ident",
"Extends :",
"Implements : Y_Implements Idents",
"Implements :",
"Idents : Ident",
"Idents : Idents ',' Ident",
"Field : VariableDecl",
"Field : FunctionDecl",
"nFields :",
"nFields : Fields",
"Fields : Field",
"Fields : Fields Field",
"InterfaceDecl : Y_Interface Ident '{' nPrototypes '}'",
"nPrototypes :",
"nPrototypes : Prototypes",
"Prototypes : Prototype",
"Prototypes : Prototypes Prototype",
"Prototype : Type Ident '(' Formals ')' ';'",
"Prototype : Y_Void Ident '(' Formals ')' ';'",
"Stmt : MatchedStmt",
"Stmt : OpenStmt",
"StmtBlock : '{' nVariableDecls nStmts '}'",
"nVariableDecls :",
"nVariableDecls : VariableDecls",
"Formals :",
"Formals : Variables",
"nStmts :",
"nStmts : Stmts",
"Variable : Type Ident",
"Variables : Variable",
"Variables : Variables ',' Variable",
"VariableDecl : Variable ';'",
"VariableDecls : VariableDecl",
"VariableDecls : VariableDecls VariableDecl",
"Stmts : Stmt",
"Stmts : Stmts Stmt",
"OpenStmt : open_if",
"OpenStmt : open_while",
"OpenStmt : open_for",
"MatchedStmt : OtherStmt",
"MatchedStmt : matched_if",
"MatchedStmt : matched_while",
"MatchedStmt : matched_for",
"OtherStmt : PrintStmt",
"OtherStmt : nexpr ';'",
"OtherStmt : ReturnStmt",
"OtherStmt : BreakStmt",
"OtherStmt : StmtBlock",
"open_if : common_if Stmt",
"open_if : common_if MatchedStmt Y_Else OpenStmt",
"open_while : common_while OpenStmt",
"open_for : common_for OpenStmt",
"matched_if : common_if MatchedStmt Y_Else MatchedStmt",
"matched_while : common_while MatchedStmt",
"matched_for : common_for MatchedStmt",
"common_if : Y_If '(' expr ')'",
"common_for : Y_For '(' expr ';' expr ';' expr ')'",
"common_for : Y_For '(' ';' expr ';' ')'",
"common_while : Y_While '(' expr ')'",
"PrintStmt : Y_Print '(' Actuals ')' ';'",
"expr : Constant",
"$$1 :",
"expr : expr '+' $$1 expr",
"$$2 :",
"expr : expr '-' $$2 expr",
"$$3 :",
"expr : expr '/' $$3 expr",
"$$4 :",
"expr : expr '*' $$4 expr",
"$$5 :",
"expr : expr '%' $$5 expr",
"$$6 :",
"expr : expr '<' $$6 expr",
"$$7 :",
"expr : expr '>' $$7 expr",
"$$8 :",
"expr : expr Y_LessEqual $$8 expr",
"$$9 :",
"expr : expr Y_GreaterEqual $$9 expr",
"$$10 :",
"expr : expr Y_NotEqual $$10 expr",
"$$11 :",
"expr : expr Y_Equal $$11 expr",
"$$12 :",
"expr : expr Y_And $$12 expr",
"$$13 :",
"expr : expr Y_Or $$13 expr",
"expr : Y_This",
"expr : '!' expr",
"expr : '-' expr",
"expr : Y_New '(' Ident ')'",
"expr : Y_NewArray '(' expr ',' newArrayType",
"expr : Y_ReadInteger '(' ')'",
"expr : Y_ReadLine '(' ')'",
"expr : '(' expr ')'",
"$$14 :",
"expr : LValue '=' $$14 expr",
"expr : LValue",
"expr : Call",
"newArrayType : Type ')'",
"newArrayType : Ident ')'",
"Constant : Y_IntConstant",
"Constant : Y_DoubleConstant",
"Constant : Y_BoolConstant",
"Constant : Y_StringConstant",
"Constant : Y_Null",
"Type : Y_Int",
"Type : Y_Double",
"Type : Y_Bool",
"Type : Y_String",
"Type : TIdent",
"Type : Type '[' ']'",
"LValue : Ident",
"LValue : expr '.' Ident",
"LValue : expr '[' expr ']'",
"Ident : Y_Identifier",
"TIdent : Y_TypeIdentifier",
"Actuals : expr",
"Actuals : Actuals ',' expr",
"nexpr :",
"nexpr : expr",
"ReturnStmt : Y_Return nexpr ';'",
"BreakStmt : Y_Break ';'",
"Call : Ident '(' Actuals ')'",
"Call : Ident '(' ')'",
"Call : expr '.' Ident '(' Actuals ')'",
"Call : expr '.' Ident '(' ')'",
};
#endif
#ifndef YYSTYPE
typedef int YYSTYPE;
#endif
#if YYDEBUG
#include <stdio.h>
#endif

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#define YYINITSTACKSIZE 500

int      yydebug;
int      yynerrs;
int      yyerrflag;
int      yychar;
short   *yyssp;
YYSTYPE *yyvsp;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* variables for the parser stack */
static short   *yyss;
static short   *yysslim;
static YYSTYPE *yyvs;
static int      yystacksize;
#line 577 "decaf.y"

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
#line 665 "y.tab.c"
/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(void)
{
    int newsize, i;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = yystacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = yyssp - yyss;
    newss = (yyss != 0)
          ? (short *)realloc(yyss, newsize * sizeof(*newss))
          : (short *)malloc(newsize * sizeof(*newss));
    if (newss == 0)
        return -1;

    yyss  = newss;
    yyssp = newss + i;
    newvs = (yyvs != 0)
          ? (YYSTYPE *)realloc(yyvs, newsize * sizeof(*newvs))
          : (YYSTYPE *)malloc(newsize * sizeof(*newvs));
    if (newvs == 0)
        return -1;

    yyvs = newvs;
    yyvsp = newvs + i;
    yystacksize = newsize;
    yysslim = yyss + newsize - 1;
    return 0;
}

#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
yyparse(void)
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;

    if (yyss == NULL && yygrowstack()) goto yyoverflow;
    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yysslim && yygrowstack())
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;

    yyerror("syntax error");

#ifdef lint
    goto yyerrlab;
#endif

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yysslim && yygrowstack())
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yyvsp[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 1:
#line 88 "decaf.y"
{top = yyval = new ParseTree("Program");
                yyval->addChild(yyvsp[0]);}
break;
case 2:
#line 92 "decaf.y"
{yyvsp[-1]->addChild(yyvsp[0]);
                top = yyval = yyvsp[-1];}
break;
case 3:
#line 98 "decaf.y"
{yyval = yyvsp[0];}
break;
case 4:
#line 100 "decaf.y"
{yyval = yyvsp[0];}
break;
case 5:
#line 102 "decaf.y"
{yyval = yyvsp[0];}
break;
case 6:
#line 104 "decaf.y"
{yyval = yyvsp[0];}
break;
case 7:
#line 109 "decaf.y"
{yyval = new ParseTree("FunctionDecl");
                yyval->addChild(yyvsp[-5]);
                yyval->addChild(yyvsp[-4]);
                yyval->addChild(yyvsp[-2]);
                yyval->addChild(yyvsp[0]);}
break;
case 8:
#line 116 "decaf.y"
{yyval = new ParseTree("FunctionDecl");
                yyval->addChild(NULL);
                yyval->addChild(yyvsp[-4]);
                yyval->addChild(yyvsp[-2]);
                yyval->addChild(yyvsp[0]);}
break;
case 9:
#line 124 "decaf.y"
{yyval = new ParseTree("ClassDecl");
                yyval->addChild(yyvsp[-5]);
                yyval->addChild(yyvsp[-4]);
                yyval->addChild(yyvsp[-3]);
                yyval->addChild(yyvsp[-1]);}
break;
case 10:
#line 132 "decaf.y"
{yyval = yyvsp[0];}
break;
case 11:
#line 134 "decaf.y"
{yyval = NULL;}
break;
case 12:
#line 139 "decaf.y"
{yyval = yyvsp[0];}
break;
case 13:
#line 141 "decaf.y"
{yyval = NULL;}
break;
case 14:
#line 145 "decaf.y"
{yyval = new ParseTree("Implements");
                yyval->addChild(yyvsp[0]);}
break;
case 15:
#line 149 "decaf.y"
{yyvsp[-2]->addChild(yyvsp[0]);
                yyval = yyvsp[-2];}
break;
case 16:
#line 154 "decaf.y"
{yyval = yyvsp[0];}
break;
case 17:
#line 156 "decaf.y"
{yyval = yyvsp[0];}
break;
case 18:
#line 159 "decaf.y"
{yyval = NULL;}
break;
case 19:
#line 161 "decaf.y"
{yyval = yyvsp[0];}
break;
case 20:
#line 165 "decaf.y"
{yyval = new ParseTree("Fields");
                yyval->addChild(yyvsp[0]);}
break;
case 21:
#line 169 "decaf.y"
{yyvsp[-1]->addChild(yyvsp[0]);
                yyval = yyvsp[-1];}
break;
case 22:
#line 174 "decaf.y"
{yyval = new ParseTree("InterfaceDecl");
                yyval->addChild(yyvsp[-3]);
                yyval->addChild(yyvsp[-1]);}
break;
case 23:
#line 178 "decaf.y"
{yyval = NULL;}
break;
case 24:
#line 180 "decaf.y"
{yyval = yyvsp[0];}
break;
case 25:
#line 184 "decaf.y"
{yyval = new ParseTree("Prototypes");
                yyval->addChild(yyvsp[0]);}
break;
case 26:
#line 188 "decaf.y"
{yyvsp[-1]->addChild(yyvsp[0]);
                yyval = yyvsp[-1];}
break;
case 27:
#line 193 "decaf.y"
{yyval = new ParseTree("Prototype");
                yyval->addChild(yyvsp[-5]);
                yyval->addChild(yyvsp[-4]);
                yyval->addChild(yyvsp[-2]);}
break;
case 28:
#line 198 "decaf.y"
{yyval = new ParseTree("Prototype");
                yyval->addChild(NULL);
                yyval->addChild(yyvsp[-4]);
                yyval->addChild(yyvsp[-2]);}
break;
case 29:
#line 206 "decaf.y"
{yyval = yyvsp[0];}
break;
case 30:
#line 208 "decaf.y"
{yyval = yyvsp[0];}
break;
case 31:
#line 213 "decaf.y"
{yyval = new ParseTree("StmtBlock");
                yyval->addChild(yyvsp[-2]);
                yyval->addChild(yyvsp[-1]);}
break;
case 32:
#line 220 "decaf.y"
{yyval = NULL;}
break;
case 33:
#line 222 "decaf.y"
{yyval = yyvsp[0];}
break;
case 34:
#line 225 "decaf.y"
{yyval = NULL;}
break;
case 35:
#line 227 "decaf.y"
{yyval = yyvsp[0];}
break;
case 36:
#line 230 "decaf.y"
{yyval = NULL;}
break;
case 37:
#line 232 "decaf.y"
{yyval = yyvsp[0];}
break;
case 38:
#line 237 "decaf.y"
{yyval = new ParseTree("Variable");
                yyval->addChild(yyvsp[-1]);
                yyval->addChild(yyvsp[0]);}
break;
case 39:
#line 244 "decaf.y"
{yyval = new ParseTree("Variables");
                yyval->addChild(yyvsp[0]);}
break;
case 40:
#line 248 "decaf.y"
{yyvsp[-2]->addChild(yyvsp[0]);
                yyval = yyvsp[-2];}
break;
case 41:
#line 253 "decaf.y"
{yyval = new ParseTree("VariableDecl");
                yyval->addChild(yyvsp[-1]);}
break;
case 42:
#line 262 "decaf.y"
{yyval = new ParseTree("VariableDecls");
                yyval->addChild(yyvsp[0]);}
break;
case 43:
#line 266 "decaf.y"
{yyvsp[-1]->addChild(yyvsp[0]);
                yyval = yyvsp[-1];}
break;
case 44:
#line 272 "decaf.y"
{yyval = new ParseTree("Stmts");
                yyval->addChild(yyvsp[0]);}
break;
case 45:
#line 276 "decaf.y"
{yyvsp[-1]->addChild(yyvsp[0]);
                yyval = yyvsp[-1];}
break;
case 46:
#line 285 "decaf.y"
{yyval = yyvsp[0];}
break;
case 47:
#line 287 "decaf.y"
{yyval = yyvsp[0];}
break;
case 48:
#line 289 "decaf.y"
{yyval = yyvsp[0];}
break;
case 49:
#line 294 "decaf.y"
{yyval = yyvsp[0];}
break;
case 50:
#line 296 "decaf.y"
{yyval = yyvsp[0];}
break;
case 51:
#line 298 "decaf.y"
{yyval = yyvsp[0];}
break;
case 52:
#line 300 "decaf.y"
{yyval = yyvsp[0];}
break;
case 53:
#line 306 "decaf.y"
{yyval = yyvsp[0];}
break;
case 54:
#line 308 "decaf.y"
{yyval = yyvsp[-1];}
break;
case 55:
#line 310 "decaf.y"
{yyval = yyvsp[0];}
break;
case 56:
#line 312 "decaf.y"
{yyval = yyvsp[0];}
break;
case 57:
#line 314 "decaf.y"
{yyval = yyvsp[0];}
break;
case 58:
#line 319 "decaf.y"
{yyvsp[-1]->addChild(yyvsp[0]);}
break;
case 59:
#line 321 "decaf.y"
{yyvsp[-3]->addChild(yyvsp[-2]);
                yyvsp[-3]->addChild(yyvsp[0]);}
break;
case 60:
#line 326 "decaf.y"
{yyvsp[-1]->addChild(yyvsp[0]);}
break;
case 61:
#line 331 "decaf.y"
{yyvsp[-1]->addChild(yyvsp[0]);}
break;
case 62:
#line 336 "decaf.y"
{yyvsp[-3]->addChild(yyvsp[-2]);
                yyvsp[-3]->addChild(yyvsp[0]);}
break;
case 63:
#line 342 "decaf.y"
{yyvsp[-1]->addChild(yyvsp[0]);}
break;
case 64:
#line 347 "decaf.y"
{yyvsp[-1]->addChild(yyvsp[0]);}
break;
case 65:
#line 352 "decaf.y"
{yyval = new ParseTree("If");
                yyval->addChild(yyvsp[-1]);}
break;
case 66:
#line 358 "decaf.y"
{yyval = new ParseTree("For");
                yyval->addChild(yyvsp[-5]);
                yyval->addChild(yyvsp[-3]);
                yyval->addChild(yyvsp[-1]);}
break;
case 67:
#line 363 "decaf.y"
{yyval = new ParseTree("For");
                yyval->addChild(yyvsp[-2]);}
break;
case 68:
#line 369 "decaf.y"
{yyval = new ParseTree("While");
                yyval->addChild(yyvsp[-1]);}
break;
case 69:
#line 376 "decaf.y"
{yyval = new ParseTree("PrintStmt");
                yyval->addChild(yyvsp[-2]);}
break;
case 70:
#line 383 "decaf.y"
{yyval = yyvsp[0];}
break;
case 71:
#line 385 "decaf.y"
{opstack.push(myTok); }
break;
case 72:
#line 386 "decaf.y"
{yyval = makeExprTree(yyvsp[-3], yyvsp[0]);}
break;
case 73:
#line 388 "decaf.y"
{opstack.push(myTok); }
break;
case 74:
#line 389 "decaf.y"
{yyval = makeExprTree(yyvsp[-3], yyvsp[0]);}
break;
case 75:
#line 391 "decaf.y"
{opstack.push(myTok); }
break;
case 76:
#line 392 "decaf.y"
{yyval = makeExprTree(yyvsp[-3], yyvsp[0]);}
break;
case 77:
#line 394 "decaf.y"
{opstack.push(myTok); }
break;
case 78:
#line 395 "decaf.y"
{yyval = makeExprTree(yyvsp[-3], yyvsp[0]);}
break;
case 79:
#line 396 "decaf.y"
{opstack.push(myTok); }
break;
case 80:
#line 397 "decaf.y"
{yyval = makeExprTree(yyvsp[-3], yyvsp[0]);}
break;
case 81:
#line 398 "decaf.y"
{opstack.push(myTok); }
break;
case 82:
#line 399 "decaf.y"
{yyval = makeExprTree(yyvsp[-3], yyvsp[0]);}
break;
case 83:
#line 400 "decaf.y"
{opstack.push(myTok); }
break;
case 84:
#line 401 "decaf.y"
{yyval = makeExprTree(yyvsp[-3], yyvsp[0]);}
break;
case 85:
#line 402 "decaf.y"
{opstack.push(myTok); }
break;
case 86:
#line 403 "decaf.y"
{yyval = makeExprTree(yyvsp[-3], yyvsp[0]);}
break;
case 87:
#line 404 "decaf.y"
{opstack.push(myTok); }
break;
case 88:
#line 405 "decaf.y"
{yyval = makeExprTree(yyvsp[-3], yyvsp[0]);}
break;
case 89:
#line 407 "decaf.y"
{opstack.push(myTok); }
break;
case 90:
#line 408 "decaf.y"
{yyval = makeExprTree(yyvsp[-3], yyvsp[0]);}
break;
case 91:
#line 409 "decaf.y"
{opstack.push(myTok); }
break;
case 92:
#line 410 "decaf.y"
{yyval = makeExprTree(yyvsp[-3], yyvsp[0]);}
break;
case 93:
#line 411 "decaf.y"
{opstack.push(myTok); }
break;
case 94:
#line 412 "decaf.y"
{yyval = makeExprTree(yyvsp[-3], yyvsp[0]);}
break;
case 95:
#line 413 "decaf.y"
{opstack.push(myTok); }
break;
case 96:
#line 414 "decaf.y"
{yyval = makeExprTree(yyvsp[-3], yyvsp[0]);}
break;
case 97:
#line 416 "decaf.y"
{yyval = new ParseTree("this");}
break;
case 98:
#line 419 "decaf.y"
{yyval = new ParseTree("Not");
                yyval->addChild(yyvsp[0]);}
break;
case 99:
#line 423 "decaf.y"
{yyval = new ParseTree("Negative");
                yyval->addChild(yyvsp[0]);}
break;
case 100:
#line 427 "decaf.y"
{yyval = new ParseTree("New");
                 yyval->addChild(yyvsp[-1]);}
break;
case 101:
#line 430 "decaf.y"
{yyval = new ParseTree("NewArray");
                 yyval->addChild(yyvsp[-2]);
                 yyval->addChild(yyvsp[0]);}
break;
case 102:
#line 434 "decaf.y"
{yyval = new ParseTree("ReadInteger");}
break;
case 103:
#line 436 "decaf.y"
{yyval = new ParseTree("ReadLine");}
break;
case 104:
#line 438 "decaf.y"
{yyval = yyvsp[-1];}
break;
case 105:
#line 440 "decaf.y"
{opstack.push(myTok); }
break;
case 106:
#line 441 "decaf.y"
{yyval = makeExprTree(yyvsp[-3], yyvsp[0]);}
break;
case 107:
#line 444 "decaf.y"
{yyval = yyvsp[0];}
break;
case 108:
#line 447 "decaf.y"
{yyval = yyvsp[0];}
break;
case 109:
#line 451 "decaf.y"
{yyval = yyvsp[-1];}
break;
case 110:
#line 453 "decaf.y"
{yyval = new ParseTree("Type");
                yyval->addChild(yyvsp[-1]);}
break;
case 111:
#line 459 "decaf.y"
{yyval = new ParseTree(myTok);}
break;
case 112:
#line 461 "decaf.y"
{yyval = new ParseTree(myTok);}
break;
case 113:
#line 463 "decaf.y"
{yyval = new ParseTree(myTok);}
break;
case 114:
#line 465 "decaf.y"
{yyval = new ParseTree(myTok);}
break;
case 115:
#line 467 "decaf.y"
{yyval = new ParseTree(myTok);}
break;
case 116:
#line 474 "decaf.y"
{yyval = new ParseTree("Type_Int");}
break;
case 117:
#line 476 "decaf.y"
{yyval = new ParseTree("Type_Double");}
break;
case 118:
#line 478 "decaf.y"
{yyval = new ParseTree("Type_Bool");}
break;
case 119:
#line 480 "decaf.y"
{yyval = new ParseTree("Type_String");}
break;
case 120:
#line 482 "decaf.y"
{yyval = new ParseTree("Type_Identifier");
                 yyval->addChild(yyvsp[0]);}
break;
case 121:
#line 486 "decaf.y"
{yyval = new ParseTree("Type_Array"); 
                yyval->addChild(yyvsp[-2]);}
break;
case 122:
#line 492 "decaf.y"
{yyval = new ParseTree("LValue");
                yyval->addChild(NULL);
                yyval->addChild(yyvsp[0]);}
break;
case 123:
#line 497 "decaf.y"
{yyval = new ParseTree("Dot");
                cout << "TESTING" << endl;
                yyval->addChild(yyvsp[-2]);
                yyval->addChild(yyvsp[0]);}
break;
case 124:
#line 503 "decaf.y"
{yyval = new ParseTree("Bracket");
                yyval->addChild(yyvsp[-3]);
                yyval->addChild(yyvsp[-1]);}
break;
case 125:
#line 511 "decaf.y"
{yyval = new ParseTree(myTok);}
break;
case 126:
#line 516 "decaf.y"
{yyval = new ParseTree(myTok);}
break;
case 127:
#line 521 "decaf.y"
{yyval = new ParseTree("Actuals");
              yyval->addChild(yyvsp[0]);}
break;
case 128:
#line 525 "decaf.y"
{yyvsp[-2]->addChild(yyvsp[0]);
                yyval = yyvsp[-2];}
break;
case 129:
#line 531 "decaf.y"
{yyval = NULL;}
break;
case 130:
#line 533 "decaf.y"
{yyval = yyvsp[0];}
break;
case 131:
#line 538 "decaf.y"
{yyval = new ParseTree("return");
                if(yyvsp[-1])
                    yyval->addChild(yyvsp[-1]);}
break;
case 132:
#line 545 "decaf.y"
{yyval = new ParseTree("Break");}
break;
case 133:
#line 551 "decaf.y"
{yyval = new ParseTree("Call");
                yyval->addChild(NULL);
                yyval->addChild(yyvsp[-3]);
                yyval->addChild(yyvsp[-1]);}
break;
case 134:
#line 557 "decaf.y"
{yyval = new ParseTree("Call");
                yyval->addChild(NULL);
                yyval->addChild(yyvsp[-2]);
                yyval->addChild(NULL);}
break;
case 135:
#line 563 "decaf.y"
{yyval = new ParseTree("Call");
                yyval->addChild(yyvsp[-5]);
                yyval->addChild(yyvsp[-3]);
                yyval->addChild(yyvsp[-1]);}
break;
case 136:
#line 569 "decaf.y"
{yyval = new ParseTree("Call");
                yyval->addChild(yyvsp[-4]);
                yyval->addChild(yyvsp[-2]);
                yyval->addChild(NULL);}
break;
#line 1469 "y.tab.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yysslim && yygrowstack())
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;

yyoverflow:
    yyerror("yacc stack overflow");

yyabort:
    return (1);

yyaccept:
    return (0);
}
