#include <vector>
#include <list>
#include <string>
#include <string.h>

#ifndef NODE_H
#define NODE_H

static int BUILDSYMBOLTABLE=2;
static int EMPTY=3;

class symbol {
public: 
   std::string name;
   std::string type; 
   std::string scope; 
   std::string tag; 
 };

class symTable { 
  public: 
     std::vector<symbol> table; 
     void insert(std:: vector<symbol> sTable, symbol sym)  { 
	sTable.push_back(sym);
     } 
     void update(std:: vector<symbol> sTable, symbol sym)  { 
		for (auto &c : sTable) {
    			if(c.name.compare(sym.name)==0) {
				c.name=sym.name;
				c.type=sym.type;
				c.scope	=sym.scope;
				c.tag=sym.tag;	
			}
		}
     }
     symbol lookup(std:: vector<symbol> sTable, symbol sym)  { 
		for (auto &c : sTable) {
    			if(c.name.compare(sym.name)==0) {
				return c;
			}
		}
	sym.name="";
	sym.type="";
	sym.scope="";
	sym.tag="";	
	return sym;
     }
};

struct classSignatureNode {
  char const* name;
  char const* extends;
  symTable *sTable; 
};

struct classSigExtendsNode {
  char const* extends;
  symTable *sTable; 
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
  symTable *sTable; 
};

struct rExprsNode {
  std::vector<rExprNode*> list;
  symTable *sTable; 
};

struct actualArgsNode{
  actualArgsNode(): rExprs(NULL), rExpr(NULL){}
  rExprsNode *rExprs ;
  rExprNode *rExpr ;
  symTable *sTable; 
};


struct lExprNode {
  lExprNode(): rExpr(NULL), strtest(NULL) {}
  const char* str;
  std::string *strtest;
  rExprNode *rExpr;
  symTable *sTable; 
};


struct whileNode {
  
};

class statementsNode;
struct statementBlockNode {
  statementBlockNode() : statements(NULL) {}
  statementsNode *statements;
  symTable *sTable; 
};

struct elifNode {
  elifNode() : rExpr(NULL), statementBlock(NULL) {}
  rExprNode* rExpr;
  statementBlockNode  *statementBlock;
  symTable *sTable; 
};

struct elifsNode {
  std::vector<elifNode*> list;
  symTable *sTable; 
};

struct methodNode {
  methodNode() : statementBlock(NULL) {}
  statementBlockNode  *statementBlock;
  symTable *sTable; 
};

struct methodsNode {
  std::vector<methodNode> list;
  symTable *sTable; 
};

struct elseNode {
elseNode(): statementBlock(NULL){}
  statementBlockNode  *statementBlock;
  symTable *sTable; 
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
  symTable *sTable; 
};


struct statementsNode {
  std::vector<statementNode> list;
  symTable *sTable; 
};

struct classBodyNode {
classBodyNode(): statements(NULL), methods(NULL){}
  const char* name="classBodyNode";
  statementsNode* statements;
  methodsNode* methods;
  symTable *sTable; 
};

struct classNode {
classNode(): sig(NULL), classBody(NULL){}
  const char* name;
  classSignatureNode* sig;
  classBodyNode* classBody;
  symTable *sTable; 
};

struct classesNode {
  std::vector<classNode> list;
  symTable *sTable; 
};


struct ProgramNode {
  classesNode classes;
  statementsNode statements;
  symTable *sTable; 
};


void checkRExpr(rExprNode *, std::vector<char const*> *);
void checkLExpr(lExprNode *, std::vector<char const*> *);
void checkStatement(statementNode , std::vector<char const*> *);
void buildSymbolTable( ProgramNode*);
void checkClassHierarchy ( std::vector<classNode> );
void checkConstructorCalls ( ProgramNode* );

extern ProgramNode *root;
#endif
	
