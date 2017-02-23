#include <iostream>
#include <string>
#include <vector>
#include "Node.h"

void checkConstructorClass(const char* name, std::vector<char const*> *classNames)
{
  int found = 0;
  for (char const * a : *classNames) {
    if (strcmp(name,a)==0) {
      found = 1;
    }
  }
  if (!found)
    std::cerr << "Constructor Class \""<< name<<"\" Not Defined" << std::endl;
 
}

void checkActualArgs(actualArgsNode *n, std::vector<char const*> *classNames,  int act)
{
  if (act==PRINT) std::cout<<"actualArgsNode: "<<std::endl;  
  if (n->rExpr!=NULL)
    checkRExpr(n->rExpr, classNames, act);
  if (n->rExprs!=NULL)
      for (rExprNode *e : n->rExprs->list)
	checkRExpr(e, classNames, act);
  
}

void checkRExpr(rExprNode *n, std::vector<char const*> *classNames,  int act)
{
  if (act==PRINT){
    std::cout<<"rExprNode: "<<n->name;
    if (strcmp(n->str,"")!=0) std::cout<<" str: "<<n->str<<std::endl;
    else std::cout<<std::endl;
  }
  if ( strcmp(n->str,"const") == 0 && act == CHECKCONSTRUCTORCALLS)
    checkConstructorClass(n->name, classNames);

  if (n->rExprFirst!=NULL)
    checkRExpr(n->rExprFirst, classNames, act);
  if (n->rExprSecond!=NULL)
    checkRExpr(n->rExprSecond, classNames, act);
  if (n->lExpr!=NULL)
    checkLExpr(n->lExpr, classNames, act);
  if (n->actualArgs!=NULL)
    checkActualArgs(n->actualArgs, classNames, act);
}

void checkLExpr(lExprNode *n, std::vector<char const*> *classNames,  int act)
{
  if (act==PRINT) std::cout<<"lExprNode: "<< n->name << std::endl;  
  if (n->rExpr!=NULL)
    checkRExpr(n->rExpr, classNames, act);
}


void checkStatementBlock(statementBlockNode *n, std::vector<char const*> *classNames,  int act)
{
  if (act==PRINT) std::cout<<"statementBlockNode"<< std::endl;  
  if (n->statements!=NULL)
    for (statementNode s : n->statements->list)
      checkStatement(s, classNames, act);
}

void checkElif(elifNode *n, std::vector<char const*> *classNames,  int act)
{
  if (act==PRINT) std::cout<<"elifNode"<< std::endl;  
  if (n->rExpr!=NULL)
    checkRExpr(n->rExpr, classNames, act);
  if (n->statementBlock!=NULL)
    checkStatementBlock(n->statementBlock, classNames, act);
}

void checkElifs(elifsNode *n, std::vector<char const*> *classNames,  int act)
{
  for (elifNode *e : n->list)
    checkElif(e, classNames, act);
}

void checkElse(elseNode *n, std::vector<char const*> *classNames,  int act)
{
  if (act==PRINT) std::cout<<"elseNode"<< std::endl;
  if (n->statementBlock!=NULL)
    checkStatementBlock(n->statementBlock, classNames, act);
}

void checkStatement(statementNode n, std::vector<char const*> *classNames,  int act=-1)
{
  if (act==PRINT) std::cout<<"statementNode: "<< std::endl;
  if (n.rExpr!=NULL)
    checkRExpr(n.rExpr, classNames, act);
  if (n.lExpr!=NULL)
    checkLExpr(n.lExpr, classNames, act);
  if (n.stblock!=NULL)
    checkStatementBlock(n.stblock, classNames, act);
  if (n.elifs!=NULL)
    checkElifs(n.elifs, classNames, act);
  if (n.elseN!=NULL)
    checkElse(n.elseN, classNames, act);
}

void checkMethod(methodNode  n, std::vector<char const*> *classNames,  int act=-1)
{
  if (act==PRINT) std::cout<<"methodNode: "<< n.name << std::endl;
  if (n.statementBlock!=NULL)
    checkStatementBlock(n.statementBlock, classNames, act);
}

void checkSignature(classSignatureNode *n, std::vector<char const*> *classNames,  int act=-1)
{
  if (act==PRINT) std::cout<<"classSignatureNode: "<<n->name<<" extends: "<<n->extends<<std::endl;
}

void checkClassBody(classBodyNode * n, std::vector<char const*> *classNames, int act=-1)
{
  if (act==PRINT) std::cout<<"classBodyNode"<<std::endl;
  // symTable *s = new symTable;
  // n->sTable=s;
  // checkSignature(n, classNames, 

  if (n->statements != NULL)
    for (statementNode s : n->statements->list)
      {
	checkStatement(s, classNames, act);
      }
  if (n->methods != NULL)
    for (methodNode m : n->methods->list)
      {
	checkMethod(m, classNames, act);
      }

}

void checkClass(classNode *n, std::vector<char const*> *classNames ,int act)
{
  if (act==PRINT) std::cout<<"classNode"<<std::endl;
  if (n->sig != NULL)
    checkSignature(n->sig, classNames, act);
  if (n->classBody != NULL)
    checkClassBody(n->classBody, classNames, act);

}


void checkProgram(ProgramNode *n, std::vector<char const*> *classNames ,int act=-1)
{
  if (act==PRINT) std::cout<<"\nProgramNode"<<std::endl;
  for (auto &c : n->classes.list)
    checkClass(&c, classNames, act);
  for (statementNode &s : n->statements.list)
    checkStatement(s, classNames, act);
}

void buildSymbolTable(ProgramNode *rootNode)
{
  std::vector<char const*> emptyClassNames;
  checkProgram(rootNode, &emptyClassNames, BUILDSYMBOLTABLE);
}

void checkConstructorCalls ( ProgramNode *rootNode ) {
  std::vector<char const*> classNames;
  classNames.push_back("Obj");
  int actList=CHECKCONSTRUCTORCALLS;

  for (auto &c : rootNode->classes.list)
    classNames.push_back(c.sig->name);

  for (auto &c : rootNode->classes.list) 
    if (c.classBody != NULL) 
      checkClassBody(c.classBody, &classNames, actList);

  for (statementNode &s : rootNode->statements.list)
    checkStatement(s, &classNames, actList);
}

void checkClassHierarchy ( std::vector<classNode> l ) {
  int done=0, addS, failed=0;
  classesNode found, search;
  for (auto &i : l)
    {
      if (strcmp(i.sig->extends, "Obj") == 0)
	found.list.push_back(i);
      else
	search.list.push_back(i);
    }
  while (!done) {
    classesNode newFound, newSearch;
    if (search.list.size()>0) {
    failed = 1;
    }
    for (auto &s : search.list) {
      addS = 1;
      for (auto &f : found.list) {
    	if ((strcmp(s.sig->extends,f.sig->name)==0)) {
	  newFound.list.push_back(s);
	  addS = 0;
	  failed = 0;
	}
      }
      if (addS)
	newSearch.list.push_back(s);
    }
    if (newFound.list.size()>0) {
      search = newSearch;
      found.list.insert(found.list.end(), newFound.list.begin(), newFound.list.end() );
    }
    if (newSearch.list.size()==0 || failed==1) {
      done = 1;
    }
  }
  if (failed == 1)
    std::cerr << "Class Hierarchy Not Well Formed\n";
  /* ==== END CHECK FOR HEIRARCHY WELL FORMEDNESS ==== */
 return;
}

void traverse(int act) {

  
  if (act==CHECKCONSTRUCTORCALLS)
    checkConstructorCalls(root);
  if (act==CHECKCLASSHIERARCHY)
    checkClassHierarchy(root->classes.list);
  if (act==BUILDSYMBOLTABLE)
    buildSymbolTable(root);
  if (act==PRINT) {
    std::vector<char const*> emptyClassNames;
    checkProgram(root, &emptyClassNames, PRINT);
  }
}

