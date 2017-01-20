%{
#include <cstdio>
#include <iostream>
using namespace std;
#define YY_DECL extern "C" int yylex()
int lineNum = 1;
%}
%%
[ \t]        ;
class          { std::cout << lineNum << " CLASS " << yytext << std::endl; }
def            { std::cout << lineNum << " DEF " << yytext << std::endl; }
extends        { std::cout << lineNum << " EXTENDS " << yytext << std::endl; }
if             { std::cout << lineNum << " IF " << yytext << std::endl; }
elif           { std::cout << lineNum << " ELIF " << yytext << std::endl; }
else           { std::cout << lineNum << " ELSE " << yytext << std::endl; }
while          { std::cout << lineNum << " WHILE " << yytext << std::endl; }
return         { std::cout << lineNum << " RETURN " << yytext << std::endl; }
[0-9]+\.[0-9]+ { std::cout << lineNum << " "<< yytext << std::endl; }
[0-9]+         { std::cout << lineNum << " Integer " << yytext << std::endl; }
[a-zA-Z0-9]+   { std::cout << lineNum << yytext << std::endl; }
\n             { ++lineNum; }
.               ;
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

  yylex();
  // parse through the input until there is no more:
  /* do {
     yyparse();
  } while (!feof(yyin));*/
}
