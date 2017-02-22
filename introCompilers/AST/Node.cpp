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

void checkActualArgs(actualArgsNode *n, std::vector<char const*> *classNames)
{
  if (n->rExpr!=NULL)
    checkRExpr(n->rExpr, classNames);
  if (n->rExprs!=NULL)
      for (rExprNode *e : n->rExprs->list)
	checkRExpr(e, classNames);
  
}

void checkRExpr(rExprNode *n, std::vector<char const*> *classNames)
{
  if ( strcmp(n->str,"const") == 0) 
    checkConstructorClass(n->name, classNames);
  
  if (n->rExprFirst!=NULL)
    checkRExpr(n->rExprFirst, classNames);
  if (n->rExprSecond!=NULL)
    checkRExpr(n->rExprSecond, classNames);
  if (n->lExpr!=NULL)
    checkLExpr(n->lExpr, classNames);
  if (n->actualArgs!=NULL)
    checkActualArgs(n->actualArgs, classNames);
}

void checkLExpr(lExprNode *n, std::vector<char const*> *classNames)
{
  if (n->rExpr!=NULL)
    checkRExpr(n->rExpr, classNames);
}


void checkStatementBlock(statementBlockNode *n, std::vector<char const*> *classNames)
{
  if (n->statements!=NULL)
    for (statementNode s : n->statements->list)
    	checkStatement(s, classNames);
}

void checkElif(elifNode *n, std::vector<char const*> *classNames)
{
  if (n->rExpr!=NULL)
    checkRExpr(n->rExpr, classNames);
  if (n->statementBlock!=NULL)
    checkStatementBlock(n->statementBlock, classNames);
}

void checkElifs(elifsNode *n, std::vector<char const*> *classNames)
{
  for (elifNode *e : n->list)
    checkElif(e, classNames);
}

void checkElse(elseNode *n, std::vector<char const*> *classNames)
{
  if (n->statementBlock!=NULL)
    checkStatementBlock(n->statementBlock, classNames);
}

void checkStatement(statementNode n, std::vector<char const*> *classNames)
{
  if (n.rExpr!=NULL)
    checkRExpr(n.rExpr, classNames);
  if (n.lExpr!=NULL)
    checkLExpr(n.lExpr, classNames);
  if (n.stblock!=NULL)
    checkStatementBlock(n.stblock, classNames);
  if (n.elifs!=NULL)
    checkElifs(n.elifs, classNames);
  if (n.elseN!=NULL)
    checkElse(n.elseN, classNames);
}

void checkMethod(methodNode  n, std::vector<char const*> *classNames)
{  
  if (n.statementBlock!=NULL)
    checkStatementBlock(n.statementBlock, classNames);
}


void checkClassBody(classBodyNode * n, std::vector<char const*> *classNames, std::vector<int> v={EMPTY})
{
  if (n->statements != NULL)
    for (statementNode s : n->statements->list)
      {
	checkStatement(s, classNames);
      }
  if (n->methods != NULL)
    for (methodNode m : n->methods->list)
      {
	checkMethod(m, classNames);
      }

}
// void checkProgramNode( ProgramNode *n, std::vector<int> v={EMPTY})
// {

// }
void buildSymbolTable(ProgramNode *rootNode)
{
  std::vector<int> actionVec;
  std::vector<char const*> emptyClassNames;
  actionVec.push_back(BUILDSYMBOLTABLE);

  // checkProgramNode(rootNode, emptyClassNames, actionVec);
}

void checkConstructorCalls ( ProgramNode *rootNode ) {
  std::vector<char const*> classNames;
  classNames.push_back("Obj");

  for (auto &c : rootNode->classes.list)
    classNames.push_back(c.sig->name);

  for (auto &c : rootNode->classes.list) 
    if (c.classBody != NULL) 
      checkClassBody(c.classBody, &classNames);


  for (statementNode &s : rootNode->statements.list)
	checkStatement(s, &classNames);
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



