#include <iostream>
#include <string>
#include "Node.h"

// class Node {
// protected:
//   std::string name;

// public:
//   Node(std::string name);
//   virtual void print(void) = 0;

// };

Node::Node(void){ setName("Empty"); return; }

Node::Node(std::string s){
  setName(s);
  return;
}


void Program::printNode(void) {
  std::cout << "node\n"; return;
};
