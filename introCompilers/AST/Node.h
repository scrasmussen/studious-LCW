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
  char const* extends;
};


struct returnNode {
  
};

struct constructorNode {
  const char* name;
};

struct rExprNode {
  int val;
  const char* str = "";
  rExprNode *rExprFirst ;
  rExprNode *rExprSecond ;
  constructorNode *constructor ;
};


struct whileNode {
  
};

struct statementNode {
  int value;
  const char* str;
  rExprNode* rExpr;
};

struct statementsNode {

  std::vector<statementNode> list;
};

struct classBodyNode {
  const char* name="classBodyNode";
  statementsNode* statements;
};

struct classNode {
  const char* name="classNode";
  classSignatureNode* sig;
  classBodyNode* classBody;
};

struct classesNode {
  std::vector<classNode> list;
};

struct ProgramNode {
  classesNode classes;
  statementsNode statements;
};

void checkClassHierarchy ( std::vector<classNode> );
void checkConstructorCalls ( ProgramNode* );

extern ProgramNode *root;
#endif
	
