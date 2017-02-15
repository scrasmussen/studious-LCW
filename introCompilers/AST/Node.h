#include <vector>
#include <list>
#include <string>
#include <string.h>

#ifndef NODE_H
#define NODE_H


struct classSignatureNode {
  char const* name;
  char const* extends;
};

struct classSigExtendsNode {
  char const* extends;
};


struct returnNode {
  
};



class actualArgsNode;
class lExprNode;

struct rExprNode {
rExprNode(): rExprFirst(NULL), rExprSecond(NULL), lExpr(NULL), actualArgs(NULL) {}
  int val;
  const char* str = "";
  const char* name = "";
  rExprNode *rExprFirst ;
  rExprNode *rExprSecond ;
  lExprNode *lExpr ;
  actualArgsNode *actualArgs;
};

struct rExprsNode {
  std::vector<rExprNode*> list;
};

struct actualArgsNode{
actualArgsNode(): rExprs(NULL), rExpr(NULL){}
  rExprsNode *rExprs ;
  rExprNode *rExpr ;
};


struct lExprNode {
lExprNode(): rExpr(NULL), strtest(NULL) {}
  const char* str;
  std::string *strtest;
  rExprNode *rExpr;
};


struct whileNode {
  
};

class statementsNode;
struct statementBlockNode {
statementBlockNode() : statements(NULL) {}
 statementsNode *statements;
};

struct elifNode {
elifNode() : rExpr(NULL), statementBlock(NULL) {}
 rExprNode* rExpr;
 statementBlockNode  *statementBlock;
};

struct elifsNode {
  std::vector<elifNode*> list;
};

struct methodNode {
methodNode() : statementBlock(NULL) {}
 statementBlockNode  *statementBlock;
};

struct methodsNode {
  std::vector<methodNode> list;
};

struct elseNode {
elseNode(): statementBlock(NULL){}
 statementBlockNode  *statementBlock;
};



struct statementNode {
statementNode():rExpr(NULL), lExpr(NULL), stblock(NULL), elifs(NULL), elseN(NULL) {}
  int value;
  const char* str;
  rExprNode* rExpr;
  lExprNode* lExpr;
  statementBlockNode* stblock;
  elifsNode* elifs;
  elseNode* elseN;
};


struct statementsNode {
  std::vector<statementNode> list;
};

struct classBodyNode {
classBodyNode(): statements(NULL), methods(NULL){}
  const char* name="classBodyNode";
  statementsNode* statements;
  methodsNode* methods;
};

struct classNode {
classNode(): sig(NULL), classBody(NULL){}
  const char* name;
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

void checkRExpr(rExprNode *, std::vector<char const*> *);
void checkLExpr(lExprNode *, std::vector<char const*> *);
void checkStatement(statementNode , std::vector<char const*> *);
void checkClassHierarchy ( std::vector<classNode> );
void checkConstructorCalls ( ProgramNode* );

extern ProgramNode *root;
#endif
	
