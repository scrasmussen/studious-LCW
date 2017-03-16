#include <vector>
#include <list>
#include <string>
#include <string.h>

#ifndef GENERATE_H
#define GENERATE_H

static int GENSTART=2;
static int GENSTATEMENTS=3;
static int GENCLASSES=5;

void generate();
std::string genRExprBit(rExprNode*,std::ofstream&,char const*);
std::string genLExprBit(lExprNode*,std::ofstream&,char const*);

extern ProgramNode *root;
#endif

