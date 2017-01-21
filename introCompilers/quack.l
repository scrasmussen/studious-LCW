%x comment str_lit
%{
#include <cstdio>
#include <iostream>
#include <string.h>
using namespace std;
#define YY_DECL extern "C" int yylex()
int lineNum = 1;
char *start;

void printl(int line, std::string token, std::string s)
{std::cout << line << " " << token << " \"" << s << "\"" << std::endl;}
%}
%%

"\""[a-zA-Z0-9\ \\0\\b\\t\\n\\r\\f\\\"\\\\]*?"\""  printl(lineNum, "STRING_LIT", yytext);
[ \t\r]        ;
class           { printl(lineNum, "CLASS", yytext); }
def             { printl(lineNum, "DEF", yytext); }
extends         { printl(lineNum, "EXTENDS", yytext); }
if              { printl(lineNum, "IF", yytext); }
elif            { printl(lineNum, "ELIF", yytext); }
else            { printl(lineNum, "ELSE", yytext); }
while           { printl(lineNum, "WHILE", yytext); }
return          { printl(lineNum, "RETURN", yytext); }
and             { printl(lineNum, "AND", yytext); }
or              { printl(lineNum, "OR", yytext); }
not             { printl(lineNum, "NOT", yytext); }
:               { printl(lineNum, ":", yytext); }
\(              { printl(lineNum, "(", yytext); }
\)              { printl(lineNum, ")", yytext); }
\.              { printl(lineNum, ".", yytext); }
\+              { printl(lineNum, "+", yytext); }
\-              { printl(lineNum, "-", yytext); }
\*              { printl(lineNum, "*", yytext); }
\/              { printl(lineNum, "/", yytext); }
\=\=            { printl(lineNum, "EQUALS", yytext); }
\<\=            { printl(lineNum, "ATMOST", yytext); }
\<              { printl(lineNum, "LESS", yytext); }
\>\=            { printl(lineNum, "ATLEAST", yytext); }
\>              { printl(lineNum, "MORE", yytext); }
\=              { printl(lineNum, "=", yytext); }
\{              { printl(lineNum, "{", yytext); }
\}              { printl(lineNum, "}", yytext); }

"/*"            BEGIN(comment);
<comment>"*"+[^*/\\n]*   ++lineNum;
<comment>\n              ++lineNum;
<comment>"*"+"/"         BEGIN(INITIAL);
"//".*\n        ++lineNum;





[0-9]+          { printl(lineNum, "INT_LIT", yytext); }
[a-zA-Z0-9_]+   { printl(lineNum, "IDENT", yytext); }
\n              { ++lineNum; }
. ;
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
  int token;

}
