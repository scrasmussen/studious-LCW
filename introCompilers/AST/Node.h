#include <vector>
#include <list>
#include <string>
#include <string.h>

#ifndef NODE_H
#define NODE_H


struct classSignatureNode {
  char const* name;
  char const* extends = "Obj";
  int searched = 0;
};

struct classSigExtendsNode {
  char* extends;
};

struct classNode {
  classSignatureNode sig;
};


struct classesNode {
  std::vector<classNode> list;
};

struct ProgramNode {
  classesNode classes;
};

void checkClassHierarchy ( std::vector<classNode> );

extern ProgramNode *root;
#endif
