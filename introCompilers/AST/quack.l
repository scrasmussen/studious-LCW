%{
#include <cstdio>
#include <iostream>
#include <string>
#include "Node.h"
#include "quack.tab.h"
#define PRINTTOKEN 0 
const char* ERROR_ESC="only \\0, \\b, \\t, \\n, \\r, \\\\ allowed";
const char* ERROR_NL="newline in quoted string";
const char* ERROR_SYNTAX="Not caught by lexer";

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
%x comment strlit3 strlit2 strlit1
%%

class            { printl("CLASS", yytext); return CLASS;}
def              { printl("DEF", yytext); return DEF;}
extends          { printl("EXTENDS", yytext); return EXTENDS;}
if               { printl("IF", yytext);  return IF;}
elif             { printl("ELIF", yytext); return ELIF;}
else             { printl("ELSE", yytext); return ELSE;}
while            { printl("WHILE", yytext); return WHILE;}
return           { printl("RETURN", yytext); return RETURN;}
and              { printl("AND", yytext); return AND;}
or               { printl("OR", yytext); return OR;}
not              { printl("NOT", yytext); return NOT;}

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
"=="             { printl("EQUALS", yytext); return EQUIV;}
"<="             { printl("ATMOST", yytext); return LEQ;}
">="             { printl("ATLEAST", yytext); return GEQ;}
[<]              { printl("LESS", yytext); return LT;}
[>]              { printl("MORE", yytext); return GT;}
[=]              { printl("=", yytext); return EQUALS;}
[{]              { printl("{", yytext); return RCURLY;}
[}]              { printl("}", yytext); return LCURLY;}

[a-zA-Z_][a-zA-Z0-9_]*   { printl("IDENT", yytext); yylval.str=strdup(yytext);  return IDENT;}
[-]?[0-9]+                   { printl("INT_LIT", yytext);/*yylval.num=atof(yytext);*/
                               yylval.str=strdup(yytext); return INT_LIT;}



["](([\\][0btnrf"\\])|[^"\n\\])*["]  { printl("STRING_LIT", yytext); yylval.str=strdup(yytext); return STRING_LIT;   
                                       /* Next few lines based on code from class */ 
                                       /* The code was cleaner and prettier */ }
["](([\\].)|[^"\n\\])*["]            { yyerror(ERROR_ESC); yylval.str=strdup(yytext); return STRING_LIT; }
["](([\\][^\n])|[^\n"])*\n           { yyerror(ERROR_NL); yylval.str=strdup(yytext); return STRING_LIT; }



["]["]["]            { BEGIN(strlit3); printl("start strlit3",yytext);}
["]["]               { BEGIN(strlit2); printl("start strlit2",yytext);}
["]                  { BEGIN(strlit1); printl("start strlit1",yytext);}
<strlit3>["]["]["]   {BEGIN(INITIAL);printl("end strlit3",yytext); yylval.str=strdup(yytext); return STRING_LIT; /*ARTLESS: WHAT'S IN STRING_LIT?*/}
<strlit3>(([^"])|(["][^"])|(["]["][^"])|\n)*  {}
<strlit2>["]["]     {BEGIN(INITIAL);printl("end strlit2",yytext); yylval.str=strdup(yytext); return STRING_LIT;}
<strlit2>(([^"])|(["][^"])|\n)*  {}
<strlit1>["]       {BEGIN(INITIAL);printl("end strlit1",yytext); yylval.str=strdup(yytext); return STRING_LIT;}
<strlit1>(([^"])|\n)*  {}



[/][/].*         { ; }
[/][*]           { BEGIN comment; }
<comment>[^*]*   { ; }
<comment>[*][^/] { ; }
<comment>[*][/]  {BEGIN INITIAL; }
[ \t\n]* {;}
<<EOF>> { return EOF; }
.  { yyerror(ERROR_SYNTAX);};
%%
