#ifndef NODE_H
#define NODE_H

class Node {
 protected:
  std::string name;
 public:
  Node(void);
  Node(std::string s);
  void setName(std::string s) {name = s;}
  virtual void printNode() {return;};
};

class Program : Node {
 public:
  void printNode();
};

#endif
