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

  // set flex to read from it instead of defaulting to STDIN:
  yyin = f;

  if (yyparse() == 0) {
    std::cout << "Finished parse with no errors\n";
  }


  // std::cout << "|ROOT| " << root->classes.list.size() << std::endl;
  /* ==== CHECK FOR HEIRARCHY WELL FORMEDNESS ==== */
  std::vector<classNode> l = root->classes.list;
  int done=0, addS, failed=0;
  classesNode found, search;
  for (auto &i : l)
    {
      if (strcmp(i.sig.extends, "Obj") == 0)
	found.list.push_back(i);
      else
	search.list.push_back(i);
    }
  while (!done) {
    classesNode newFound, newSearch;
    failed = 1;
    for (auto &s : search.list) {
      addS = 1;
      for (auto &f : found.list) {
    	if ((strcmp(s.sig.extends,f.sig.name)==0)) {
	  newFound.list.push_back(s);
	  addS = 0;
	  failed = 0;
	}
      }
      if (addS)
	newSearch.list.push_back(s);
    }
    if (newFound.list.size()>0) {
      search = newSearch;
      found.list.insert(found.list.end(), newFound.list.begin(), newFound.list.end() );
    }
    if (newSearch.list.size()==0 || failed==1) {
      done = 1;
    }
  }
  if (failed == 1)
    std::cerr << "Class Hierarchy Not Well Formed\n";
  /* ==== END CHECK FOR HEIRARCHY WELL FORMEDNESS ==== */
  
}
