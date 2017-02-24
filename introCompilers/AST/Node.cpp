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

void checkMethodReturn(methodReturnNode *n, std::vector<char const*> *classNames,  int act)
{
  if (act==BUILDSYMBOLTABLE) {
    symbol s; s.name=n->name; s.type="STR"; s.scope="[NULL]"; s.tag="METHODRETURN";
    n->sTable->insert(s);
  }
}

void checkMethod(methodNode  n, std::vector<char const*> *classNames,  int act=-1)
{
  if (act==PRINT) std::cout<<"methodNode: "<< n.name << std::endl;
  if (act==BUILDSYMBOLTABLE) {
    symbol s; s.name=n.name; s.type="method"; s.scope="[NULL]"; s.tag="METHOD";
    n.sTable->insert(s);

    if (n.fArguments!=NULL)
      n.fArguments->sTable=n.sTable;
    if (n.methodReturn!=NULL)
      n.methodReturn->sTable=n.sTable;
  }
  if (n.fArguments!=NULL)
    checkFormalArguments(n.fArguments, classNames, act);
  if (n.statementBlock!=NULL)
    checkStatementBlock(n.statementBlock, classNames, act);
  if (n.methodReturn!=NULL)
    checkMethodReturn(n.methodReturn, classNames, act);
}

void checkArguments(argumentsNode *n, std::vector<char const*> *classNames,  int act){
  if (act==PRINT) std::cout<<"ArgumentsNode: "<<std::endl;
  for (argumentNode *c : n->list){
     if (act=BUILDSYMBOLTABLE) {
     	symbol a;
     	a.name=c->name; a.type=c->type; a.scope="class";  a.tag="class Arguments";
     	n->sTable->insert(a);
     }
  }
  if (act==PRINTST)
    n->sTable->print();
}

void checkArgument(argumentNode *n, std::vector<char const*> *classNames,  int act){
  if (act==PRINT) std::cout<<"ArgumentsNode: "<<std::endl;
  
  //if (act==PRINTST)
    //n->sTable->print();
}

void checkFormalArguments(formalArgumentsNode *n, std::vector<char const*> *classNames,  int act)
{
  if (act==PRINT) std::cout<<"formalArgumentsNode: "<<n->name<<std::endl;

  if (n->name != NULL && act==BUILDSYMBOLTABLE) {
  	symbol a;
  	a.name=n->name; a.type=n->type; a.scope="class";  a.tag="class Arguments";
  	n->sTable->insert(a);
  }
  if (n->arguments != NULL) {
    if (act=BUILDSYMBOLTABLE) {
       //symTable* st1= new symTable;
       //st1=n->sTable;
       n->arguments->sTable=n->sTable;
       //std::cout<<n->arguments->list.size()<<" size of the list"<<std::endl;
       //n->arguments->sTable->print();;
       //n->sTable=st1;
    }
    //for (argumentNode *e : n->arguments->list)
    //	checkArgument(e, classNames, act);
    
    checkArguments(n->arguments, classNames, act);
  }
}

void checkSignature(classSignatureNode *n, std::vector<char const*> *classNames,  int act)
{
  if (act==PRINT) std::cout<<"classSignatureNode: "<<n->name<<" extends: "<<n->extends<<std::endl;

  if (n->name != NULL) {
  	symbol a;
  	a.name=n->name; a.type=n->name; a.scope="class";  a.tag="CLASS";
  	n->sTable->insert(a);
  }
  if (n->extends != NULL) {
  	symbol a;
  	a.name=n->extends; a.type=n->extends; a.scope="class";  a.tag="EXTENDS";
  	n->sTable->insert(a);
  }
  if (n->fArguments != NULL) {
    if (act=BUILDSYMBOLTABLE) {
       //symTable* st1= new symTable;
       //st1=n->sTable;
       n->fArguments->sTable=n->sTable;
       //n->sTable=st1;
    }
    checkFormalArguments(n->fArguments, classNames, act);
  }
}

void checkClassBody(classBodyNode * n, std::vector<char const*> *classNames, int act=-1)
{
  if (act==PRINT) std::cout<<"classBodyNode"<<std::endl;
  // symTable *s = new symTable;
  // n->sTable=s; // checkSignature(n, classNames, 

  if (n->methods != NULL)
    for (methodNode m : n->methods->list)
      {
	if (act=BUILDSYMBOLTABLE) {
	  m.sTable=n->sTable;
	}
	checkMethod(m, classNames, act);
      }
  if (n->statements != NULL)
    for (statementNode s : n->statements->list)
      {
	checkStatement(s, classNames, act);
      }
}

void checkClass(classNode *n, std::vector<char const*> *classNames ,int act)
{
  if (act==PRINT) std::cout<<"classNode"<<std::endl;
  if (n->sig != NULL) {
    if (act=BUILDSYMBOLTABLE) {
       //n->sTable->prev->print();
       //symTable* st1= new symTable;
       //st1=n->sTable;
       n->sig->sTable=n->sTable;
       //n->sTable=st1;
    }
    if (act==PRINTST)
      n->sig->sTable->print();
    checkSignature(n->sig, classNames, act);
  }
  if (n->classBody != NULL) {
    if (act=BUILDSYMBOLTABLE){
      n->classBody->sTable=n->sTable;
    }
    checkClassBody(n->classBody, classNames, act);
  }

}


void checkProgram(ProgramNode *n, std::vector<char const*> *classNames ,int act)
{
  if (act==PRINT) std::cout<<"\nProgramNode"<<std::endl;

  if (act=BUILDSYMBOLTABLE) {
  	//n->sTable= new symTable;
  	symTable* st= new symTable;
  	symbol a;
  	a.name="OBJ"; a.type="OBJ"; a.scope="GLOBAL";  a.tag="CLASS";
  	st->insert(a);
  	
	a.name="INT";  a.type="INT";  a.scope="GLOBAL";  a.tag="CLASS";
  	st->insert(a);
  	
	a.name="STRING";  a.type="STRING";  a.scope="GLOBAL";  a.tag="CLASS";
  	st->insert(a);
  	
	a.name="BOOLEAN";  a.type="BOOLEAN";  a.scope="GLOBAL";  a.tag="CLASS";
	st->insert(a);
  	st->setPrev(st); 
  	st->setCurrent(st); 
  	n->sTable=st;
  }
  if (act==PRINTST)
    n->sTable->print();
  for (auto &c : n->classes.list)
    classNames->push_back(c.sig->name);
  for (auto &c : n->classes.list) {
    if (act=BUILDSYMBOLTABLE) {
      symTable* st1= new symTable;
      st1->setPrev(n->sTable);
      st1->setCurrent(n->sTable);
      c.sTable=st1;
    }
    checkClass(&c, classNames, act);
  }
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

