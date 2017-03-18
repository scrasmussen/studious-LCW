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
static int CHECKREDEF=17;
static int CHECKMETHODREDEF=19;
static int BUILDLCA=23;
static int TYPEUPDATE=29;
static int CHECKARGTYPE=31;
static int DECLARATION=37;
static int CHECKRETURNTYPE=41;
static int CHECKMETHOD=43;
static int CHECKLCA=47;
static int CHECKOBJECT=49;
static int CHECKLOOPINTERSECTION=53;
static int CHECKLOGIC=57;
static int CHECKUNDEFINEDV=59;
extern int errornum;
std::string leastCommonAnc(std::string, std::string);

class symbol {
public:
 symbol(): name(""),type(""),scope(""),tag("") {linenum=0;}
   std::string name="";
   std::string type=""; 
   std::string scope=""; 
   std::string tag="";
   int linenum;
};

class symTable {
 public:
 symTable() : prev(NULL), next(NULL){}
  std::vector<symbol> table;
  symTable* prev;
  symTable* next;
  int travarsed=0;

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

  void update(symbol sym,std::string newtype) {
    for (symbol &s : this->table) {
      if (s.name.compare(sym.name)==0) {
        if(s.type!="" && s.type!="[NULL]") {
           std::string ret=leastCommonAnc(s.type,newtype); 
	   if (ret!="") s.type=ret;
	   else s.type=newtype;
        }
	else s.type=newtype;
	//s.scope=sym.scope;
	//s.tag=sym.tag;
      }
      //this->print();
    }
  }

  symbol lookup(symbol sym) {
    if (this->table.size()>0) {
      for (symbol s : this->table) {
	if(s.name.compare(sym.name)==0)
	  return s;
      }
    }
    sym.name="";
    sym.type="";
    sym.scope="";
    sym.tag="";	
    return sym;
  }

  std::string typeLookup(std::string name) {
    if (this->table.size()>0) {
      for (symbol s : this->table) {
	if(s.name.compare(name)==0)
	  return s.type;
      }
    }
    return "";
  }
  
  // Artless: Remove??
  std::string getType(std::string name) {
    if (this->table.size()>0) {
      for (symbol s : this->table) {
	if(s.name.compare(name)==0)
	  return s.type;
      }
    }
    return "(JK)->(JK)";
  }
};

struct lca {
  std::string name, extends;
};

struct returnInfo {
  std::string type;
  int linenum;
};

struct argumentNode {
argumentNode(): name(NULL), type(NULL), sTable(NULL){}
  ~argumentNode(){if (sTable) delete sTable;}
  char const* name="";
  char const* type;
  int linenum;
  symTable *sTable;
};


struct argumentsNode {
argumentsNode(): sTable(NULL) {}
  ~argumentsNode(){if (sTable) delete sTable;}
  std::vector<argumentNode *> list;
  symTable *sTable;
};

struct formalArgumentsNode {
formalArgumentsNode(): name(NULL), type(NULL), sTable(NULL) {}
  ~formalArgumentsNode(){if (sTable) delete sTable;}
  char const* name="";
  char const* type;
  std::vector<argumentNode *> list;
  symTable *sTable; 
};

struct classSignatureNode {
classSignatureNode(): extends(NULL), fArguments(NULL), sTable(NULL) {}
  ~classSignatureNode(){if (sTable) delete sTable;}
  char const* name;
  char const* extends;
  formalArgumentsNode *fArguments; 
  symTable *sTable; 
};

struct classSigExtendsNode {
classSigExtendsNode(): extends(NULL), sTable(NULL) {}
  ~classSigExtendsNode(){if (sTable) delete sTable;}
  char const* extends;
  symTable *sTable; 
};

class actualArgsNode;
class lExprNode;

struct rExprNode {
rExprNode() : rExprFirst(NULL), rExprSecond(NULL), lExpr(NULL), actualArgs(NULL), sTable(NULL), name(NULL) {linenum=-1;}
  ~rExprNode(){if (sTable) delete sTable;}
  int val;
  const char* str = "";
  const char* name = "";
  int linenum;
  rExprNode *rExprFirst ;
  rExprNode *rExprSecond ;
  lExprNode *lExpr ;
  actualArgsNode *actualArgs;
  symTable *sTable; 
};

struct rExprsNode {
rExprsNode() : sTable(NULL) {}
  ~rExprsNode(){if (sTable) delete sTable;}
  std::vector<rExprNode*> list;
  symTable *sTable; 
};

struct actualArgsNode{
actualArgsNode() : sTable(NULL){}
  ~actualArgsNode(){if (sTable) delete sTable;}
  std::vector<rExprNode*> list;
  symTable *sTable; 
};

struct lExprNode {
lExprNode(): rExpr(NULL), sTable(NULL) {}
  ~lExprNode(){if (sTable) delete sTable;}
  const char* str;
  const char* name;
  int linenum;
  rExprNode *rExpr;
  symTable *sTable; 
};

class statementsNode;
struct statementBlockNode {
statementBlockNode() : statements(NULL), sTable(NULL) {}
  ~statementBlockNode(){if (sTable) delete sTable;}
  statementsNode *statements;
  symTable *sTable; 
};

struct elifNode {
elifNode() : rExpr(NULL), statementBlock(NULL), sTable(NULL) {}
  ~elifNode(){if (sTable) delete sTable;}
  rExprNode* rExpr;
  statementBlockNode  *statementBlock;
  symTable *sTable; 
};

struct elifsNode {
  ~elifsNode(){if (sTable) delete sTable;}
  std::vector<elifNode*> list;
  symTable *sTable; 
};

struct methodReturnNode {
methodReturnNode() : sTable(NULL) {}
  ~methodReturnNode(){if (sTable) delete sTable;}
  symTable *sTable;
  const char* name="";
};

struct methodNode {
methodNode() : statementBlock(NULL), fArguments(NULL), sTable(NULL) {}
  ~methodNode(){if (sTable) delete sTable;}
  statementBlockNode  *statementBlock;
  formalArgumentsNode* fArguments;
  symTable *sTable; 
  const char* name;
  const char* returnType;
  int linenum;
};

struct methodsNode {
methodsNode() : sTable(NULL) {}
  ~methodsNode(){if (sTable) delete sTable;}
  std::vector<methodNode> list;
  symTable *sTable; 
};

struct elseNode {
elseNode() : statementBlock(NULL), sTable(NULL) {}
  ~elseNode(){if (sTable) delete sTable;}
  statementBlockNode  *statementBlock;
  symTable *sTable; 
};


struct statementNode {
statementNode() : str(NULL),name(NULL),rExpr(NULL), lExpr(NULL), stblock(NULL), elifs(NULL), elseN(NULL), sTable(NULL) {linenum=-1;}
  /* ~statementNode(){if (sTable) delete sTable;} */
  int value;
  const char* str;
  const char* name;
  int linenum;
  rExprNode* rExpr;
  lExprNode* lExpr;
  statementBlockNode* stblock;
  elifsNode* elifs;
  elseNode* elseN;
  symTable *sTable; 
};


struct statementsNode {
statementsNode() : sTable(NULL) {}
  ~statementsNode(){if (sTable) delete sTable;}
  std::vector<statementNode> list;
  symTable *sTable; 
};

struct classBodyNode {
classBodyNode() : statements(NULL), methods(NULL), sTable(NULL) {}
  ~classBodyNode(){if (sTable) delete sTable;}
  const char* name="classBodyNode";
  statementsNode* statements;
  methodsNode* methods;
  symTable *sTable; 
};

struct classNode {
classNode() : sig(NULL), classBody(NULL), sTable(NULL) {}
  ~classNode(){if (sTable) delete sTable;}
  classSignatureNode* sig;
  classBodyNode* classBody;
  symTable *sTable;
  int linenum;
};

struct classesNode {
classesNode() : sTable(NULL) {}
  ~classesNode(){if (sTable) delete sTable;}
  std::vector<classNode> list;
  symTable *sTable; 
};


struct ProgramNode {
ProgramNode() : sTable(NULL) {}
  ~ProgramNode(){if (sTable) delete sTable;}
  classesNode classes;
  statementsNode statements;
  symTable *sTable; 
};

void traverse(int);
void checkRExpr(rExprNode *, std::vector<char const*> *, int);
void checkLExpr(lExprNode *, std::vector<char const*> *,  int);
void checkClass(classNode *, std::vector<char const*> *, int);
void checkStatement(statementNode *, std::vector<char const*> *, int);
void checkFormalArguments(formalArgumentsNode *, std::vector<char const*> *, int);
void buildSymbolTable( ProgramNode*);
void checkClassHierarchy ( std::vector<classNode> );
void checkConstructorCalls ( ProgramNode* );
void goToRoot(symTable *);
void fetchType(symTable *,symbol,symTable *);
void buildLCA();
void checkRedef();
void error(std::string,int);
symbol searchTillRoot(symTable *,symbol);
std::string getReturnType(std::string);
std::string getArgTypes(std::string);
extern ProgramNode *root;
#endif
	
