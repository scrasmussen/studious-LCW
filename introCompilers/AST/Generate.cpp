#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Node.h"
#include "Generate.h"

void genLExpr(lExprNode *n, std::ofstream &f, int act)
{
  if (act==GENSTATEMENT)
    f<<n->name;
  //   std::cout<<n->str<<" "<<n->name<<std::endl;
}

void genStatement(statementNode *n, std::ofstream &f, int act)
{
  if (n->lExpr!=NULL)
    if (act==GENSTATEMENT)
      genLExpr(n->lExpr, f, act);

  if (act==GENSTATEMENT)
    if (strcmp(n->str,"ASSIGN")==0)
      f<<"=";
  
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

void genProgram(ProgramNode *n, std::ofstream &f, int act)
{
  if (act==GENSTART) {
    f<<"#include <iostream>\n";
  }

  // TODO CLASS NODES
  
  if (act==GENSTATEMENT) {
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
  genProgram(root, f, GENSTATEMENT);

  f.close();
  std::cout<<"Fin Generate"<<std::endl;
}
