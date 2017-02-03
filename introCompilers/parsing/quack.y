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
  char *sval;
}

// define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the union:
%token <ival> INT
%token <fval> FLOAT
%token <sval> STRING

// KEYWORDS
%token CLASS DEF EXTENDS IF ELIF ELSE WHILE RETURN
// PUNCTUATION

// IDENTIFIERS
%token <strval> IDENT
%token ELLIPSIS
// INTEGER AND STRING LITERALS
%token <strval> INT_LIT STRING_LIT

%%

// ---GRAMMAR---
quack:
/*  INT quack      { std::cout << "bison found an int: " << $1 << std::endl; }
  | FLOAT quack  { std::cout << "bison found a float: " << $1 << std::endl; }
  | STRING quack { std::cout << "bison found a string: " << $1 << std::endl; }
  | INT          { std::cout << "bison found an int: " << $1 << std::endl; }
  | FLOAT        { std::cout << "bison found a float: " << $1 << std::endl; }
  | STRING       { std::cout << "bison found a string: " << $1 << std::endl; }*/
// Keywords
  | CLASS quack  { std::cout << "PARSER: CLASS" << std::endl; }
  | IDENT quack  { std::cout << "PARSER: IDENT" << std::endl; }
  ;
