// Bison quack.y file based on tutorial from http://aquamentus.com/flex_bison.html
%{

  #include <iostream>
  #include <string>
  #include <list>
  #include "Node.h"
  #include "lex.yy.h"
  #include "quack.tab.h"

  using namespace std;
  int printparse=0;

  void msg (std::string s) {
    if (printparse)
      std::cout << s << std::endl;
  }

  /* extern struct ProgramNode *root; */
  ProgramNode *root = new ProgramNode;
  
  /* int yylex(); */
  extern void yyerror(const char *msg);
%}

// Bison by defines a C union holding each of the types of tokens that Flex could return,
// and have Bison use that union instead of "int" for the definition of "yystype":
%union {
  int num;
  float fval;
  const char* str;
  struct ProgramNode* prog;
  struct classesNode* classesN;
  struct classNode* classN;
  struct statementsNode* statementsN;
  struct statementNode* statementN;
  struct classSignatureNode* classSigN;
  struct classBodyNode* classBodyN;
  struct rExprNode* rExprN;
  struct lExprNode* lExprN;
  struct statementBlockNode* sBlock;
  struct elifsNode* elifs;
  struct elifNode* elif;
  struct methodsNode* methods;
  struct methodNode* method;
  struct methodReturnNode* methodReturn;
  struct elseNode* elsE;
  struct rExprsNode* rExprNs;
  struct actualArgsNode* ActArgs;
  struct formalArgumentsNode* fArgs;
  struct argumentsNode* Args;
  struct argumentNode* Arg; } // define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the union:

// KEYWORDS
%token CLASS "class"
%token DEF EXTENDS ELIF ELSE WHILE RETURN
%token IF 
%token AND "and" 
%token OR "or" 
%token NOT "not"
%token SEMICOLON ";"
%token COLON ":"
%token LPAREN "("
%token RPAREN ")"
%token DOT "."
%token COMMA ","
%token PLUS "+"
%token MINUS "-"
%token NEG 
%token TIMES "*"
%token DIV "/"
%token EQUIV "=="
%token LEQ "<="
%token GEQ ">="
%token LT "<"
%token GT ">" 
%token EQUALS "="
%token RCURLY "{"
%token LCURLY "}"

// PUNCTUATION
//%left ATMOST ATLEAST
//%left EQUALS LESS MORE
//%left NEG
//%right PLUS MINUS
//%left MULT DIVIDE
//%left DOT

//%right EQUALS LT GT LEQ GEQ
//%left MINUS PLUS
//%left DIF TIMES
//%left LPAREN
//%left RPAREN
//%left LCURLY
//%left RCURLY
//%nonassoc EQUIV AND OR NOT

%left  AND OR NOT
%left EQUIV LT GT LEQ GEQ
%left NEG
%left PLUS MINUS 
%left TIMES DIV 
%left DOT

// IDENTIFIERS
%token <str> IDENT
// INTEGER AND STRING LITERALS
%token <str> INT_LIT STRING_LIT

%type <prog> Program
%type <classesN> Classes
%type <classN> Class
%type <statementsN> Statements
%type <statementN> Statement
%type <classSigN> Class_Signature
%type <classBodyN> Class_Body
%type <str> Class_Sig_Extends
%type <rExprN> R_Expr
%type <rExprNs> R_Exprs
%type <lExprN> L_Expr
%type <sBlock> Statement_Block 
%type <elifs> Elifs 
%type <elif> elif 
%type <methods> Methods 
%type <method> Method 
%type <methodReturn> Method_Opt 
%type <elsE> Else 
%type <ActArgs> Actual_Args
%type <fArgs> Formal_Args
%type <Args> arguments
//%type <Arg> argument

%start Program
%%
// ----GRAMMAR----
Program
: Classes Statements {;msg("Program: Class Statement");
   classesNode *c=$1;
   root->classes=*c;
   statementsNode *s=$2;
   root->statements=*s;
 }
;

Classes
: /* empty */ { $$ = new classesNode; }
| Classes Class { msg("Classes: Classes Class");
   classesNode *c=$1; classNode*n=$2;
   c->list.push_back(*n);
 }
;

Statements
: /* empty */ { $$ = new statementsNode; }
| Statements Statement {msg("Statements: Statement Statements");
   statementsNode *c=$1; statementNode*n=$2;
   c->list.push_back(*n);
 }

;

Class
: Class_Signature Class_Body
{msg("Class: Class_Signature Class_body");
  classNode *n = new classNode;
  n->sig=$1;
  n->classBody=$2;
  $$=n;
}
;

Class_Signature
: "class" IDENT "(" Formal_Args ")" Class_Sig_Extends
  { msg("Class_Signature: class IDENT ( Formal_Args )");
    classSignatureNode *n = new classSignatureNode;
    n->name=$2; //cout<<$2<<endl;
    n->fArguments=$4; //cout<<$2<<endl;
    n->extends=$6;
    $$ = n;
  }
;

Class_Sig_Extends
: /* empty */ {$$="Obj";}
| EXTENDS IDENT {$$=$2;}
;

Formal_Args
:  { $$ = new formalArgumentsNode; }
| IDENT ":" IDENT arguments {
  formalArgumentsNode *n = new formalArgumentsNode;
  argumentNode *argNode = new argumentNode;
  argNode->name=$1;
  argNode->type=$3;

  argumentsNode *tmpList = $4;
  std::vector<argumentNode*> argList = tmpList->list;
  argList.push_back(argNode);
  n->list=argList;
  $$=n; 
  msg("Formal_Args: IDENT : IDENT Idents");
}
;

arguments
: { $$ = new argumentsNode; }
| "," IDENT ":" IDENT arguments {
  argumentsNode *n = $5;
  argumentNode *node = new argumentNode;
  node->name=$2;
  node->type=$4;
  n->list.push_back(node);
  $$=n;
  msg("Idents: Idents Ident");
}
;
/*

Formal_Args
: 
| IDENT ":" IDENT Idents {msg("Formal_Args: IDENT : IDENT Idents");}
;

Idents
: 
| Idents Ident {msg("Idents: Idents Ident");}
;

Ident
: "," IDENT ":" IDENT {msg("Ident: , IDENT : IDENT");}
*/

Class_Body : "{" Statements Methods "}" {msg("Class_Body: { Statements Methods }");
   classBodyNode *node = new classBodyNode;
   node->statements=$2;
   node->methods=$3;
   $$=node;
};

Statement
: WHILE R_Expr Statement_Block {
   statementNode *node = new statementNode;
   node->str="WHILE";
   node->rExpr=$2;
   node->stblock=$3;
   $$=node;

  msg("Statement:WHILE R_Expr Statement_Block");
}
| IF R_Expr Statement_Block Elifs Else {
   statementNode *node = new statementNode;
   node->str="IF";
   node->rExpr=$2;
   node->stblock=$3;
   node->elifs=$4;
   node->elseN=$5;
   $$=node;
   msg("Statement: IF R_Expr quack");
} 
| Statement_Block {
   statementNode *node = new statementNode;
   node->stblock=$1;
   $$=node;
   msg("Statement: Statement_Block");

}
| L_Expr "="  R_Expr ";" {

   statementNode *node = new statementNode;
   node->str="assignment";
   node->rExpr=$3;
   node->lExpr=$1;
   $$=node;
   msg("Statement: L_Expr = R_Expr ;");
}

| L_Expr ":" IDENT "=" R_Expr ";" {
   statementNode *node = new statementNode;
   node->str="assignment longer";
   node->name=$3;
   node->rExpr=$5;
   node->lExpr=$1;
   $$=node;
   msg("Statement: L_Expr : IDENT = R_Expr ;");
   }
| R_Expr ";" {
   statementNode *node = new statementNode;
   node->rExpr=$1;
   $$=node;
   msg("Statement: R_Expr ;");
   }
| RETURN ";" {
   statementNode *node = new statementNode;
   $$=node;
   msg("Statement: RETURN ;");
   }
| RETURN R_Expr ";" {
   statementNode *node = new statementNode;
   node->rExpr=$2;
   $$=node;
   msg("Statement: RETURN R_Expr ;");
}
;

Elifs: { $$=new elifsNode;}
        |Elifs elif {
	elifsNode *c=$1; elifNode *n=$2;
        c->list.push_back(n);
	}
        ;
elif: ELIF R_Expr Statement_Block {
        elifNode *n = new elifNode;
	n->rExpr=$2;
	n->statementBlock=$3;
	$$=n;	
	}
	;
Else
: {
  $$=new elseNode;
  } 
| ELSE Statement_Block {
  elseNode *node=new elseNode;
  node->statementBlock=$2;
  $$=node;
  msg("Else: ELSE Statement_Block");
  }
;

Statement_Block
: "{" Statements  "}" {
   statementBlockNode *node = new statementBlockNode;
   statementsNode *s=$2;//monil 
   node->statements=s;
   $$=node;
msg("Statement_Block: { Statement }");
}
;


Methods
:  {
   $$=new methodsNode;
   } 
| Methods Method {
   methodsNode *c=$1; methodNode *n=$2;
   c->list.push_back(*n);
   msg("Methods: Method Methods");
   }
;

Method
: DEF IDENT "(" Formal_Args ")" Method_Opt Statement_Block {
   methodNode *n = new methodNode;
   n->statementBlock=$7;
   n->fArguments=$4;
   n->methodReturn=$6;
   n->name=$2;
   $$=n;
   msg("Method: DEF IDENT ( Formal_args ) Method_Opt Statement_Block");
   }
;

Method_Opt
:  {
   $$=new methodReturnNode;
   } 
| ":" IDENT {
   methodReturnNode *n = new methodReturnNode;
   n->name=$2;
   $$=n;
   msg("Method_Opt: : IDENT");
}
;

L_Expr
: IDENT {
   lExprNode *node=new lExprNode;
   node->str=$1;
   node->name=$1;
   $$=node;
   msg("L_Expr: IDENT");
   }
| R_Expr "." IDENT {
   lExprNode *node=new lExprNode;
   node->str=$3;
   node->name=$3;
   node->rExpr=$1;
   $$=node;
   msg("L_Expr: R_Expr . IDENT");
}
;

R_Expr
: STRING_LIT {msg("R_Expr: STRING_LIT"); rExprNode *rN = new rExprNode; rN->str=$1; $$ = rN;} 
| INT_LIT { msg("R_Expr: INT_LIT"); rExprNode *rN = new rExprNode; rN->val=std::stoi($1); $$ = rN;}
| L_Expr {
   rExprNode *rN = new rExprNode;
   rN->lExpr=$1; 
   $$=rN;
   msg("R_Expr: L_Expr");
   }

/*l%left EQUALS LT GT LEQ GEQ */
| R_Expr PLUS R_Expr { 
   rExprNode *rN = new rExprNode;
   rN->rExprFirst=$1;
   rN->rExprSecond=$3;
   $$=rN;
   msg("R_Expr: R_Expr + R_Expr");
   }
| R_Expr MINUS R_Expr {
   rExprNode *rN = new rExprNode;
   rN->rExprFirst=$1;
   rN->rExprSecond=$3;
   $$=rN;
   msg("R_Expr: R_Expr - R_Expr");
   }
| R_Expr TIMES R_Expr {
   rExprNode *rN = new rExprNode;
   rN->rExprFirst=$1;
   rN->rExprSecond=$3;
   $$=rN;
   msg("R_Expr: R_Expr * R_Expr");
   }
| R_Expr DIV R_Expr {
   rExprNode *rN = new rExprNode;
   rN->rExprFirst=$1;
   rN->rExprSecond=$3;
   $$=rN;
   msg("R_Expr: R_Expr / R_Expr");
   }
| MINUS R_Expr %prec NEG {
   rExprNode *rN = new rExprNode;
   rN->rExprFirst=$2;
   $$=rN;
   msg("R_Expr: - R_Expr");
   } 
| R_Expr EQUIV R_Expr {
   rExprNode *rN = new rExprNode;
   rN->rExprFirst=$1;
   rN->rExprSecond=$3;
   $$=rN;
   msg("R_Expr: R_Expr == R_Expr");
   }
| R_Expr LEQ R_Expr {
   rExprNode *rN = new rExprNode;
   rN->rExprFirst=$1;
   rN->rExprSecond=$3;
   $$=rN;
   msg("R_Expr: R_Expr <= R_Expr");
   }
| R_Expr LT R_Expr {
   rExprNode *rN = new rExprNode;
   rN->rExprFirst=$1;
   rN->rExprSecond=$3;
   $$=rN;
   msg("R_Expr: R_Expr < R_Expr");
   }
| R_Expr GEQ R_Expr {
   rExprNode *rN = new rExprNode;
   rN->rExprFirst=$1;
   rN->rExprSecond=$3;
   $$=rN;
   msg("R_Expr: R_Expr >= R_Expr");
   }
| R_Expr GT R_Expr {
   rExprNode *rN = new rExprNode;
   rN->rExprFirst=$1;
   rN->rExprSecond=$3;
   $$=rN;
   msg("R_Expr: R_Expr > R_Expr");
   }
| R_Expr AND R_Expr {
   rExprNode *rN = new rExprNode;
   rN->rExprFirst=$1;
   rN->rExprSecond=$3;
   $$=rN;
   msg("R_Expr: R_Expr AND R_Expr");
   }
| R_Expr OR R_Expr {

   rExprNode *rN = new rExprNode;
   rN->rExprFirst=$1;
   rN->rExprSecond=$3;
   $$=rN;
   msg("R_Expr: R_Expr OR R_Expr");
   }
| NOT R_Expr {
   rExprNode *rN = new rExprNode;
   rN->rExprFirst=$2;
   $$=rN;
   msg("R_Expr: NOT R_Expr");
   } 

| R_Expr "." IDENT "(" Actual_Args ")" {
  //constructorNode *cN = new constructorNode; rExprNode *rN = new rExprNode;
  //cN->name=$3; rN->constructor=cN; rN->rExprFirst=$1; 
  rExprNode *rN = new rExprNode;
  rN->rExprFirst=$1; 
  rN->rExprSecond=NULL; 
  rN->name=$3;
  rN->actualArgs=$5;
  rN->str="method";
  $$=rN;
  msg("R_Expr: R_Expr . IDENT ( Actual_Args )");}
| IDENT "(" Actual_Args ")"{ 
  //constructorNode *cN = new constructorNode; rExprNode *rN = new rExprNode;
  //cN->name=$1; rN->constructor=cN; 
  //rN->str="constructor"; $$=rN; 
  rExprNode *rN = new rExprNode;
  rN->rExprFirst=NULL; 
  rN->name=$1; 
  rN->str="const";
  rN->actualArgs=$3;
  $$=rN;
  msg("R_Expr: IDENT ( Actual_Args )");
}
| "(" R_Expr ")" {
  rExprNode *rN = new rExprNode;
  rN->rExprFirst=$2; 
  rN->rExprSecond=NULL; 
  //rN->str="const"; 
  $$=rN;
  msg("R_Expr: ( R_Expr )");}
;

Actual_Args
: {
  $$=new actualArgsNode; 
  }
| R_Expr R_Exprs {
  actualArgsNode *rN = new actualArgsNode;
  rN->rExpr=$1;
  rN->rExprs=$2;
  $$=rN;
  msg("Actual_Args: R_Expr R_Exprs");
  }
;

R_Exprs
: {
  $$=new rExprsNode; 
  }
| R_Exprs "," R_Expr {
  rExprsNode *c=$1;
  rExprNode *n=$3;
  c->list.push_back(n);
  $$=c;
  msg("R_Exprs: , R_Expr");
  }
;

