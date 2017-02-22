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

void checkActualArgs(actualArgsNode *n, std::vector<char const*> *classNames,  std::vector<int> v)
{
  if (n->rExpr!=NULL)
    checkRExpr(n->rExpr, classNames, v);
  if (n->rExprs!=NULL)
      for (rExprNode *e : n->rExprs->list)
	checkRExpr(e, classNames, v);
  
}

void checkRExpr(rExprNode *n, std::vector<char const*> *classNames,  std::vector<int> v)
{
  if ( strcmp(n->str,"const") == 0) 
    checkConstructorClass(n->name, classNames);
  
  if (n->rExprFirst!=NULL)
    checkRExpr(n->rExprFirst, classNames, v);
  if (n->rExprSecond!=NULL)
    checkRExpr(n->rExprSecond, classNames, v);
  if (n->lExpr!=NULL)
    checkLExpr(n->lExpr, classNames, v);
  if (n->actualArgs!=NULL)
    checkActualArgs(n->actualArgs, classNames, v);
}

void checkLExpr(lExprNode *n, std::vector<char const*> *classNames,  std::vector<int> v)
{
  if (n->rExpr!=NULL)
    checkRExpr(n->rExpr, classNames, v);
}


void checkStatementBlock(statementBlockNode *n, std::vector<char const*> *classNames,  std::vector<int> v)
{
  if (n->statements!=NULL)
    for (statementNode s : n->statements->list)
      checkStatement(s, classNames, v);
}

void checkElif(elifNode *n, std::vector<char const*> *classNames,  std::vector<int> v)
{
  if (n->rExpr!=NULL)
    checkRExpr(n->rExpr, classNames, v);
  if (n->statementBlock!=NULL)
    checkStatementBlock(n->statementBlock, classNames, v);
}

void checkElifs(elifsNode *n, std::vector<char const*> *classNames,  std::vector<int> v)
{
  for (elifNode *e : n->list)
    checkElif(e, classNames, v);
}

void checkElse(elseNode *n, std::vector<char const*> *classNames,  std::vector<int> v)
{
  if (n->statementBlock!=NULL)
    checkStatementBlock(n->statementBlock, classNames, v);
}

void checkStatement(statementNode n, std::vector<char const*> *classNames,  std::vector<int> v={EMPTY})
{
  if (n.rExpr!=NULL)
    checkRExpr(n.rExpr, classNames, v);
  if (n.lExpr!=NULL)
    checkLExpr(n.lExpr, classNames, v);
  if (n.stblock!=NULL)
    checkStatementBlock(n.stblock, classNames, v);
  if (n.elifs!=NULL)
    checkElifs(n.elifs, classNames, v);
  if (n.elseN!=NULL)
    checkElse(n.elseN, classNames, v);
}

void checkMethod(methodNode  n, std::vector<char const*> *classNames,  std::vector<int> v={EMPTY})
{  
  if (n.statementBlock!=NULL)
    checkStatementBlock(n.statementBlock, classNames, v);
}


void checkClassBody(classBodyNode * n, std::vector<char const*> *classNames, std::vector<int> v={EMPTY})
{
  if (n->statements != NULL)
    for (statementNode s : n->statements->list)
      {
	checkStatement(s, classNames, v);
      }
  if (n->methods != NULL)
    for (methodNode m : n->methods->list)
      {
	checkMethod(m, classNames, v);
      }

}


// void call(ACTION, node)
//    if (action=BUILDSYMBOLTABLE)
//           node.insert(name, type, scope)
//


// void checkProgramNode( ProgramNode *n, std::vector<int> v={EMPTY})
// {
//     for action in actionVec
//         call(action)
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



