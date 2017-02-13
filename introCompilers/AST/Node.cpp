#include <iostream>
#include <string>
#include <vector>
#include "Node.h"

void checkConstructorCalls ( ProgramNode *rootNode ) {
  std::vector<classNode> classList = rootNode->classes.list;
  std::vector<statementNode> statementList = rootNode->statements.list;

  for (auto &c : classList) {
    if (c.classBody != NULL) {
	    std::cout << c.classBody->name << std::endl;
	    for (auto $s : c.classBody->statements->list)
	      {
		
	      }
    }
      // std::cout << c->classBody.name << std::endl;
    // std::vector<statementNode> csList = c.classBody->statements->list;
  }

  
  for (auto &s : statementList) {
    
  }

  for (auto &c : classList)
    std::cout << c.sig->name << std::endl;
}

void checkClassHierarchy ( std::vector<classNode> l ) {
  int done=0, addS, failed=0;
  classesNode found, search;
  for (auto &i : l)
    {
      if (strcmp(i.sig->extends, "Obj") == 0)
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
    	if ((strcmp(s.sig->extends,f.sig->name)==0)) {
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
 return;
}



