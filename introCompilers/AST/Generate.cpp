#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Node.h"
#include "Generate.h"

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
  if (n->rExpr!=NULL) {
     if (act==BUILDSYMBOLTABLE) {
      //std::cout<<"DE"<<std::endl;
       n->rExpr->sTable=n->sTable;
       //n->sTable->print();
    }
    checkRExpr(n->rExpr, classNames, act);
  }

 if (n->stblock!=NULL) {
     if (act==BUILDSYMBOLTABLE) {
        n->stblock->sTable=n->sTable; 
      // n->stblock->sTable=n->sTable;
    }
     checkStatementBlock(n->stblock, classNames, act);
  }

  if (n->elifs!=NULL) {
    if (act==BUILDSYMBOLTABLE) {
      n->elifs->sTable=n->sTable->prev;
    }
    checkElifs(n->elifs, classNames, act);
  }

  if (n->elseN!=NULL) {
    if (act==BUILDSYMBOLTABLE) {
      n->elseN->sTable=n->sTable->prev;
    }
   checkElse(n->elseN, classNames, act);
  }
  */
}


void genSignature(classSignatureNode *n, std::ofstream &f, int act)
{
  // name, extends, fArguments
  f<<n->name<<"(";
  int arity=0;

  for (int i = n->fArguments->list.size(); i --> 0; ) {
    auto a = n->fArguments->list[i];
    if (arity==0)
      arity=1;
    else
      f<<",";
    writeType(f,a->type);
    f<<" ";
    f<<a->name;
  }
  f<<"){}\n";
}

void genClassBody(classBodyNode *n, std::ofstream &f, int act)
{
  if (n->statements!=NULL) {
    // for (statementNode &s : n->statements.list) {
    //   genStatement(&s,f,act);
    //   f<<";"<<std::endl;
    // }
  }
  if (n->methods!=NULL) {
    // for (methodNode &s : n->statements.list) {
    //   genMethod(&s,f,act);
    //   f<<";"<<std::endl;
    // }
  }

}

void genClass(classNode *n, std::ofstream &f, int act)
{
  if (n->sig != NULL)
    genSignature(n->sig, f, act);
  if (n->classBody != NULL)
    genClassBody(n->classBody, f, act);
}

void genProgram(ProgramNode *n, std::ofstream &f, int act)
{
  if (act==GENSTART) {
    f<<"#include <stdio.h>\n";
  }

  if (act==GENCLASSES)
  for (auto &c : n->classes.list)
    genClass(&c, f, act);

  
  if (act==GENSTATEMENTS) {
    f<<"void main()\n{\n";
    for (statementNode &s : n->statements.list) {
      genStatement(&s,f,act);
      f<<";"<<std::endl;
    }
    f<<"}\n";
  }

}

void generate(){
  std::ofstream f;
  f.open("output.c");
  if (!f.is_open())
    std::cerr<<"Error opening output.c to generate code\n";
  genProgram(root, f, GENSTART);
  genProgram(root, f, GENCLASSES);
  genProgram(root, f, GENSTATEMENTS);

  f.close();
  std::cout<<"Fin Generate"<<std::endl;
}
