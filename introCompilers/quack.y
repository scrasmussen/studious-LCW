// Bison quack.y file based on tutorial from http://aquamentus.com/flex_bison.html
%{
  #include <cstdio>
  #include <iostream>
  using namespace std;

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

%%
 // this is the actual grammar that bison will parse, but for right now it's just
 // something silly to echo to the screen what bison gets from flex.  We'll
 // make a real one shortly:
quack:
  INT quack      { cout << "bison found an int: " << $1 << endl; }
  | FLOAT quack  { cout << "bison found a float: " << $1 << endl; }
  | STRING quack { cout << "bison found a string: " << $1 << endl; }
  | INT          { cout << "bison found an int: " << $1 << endl; }
  | FLOAT        { cout << "bison found a float: " << $1 << endl; }
  | STRING       { cout << "bison found a string: " << $1 << endl; }
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
    cout << "Unable to open input file!" << endl;
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
  cout << "QUACK, parse error!  Message: " << s << endl;
  exit(-1);
}
