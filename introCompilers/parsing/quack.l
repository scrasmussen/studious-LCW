%{
#include <cstdio>
#include <iostream>
#include <string.h>
#include "quack.tab.h"


// void yyerror (const char *msg) {
//    fprintf(stderr,"%d: %s (at '%s')\n", yylineno, msg, yytext);
// }
void yyerror(const char *p) {std::cerr << "Error!" << std::endl; }

int lineNum = 1;
char *start;
void printl(int line, std::string token, std::string s)
{std::cout << line << " " << token << " \"" << s << "\"" << std::endl;}
%}
%option yylineno
%option warn nodefault
%option header-file="lex.yy.h"
%x comment str_lit1 str_lit2 str_lit3
%%

[ \t\r]        ;
class           { return CLASS; }
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

"\"\"\""            { BEGIN(str_lit3);std::cout << lineNum <<"\"";}
"\"\""              {std::cout << lineNum <<"\""; BEGIN(str_lit2);}
"\""                {std::cout << lineNum <<"\""; BEGIN(str_lit1);}
<str_lit3>"\"\"\""   {std::cout << yytext << "\" "<< std::endl; BEGIN(INITIAL);}
<str_lit2>"\"\""     {std::cout << yytext << "\" "<< std::endl; BEGIN(INITIAL);}
<str_lit1>"\""       {std::cout << yytext << "\" "<< std::endl; BEGIN(INITIAL);}
<str_lit3,str_lit2,str_lit1>\n                    ++lineNum;
<str_lit3,str_lit2,str_lit1>\\\\                  {std::cout << yytext;}
<str_lit3,str_lit2,str_lit1>\\[^0btnrf\"][.]*     {
fprintf(stderr, "Illegal escape code: \"");
fprintf(stderr, yytext);
fprintf(stderr, "\"\n"); }

[0-9]+          { printl(lineNum, "INT_LIT", yytext); }
[a-zA-Z0-9_]+   { printl(lineNum, "IDENT", yytext); }
\n              { ++lineNum; }
. ;
%%
