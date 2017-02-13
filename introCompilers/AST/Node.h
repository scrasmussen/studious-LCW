#include <vector>
#include <list>
#include <string>
#include <string.h>

#ifndef NODE_H
#define NODE_H
struct returnNode {
  
};

struct constructorNode {
  const char* name;
};

struct rExprNode {
  int val;
  const char* str = "";
  constructorNode constructor;
};

struct argumentNode { //monil
  char const* type;
  char const* name;
};

struct formalArgumentsNode { //monil
  std::vector<argumentNode> list;
};


struct classSignatureNode {
  char const* name;
  char const* extends = "Obj";
  int searched = 0;
  formalArgumentsNode fargs; //monil
};

struct classSigExtendsNode {
  char const* extends;
};

struct classNode {
  classSignatureNode sig;
};


struct statementNode {
  int value;
  const char* str;
};

struct classesNode {
  std::vector<classNode> list;
};

struct statementsNode {
  std::vector<statementNode> list;
};

struct ProgramNode {
  classesNode classes;
  statementsNode statements;
};

void checkClassHierarchy ( std::vector<classNode> );

extern ProgramNode *root;
#endif
