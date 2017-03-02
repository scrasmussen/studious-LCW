#include <iostream>
#include <string>
#include <vector>
#include "Node.h"
int marker=0;
const char* flag="";

std::string leastCommonAnc(std::string lType, std::string rType) {
  std::vector<lca> lcaList;
  lca Obj, String, Int, Boolean, Nothing;
  for (auto &i : root->classes.list) {
    lca a;
    a.name=i.sig->name;
    a.extends=i.sig->extends;
    lcaList.push_back(a);
  }
  Obj.name="Obj"; Obj.extends="Obj";
  String.name="String"; String.extends="Obj";
  Int.name="Int"; Int.extends="Obj";
  Boolean.name="Boolean"; Boolean.extends="Obj";
  Nothing.name="Nothing"; Nothing.extends="Obj";
  lcaList.push_back(Obj);
  lcaList.push_back(String);
  lcaList.push_back(Int);
  lcaList.push_back(Boolean);
  lcaList.push_back(Nothing);
  std::string rTypeOriginal=rType;

  int ifPrint=0;

  while (lType != rType) {
    if (ifPrint)
      std::cout<<lType<<"|"<<rType<<std::endl;
    for (lca i : lcaList)
      {
	if (rType==i.name) {
	  rType=i.extends;
	  break;
	}
      }
    if (rType=="Obj" && lType!="Obj") {
      if (ifPrint)
	std::cout<<lType<<"|"<<rType<<std::endl;
      rType=rTypeOriginal;
      for (lca i : lcaList)
      {
	if (lType==i.name) {
	  lType=i.extends;
	  break;
	}
      }
    }
  }
  if (ifPrint)
    std::cout<<"Returning "<<lType<<std::endl;
  return lType;
}

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
  if (n->rExpr!=NULL) {
     if(act==BUILDSYMBOLTABLE) {
        n->rExpr->sTable=n->sTable;  
    } 
    checkRExpr(n->rExpr, classNames, act);
  } 
  if (n->rExprs!=NULL) {
      for (rExprNode *e : n->rExprs->list) {
	 if(act==BUILDSYMBOLTABLE) {
             e->sTable=n->sTable;  
         } 
         checkRExpr(e, classNames, act);
      }
   } 
}

void checkRExpr(rExprNode *n, std::vector<char const*> *classNames,  int act)
{
  flag=n->str;  //std::cout<<flag<<std::endl;
  if (act==PRINT){
    std::cout<<"rExprNode: "<<n->name;
    if (strcmp(n->str,"")!=0) std::cout<<" str: "<<n->str<<std::endl;
    else std::cout<<std::endl;
  }
  if ( strcmp(n->str,"string") == 0 && act == BUILDSYMBOLTABLE) {
     symbol s; s.name=n->name; s.type="STRING"; s.scope="[NULL]"; s.tag="STRING_LIT";
     n->sTable->insert(s);
  }

  if ( strcmp(n->str,"int") == 0 && act == BUILDSYMBOLTABLE) {
     symbol s; s.name=n->name; s.type="INT"; s.scope="[NULL]"; s.tag="INT_LIT";
     n->sTable->insert(s);
  }
  
  if(act==BUILDSYMBOLTABLE) {
    if(strcmp(flag,"method")==0) {
     symbol s; s.name=n->name; s.type="[NULL]"; s.scope="[NULL]"; s.tag="Method call";
     n->sTable->insert(s);
    }
    if(strcmp(flag,"const")==0) {
     symbol s; s.name=n->name; s.type="[NULL]"; s.scope="[NULL]"; s.tag="const call";
     n->sTable->insert(s);
    }
  }
  if (n->lExpr!=NULL){
     if(act==BUILDSYMBOLTABLE) {
        n->lExpr->sTable=n->sTable;  
     } 
     checkLExpr(n->lExpr, classNames, act);
  } 
  if ( strcmp(n->str,"const") == 0 && act == CHECKCONSTRUCTORCALLS)
    checkConstructorClass(n->name, classNames);

  if (n->rExprFirst!=NULL) {
     if(act==BUILDSYMBOLTABLE) {
        n->rExprFirst->sTable=n->sTable;  
     } 
    checkRExpr(n->rExprFirst, classNames, act);
  } 
  if (n->rExprSecond!=NULL){ 
     if(act==BUILDSYMBOLTABLE) {
        n->rExprSecond->sTable=n->sTable;  
     } 
    checkRExpr(n->rExprSecond, classNames, act);
  } 
  if (n->actualArgs!=NULL) {
     if(act==BUILDSYMBOLTABLE) {
        n->actualArgs->sTable=n->sTable;  
     } 
    checkActualArgs(n->actualArgs, classNames, act);
  }
}

void checkLExpr(lExprNode *n, std::vector<char const*> *classNames,  int act)
{
  if (act==PRINT) std::cout<<"lExprNode: "<< n->name << std::endl;  
  if (n->rExpr!=NULL) {
    if(act==BUILDSYMBOLTABLE) {
        symbol s; s.name=n->name; s.type="[NULL]"; s.scope="[NULL]"; s.tag="[NULL]";
        n->sTable->insert(s);
        n->rExpr->sTable=n->sTable;  
    } 
    checkRExpr(n->rExpr, classNames, act);
  }
  else{
     if(act==BUILDSYMBOLTABLE) {
        symbol s; s.name=n->name; s.type="[NULL]"; s.scope="[NULL]"; s.tag="[NULL]";
        n->sTable->insert(s);
    } 
  } 
}


void checkStatementBlock(statementBlockNode *n, std::vector<char const*> *classNames,  int act)
{
  if (act==PRINT) std::cout<<"statementBlockNode"<< std::endl;  
  if (n->statements!=NULL) {
    for (statementNode &s : n->statements->list) {
        if(act==BUILDSYMBOLTABLE) {
           s.sTable=n->sTable;  
        } 
        checkStatement(&s, classNames, act);
     }
  }
}

void checkElif(elifNode *n, std::vector<char const*> *classNames,  int act)
{
  if (act==PRINT) std::cout<<"elifNode"<< std::endl;  
  if (n->rExpr!=NULL) {
    if (act==BUILDSYMBOLTABLE) {
       n->rExpr->sTable=n->sTable;
    }
    checkRExpr(n->rExpr, classNames, act);
  }
  if (n->statementBlock!=NULL) {
    if (act==BUILDSYMBOLTABLE) {
       n->statementBlock->sTable=n->sTable;
    }
    checkStatementBlock(n->statementBlock, classNames, act);
  }
}

void checkElifs(elifsNode *n, std::vector<char const*> *classNames,  int act)
{
  for (elifNode *e : n->list) {
    if (act==BUILDSYMBOLTABLE) {
      symTable *st1= new symTable;  
      st1->setPrev(n->sTable);
      n->sTable=st1;
    }
    checkElif(e, classNames, act);
  }
}

void checkElse(elseNode *n, std::vector<char const*> *classNames,  int act)
{
  if (act==PRINT) std::cout<<"elseNode"<< std::endl;

  //if (act==PRINTST) n->sTable->print();
  if (n->statementBlock!=NULL) {
    if (act==BUILDSYMBOLTABLE) {
      symTable *st1= new symTable;  
      st1->setPrev(n->sTable);
      n->sTable=st1;
    }
    checkStatementBlock(n->statementBlock, classNames, act);
  }
}

void checkStatement(statementNode* n, std::vector<char const*> *classNames,  int act)
{
  // statement: rExpr, lExpr, stblock, elifs, elseN
  if (act==PRINT) std::cout<<"statementNode: "<< std::endl;
  if (act==PRINTST){
    std::cout<<std::endl<<"printing statement"; 
    n->sTable->print();
  }
  if (n->str=="WHILE") {
     if (act==BUILDSYMBOLTABLE) {
       symTable *st1= new symTable;  
       st1->setPrev(n->sTable);
       n->sTable=st1;
    }
  }

  if (n->str=="IF") {
     if (act==BUILDSYMBOLTABLE) {
       symTable *st1= new symTable;  
       st1->setPrev(n->sTable);
       n->sTable=st1;
    }
  }
 
  if (n->rExpr!=NULL) {
     if (act==BUILDSYMBOLTABLE) {
       n->rExpr->sTable=n->sTable;
    }
    checkRExpr(n->rExpr, classNames, act);
  }
  if (n->stblock!=NULL) {
     if (act==BUILDSYMBOLTABLE) {
       n->stblock->sTable=n->sTable;
    }
     checkStatementBlock(n->stblock, classNames, act);
  }

  
  if (n->lExpr!=NULL) {
     if (act==BUILDSYMBOLTABLE) {
      n->lExpr->sTable=n->sTable;
    }
     checkLExpr(n->lExpr, classNames, act);
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
}

void checkMethodReturn(methodReturnNode *n, std::vector<char const*> *classNames,  int act)
{
  if (act==BUILDSYMBOLTABLE) {
    symbol s; s.name=n->name; s.type="STR"; s.scope="[NULL]"; s.tag="METHODRETURN";
    n->sTable->insert(s);
  }
}

void checkMethod(methodNode* n, std::vector<char const*> *classNames,  int act)
{
  if (act==PRINT) std::cout<<"methodNode: "<< n->name << std::endl;
  if (act==PRINTST){
    std::cout<<std::endl<<"printing method: "<<n->name; 
    n->sTable->print();
  }
  if (act==BUILDSYMBOLTABLE) {
    std::string type = "(";
    if (n->fArguments!=NULL) {
      int arity=0;
      // Get input arguments
      for (argumentNode *arg : n->fArguments->list) {
	arity = 1;
	type.append(arg->type);
	type.append(",");	
      }
      if (arity==1)
	type.replace(type.end()-1,type.end(),"");
      type.append(")->(");
      // get return type
      type.append(n->methodReturn->name);
      type.append(")");

      symbol s; s.name=n->name; s.type=type.c_str(); s.scope="[NULL]"; s.tag="METHOD";
      n->sTable->prev->insert(s);
      // n->sTable->insert(s);
    }

    if (n->fArguments!=NULL)
      n->fArguments->sTable=n->sTable;
    if (n->statementBlock!=NULL)
      n->statementBlock->sTable=n->sTable; 
    if (n->methodReturn!=NULL)
      n->methodReturn->sTable=n->sTable; 
  }
  if (n->fArguments!=NULL)
    checkFormalArguments(n->fArguments, classNames, act);
  if (n->statementBlock!=NULL)
    checkStatementBlock(n->statementBlock, classNames, act);
  if (n->methodReturn!=NULL)
    checkMethodReturn(n->methodReturn, classNames, act);
}

void checkArguments(argumentsNode *n, std::vector<char const*> *classNames,  int act){
  if (act==PRINT) std::cout<<"ArgumentsNode: "<<std::endl;
  for (argumentNode *c : n->list){
     if (act==BUILDSYMBOLTABLE) {
     	symbol a;
     	a.name=c->name; a.type=c->type; a.scope="class";  a.tag="class Arguments";
     	n->sTable->insert(a);
     }
  }
  // if (act==PRINTST)
  //   n->sTable->print();
}

void checkFormalArguments(formalArgumentsNode *n, std::vector<char const*> *classNames,  int act)
{
  if (act==PRINT) std::cout<<"formalArgumentsNode"<<std::endl;
  // if (act==PRINTST && n->sTable!=NULL) n->sTable->print();
  if (act==BUILDSYMBOLTABLE) {
    for (argumentNode* arg : n->list) {
      symbol a;
      a.name=arg->name; a.type=arg->type;
      // ===TODO=== Need to check whether class or method arguments
      a.scope="class/method";  a.tag="class/method Arguments";
      n->sTable->insert(a);
    }
  }

  // checkArguments(n->arguments, classNames, act);
  /*if (n->arguments != NULL) {
    // arguments has been changed to list of arguments within a formalArgumentsNode, no need for
    // sTable, no need for sTable in argumentNode?
    // if (act==BUILDSYMBOLTABLE)
    //   n->arguments->sTable=n->sTable; 
    }*/
}

void checkSignature(classSignatureNode *n, std::vector<char const*> *classNames,  int act)
{
  if (act==PRINT) std::cout<<"classSignatureNode: "<<n->name<<" extends: "<<n->extends<<std::endl;

  if (n->name != NULL) {
        if (act==BUILDSYMBOLTABLE) {
	  symbol a;
	  a.name=n->name; a.type=n->name; a.scope="class";  a.tag="CLASS";
	  n->sTable->insert(a);
	}
  }
  if (n->extends != NULL) {
    if (act==BUILDSYMBOLTABLE) {
  	symbol a;
  	a.name=n->extends; a.type=n->extends; a.scope="class";  a.tag="EXTENDS";
  	n->sTable->insert(a);
    }
  }
  if (n->fArguments != NULL) {
    if (act==BUILDSYMBOLTABLE) {
       //symTable* st1= new symTable;
       //st1=n->sTable;
       n->fArguments->sTable=n->sTable;
       //n->sTable=st1;
    }
    checkFormalArguments(n->fArguments, classNames, act);
  }
}

void checkClassBody(classBodyNode * n, std::vector<char const*> *classNames, int act)
{
  if (act==PRINT) std::cout<<"classBodyNode"<<std::endl;
  // symTable *s = new symTable;
  // n->sTable=s; // checkSignature(n, classNames, 

  if (n->methods != NULL)
    for (methodNode &m : n->methods->list)
      {
	if (act==BUILDSYMBOLTABLE) {
	  symTable* st= new symTable;
	  st->setPrev(n->sTable);
	  m.sTable=st;
	}
	checkMethod(&m, classNames, act);
      }

  if (n->statements != NULL) {
    symTable * st=new symTable;
    st->setPrev(n->sTable); 
    for (statementNode &s : n->statements->list) {
      if (act==BUILDSYMBOLTABLE) {
	s.sTable=st;
      }
      checkStatement(&s, classNames, act);
    }
  }
}

void checkClass(classNode *n, std::vector<char const*> *classNames ,int act)
{
  if (act==PRINT) std::cout<<"classNode:"<<std::endl;
  if (act==PRINTST) {
    std::cout<<std::endl<<"printing class: "; 
    n->sTable->print();
  }

  if (n->sig != NULL) {
    if (act==BUILDSYMBOLTABLE) {
       n->sig->sTable=n->sTable;
    }
    checkSignature(n->sig, classNames, act);
  }
  if (n->classBody != NULL) {
    if (act==BUILDSYMBOLTABLE){
      n->classBody->sTable=n->sTable;
    }
    checkClassBody(n->classBody, classNames, act);
  }

}


void checkProgram(ProgramNode *n, std::vector<char const*> *classNames ,int act)
{
  // ProgramNode: classes, statements
  if (act==PRINT) std::cout<<"\nProgramNode"<<std::endl;
  if (act==BUILDSYMBOLTABLE) {
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

	a.name="NOTHING";  a.type="NOTHING";  a.scope="NOTHING";  a.tag="NOTHING";
	st->insert(a);

  	st->setPrev(st); 
  	n->sTable=st;
  }
  if (act==PRINTST) {
    std::cout<<std::endl<<"Printing root";
    n->sTable->print();
  }

  // Classes
  for (auto &c : n->classes.list)
    classNames->push_back(c.sig->name);

  // Add symbol tables and then order them to represent hierarchy
  for (auto &c : n->classes.list)
    if (act==BUILDSYMBOLTABLE) {
      symTable* newScope = new symTable;
      c.sTable = newScope;
    }
  for (auto &c : n->classes.list)
    if (act==BUILDSYMBOLTABLE) {
      std::string extends = c.sig->extends;
      if (extends=="Obj")
      	c.sTable->setPrev(n->sTable);
      else {
      	for (auto &superClass : n->classes.list) {
	  std::string name = superClass.sig->name;
	  if (extends==name)
	    c.sTable->setPrev(superClass.sTable);
	}
      }
    }

  for (auto &c : n->classes.list)
    checkClass(&c, classNames, act);

  // Statements
  symTable* st= new symTable;
  st->setPrev(n->sTable);
  symTable* current=st;
  for (statementNode &s : n->statements.list) {
    if (act==BUILDSYMBOLTABLE && marker==0) {
      s.sTable=current;
    }
/*     if (act==BUILDSYMBOLTABLE && marker==1) {
 *     symTable* st1= new symTable;
 *     st1->setPrev(current);
 *     s.sTable=st1;
 *   }
*/
    checkStatement(&s, classNames, act);
  } 
}

void buildSymbolTable(ProgramNode *rootNode)
{
  std::vector<char const*> emptyClassNames;
  checkProgram(rootNode, &emptyClassNames, BUILDSYMBOLTABLE);
}

void printSymbolTable(ProgramNode *rootNode)
{
  std::vector<char const*> emptyClassNames;
  checkProgram(rootNode, &emptyClassNames, PRINTST);
}

void checkConstructorCalls ( ProgramNode *rootNode ) {
  std::vector<char const*> classNames;
  classNames.push_back("Obj");
  int act=CHECKCONSTRUCTORCALLS;

  for (auto &c : rootNode->classes.list)
    classNames.push_back(c.sig->name);

  for (auto &c : rootNode->classes.list) 
    if (c.classBody != NULL) 
      checkClassBody(c.classBody, &classNames, act);

  for (statementNode &s : rootNode->statements.list)
    checkStatement(&s, &classNames, act);
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
  if (act==PRINTST)
    printSymbolTable(root);
  if (act==PRINT) {
    std::vector<char const*> emptyClassNames;
    checkProgram(root, &emptyClassNames, PRINT);
  }
}

