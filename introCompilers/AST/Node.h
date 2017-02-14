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


class lExprNode;
struct rExprNode {
  int val;
  const char* str = "";
  const char* name = "";
  rExprNode *rExprFirst ;
  rExprNode *rExprSecond ;
  lExprNode *lExpr ;
  constructorNode *constructor ;
};

struct lExprNode {
  const char* str = "";
  rExprNode *rExpr;
};


struct whileNode {
  
};

class statementsNode;
struct statementBlockNode {
 //statementsNode *statements; 
 statementsNode *statements;
};

struct elifNode {
 rExprNode* rExpr;
 statementBlockNode  *statementBlock;
};

struct elifsNode {
  std::vector<elifNode> list;
 
};

struct methodNode {
 statementBlockNode  *statementBlock;
};

struct methodsNode {
  std::vector<methodNode> list;
 
};
struct elseNode {
 statementBlockNode  *statementBlock;
};



struct statementNode {
  int value;
  const char* str;
  rExprNode* rExpr;
  lExprNode* lExpr;
  statementBlockNode* stblock; 
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
	
