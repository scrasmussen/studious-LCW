// Bison quack.y file based on tutorial from http://aquamentus.com/flex_bison.html
%{
  #include <cstdio>
  #include <iostream>

  // Bison stuff
  extern "C" int yylex();
  extern "C" int yyparse();
  extern "C" FILE *yyin;

  void yyerror(const char *s);
%}

// Bison by defines a C union
// holding each of the types of tokens that Flex could return, and have Bison
// use that union instead of "int" for the definition of "yystype":
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
%token <sval> CLASS
%token <sval> DEF
%token <sval> EXTENDS
%token <sval> IF
%token <sval> ELIF
%token <sval> ELSE
%token <sval> WHILE
%token <sval> RETURN
%%
 // this is the actual grammar that bison will parse, but for right now it's just
 // something silly to echo to the screen what bison gets from flex.  We'll
 // make a real one shortly:
quack:
  INT quack      { std::cout << "bison found an int: " << $1 << std::endl; }
  | FLOAT quack  { std::cout << "bison found a float: " << $1 << std::endl; }
  | STRING quack { std::cout << "bison found a string: " << $1 << std::endl; }
  | INT          { std::cout << "bison found an int: " << $1 << std::endl; }
  | FLOAT        { std::cout << "bison found a float: " << $1 << std::endl; }
  | STRING       { std::cout << "bison found a string: " << $1 << std::endl; }
// Keywords
  | CLASS quack  { std::cout << "LINE# CLASS " << std::endl; }
  | DEF quack  { std::cout << "LINE# DEF " << std::endl; }
  | EXTENDS quack  { std::cout << "LINE# EXTENDS " << std::endl; }
  | IF quack  { std::cout << "LINE# IF " << std::endl; }
  | ELIF quack  { std::cout << "LINE# ELIF " << std::endl; }
  | ELSE quack  { std::cout << "LINE# ELSE " << std::endl; }
  | WHILE quack  { std::cout << "LINE# WHILE " << std::endl; }
  | RETURN quack  { std::cout << "LINE# RETURN " << std::endl; }
  ;
%%

int main(int argc, char*argv[]) {
  // open a file handle to a particular file:

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " QUACK.INPUT.FILE" << std::endl;
    return 1;
  }
  FILE *f = fopen(argv[1], "r");
  // make sure it is valid:
  if (!f) {
    std::cout << "Unable to open input file!" << std::endl;
    return -1;
  }
  // set flex to read from it instead of defaulting to STDIN:
  yyin = f;

  // parse through the input until there is no more:
  do {
    yyparse();
  } while (!feof(yyin));
}

void yyerror(const char *s) {
  std::cout << "QUACK, parse error!  Message: " << s << std::endl;
  exit(-1);
}
