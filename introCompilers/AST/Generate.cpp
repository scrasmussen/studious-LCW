#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Node.h"
#include "Generate.h"

int FIN=1;

void genStructLine(std::ofstream &f,char const* name) {
  f<<"struct class_"<<name<<"_struct;\n";
}
void genTypeDefStructLine(std::ofstream &f,char const* name){
  f<<"typedef struct class_"<<name<<"_struct* class_Pt;\n";
}
void genClassStructLine(std::ofstream &f,char const* name) {
  f<<"struct class_"<<name<<"_struct the_class_"<<name<<"_struct;\n";
}
void genTheClassExternLine(std::ofstream &f,char const* name) {
  f<<"extern class_"<<name<<" the_class_"<<name<<";\n";
}

void genClassArgLines(std::ofstream &f, classSignatureNode *n) {
  f<<"typedef struct obj_"<<n->name<<"_struct {\n";
  f<<"  class_"<<n->name<<" clazz;\n";
  for (int i = n->fArguments->list.size(); i --> 0; ) {
    auto a = n->fArguments->list[i];
    f<<"  obj_"<<a->type<<" "<<a->name<<";\n";
  }
  f<<"} * obj_"<<n->name<<";\n";
}


void writeType(std::ofstream &f,char const* type)
{
  if (strcmp(type,"Int")==0)
    f<<"int";
}

void genLExpr(lExprNode *n, std::ofstream &f, int act)
{
  if (act==GENSTATEMENTS)
    f<<n->name;
}

void genStatement(statementNode *n, std::ofstream &f, int act)
{
  if (n->lExpr!=NULL)
    if (act==GENSTATEMENTS)
      genLExpr(n->lExpr, f, act);

  if (act==GENSTATEMENTS)
    if (strcmp(n->str,"ASSIGN")==0)
      f<<"=";

  // NEED TO FIGURE OUT CORRECT ORDER TO TRAVERSE THESE NODES
  /*
    if (n->rExpr!=NULL)
      checkRExpr(n->rExpr, classNames, act);

    if (n->stblock!=NULL)
      checkStatementBlock(n->stblock, classNames, act);

    if (n->elifs!=NULL)
      checkElifs(n->elifs, classNames, act);

    if (n->elseN!=NULL)
      checkElse(n->elseN, classNames, act);
  */
}


void genSignature(classSignatureNode *n, std::ofstream &f, int act)
{
  // name, extends, fArguments

  /* gen struct and typedef struct */
  genStructLine(f,n->name);
  genTypeDefStructLine(f,n->name);
  genClassStructLine(f,n->name);
  genTheClassExternLine(f,n->name);
  
  f<<std::endl;
  genClassArgLines(f,n);
}

void genMethodArgTypes(formalArgumentsNode *n, std::ofstream &f)
{
  int arity=0;
  for (argumentNode *a : n->list) {
    if (arity==0) {
      arity=1;
      f<<"obj_"<<a->type;
    }
    else
      f<<", obj_"<<a->type;
  }
}

void genMethodArgs(formalArgumentsNode *n, std::ofstream &f)
{
  int arity=0;
  for (int i = n->list.size(); i --> 0; ) {
    auto a = n->list[i];
    if (arity==0) {
      arity=1;
      f<<"obj_"<<a->type;
      f<<" "<<a->name;
    }
    else {
      f<<", obj_"<<a->type;
      f<<" "<<a->name;
    }
  }
}

void genFuncPointerFor(std::string str, std::string returnstr, classNode *n, std::ofstream &f)
{
  int found=0;
  for (symbol s : n->sTable->table) {
    if (s.name==str)
      found=1;
  }
  if (!found && strcmp(n->sig->extends,"Obj")==0) {
    f<<"  obj_" <<returnstr;
    f<<" (*"<<str<<") (obj_Obj);\n";
  }
  // ARTLESS TODO: WHAT ABOUT BOOLEAN?
}

void genBuiltinFuncPointers(classNode *n, std::ofstream &f, char const *name)
{
  // Constructor Function Pointer
  f<<"  obj_"<<name<<" (*constructor) (";
  genMethodArgTypes(n->sig->fArguments,f);
  f<<");\n";

  // Builtin Classes
  genFuncPointerFor("STRING","String",n,f);
  genFuncPointerFor("PRINT","Obj",n,f);  
  // genFuncPointerFor("EQUALS","Boolean",n,f);

  // std::string returnType=getReturnType(s.type);
  // std::string argType=getArgTypes(s.type);
  // f<<"  obj_"<<returnType<<" (*"<<s.name<< ") (";
  // if (argType.length()==2)
  // 	f<<")\n";
  // else
  // 	f<<"TODO:"<<argType<<")\n";
}

void genClassFuncPointerStruct(classNode *n, std::ofstream &f, char const *name, int act)
{
  f<<"struct class_"<<name<<"_struct {\n";
  // Generate guaranteed built in methods
  genBuiltinFuncPointers(n,f,name);
  
  for (methodNode m : n->classBody->methods->list) {
    f<<"  obj_"<<m.returnType<<" (*"<<m.name<<") (";
    // Generate method arg types
    genMethodArgTypes(m.fArguments,f);
    f<<");\n";
  }
  f<<"}\n";
}

void genLExprBit(lExprNode *n, std::ofstream &f, char const *name)
{
  if (strcmp(n->str,"")==0)
    f<<"  "<<n->name;
  if (strcmp(n->str,".")==0) {
    genRExprBit(n->rExpr,f,name);
    f<<"."<<n->name;
  }
}

void genRExprBit(rExprNode *n, std::ofstream &f, char const *name)
{
  if (strcmp(n->str,"string_lit")==0)
    f<<n->name;
  if (strcmp(n->str,"lexpr")==0) {
    genLExprBit(n->lExpr,f,name);
  }
}

void genConstructor(classNode *n, std::ofstream &f, char const *name, int act)
{
  f<<"obj_"<<name<<" new_"<<name<<"(";

  genMethodArgs(n->sig->fArguments, f);
  f<<") {\n";
  f<<"  obj_"<<name<<" item = (obj_"<<name<<")\n";
  f<<"  malloc(sizeof(struct obj_"<<name<<"_struct));\n";
  f<<"  item->clazz = the_class_"<<name<<";\n";

  for (statementNode &s : n->classBody->statements->list) {
  if (s.lExpr!=NULL) {
  genLExprBit(s.lExpr,f,name);
  if (strcmp(s.str,"ASSIGN")==0)
    f<<" = ";
  genRExprBit(s.rExpr,f,name);
}
  f<<";\n";
}

  f<<"  return item;\n";
  f<<"}\n";
}


void genStructs(classNode *n, std::ofstream &f, char const *name, int act)
{
  // THIS WILL BE THE FUNCTION POINTERS
  genClassFuncPointerStruct(n,f,name,act);

  // THIS WILL BE THE STRUCT
  if (n->classBody!=NULL && n->classBody->statements!=NULL) {
    f<<"\n";
    genConstructor(n,f,name,act);
    for (methodNode &s : n->classBody->methods->list) {
      // genClassStructVar(&s,f,act);
    }

  }

}

void genClass(classNode *n, std::ofstream &f, int act)
{
  if (n->sig != NULL)
    genSignature(n->sig, f, act);
  f<<std::endl;
  if (n->classBody != NULL)
    genStructs(n, f, n->sig->name, act);
}

void genProgram(ProgramNode *n, std::ofstream &f, int act)
{
  if (act==GENSTART) {
    f<<"#include <stdio.h>\n";
    f<<"#include \"Builtins.h\"\n\n";
  }

  if (act==GENCLASSES)
  for (auto &c : n->classes.list)
    genClass(&c, f, act);

  
  if (act==GENSTATEMENTS) {
    f<<"\nint main()\n{\n";
    for (statementNode &s : n->statements.list) {
      genStatement(&s,f,act);
      f<<";"<<std::endl;
    }

    if (FIN) f<<"printf(\"Fin\\n\");\n";
    f<<"}\n\n";
  }

}

void generate(){
  std::ofstream f("output.c");

  if (!f.is_open())
    std::cerr<<"Error opening output.c to generate code\n";
  genProgram(root, f, GENSTART);
  genProgram(root, f, GENCLASSES);
  genProgram(root, f, GENSTATEMENTS);

  f.close();
  std::cout<<"Fin Generate"<<std::endl;
}
