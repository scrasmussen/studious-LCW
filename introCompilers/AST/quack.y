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
  char* str;
  struct ProgramNode* prog;
  struct classesNode* classesN;
  struct classNode* classN;
  struct classSignatureNode* classSigN;
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
%type <classSigN> Class_Signature

%start Program
%%
// ----GRAMMAR----
Program
: Classes Statements { /*result = $1*/ ;msg("Program: Class Statement");
   classesNode *c=$1;
   std::cout<<c->list.size()<<std::endl;
   /* std::cout<<root->node->t<<std::endl; */
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
: /* empty */
| Statements Statement {msg("Statements: Statement Statements");}

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
    $$ = n;
    /* std::cout<<$2<<n->val<<std::endl; */
  }
;

Class_Sig_Extends
: /* empty */
| EXTENDS IDENT
;

Formal_Args
: /* empty */
| IDENT ":" IDENT Idents {msg("Formal_Args: IDENT : IDENT Idents");}
;

Idents
: /* empty */ 
| Idents Ident {msg("Idents: Idents Ident");}
;

Ident
: "," IDENT ":" IDENT {msg("Ident: , IDENT : IDENT");}

Class_Body : "{" Statements Methods "}" {msg("Class_Body: { Statements Methods }");};

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
: STRING_LIT {msg("R_Expr: STRING_LIT");} 
| INT_LIT {/*printf("%s+\n",$1);*/
   msg("R_Expr: INT_LIT");}
| L_Expr {msg("R_Expr: L_Expr");}
/* ARTLESS PRESIDENTZ */
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
/* ARTLESS BOOLEAN? */
| R_Expr "AND" R_Expr {msg("R_Expr: R_Expr AND R_Expr");}
| R_Expr "OR" R_Expr {msg("R_Expr: R_Expr OR R_Expr");}
| "NOT" R_Expr {msg("R_Expr: NOT R_Expr");}
| R_Expr "." IDENT "(" Actual_Args ")" {msg("R_Expr: R_Expr . IDENT ( Actual_Args )");}
| IDENT "(" Actual_Args ")" {msg("R_Expr: IDENT ( Actual_Args )");}
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

