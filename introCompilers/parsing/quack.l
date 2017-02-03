%{
#include <cstdio>
#include <iostream>
#include <string.h>
#include "quack.tab.h"
#define PRINTTOKEN 1
const char* ERROR_ESC="ERROR: only \\0, \\b, \\t, \\n, \\r, \\\\ allowed";
const char* ERROR_NL="ERROR: newline in quoted string";

// void yyerror (const char *msg) {
//    fprintf(stderr,"%d: %s (at '%s')\n", yylineno, msg, yytext);
// }
void yyerror(const char *p) {std::cerr << "Error at " << yylineno << " " << p << std::endl; }
char *start;

void printl(std::string token, std::string s) {
  if (PRINTTOKEN == 1) 
    std::cout << yylineno << " " << token << " \"" << s << "\"" << std::endl;
}

%}
%option yylineno
%option warn nodefault
%option header-file="lex.yy.h"
%x comment str_lit1 str_lit2 str_lit3
%%

class           { printl("CLASS", yytext); return CLASS;}
def             { printl("DEF", yytext); return DEF;}
extends         { printl("EXTENDS", yytext); return EXTENDS;}
if              { printl("IF", yytext); return IF;}
elif            { printl("ELIF", yytext); return ELIF;}
else            { printl("ELSE", yytext); return ELSE;}
while           { printl("WHILE", yytext); return WHILE;}
return          { printl("RETURN", yytext); return RETURN;}
and             { printl("AND", yytext); }
or              { printl("OR", yytext); }
not             { printl("NOT", yytext); }
:               { printl(":", yytext); }

[(]              { printl("(", yytext); }
[)]              { printl(")", yytext); }
[.]              { printl(".", yytext); }
[+]              { printl("+", yytext); }
[-]              { printl("-", yytext); }
[*]              { printl("*", yytext); }
[/]              { printl("/", yytext); }
"=="            { printl("EQUALS", yytext); }
"<="            { printl("ATMOST", yytext); }
">="            { printl("ATLEAST", yytext); }
[<]              { printl("LESS", yytext); }
[>]              { printl("MORE", yytext); }
[=]              { printl("=", yytext); }
[{]              { printl("{", yytext); }
[}]              { printl("}", yytext); }

[a-zA-Z_][a-zA-Z0-9_]*   { printl("IDENT", yytext); return IDENT;}
[0-9]+                   { printl("INT_LIT", yytext); return INT_LIT;}

["](([\\][0btnrf"\\])|[^"\n\\])*["]  { return STRING_LIT; }
["](([\\].)|[^"\n\\])*["]            { yyerror(ERROR_ESC); return STRING_LIT; }
["](([\\][^\n])|[^\n"])*\n           { yyerror(ERROR_NL);  return STRING_LIT; }

["]["]["]       { BEGIN str_lit3; }
<str_lit3>["]["]["] { BEGIN INITIAL; return STRING_LIT; }

[/][/].*         { ; }
[/][*]           { BEGIN comment; }
<comment>[^*]*   { ; }
<comment>[*][^/] { ; }
<comment>[*][/]  {BEGIN INITIAL; }

\.\.\.  { return ELLIPSIS; }

[ \t\n]* {;}

<<EOF>> { return EOF; }
.  { printl("WARNING: ERROR", yytext);};
%%
