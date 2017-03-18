#include <vector>
#include <list>
#include <string>
#include <string.h>

#ifndef GENERATE_H
#define GENERATE_H

static int GENSTART=2;
static int GENSTATEMENTS=3;
static int GENCLASSES=5;
static int NOTAMETHODDEF=7;
static int METHODDEF=11;
static int NOTYPE=13;
static int JUSTTYPE=17;
static int REG=19;

void generate();
std::string genRExprBit(rExprNode*,std::ofstream&,char const*, int);
std::string genLExprBit(lExprNode*,std::ofstream&,char const*,std::string,int);
std::string genStatement(statementNode*,std::ofstream&,int);

extern ProgramNode *root;
#endif

