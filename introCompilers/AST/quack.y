// Bison quack.y file based on tutorial from http://aquamentus.com/flex_bison.html
%{

  #include <iostream>
  #include <string>
  #include <list>
  #include "Node.h"
  #include "lex.yy.h"
  #include "quack.tab.h"


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
  struct rExprNode* rExprN;
  struct formalArgumentsNode* formalA;
}

// define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the union:

// KEYWORDS
%token CLASS "class"
%token DEF EXTENDS ELIF ELSE WHILE RETURN
%token IF
%token AND OR NOT

%token SEMICOLON ";"
%token COLON ":"
%token LPAREN "("
%token RPAREN ")"
%token DOT "."
%token COMMA ","
%token PLUS "+"
%token MINUS "-"
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
%right EQUALS LT GT LEQ GEQ
%left MINUS PLUS
%left DIF TIMES
%left LPAREN
%left RPAREN
%left LCURLY
%left RCURLY
%nonassoc EQUIV AND OR NOT

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
%type <formalA> Formal_Args 
%type <str> Class_Sig_Extends
%type <rExprN> R_Expr


%start Program
%%
// ----GRAMMAR----
Program
: Classes Statements { /*result = $1*/ ;msg("Program: Class Statement");
   classesNode *c=$1;
   //statementsNode *s=$2;
   root->classes=*c;
   //root->statements=*s;
   // Add    root->statementss=*s;
   /* std::cout<<c->list.size()<<std::endl; */
   /* std::cout<<root->node->t<<std::endl; */
 }
;

Classes
: /* empty */ { $$ = new classesNode; }
| Classes Class { msg("Classes: Classes Class");
   classesNode *c=$1; classNode *n=$2;
   c->list.push_back(*n);
 }
;

Statements
: /* empty */ { //$$ = new statementsNode; 
}
| Statements Statement {msg("Statements: Statement Statements");
   //statementsNode *s=$1; statementNode*n=$2;
   //s->list.push_back(*n);
 }

;

Class
: Class_Signature Class_Body
{msg("Class: Class_Signature Class_body");
  classNode *n = new classNode;
  classSignatureNode *csn = $1;
  n->sig=*csn;
  $$=n;
}
;

Class_Signature
: "class" IDENT "(" Formal_Args ")" Class_Sig_Extends
  { msg("Class_Signature: class IDENT ( Formal_Args )");
    classSignatureNode *n = new classSignatureNode;
    n->name=$2;
    n->extends=$6;
    formalArgumentsNode *f=$4;
    n->fargs=$4;
    $$ = n;
  }
;

Class_Sig_Extends
: /* empty */ {$$="Obj";}
| EXTENDS IDENT {$$=$2;}
;
Formal_Args: {$$=new formalArgumentsNode; 
}
        | "," IDENT ":" IDENT Formal_Args {
    	/*formalArgumentsNode *c=$5; 
	argumentNode n;
	n.type=$2;
	n.name=$4;        
   	c->list.push_back(n);*/
	} 
        | IDENT ":" IDENT Formal_Args {
    	//formalArgumentsNode *c=$4; 
	//argumentNode n=new argumentNode;
	//n.type=$1;
	//n.name=$3;        
   	//c->list.push_back(n);
	}       
        ;
/*arguments:
         IDENT COLON IDENT "," arguments {
	argumentNode *n;
	n->type=$1; n->name=$3;
   	c->list.push_back(*n);
    	formalArgumentsNode *f=$4;
        c->list.push_back(*f);
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
};

Statement
: WHILE R_Expr Statement_Block {msg("Statement:WHILE R_Expr Statement_Block");}
| IF R_Expr Statement_Block Elifs Else {msg("Statement: IF R_Expr quack");} 
| Statement_Block {msg("Statement: Statement_Block");}
| L_Expr "="  R_Expr ";" {msg("Statement: L_Expr = R_Expr ;");}
| L_Expr ":" IDENT "=" R_Expr ";" {msg("Statement: L_Expr : IDENT = R_Expr ;");}
| R_Expr ";" {msg("Statement: R_Expr ;");}
| RETURN ";" {msg("Statement: RETURN ;");}
| RETURN R_Expr ";" {msg("Statement: RETURN R_Expr ;");}
;

Elifs
: /* empty */
| ELIF R_Expr Statement_Block Elifs {msg("Elifs: ELIF R_Expr Statement_Block");}
;

Else
: /* empty */
| ELSE Statement_Block {msg("Else: ELSE Statement_Block");}
;

Statement_Block
: "{" "}" {msg("Statement_Block: { }");}
| "{" Statements  "}" {msg("Statement_Block: { Statement }");}
;

Methods
: /* empty */
| Methods Method {msg("Methods: Method Methods");}
;

Method
: DEF IDENT "(" Formal_Args ")" Method_Opt Statement_Block {msg("Method: DEF IDENT ( Formal_args ) Method_Opt Statement_Block");}
;

Method_Opt
: /* empty */
| ":" IDENT {msg("Method_Opt: : IDENT");}
;

L_Expr
: IDENT {msg("L_Expr: IDENT");}
| R_Expr "." IDENT {msg("L_Expr: R_Expr . IDENT");}
;

R_Expr
: STRING_LIT {msg("R_Expr: STRING_LIT"); rExprNode *rN = new rExprNode; rN->str=$1; $$ = rN;} 
| INT_LIT { msg("R_Expr: INT_LIT"); rExprNode *rN = new rExprNode; rN->val=std::stoi($1); $$ = rN;}
| L_Expr {msg("R_Expr: L_Expr");}

| R_Expr "+" R_Expr { msg("R_Expr: R_Expr + R_Expr");}
| R_Expr "-" R_Expr {msg("R_Expr: R_Expr - R_Expr");}
| R_Expr "*" R_Expr {msg("R_Expr: R_Expr * R_Expr");}
| R_Expr "/" R_Expr {msg("R_Expr: R_Expr / R_Expr");}
| "-" R_Expr {msg("R_Expr: - R_Expr");}
| R_Expr "==" R_Expr {msg("R_Expr: R_Expr == R_Expr");}
| R_Expr "<=" R_Expr {msg("R_Expr: R_Expr <= R_Expr");}
| R_Expr "<" R_Expr {msg("R_Expr: R_Expr < R_Expr");}
| R_Expr ">=" R_Expr {msg("R_Expr: R_Expr >= R_Expr");}
| R_Expr ">" R_Expr {msg("R_Expr: R_Expr > R_Expr");}

| R_Expr "AND" R_Expr {msg("R_Expr: R_Expr AND R_Expr");}
| R_Expr "OR" R_Expr {msg("R_Expr: R_Expr OR R_Expr");}
| "NOT" R_Expr {msg("R_Expr: NOT R_Expr");}
| R_Expr "." IDENT "(" Actual_Args ")" {msg("R_Expr: R_Expr . IDENT ( Actual_Args )");}
| IDENT "(" Actual_Args ")"
{ constructorNode *cN = new constructorNode; rExprNode *rN = new rExprNode;
  cN->name=$1; rN->constructor=*cN; $$=rN;
  msg("R_Expr: IDENT ( Actual_Args )");}
| "(" R_Expr ")" {msg("R_Expr: ( R_Expr )");}
;

Actual_Args
: /* empty */
| R_Expr R_Exprs {msg("Actual_Args: R_Expr R_Exprs");}
;

R_Exprs
: /* empty */
| "," R_Expr {msg("R_Exprs: , R_Expr");}
;

