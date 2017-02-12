#include <iostream>
#include <string>
#include <vector>
#include "Node.h"

Node::Node(void){ setName("Empty"); return; }
Node::~Node(void){ return; }

Node::Node(std::string s){
  setName(s);
  return;
}


void pNode::printNode(void) {
  std::cout << "In Program node\n"; return;
};

void Classes::printNode(void) {
  std::cout << "In Classes node\n"; return;
};
