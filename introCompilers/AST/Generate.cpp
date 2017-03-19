#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "Node.h"
#include "Generate.h"

int Q=1;
int FIN=1;
int var=0;
std::string TYPE;
std::vector<std::string> currentNames, tmpNames, singleton;


void clearNames() {
  currentNames.clear();
  tmpNames.clear();
}

// credit goes to Charles Salvia on stackoverflow for being a boss
bool isNumber(const std::string& s)
{
  return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

// replace was plundered from Mateen Ulhaq  on stackoverflow
std::string replace(std::string s, const std::string toReplace, const std::string replaceWith) {
  if (s.find(toReplace)==std::string::npos)
    return s;
  return(s.replace(s.find(toReplace), toReplace.length(), replaceWith));
}

std::string cleanString(std::string word) {
  // ARTLESS TODO: Add all other characters in string that could cause trouble
  if (isNumber(word))
    word = "num"+word;

  word=replace(word,"\"","Q");
  word=replace(word,"\"","Q");
  word=replace(word,"(","lparen");
  word=replace(word,")","rparen");
  word=replace(word,",","comma");
  word=replace(word,"->","REF");
  word=replace(word,"!","excla");
  word=replace(word,"@","at");
  word=replace(word,"#","bang");
  word=replace(word,"$","dollar");
  word=replace(word,"%","perc");
  word=replace(word,"^","carrot");
  word=replace(word,"&","and");
  word=replace(word,"*","star");
  word=replace(word,"[","lbrack");
  word=replace(word,"]","rbrack");
  word=replace(word,":","semic");
  word=replace(word,";","colon");
  word=replace(word,"<","gt");
  word=replace(word,">","gt");
  word=replace(word,".","period");
  word=replace(word,"?","que");
  word=replace(word,"/","fslash");
  word=replace(word,"\\","bflash");
  word=replace(word,"|","pipe");
  word=replace(word,"{","rcurl");
  word=replace(word,"}","lcurl");
  word=replace(word,"-","minus");
  word=replace(word,"_","uscore");
  word=replace(word,"+","plus");
  word=replace(word,"=","equal");
  word=replace(word,"~","aprox");
  word=replace(word,"`","what");
  
  return word;
}

void genStructLine(std::ofstream &f,char const* name) {
  f<<"struct class_"<<name<<"_struct;\n";
}
void genTypeDefStructLine(std::ofstream &f,char const* name){
  f<<"typedef struct class_"<<name<<"_struct* class_"<<name<<";\n";
}
void genClassStructLine(std::ofstream &f,char const* name) {
  f<<"struct class_"<<name<<"_struct the_class_"<<name<<"_struct;\n";
}
void genTheClassExternLine(std::ofstream &f,char const* name) {
  f<<"extern class_"<<name<<" the_class_"<<name<<";\n";
}

void genClassArgLines(std::ofstream &f, classNode *n) {
  f<<"typedef struct obj_"<<n->sig->name<<"_struct {\n";
  f<<"  class_"<<n->sig->name<<" clazz;\n";

  clearNames();
  for (auto a :  n->sig->fArguments->list) {
    std::string aName = a->name;
    f<<"  obj_"<<a->type<<" "<<aName<<";\n";
    currentNames.push_back(aName);
  }

  for (statementNode s :  n->classBody->statements->list) {
    if (strcmp(s.str,"ASSIGN")==0) {
      char const *blank="[A1]";
      std::string r1 = genLExprBit(s.lExpr, f, blank, "[B]",JUSTTYPE);
      std::string r2 = genRExprBit(s.rExpr, f, "[C]",JUSTTYPE);
	    

      symbol sym;
      sym.name=r2;
      sym=s.sTable->lookup(sym);
      if(std::find(currentNames.begin(), currentNames.end(), r1) == currentNames.end()) {
	currentNames.push_back(r2);
	if (sym.type!="")
	  f<<"  obj_"<<sym.type<<" "<<r1<<";\n";
      }
    }
  }
  
  f<<"} * obj_"<<n->sig->name<<";\n";
}

std::string genStatement(statementNode *n, std::ofstream &f, int act)
{
  // statementNode: rExpr, lExpr, stblock, elifs, elseN
  std::string res="",r1,r2, type="  ";

  if (strcmp(n->str,"ASSIGN")==0) {
    char const * blank="[A]";
    r1 = genLExprBit(n->lExpr, f, blank, "[B]",REG); // TODO, NEED TO TYPE THIS
    r2 = genRExprBit(n->rExpr, f, "[C]",REG);
    if(std::find(currentNames.begin(), currentNames.end(), r1) == currentNames.end()) {
      type="  "+TYPE;
      currentNames.push_back(r1);
    }
    if (!NOTYPE)
      f<<type;
    
    f<<" "<<r1;
    f<<"=";
    f<<r2;
    f<<";\n";
    return res;
  }
  
  if (strcmp(n->str,"RETURN")==0) {
    r1=genRExprBit(n->rExpr,f,"[NAME]",REG);
    f<<"  return "<<r1<<";\n";
    return res;
  }

  if (strcmp(n->str,"RETURN VOID")==0) {
    f<<"  return;\n";
    return res;
  }

  if (strcmp(n->str,"WHILE")==0) {
    r1=genRExprBit(n->rExpr,f,"[NAME]",REG);
    f<<"  while(";
    f<<r1;
    f<<") {\n";
    if (n->stblock->statements!=NULL) {
      for (statementNode s : n->stblock->statements->list) {
	genStatement(&s,f,GENSTATEMENTS);
      }
    }
    f<<"}\n";
    return res;
  }

  if (strcmp(n->str,"IF")==0) {
    // rExpr, stblock, elifs, elseN
    r1=genRExprBit(n->rExpr,f,"[NAME]",REG);
    f<<"  if(";
    f<<r1;
    f<<") {\n";
    if (n->stblock->statements!=NULL) {
      for (statementNode s : n->stblock->statements->list) {
	genStatement(&s,f,GENSTATEMENTS);
      }
    }
    f<<"  }\n";

    // else if statements
    for (elifNode *e : n->elifs->list) {
      r1=genRExprBit(e->rExpr,f,"[NAME]",REG);
      f<<"  else if(";
      f<<r1;
      f<<") {\n";
      if (e->statementBlock->statements!=NULL) {
	for (statementNode s : e->statementBlock->statements->list) {
	  genStatement(&s,f,GENSTATEMENTS);
	}
      }
      f<<"  }\n";
    }

    if (n->elseN!=NULL) {
      f<<"  else {\n";
      if (n->elseN->statementBlock!=NULL && n->elseN->statementBlock->statements!=NULL) {
	for (statementNode s : n->elseN->statementBlock->statements->list) {
	  genStatement(&s,f,GENSTATEMENTS);
	}
      }
      f<<"  }\n";
    }
    return res;
  }

  if (strcmp(n->str,"REXPR; ONLY")==0) {
    r1=genRExprBit(n->rExpr,f,"",REG);  // TODO : TEST, what will create this?
    // f<<r1<<std::endl;
    return res;
  }

  // n->str=LONG_ASSIGN is not implemented, there are no test cases so it's not beign down right now
  f<<"===TODO:"<<n->str<<std::endl;

  return res;
}


void genSignature(classNode *n, std::ofstream &f, int act)
{
  // name, extends, fArguments

  /* gen struct and typedef struct */
  genStructLine(f,n->sig->name);
  genTypeDefStructLine(f,n->sig->name);
  genClassStructLine(f,n->sig->name);
  genTheClassExternLine(f,n->sig->name);
  
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

void genMethodArgs(formalArgumentsNode *n, std::ofstream &f, char const *name, int isMethodDef)
{
  int arity=0;

  if (isMethodDef==METHODDEF) {
    f<<"obj_"<<name<<" item";
    arity=1;
  }
  
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

void genBuiltinFuncPointers(std::string ext, std::vector<std::string> defined, std::ofstream &f)
{
  std::string midmeth="_method_"; //###
  f<<"  // build-in methods\n";
  // TODO Have to check what has been defined?? Maybe it will work without that?
  if (ext=="Obj") {
    // ext="obj";
    // STRING, PRINT, EQUALS
    int s=1,p=1,e=1;
    for (std::string meth : defined) {
      if (meth == "STRING") s=0;
      if (meth == "PRINT") p=0;
      if (meth == "EQUALS") e=0;
    }
    if (s) {
      f<<"  obj_String (*STRING) (obj_Obj);\n";
      singleton.push_back(ext+midmeth+"STRING");
    }
    if (p) {
      f<<"  obj_Obj (*PRINT) (obj_Obj);\n";
      singleton.push_back(ext+midmeth+"PRINT");
    }
    if (e) {
      f<<"  obj_Boolean (*EQUALS) (obj_Obj, obj_Obj);\n";
      singleton.push_back(ext+midmeth+"EQUALS");
    }
  }

  else if (ext=="String") {
    ext="string";
    int s=1,p=1,e=1;
    for (std::string meth : defined) {
      if (meth == "STRING") s=0;
      if (meth == "PRINT") p=0;
      if (meth == "EQUALS") e=0;
    }
    if (s) {
      f<<"  obj_String (*STRING) (obj_String);\n";
      singleton.push_back(ext+midmeth+"STRING");
    }
    if (p) {
      f<<"  obj_String (*PRINT) (obj_String);\n";
      singleton.push_back(ext+midmeth+"PRINT");
    }
    if (e) {
      f<<"  obj_Boolean (*EQUALS) (obj_String, obj_Obj);\n";
      singleton.push_back(ext+midmeth+"EQUALS");
    }
    f<<"// ===DO WE NEED TO ADD MORE??===\n";
    f<<"// ===YES===\n";
  }

  else if (ext=="Int") {
    ext="int";
    int s=1,p=1,e=1,l=1,m1=1,m2=1,m3=1,m4=1;
    for (std::string meth : defined) {
      if (meth == "STRING") s=0;
      if (meth == "PRINT") p=0;
      if (meth == "EQUALS") e=0;
      if (meth == "LESS") l=0;
      if (meth == "PLUS") m1=0;
      if (meth == "MINUS") m2=0;
      if (meth == "TIMES") m3=0;
      if (meth == "DIVIDE") m4=0;
    }
    if (s) {
      f<<"  obj_String (*STRING) (obj_Int);\n";
      singleton.push_back(ext+midmeth+"STRING");
    }
    if (p) {
      f<<"  obj_Obj (*PRINT) (obj_Obj);\n";
      singleton.push_back(ext+midmeth+"PRINT");
    }
    if (e) {
      f<<"  obj_Boolean (*EQUALS) (obj_Int, obj_Obj);\n";
      singleton.push_back(ext+midmeth+"EQUALS");
    }
    if (l) {
    f<<"  obj_Boolean (*LESS) (obj_Int, obj_Int);\n";
      singleton.push_back(ext+midmeth+"LESS");
    }
    if (m1) {
      f<<"  obj_Int (*PLUS) (obj_Int, obj_Int);\n";
      singleton.push_back(ext+midmeth+"PLUS");
    }
    if (m2) {
      f<<"  obj_Int (*MINUS) (obj_Int, obj_Int);\n";
      singleton.push_back(ext+midmeth+"MINUS");
    }
    if (m3) {
      f<<"  obj_Int (*TIMES) (obj_Int, obj_Int);\n";
      singleton.push_back(ext+midmeth+"TIMES");
    }
    if (m4) {
      f<<"  obj_Int (*DIVIDE) (obj_Int, obj_Int);\n";
      singleton.push_back(ext+midmeth+"DIVIDE");
    }
    //    f<<"  obj_Int (*constructor) ( void );\n";
  }

  else if (ext=="Boolean") {
    ext="boolean";
    int s=1,p=1,e=1;
    for (std::string meth : defined) {
      if (meth == "STRING") s=0;
      if (meth == "PRINT") p=0;
      if (meth == "EQUALS") e=0;
    }
    if (s) {
      f<<"  obj_String (*STRING) (obj_Boolean);\n";
      singleton.push_back(ext+midmeth+"STRING");
    }
    if (p) {
      f<<"  obj_Obj (*PRINT) (obj_Obj);\n";
      singleton.push_back(ext+midmeth+"PRINT");
    }
    if (e) {
      f<<"  obj_Boolean (*EQUALS) (obj_Obj, obj_Obj);\n";
      singleton.push_back(ext+midmeth+"EQUALS");
    }
  }

  else if (ext=="Nothing") {
    ext="nothing";
    int s=1,p=1,e=1;
    for (std::string meth : defined) {
      if (meth == "STRING") s=0;
      if (meth == "PRINT") p=0;
      if (meth == "EQUALS") e=0;
    }
    if (s) {
      f<<"  obj_String (*STRING) (obj_Nothing);\n";
      singleton.push_back(ext+midmeth+"STRING");
    }
    if (p) {
      f<<"  obj_Obj (*PRINT) (obj_Obj);\n";
      singleton.push_back(ext+midmeth+"PRINT");
    }
    if (e) {
      f<<"  obj_Boolean (*EQUALS) (obj_Obj, obj_Obj);\n";
      singleton.push_back(ext+midmeth+"EQUALS");
    }
  }
}

void genFuncPointers(std::string ext, std::vector<std::string> defined, std::ofstream &f) {
  // check is extend class in defined classes
  for (classNode &c : root->classes.list) {
    std::string className=c.sig->name;
    if (ext==className) {
      int proceed=0;
      std::string newExt = c.sig->extends;
      if (c.classBody!=NULL && c.classBody->methods!=NULL)
	proceed=1;
      if (proceed) {
	for (methodNode &m : c.classBody->methods->list) {
	  std::string methName = m.name;
	  if(std::find(defined.begin(), defined.end(), methName) == defined.end()) {
	    defined.push_back(methName);
	    std::string single;
	    single = className+"_method_"+methName;
	    singleton.push_back(single);
	    
	    f<<"  obj_"<<m.returnType<<" (*"<<methName<<") (";
	    // Generate method arg types
	    genMethodArgTypes(m.fArguments,f);
	    f<<");\n";
	  }
	}
      }
      genFuncPointers(newExt,defined, f);
    }
  }
  // check is extend class in built-in classes
  genBuiltinFuncPointers(ext,defined,f);
}

void genClassFuncPointerStruct(classNode *n, std::ofstream &f, char const *name, int act)
{
  f<<"struct class_"<<name<<"_struct {\n";
  // Generate guaranteed built in methods
    // Constructor Function Pointer
  f<<"  obj_"<<name<<" (*constructor) (";
  genMethodArgTypes(n->sig->fArguments,f);
  f<<");\n";
  // genBuiltinFuncPointers(n,f,name);  // MOVING THIS IN HERE

  std::vector<std::string> defined;

  f<<"  // class's methods\n";
  for (methodNode m : n->classBody->methods->list) {
    std::string single, className=name, methodName=m.name;
    single = className+"_method_"+methodName;
    defined.push_back(m.name);
    singleton.push_back(single);

    f<<"  obj_"<<m.returnType<<" (*"<<m.name<<") (";
    // Generate method arg types
    genMethodArgTypes(m.fArguments,f);
    f<<");\n";
  }

  // Builtin Classes
  std::string ext = n->sig->extends;
  genFuncPointers(ext, defined, f);
  
  f<<"};\n";
}

// Generate the lExpr, can only be IDENT or R_Expr.IDENT
std::string genLExprBit(lExprNode *n, std::ofstream &f, char const *name, std::string argName, int act)
{
  std::string res="";
  if (strcmp(n->str,"")==0) {
    res.append(n->name);
    return res;
  }
  else if (act==JUSTTYPE && strcmp(n->str,".")==0) {
    // res = genRExprBit(n->rExpr,f,name);
    res.append(n->name);
    return res;
  }
  else if (strcmp(n->str,".")==0) {
    res = genRExprBit(n->rExpr,f,name,act);
    if (res == "this")
      res = "item";
    res.append("->");
    res.append(n->name);
    return res;
  }
  else
    res.append("ERROR:!!");
  if (Q) std::cout<<"genLEXPRBit:"<<res<<std::endl;
  return res;
}

std::string genActualArgsBit(actualArgsNode *n,std::ofstream &f,char const *name)
{
  std::string s="";
  int arity=0;
  for (rExprNode *e : n->list) {
    if (arity==0)
      arity=1;
    else
      s.append(",");
    s.append(genRExprBit(e,f,name,REG));
    //std::cout<<"ART"<<s<<std::endl;
  }
  return s;
}

// Function genRExprBit
std::string genRExprBit(rExprNode *n, std::ofstream &f, char const *name, int act)
{
  int found=0;
  std::string res="",r1,r2,r3,type="TYPE",resname="res";

  if (strcmp(n->str,"string_lit")==0) {
    found=1;
    res.append(n->name);
    if (Q) std::cout<<"string_lit:"<<res<<std::endl;
    TYPE="char const *";
    return res;
  }

  if (strcmp(n->str,"int_lit")==0) {
    found=1;
    res.append(n->name);
    if (Q) std::cout<<"int_lit:"<<res<<std::endl;
    TYPE="int";
    return res;
  }

  if (strcmp(n->str,"lexpr")==0) {
    found=1;
    res.append(genLExprBit(n->lExpr,f,name,"item",REG));
    if (Q) std::cout<<"lexpr:"<<res<<std::endl;
    return res;
  }

  // in genRExprBit
  if (strcmp(n->str,"PLUS")==0 || (strcmp(n->str,"MINUS")==0) ||
      strcmp(n->str,"TIMES")==0 || (strcmp(n->str,"DIVIDE")==0)){
    found=1;
    r1=genRExprBit(n->rExprFirst,f,name,act);
    res.append(cleanString(r1));
    
    r3=genRExprBit(n->rExprSecond,f,name,act);
    res.append(cleanString(r3));

    type="";
 // ===MONIL===
    if(std::find(tmpNames.begin(), tmpNames.end(), res) == tmpNames.end()) {
      type = "obj_TYPE";
      if(r1.find("->")!=std::string::npos) {
        std::string temp=r1.substr(r1.find("->")+2,r1.length());
        //std::cout<<temp<<std::endl;
        symbol sym;
        sym.name=temp;
        //if (n->sTable->prev !=NULL) n->sTable->prev->print();
        sym=n->sTable->lookup(sym);
        f<<std::endl<<"  Obj_"<<sym.type<<" "<<res<<";";
        type=sym.type;
      }
      else {
//todo
        symbol sym;
        sym.name=r1;
        sym=n->sTable->lookup(sym);
        //std::cout<<r1<<"|"<<r1.find("->")<<"---------"<<sym.type<<std::endl;
        f<<std::endl<<"  Obj_"<<sym.type<<" "<<res<<";";
        type=sym.type;

      }
      tmpNames.push_back(res);

    }
    TYPE="obj_TYPE";

    f<<std::endl<<"  "<<res<<" = "<<res<<"->clazz->";
    // ===MONIL===

 
    if (strcmp(n->str,"PLUS")==0 && act!=JUSTTYPE) f<<"PLUS";
    if (strcmp(n->str,"MINUS")==0 && act!=JUSTTYPE) f<<"MINUS";
    if (strcmp(n->str,"TIMES")==0 && act!=JUSTTYPE) f<<"TIMES";
    if (strcmp(n->str,"DIVIDE")==0 && act!=JUSTTYPE) f<<"DIVIDE";
    if (act!=JUSTTYPE) {
      f<<"(";
      f<<r1<<","<<r3<<");\n";
    }
    if (Q) std::cout<<"PLUS|MINUS|TIMES|DIVIDE:"<<res<<std::endl;
    return res;
  }

  // in genRExprBit
  if (strcmp(n->str,"const")==0) {
    found=1;
    resname.append(n->name);
    r1 = genActualArgsBit(n->actualArgs,f,name);

    std::string type="";
    std::string t = n->name;
    if(std::find(tmpNames.begin(), tmpNames.end(), resname) == tmpNames.end()) {
      type = "obj_"+t;
      tmpNames.push_back(resname);
    }
    TYPE="obj_"+t;
    if (act!=JUSTTYPE) {
      f<<"  "<<type<<" "<<resname<<" = ";
      f<<"the_class_"<<(n->name)<<"->constructor(";
      f<<r1;
      f<<");\n";
    }
    if (Q) std::cout<<"constructor:"<<res<<std::endl;
    return resname;
  }
/*
  if (strcmp(n->str,"method")==0) {
    r1 = genRExprBit(n->rExprFirst,f,name,act);
    r2 = genActualArgsBit(n->actualArgs,f,name);
    resname.append(n->name);
    if (act!=JUSTTYPE) {
      f<<"  obj_"<<type<<" "<<resname<<"="<<r1<<"->clazz->"<<n->name<<"("<<r2<<");\n";
    }
    return resname;
  }
*/
//////////MONIL
  if (strcmp(n->str,"method")==0) {
    r1 = genRExprBit(n->rExprFirst,f,name,act);
    r2 = genActualArgsBit(n->actualArgs,f,name);
    //std::cout<<n->name<<"-------------"<<n->str<<std::endl;
    //std::cout<<n->str<<"-------- "<<std::endl;
    if (n->rExprFirst!=NULL) {
       symbol sym;
       std::cout<<n->rExprFirst->name<<"-------------"<<n->rExprFirst->str<<std::endl;
       if (strcmp(n->rExprFirst->str,"lexpr")!=0) {
          sym.name= std::string(n->rExprFirst->name); 
          sym=n->sTable->lookup(sym);
          std::string temp="";
          if (sym.type=="String")  temp="str_literal";
          if (sym.type=="Int")  temp="int_literal";
          //sym=n->sTable->prev->next->lookup(sym);
          //std::cout<<n->rExprFirst->name<<" "<<sym.type<<std::endl;
          f<<"  obj_"<<sym.type<<" "<<"var"<<var<<"="<<temp<<"("<<sym.name<<");\n";
          //f<<"  "<<"var"<<var<<"="<<sym.typesym.name<<";\n";
          var+=1; 
          if(strcmp(n->name,"PRINT")==0)
          if (act!=JUSTTYPE) 
             f<<"  var"<<(var-1)<<"->clazz->"<<n->name<<"("<<"var"<<(var-1)<<");\n";
          else
          if (act!=JUSTTYPE) 
             f<<"  var"<<(var-1)<<"->clazz->"<<n->name<<"("<<r2<<");\n";
       }
       else {
          
          sym.name= std::string(n->rExprFirst->lExpr->name); 
          sym=n->sTable->lookup(sym);
          if (sym.type=="String" || sym.type=="Int") 
          if (act!=JUSTTYPE) 
          f<<"  item"<<"->"<<std::string(n->rExprFirst->lExpr->name)<<"->clazz->"<<n->name<<"("<<"(obj_obj) item->"<<std::string(n->rExprFirst->lExpr->name)<<");\n";
          else  
          if (act!=JUSTTYPE) 
          f<<"  "<<std::string(n->rExprFirst->lExpr->name)<<"->clazz->"<<n->name<<"("<<std::string(n->rExprFirst->lExpr->name)<<");\n";
       }
    }
    else if (n->rExprFirst->lExpr->name!=NULL) {
      //else if (n->rExprFirst->lExpr->name!=NULL&&strcmp(n->rExprFirst->lExpr->rExpr->lExpr->name,"this")==0) {
      //std::cout<<"asdfasdf"<< std::string(n->rExprFirst->lExpr->name)<<std::endl;
      //if(strcmp(n->name,"PRINT")==0)
      //f<<"  item"<<"->"<<std::string(n->rExprFirst->lExpr->name)<<"->clazz->"<<n->name<<"("<<"(obj_obj) item->"<<std::string(n->rExprFirst->lExpr->name)<<");\n";
    }
    resname.append(n->name);
   /* if(strcmp(n->name,"PRINT")==0)
    f<<"  var"<<(var-1)<<"->clazz->"<<n->name<<"("<<"var"<<(var-1)<<");\n";
    else
    f<<"  var"<<(var-1)<<"->clazz->"<<n->name<<"("<<r2<<");\n";
*/ 
    return resname;
  }
//////MONIL/////



  // in genRExprBit    
  if (strcmp(n->str,"LESS")==0 || strcmp(n->str,"MORE")==0 ||
      strcmp(n->str,"ATLEAST")==0 || strcmp(n->str,"ATMOST")==0) {
    std::string comparison;
    if (strcmp(n->str,"LESS")==0)
      comparison = "<";
    if (strcmp(n->str,"MORE")==0)
      comparison = ">";
    if (strcmp(n->str,"ATLEAST")==0)
      comparison = ">=";
    if (strcmp(n->str,"ATMOST")==0)
      comparison = "<=";

    
    r1 = genRExprBit(n->rExprFirst,f,name,act);
    if (act==REG && TYPE!="int")
      f<<"  "<<TYPE<<" "<<r1<<";\n";
    r2 = genRExprBit(n->rExprSecond,f,name,act);
    if (act==REG && TYPE!="int")
      f<<"  "<<TYPE<<" "<<r2<<";\n";
    if (act==REG)
      f<<"  "<<TYPE<<" "<<r1<<";\n";
    res.append(r1);
    res.append(comparison);    
    res.append(r2);
    return res;
  }

  if (strcmp(n->str,"FIRSTBRACE")==0) { //TODO TEST 
    res.append("(");
    r1 = genRExprBit(n->rExprFirst,f,name,act);
    res.append(r1);
    res.append(")");
    return res;
  }

  if (strcmp(n->str,"NOT")==0) { //TODO TEST 
    res.append("!");
    r1 = genRExprBit(n->rExprFirst,f,name,act);
    res.append(r1);
    return res;
  }

  if (strcmp(n->str,"AND")==0 || strcmp(n->str,"OR")==0 || strcmp(n->str,"EQUALS")==0) { //TODO TEST
    r1 = genRExprBit(n->rExprFirst,f,name,act);
    r2 = genRExprBit(n->rExprSecond,f,name,act);
    res.append(r1);
    if (strcmp(n->str,"AND")==0)
      res.append(" && ");
    if (strcmp(n->str,"OR")==0)
      res.append(" || ");
    if (strcmp(n->str,"EQUALS")==0)
      res.append(" == ");
    res.append(r2);
    return res;
  }

  if (strcmp(n->str,"NEG")==0) {
    r1 = genRExprBit(n->rExprFirst,f,name,act);
    res.append("-");
    res.append(r1);
    return res;
  }

  if (!found)
    f<<"===ARTLESS:"<<n->str<<std::endl;
  return "";
}

void genConstructor(classNode *n, std::ofstream &f, char const *name, int act)
{
  f<<"obj_"<<name<<" new_"<<name<<"(";
    
  genMethodArgs(n->sig->fArguments,f,name,NOTAMETHODDEF);
  f<<") {\n";
  std::string argName="  item";
  f<<"  obj_"<<name<<" "<<argName<<" = (obj_"<<name<<")\n";
  f<<"  malloc(sizeof(struct obj_"<<name<<"_struct));\n";
  f<<"  item->clazz = the_class_"<<name<<";\n";
  clearNames();
  for (statementNode &s : n->classBody->statements->list) {
    TYPE="";
    genStatement(&s,f,NOTYPE);
  }

  f<<"  return item;\n";
  f<<"};\n";
}



void genClassMethods(methodNode *n, std::ofstream &f, char const *name, int act) {
  // methodNode: statementBlock, fArguments
  f<<"obj_"<<n->returnType<<" "<<name<<"_method_"<<n->name<<"(";
  genMethodArgs(n->fArguments,f,name,METHODDEF);  
  f<<"){\n";

  // ARTLESS TODO
  std::string r1="";
  clearNames();
  
  if (n->statementBlock==NULL) return;
  if (n->statementBlock->statements==NULL) return;

  for (statementNode s : n->statementBlock->statements->list) {
    TYPE="";
    genStatement(&s,f,GENSTATEMENTS);
  }

  f<<"};\n\n";
}

void genSingleton(classNode *n, std::ofstream &f, char const *name)
{

  f<<"struct class_"<<name<<"_struct the_class_"<<name<<"_struct = {\n";
  f<<"  new_"<<name;
  for (std::string name : singleton) {
    f<<",\n  ";
    f<<name;
  }
  f<<"\n};\n";
}

void genStructs(classNode *n, std::ofstream &f, char const *name, int act)
{
  singleton.clear();
  // THIS WILL BE THE FUNCTION POINTERS
  genClassFuncPointerStruct(n,f,name,act);

  // THIS WILL BE THE STRUCT
  if (n->classBody!=NULL && n->classBody->statements!=NULL) {
    f<<"\n";

    genConstructor(n,f,name,act);
    for (methodNode &s : n->classBody->methods->list) {
      genClassMethods(&s,f,name,act);
    }
  }
  // GEN THE SINGLETON
  genSingleton(n,f,name);
  f<<"class_"<<name<<" the_class_"<<name<<" = "<<"&the_class_"<<name<<"_struct;\n";
}

void genClass(classNode *n, std::ofstream &f, int act)
{
  if (n->sig != NULL)
    genSignature(n, f, act);
  f<<std::endl;
  if (n->classBody != NULL)
    genStructs(n, f, n->sig->name, act);
}

void genProgram(ProgramNode *n, std::ofstream &f, int act)
{
  if (act==GENSTART) {
    f<<"#include <stdio.h>\n";
    f<<"#include <stdlib.h>\n";
    f<<"#include \"Builtins.h\"\n\n";
  }

  if (act==GENCLASSES)
  for (auto &c : n->classes.list)
    genClass(&c, f, act);

  
  if (act==GENSTATEMENTS) {
    clearNames();
    f<<"\nint main()\n{\n";
    for (statementNode &s : n->statements.list) {
      TYPE="";
      genStatement(&s,f,act);
    }

    if (FIN) f<<"  printf(\"Fin\\n\");\n";
    f<<"}\n\n";
  }

}

void generate(std::string fileName){
  fileName = fileName.substr(0,fileName.size()-3);
  fileName = fileName.substr(fileName.find_last_of("\\/")+1,fileName.size());
  fileName = fileName + ".c";
  std::ofstream f(fileName);

  if (!f.is_open())
    std::cerr<<"Error opening output.c to generate code\n";
  genProgram(root, f, GENSTART);
  genProgram(root, f, GENCLASSES);
  genProgram(root, f, GENSTATEMENTS);

  f.close();
  std::cout<<"Fin Generate"<<std::endl;
}
