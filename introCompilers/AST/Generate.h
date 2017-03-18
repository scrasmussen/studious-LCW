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

void generate();
std::string genRExprBit(rExprNode*,std::ofstream&,char const*);
std::string genLExprBit(lExprNode*,std::ofstream&,char const*, std::string);

extern ProgramNode *root;
#endif

