/*
 * scanner and parser
 */
#include <cstdio>
#include <iostream>
#include "quack.tab.h"
#include "lex.yy.h"
#include "Node.h"

extern int yyparse();
extern void yyrestart(FILE *f);
extern void yyerror(const char *s);
extern int yy_flex_debug;

extern struct ProgramNode *root;

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
  std::vector<classNode> l = root->classes.list;
  
  // set flex to read from it instead of defaulting to STDIN:
  yyin = f;
  if (yyparse() == 0) {
    std::cout << "Finished parse with no errors\n";
  }

  /* === TRAVERSAL ACTIONS === */
  traverse(CHECKCLASSHIERARCHY);
  traverse(CHECKCONSTRUCTORCALLS);
  traverse(BUILDSYMBOLTABLE);
  
}
