%{
#include <cstdio>
#include <iostream>
#include <string.h>
#include "quack.tab.h"
#define PRINTTOKEN 0
const char* ERROR_ESC="only \\0, \\b, \\t, \\n, \\r, \\\\ allowed";
const char* ERROR_NL="newline in quoted string";

void yyerror(const char *p) {std::cerr << yylineno << ":" << p << " (at  '" << yytext <<"')" << std::endl; }
char *start;

void printl(std::string token, std::string s) {
  if (PRINTTOKEN == 1) 
    std::cout << "  " << yylineno << " " << token << " \"" << s << "\"" << std::endl;
}

%}
%option yylineno
%option warn nodefault
%option header-file="lex.yy.h"
%x comment str_lit1 str_lit2 str_lit3
%%

class           { printl("Class", yytext); return CLASS;}
def             { printl("DEF", yytext); return DEF;}
extends         { printl("EXTENDS", yytext); return EXTENDS;}
if              { printl("IF", yytext); return IF;}
elif            { printl("ELIF", yytext); return ELIF;}
else            { printl("ELSE", yytext); return ELSE;}
while           { printl("WHILE", yytext); return WHILE;}
return          { printl("RETURN", yytext); return RETURN;}
and             { printl("AND", yytext); return AND;}
or              { printl("OR", yytext); return OR;}
not             { printl("NOT", yytext); return NOT;}

[:]              { printl(":", yytext); return COLON;}
[;]              { printl(";", yytext); return SEMICOLON;}
[(]              { printl("(", yytext); return LPAREN;}
[)]              { printl(")", yytext); return RPAREN;}
[.]              { printl(".", yytext); return DOT;}
[,]              { printl(".", yytext); return COMMA;}
[+]              { printl("+", yytext); return PLUS;}
[-]              { printl("-", yytext); return MINUS;}
[*]              { printl("*", yytext); return TIMES;}
[/]              { printl("/", yytext); return DIV;}
"=="            { printl("EQUALS", yytext); return EQUIV;}
"<="            { printl("ATMOST", yytext); return LEQ;}
">="            { printl("ATLEAST", yytext); return GEQ;}
[<]              { printl("LESS", yytext); return LT;}
[>]              { printl("MORE", yytext); return GT;}
[=]              { printl("=", yytext); return EQUALS;}
[{]              { printl("{", yytext); return RCURLY;}
[}]              { printl("}", yytext); return LCURLY;}

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
