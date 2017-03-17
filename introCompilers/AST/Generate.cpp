#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "Node.h"
#include "Generate.h"

int Q=1;
int FIN=1;
std::string TYPE;
std::vector<std::string> currentNames, tmpNames;


void clearNames() {
  currentNames.clear();
  tmpNames.clear();
}

// replace was plundered from Mateen Ulhaq  on stackoverflow
std::string replace(std::string s, const std::string toReplace, const std::string replaceWith) {
  if (s.find(toReplace)==std::string::npos)
    return s;
  return(s.replace(s.find(toReplace), toReplace.length(), replaceWith));
}

std::string cleanString(std::string word) {
  // ARTLESS TODO: Add all other characters in string that could cause trouble
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

void genClassArgLines(std::ofstream &f, classSignatureNode *n) {
  f<<"typedef struct obj_"<<n->name<<"_struct {\n";
  f<<"  class_"<<n->name<<" clazz;\n";
  for (int i = n->fArguments->list.size(); i --> 0; ) {
    auto a = n->fArguments->list[i];
    f<<"  obj_"<<a->type<<" "<<a->name<<";\n";
  }
  f<<"} * obj_"<<n->name<<";\n";
}

void genLExpr(lExprNode *n, std::ofstream &f, int act)
{
  if (act==GENSTATEMENTS)
    f<<n->name;
}

std::string genStatement(statementNode *n, std::ofstream &f, int act)
{
  // statementNode: rExpr, lExpr, stblock, elifs, elseN
  act=GENSTATEMENTS;
  std::string res="",r1,r2, type="  ";
  if (act==GENSTATEMENTS) {

  if (strcmp(n->str,"ASSIGN")==0) {
    char const * blank="[A]";
    r1 = genLExprBit(n->lExpr, f, blank, "[B]"); // TODO, NEED TO TYPE THIS
    r2 = genRExprBit(n->rExpr, f, "[C]");
    if(std::find(currentNames.begin(), currentNames.end(), r1) == currentNames.end()) {
      type="  "+TYPE;
      // type="  getType("+r1+")  ";
      currentNames.push_back(r1);
    }
    
    f<<type<<" "<<r1;
    f<<"=";
    f<<r2;
    f<<";\n";
    return res;
  }

  if (strcmp(n->str,"RETURN")==0) {
    r1=genRExprBit(n->rExpr,f,"[NAME]");
    f<<"  return "<<r1<<";\n";
    return res;
  }

  if (strcmp(n->str,"RETURN VOID")==0) {
    f<<"  return;\n";
    return res;
  }

  if (strcmp(n->str,"WHILE")==0) {
    r1=genRExprBit(n->rExpr,f,"[NAME]");
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
    r1=genRExprBit(n->rExpr,f,"[NAME]");
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
      r1=genRExprBit(e->rExpr,f,"[NAME]");
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
    r1=genRExprBit(n->rExpr,f,"");  // TODO : TEST, what will create this?
    // f<<r1<<std::endl;
    return res;
  }

  // n->str=LONG_ASSIGN is not implemented, there are no test cases so it's not beign down right now
  f<<"===TODO:"<<n->str<<std::endl;
  }
  return res;
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

void genBuiltinFuncPointers(std::string ext, std::vector<std::string> defined, std::ofstream &f)
{

  f<<"// build-in methods\n";
  // TODO Have to check what has been defined?? Maybe it will work without that?
  if (ext=="Obj") {
    // STRING, PRINT, EQUALS
    f<<"  obj_String (*STRING) (obj_Obj);\n";
    f<<"  obj_Obj (*PRINT) (obj_Obj);\n";
    f<<"  obj_Boolean (*EQUALS) (obj_Obj, obj_Obj);\n";
  }
  else if (ext=="String") {
    f<<"  obj_String (*STRING) (obj_String);\n";
    f<<"  obj_String (*PRINT) (obj_String);\n";
    f<<"  obj_Boolean (*EQUALS) (obj_String, obj_Obj);\n";
    f<<"// ===DO WE NEED TO ADD MORE??===\n";
    f<<"// ===YES===\n";
  }
  else if (ext=="Int") {
    f<<"  obj_Int (*constructor) ( void );\n";
    f<<"  obj_String (*STRING) (obj_Int);\n";
    f<<"  obj_Obj (*PRINT) (obj_Obj);\n";
    f<<"  obj_Boolean (*EQUALS) (obj_Int, obj_Obj);\n";
    f<<"  obj_Boolean (*LESS) (obj_Int, obj_Int);\n";
    f<<"  obj_Int (*PLUS) (obj_Int, obj_Int);\n";
  }
  else if (ext=="Boolean") {
    f<<"  obj_String (*STRING) (obj_Boolean);\n";
    f<<"  obj_Obj (*PRINT) (obj_Obj);\n";
    f<<"  obj_Boolean (*EQUALS) (obj_Obj, obj_Obj);\n";
  }
  else if (ext=="Nothing") {
    f<<"  obj_String (*STRING) (obj_Nothing);\n";
    f<<"  obj_Obj (*PRINT) (obj_Obj);\n";
    f<<"  obj_Boolean (*EQUALS) (obj_Obj, obj_Obj);\n";
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
  f<<"// class's methods\n";
  for (methodNode m : n->classBody->methods->list) {
    defined.push_back(m.name);
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
std::string genLExprBit(lExprNode *n, std::ofstream &f, char const *name, std::string argName)
{
  std::string res="";
  if (strcmp(n->str,"")==0) {
    res.append(n->name);
    return res;
  }
  else if (strcmp(n->str,".")==0) {
    res.append(argName);
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
    s.append(genRExprBit(e,f,name));
    std::cout<<"ART"<<s<<std::endl;
  }
  return s;
}

std::string genRExprBit(rExprNode *n, std::ofstream &f, char const *name)//, char const *clazz)
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
    res.append(genLExprBit(n->lExpr,f,name,"item"));
    if (Q) std::cout<<"lexpr:"<<res<<std::endl;
    return res;
  }

  if (strcmp(n->str,"PLUS")==0 || (strcmp(n->str,"MINUS")==0) ||
      strcmp(n->str,"TIMES")==0 || (strcmp(n->str,"DIVIDE")==0)){
    found=1;
    r1=genRExprBit(n->rExprFirst,f,name);
    res.append(cleanString(r1));

    r3=genRExprBit(n->rExprSecond,f,name);
    res.append(cleanString(r3));

    type="";
    // ===MONIL===
    if(std::find(tmpNames.begin(), tmpNames.end(), res) == tmpNames.end()) {
      type = "obj_TYPE";
      tmpNames.push_back(res);
    }
    TYPE="obj_TYPE";
    f<<std::endl<<"  "<<type<<" "<<res<<" = "<<"[item]->clazz->";
    // ===MONIL===
    
    if (strcmp(n->str,"PLUS")==0) f<<"PLUS";
    if (strcmp(n->str,"MINUS")==0) f<<"MINUS";
    if (strcmp(n->str,"TIMES")==0) f<<"TIMES";
    if (strcmp(n->str,"DIVIDE")==0) f<<"DIVIDE";
    f<<"(";
    f<<r1<<","<<r3<<");\n";
    if (Q) std::cout<<"PLUS|MINUS|TIMES|DIVIDE:"<<res<<std::endl;
    return res;
  }
  
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
    f<<"  "<<type<<" "<<resname<<" = ";
    f<<"the_class_"<<(n->name)<<"->constructor(";
    f<<r1;
    f<<");\n";
    if (Q) std::cout<<"constructor:"<<res<<std::endl;
    return resname;
  }

  if (strcmp(n->str,"method")==0) {
    r1 = genRExprBit(n->rExprFirst,f,name);
    r2 = genActualArgsBit(n->actualArgs,f,name);
    resname.append(n->name);
    f<<"  obj_"<<type<<" "<<resname<<"="<<r1<<"->clazz->"<<n->name<<"("<<r2<<");\n";
    return resname;
  }


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

    
    r1 = genRExprBit(n->rExprFirst,f,name);
    r2 = genRExprBit(n->rExprSecond,f,name);
    res.append(r1);
    res.append(comparison);    
    res.append(r2);
    return res;
  }

  if (strcmp(n->str,"FIRSTBRACE")==0) { //TODO TEST 
    res.append("(");
    r1 = genRExprBit(n->rExprFirst,f,name);
    res.append(r1);
    res.append(")");
    return res;
  }

  if (strcmp(n->str,"NOT")==0) { //TODO TEST 
    res.append("!");
    r1 = genRExprBit(n->rExprFirst,f,name);
    res.append(r1);
    return res;
  }

  if (strcmp(n->str,"AND")==0 || strcmp(n->str,"OR")==0 || strcmp(n->str,"EQUALS")==0) { //TODO TEST
    r1 = genRExprBit(n->rExprFirst,f,name);
    r2 = genRExprBit(n->rExprSecond,f,name);
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
    r1 = genRExprBit(n->rExprFirst,f,name);
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
    genStatement(&s,f,GENSTATEMENTS);
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

void genStructs(classNode *n, std::ofstream &f, char const *name, int act)
{
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
