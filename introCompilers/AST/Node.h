#include <vector>
#include <list>

#ifndef NODE_H
#define NODE_H


class Node {
 protected:
  std::string name;
  int val;
 public:
  Node(void);
  ~Node(void);
  Node(std::string s);
  void setName(std::string s) {name = s;}
  virtual void printNode() {return;};
};

class Classes : Node {
 public:
  void printNode();
};

class statement {
 public:
  virtual void print() {}
  virtual void evaluate() = 0;
};

class pNode {
 protected:
  std::list<statement *> *stmts;
 public:
  int t = 8;
  void printNode();
};



class cNode {
 public:
  void p() {std::cout<<"test\n";};
};




struct classSignatureNode {
  char* name;
  int val = -1;
};

struct classNode {
  std::vector<classSignatureNode> list;
};

struct ProgramNode {
  int val=7;
};


extern ProgramNode *root;
#endif
