// Bison quack.y file based on tutorial from http://aquamentus.com/flex_bison.html
%{

  #include <iostream>
  #include "lex.yy.h"
  #include "quack.tab.h"

  /* int yylex(); */
  extern void yyerror(const char *msg);
  extern int lineNum;
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
 //KEYWORDS
%token CLASS
 //%token <sval> DEF
 //%token <sval> EXTENDS
 //%token <sval> IF
 //%token <sval> ELIF
 //%token <sval> ELSE
 //%token <sval> WHILE
 //%token <sval> RETURN
%%

 // this is the actual grammar that bison will parse, but for right now it's just
 // something silly to echo to the screen what bison gets from flex.  We'll
 // make a real one shortly:
quack:
/*  INT quack      { std::cout << "bison found an int: " << $1 << std::endl; }
  | FLOAT quack  { std::cout << "bison found a float: " << $1 << std::endl; }
  | STRING quack { std::cout << "bison found a string: " << $1 << std::endl; }
  | INT          { std::cout << "bison found an int: " << $1 << std::endl; }
  | FLOAT        { std::cout << "bison found a float: " << $1 << std::endl; }
  | STRING       { std::cout << "bison found a string: " << $1 << std::endl; }*/
// Keywords
  | CLASS quack  { std::cout << lineNum << " CLASS \"class\"" << std::endl; }
/*  | DEF quack  { std::cout << "LINE# DEF " << std::endl; }
  | EXTENDS quack  { std::cout << "LINE# EXTENDS " << std::endl; }
  | IF quack  { std::cout << "LINE# IF " << std::endl; }
  | ELIF quack  { std::cout << "LINE# ELIF " << std::endl; }
  | ELSE quack  { std::cout << "LINE# ELSE " << std::endl; }
  | WHILE quack  { std::cout << "LINE# WHILE " << std::endl; }
  | RETURN quack  { std::cout << "LINE# RETURN " << std::endl; } */
  ;
