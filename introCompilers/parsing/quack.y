// Bison quack.y file based on tutorial from http://aquamentus.com/flex_bison.html
%{

  #include <iostream>
  #include <string.h>
  #include "lex.yy.h"
  #include "quack.tab.h"

  int printparse=1;

  void msg (std::string s) {
    if (printparse)
      std::cout << s << std::endl;
  }
  
  /* int yylex(); */
  extern void yyerror(const char *msg);
%}

// Bison by defines a C union holding each of the types of tokens that Flex could return,
// and have Bison use that union instead of "int" for the definition of "yystype":
%union {
  int ival;
  float fval;
  char* strval;
}

// define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the union:

// KEYWORDS
%token CLASS "class"
%token DEF EXTENDS ELIF ELSE WHILE RETURN
%token IF
%token AND OR NOT

%token SEMICOLON ";"
%token COLON ":"
%token LPAREN "("
%token RPAREN ")"
%token DOT "."
%token COMMA ","
%token PLUS "+"
%token MINUS "-"
%token TIMES "*"
%token DIV "/"
%token EQUIV "=="
%token LEQ "<="
%token GEQ ">="
%token LT "<"
%token GT ">" 
%token EQUALS "="
%token RCURLY "{"
%token LCURLY "}"

// PUNCTUATION

// IDENTIFIERS
%token <strval> IDENT
%token ELLIPSIS
// INTEGER AND STRING LITERALS
%token <strval> INT_LIT STRING_LIT
%start Program
%%
// ----GRAMMAR----
Program: | Classes Statements {msg("Program: Class Statement");};

Classes
: /* empty */
| Classes Class {msg("Classes: Classes Class");}
;

Statements
: /* empty */
| Statement Statements {msg("Statements: Statement Statements");}
| Statement  {msg("Statements:  Statement");}
;

Class
: Class_Signature Class_Body {msg("Class: Class_Signature Class_body");}
;

Class_Signature
: "class" IDENT "(" Formal_Args ")" {msg("Class_Signature: class IDENT ( Formal_Args )");}
/*| "class" IDENT "(" Formal_Args ")" EXTENDS IDENT {msg("Class_Signature: class IDENT ( Formal_Args )");}*/
/* ARTLESS need to add extends ident */
;

Formal_Args
: /* empty */
| IDENT ":" IDENT Idents {msg("Formal_Args: IDENT : IDENT Idents");}
;

Idents
: /* empty */
| Ident Idents {msg("Idents: Idents Ident");}
;

Ident
: "," IDENT ":" IDENT {msg("Ident: , IDENT : IDENT");}

Class_Body : "{" Statements Methods "}" {msg("Class_Body: { Statements Methods }");};

Statement
: /* empty */
| WHILE R_Expr Statement_Block {msg("Statement:WHILE etc");}   /* ARTLESS FINISH IF ELIF ELSE WHILE */
| IF R_Expr Statement_Block {msg("Statement: IF R_Expr quack");} 
| Statement_Block {msg("Statement: Statement_Block");}
| L_Expr "="  R_Expr ";" {msg("Statement: L_Expr = R_Expr ;");}
| L_Expr ":" IDENT "=" R_Expr ";" {msg("Statement: L_Expr : IDENT = R_Expr ;");}
| R_Expr ";" {msg("Statement: R_Expr ;");}
| RETURN ";" {msg("Statement: RETURN ;");}
| RETURN R_Expr ";" {msg("Statement: RETURN R_Expr ;");}
;

Statement_Block
: "{" "}" {msg("Statement_Block: { }");}
| "{" Statements  "}" {msg("Statement_Block: { Statement }");}
;

Methods
: /* empty */
| Method Methods {msg("Methods: Method Methods");}
;

Method
: DEF IDENT "(" Formal_Args ")" Statement_Block {msg("Method: DEF IDENT ( Formal_args ) Statement_Block");}
| DEF IDENT "(" Formal_Args ")" ":" IDENT Statement_Block {msg("Method: DEF IDENT ( Formal_args ) : IDENT Statement_Block");}
;

L_Expr
: IDENT {msg("L_Expr: IDENT");}
| R_Expr "." IDENT {msg("L_Expr: R_Expr . IDENT");}
;

R_Expr
: STRING_LIT {msg("R_Expr: STRING_LIT");} 
| INT_LIT {msg("R_Expr: INT_LIT");}
| L_Expr {msg("R_Expr: L_Expr");}
| R_Expr "+" R_Expr {msg("R_Expr: R_Expr + R_Expr");}
| R_Expr "-" R_Expr {msg("R_Expr: R_Expr - R_Expr");}
| R_Expr "*" R_Expr {msg("R_Expr: R_Expr * R_Expr");}
| R_Expr "/" R_Expr {msg("R_Expr: R_Expr / R_Expr");}
| "-" R_Expr {msg("R_Expr: - R_Expr");}
| R_Expr "==" R_Expr {msg("R_Expr: R_Expr == R_Expr");}
| R_Expr "<=" R_Expr {msg("R_Expr: R_Expr <= R_Expr");}
| R_Expr "<" R_Expr {msg("R_Expr: R_Expr < R_Expr");}
| R_Expr ">=" R_Expr {msg("R_Expr: R_Expr >= R_Expr");}
| R_Expr ">" R_Expr {msg("R_Expr: R_Expr > R_Expr");}
| R_Expr "AND" R_Expr {msg("R_Expr: R_Expr AND R_Expr");}
| R_Expr "OR" R_Expr {msg("R_Expr: R_Expr OR R_Expr");}
| "NOT" R_Expr {msg("R_Expr: NOT R_Expr");}
| R_Expr "." IDENT "(" Actual_Args ")" {msg("R_Expr: R_Expr . IDENT ( Actual_Args )");}
;

Actual_Args
: /* empty */
| R_Expr R_Exprs {msg("Actual_Args: R_Expr R_Exprs");}
;

R_Exprs
: "," R_Expr {msg("R_Exprs: , R_Expr");}
;

