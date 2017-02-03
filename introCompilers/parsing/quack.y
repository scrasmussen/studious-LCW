// Bison quack.y file based on tutorial from http://aquamentus.com/flex_bison.html
%{

  #include <iostream>
  #include "lex.yy.h"
  #include "quack.tab.h"

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

// START
%start Program
// KEYWORDS
%token CLASS "class"
%token DEF EXTENDS IF ELIF ELSE WHILE RETURN
%token AND OR NOT SEMICOLON
%token LPAREN '('
%token RPAREN ')'
%token DOT '.'
%token PLUS '+'
%token MINUS '-'
%token TIMES '*'
%token DIV '/'
%token EQUIV "=="
%token LEQ "<="
%token GEQ ">="
%token LT '<'
%token GT '>' 
%token EQUALS '='
%token RCURLY '{'
%token LCURLY '}'


// PUNCTUATION

// IDENTIFIERS
%token <strval> IDENT
%token ELLIPSIS
// INTEGER AND STRING LITERALS
%token <strval> INT_LIT STRING_LIT
%%
// ----GRAMMAR----
Program : Class Statement 
  | Class
  | Statement 
  ;

Class : Class_Signature Class_Body;

Class_Signature : 'class' IDENT '(' quack ')' {std::cout<<'}}}}'<<$2<<std::endl;};
Class_Body : quack;

Statement : /* empty? */
  | WHILE R_Expr Statement_Block
  | quack
  ;

R_Expr : quack;

Statement_Block : quack;

quack:
// Keywords
  | CLASS quack
  | DEF quack
  | EXTENDS quack
  | IF quack
  | ELSE quack
  | RETURN quack
// IDENTIFIERS
  | IDENT quack
  | INT_LIT quack
  | LPAREN quack
  | RPAREN quack
  | DOT quack
  | PLUS quack
  | MINUS quack
  | TIMES quack
  | DIV quack
  | EQUIV quack
  | LEQ quack
  | GEQ quack
  | LT quack
  | GT quack
  | EQUALS quack
  | RCURLY quack
  | LCURLY quack
  ;
