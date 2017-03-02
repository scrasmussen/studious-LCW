#include <vector>
#include <list>
#include <string>
#include <string.h>

#ifndef NODE_H
#define NODE_H

static int BUILDSYMBOLTABLE=2;
static int EMPTY=3;
static int CHECKCONSTRUCTORCALLS=5;
static int CHECKCLASSHIERARCHY=7;
static int PRINT=11;
static int PRINTST=13;

class symbol {
public:
 symbol(): name(""),type(""),scope(""),tag("") {}
   std::string name="";
   std::string type=""; 
   std::string scope=""; 
   std::string tag=""; 
};

class symTable {
 public:
 symTable() : prev(NULL){}
  std::vector<symbol> table;
  symTable* prev;

  /* === METHODS === */
  void print() {
    std::cout<<std::endl;
    for (symbol s : table) {
      std::cout << s.name << "|" << s.type << "|" << s.scope << "|" << s.tag << std::endl;
    }
  }

  void size() { std::cout<<table.size()<<std::endl; }
  
  void setPrev(symTable * p) {
    prev = p;
  }

  void insert(symbol sym) {
    table.push_back(sym);
  }

  void update(symbol sym) {
    for (symbol s : table)
      if (s.name.compare(sym.name)==0) {
	s.name=sym.name;
	s.type=sym.type;
	s.scope=sym.scope;
	s.tag=sym.tag;	
    }
  }

  symbol lookup(symbol sym) {
    for (symbol s : table) {
      if(s.name.compare(sym.name)==0)
	return s;
    }
    sym.name="";
    sym.type="";
    sym.scope="";
    sym.tag="";	
    return sym;
  }
};

struct lca {
  std::string name, extends;
};

struct argumentNode {
argumentNode(): name(NULL), type(NULL), sTable(NULL){}
  char const* name="";
  char const* type;
  symTable *sTable;
};


struct argumentsNode {
argumentsNode(): sTable(NULL) {}
  std::vector<argumentNode *> list;
  symTable *sTable;
};

struct formalArgumentsNode {
formalArgumentsNode(): name(NULL), type(NULL), sTable(NULL) {}
  char const* name="";
  char const* type;
  std::vector<argumentNode *> list;
  symTable *sTable; 
};



struct classSignatureNode {
classSignatureNode(): extends(NULL), fArguments(NULL), sTable(NULL) {}
  char const* name;
  char const* extends;
  formalArgumentsNode *fArguments; 
  symTable *sTable; 
};

struct classSigExtendsNode {
classSigExtendsNode(): extends(NULL), sTable(NULL) {}
  char const* extends;
  symTable *sTable; 
};


struct returnNode {
  
};



class actualArgsNode;
class lExprNode;

struct rExprNode {
rExprNode() : rExprFirst(NULL), rExprSecond(NULL), lExpr(NULL), actualArgs(NULL), sTable(NULL) {}
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
rExprsNode() : sTable(NULL) {}
  std::vector<rExprNode*> list;
  symTable *sTable; 
};

struct actualArgsNode{
actualArgsNode() : rExprs(NULL), rExpr(NULL), sTable(NULL){}
  rExprsNode *rExprs ;
  rExprNode *rExpr ;
  symTable *sTable; 
};


struct lExprNode {
lExprNode(): rExpr(NULL), sTable(NULL) {}
  const char* str;
  const char* name;
  rExprNode *rExpr;
  symTable *sTable; 
};


struct whileNode {
  
};

class statementsNode;
struct statementBlockNode {
statementBlockNode() : statements(NULL), sTable(NULL) {}
  statementsNode *statements;
  symTable *sTable; 
};

struct elifNode {
elifNode() : rExpr(NULL), statementBlock(NULL), sTable(NULL) {}
  rExprNode* rExpr;
  statementBlockNode  *statementBlock;
  symTable *sTable; 
};

struct elifsNode {
  std::vector<elifNode*> list;
  symTable *sTable; 
};

struct methodReturnNode {
methodReturnNode() : sTable(NULL) {}
  symTable *sTable;
  const char* name="";
};

struct methodNode {
methodNode() : statementBlock(NULL), fArguments(NULL), methodReturn(NULL), sTable(NULL) {}
  statementBlockNode  *statementBlock;
  formalArgumentsNode* fArguments;
  methodReturnNode* methodReturn;
  symTable *sTable; 
  const char* name="";
};

struct methodsNode {
methodsNode() : sTable(NULL) {}
  std::vector<methodNode> list;
  symTable *sTable; 
};

struct elseNode {
elseNode() : statementBlock(NULL), sTable(NULL) {}
  statementBlockNode  *statementBlock;
  symTable *sTable; 
};


struct statementNode {
statementNode() : rExpr(NULL), lExpr(NULL), stblock(NULL), elifs(NULL), elseN(NULL), sTable(NULL) {}
  int value;
  const char* str;
  const char* name;
  rExprNode* rExpr;
  lExprNode* lExpr;
  statementBlockNode* stblock;
  elifsNode* elifs;
  elseNode* elseN;
  symTable *sTable; 
};


struct statementsNode {
statementsNode() : sTable(NULL) {}
  std::vector<statementNode> list;
  symTable *sTable; 
};

struct classBodyNode {
classBodyNode() : statements(NULL), methods(NULL), sTable(NULL) {}
  const char* name="classBodyNode";
  statementsNode* statements;
  methodsNode* methods;
  symTable *sTable; 
};

struct classNode {
classNode() : sig(NULL), classBody(NULL), sTable(NULL) {}
  classSignatureNode* sig;
  classBodyNode* classBody;
  symTable *sTable; 
};

struct classesNode {
classesNode() : sTable(NULL) {}
  std::vector<classNode> list;
  symTable *sTable; 
};


struct ProgramNode {
ProgramNode() : sTable(NULL) {}
  classesNode classes;
  statementsNode statements;
  symTable *sTable; 
};

void traverse(int);
void checkRExpr(rExprNode *, std::vector<char const*> *, int);
void checkLExpr(lExprNode *, std::vector<char const*> *,  int);
void checkStatement(statementNode *, std::vector<char const*> *, int);
void checkFormalArguments(formalArgumentsNode *, std::vector<char const*> *, int);
void buildSymbolTable( ProgramNode*);
void checkClassHierarchy ( std::vector<classNode> );
void checkConstructorCalls ( ProgramNode* );

extern ProgramNode *root;
#endif
	
