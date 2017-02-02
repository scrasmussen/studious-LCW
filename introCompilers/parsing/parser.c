/*
 * scanner and parser
 */
#include <cstdio>
#include <iostream>
#include "quack.tab.h"
#include "lex.yy.h"

extern int yyparse();
extern void yyrestart(FILE *f);
extern void yyerror(const char *s);
extern int lineNum;
extern int yy_flex_debug;

int main(int argc, char*argv[]) {

  std::cout << "Fin" << std::endl;
}
