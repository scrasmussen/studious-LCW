#include <iostream>
#include <string>
#include <vector>
#include "Node.h"

void checkStatementBlock(statementBlockNode *n, std::vector<char const*> *classNames) {

  /* ===== TODO, VOID STATEMENT NOT ENTERING ===== */
  //std::cout<<"NOTNULL\n";
    // statementsNode *a=(statementsNode *) n->statements;
      // if (!n->statements) 
	// std::cout<<"NOTNULL\n";
}

  void checkConstructor(constructorNode *n,  std::vector<char const*> *classNames)
  {
  //std::cout<<"CHECKING\n";
    if (n !=NULL || n->name!=NULL) {
      // std::cout << "NAME is not NULL  " << "\n";
	//	std::cout << "NAME is not NULL  " << n->name << "\n";
    }
  else {
    //std::cout<<"NULLL NAME +++++++\n";
      }
	     //std::cout<<"DONE CHECKING\n";
    int found = 0;
      //for (char const * a : *classNames) {
      // if (strcpy(n->name,a)==0) {
      // 	found = 1;
      // std::cout << a << std::endl;
      // 	}
      //}

      //if (!found)
      // std::cerr << "Constructor Class Not Defined" << std::endl;"
  }
	     
void checkRExpr(rExprNode *n, std::vector<char const*> *classNames)
{
  // const char* str = "";
  // rExprNode *rExprFirst ;
  // rExprNode *rExprSecond ;
  // constructorNode *constructor ;
  
  if ( n->str="const"){
    //std::cout << n->name <<"REXPR\n";
    int found = 0;
      for (char const * a : *classNames) {
       if (strcmp(n->name,a)==0) {
              found = 1;
              //std::cout << a << std::endl;
       }
      }
    if (!found)
       std::cerr << "Constructor Class \""<< n->name<<"\" Not Defined" << std::endl;
  }
    // if (n->rExprFirst!=NULL){
    //   std::cout << "NULL+++REXPR\n";
    //   checkRExpr(n->rExprFirst, classNames);
    //   }
  // if (n->rExprSecond!=NULL)
  //if (n->constructor != NULL)
   // checkConstructor(n->constructor, classNames);

}
  
void checkStatement(statementNode n, std::vector<char const*> *classNames)
{
  // for (char const * a : *classNames)
  //   std::cout << a << std::endl;

  // ====Statement Struct====
  // int value;
  // const char* str;
  // rExprNode* rExpr;
  // statementBlockNode* stblock;
  if (n.rExpr!=NULL)
    checkRExpr(n.rExpr, classNames);
/*jjjjj if (n.stblock!=NULL && n.stblock->statements!=NULL)
    //checkStatementBlock(n.stblock, classNames);
    for (statementNode s : n.stblock->statements->list)
      {
	checkStatement(s, classNames);
      }*/ 

}

void checkMethod(methodNode  n, std::vector<char const*> *classNames)
{
  if (n.statementBlock != NULL && n.statementBlock->statements!=NULL)

    for (statementNode s : n.statementBlock->statements->list)
      {
	checkStatement(s, classNames);
      }

//  if (n->methods != NULL)
 //   for (methodsNode m : n->methods->list)
  //    {
//	checkMethods(m, classNames);
 //     }

}


void checkClassBody(classBodyNode * n, std::vector<char const*> *classNames)
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

void checkConstructorCalls ( ProgramNode *rootNode ) {
  std::vector<char const*> classNames;
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



