#include "CartesianTree.h"
#include "Node.h"
#include <cstdlib>
#include <iostream>
#include <vector>

namespace complex {

bool CartesianTree::insert(std::string key, unsigned int priority) {
  Node *node = new Node(key, priority);

  if (is_empty()) {
    root = node;
    return true;
  }

  std::vector<Node *> stack;
  stack.push_back(root);

  while (stack.at(stack.size() - 1) != nullptr) {
    if (key < stack.at(stack.size() - 1)->key) {
      stack.push_back(stack.at(stack.size() - 1)->left);
    } else if (key > stack.at(stack.size() - 1)->key) {
      stack.push_back(stack.at(stack.size() - 1)->right);
    } else {
      std::cout << "Error, key of inserted node matches already present key in "
                   "the tree"
                << std::endl;
      return false;
    }
  }

  for (auto element : stack) {
    std::cout << element->key << element->priority << std::endl;
  }

  // we are at the correct leaf
  std::cout << node->key << node->priority << std::endl;

  return true;
}

Node *CartesianTree::find(std::string key) {
  Node *current = root;

  while (current != nullptr) {
    if (key < current->key) {
      current = current->left;
    } else if (key > current->key) {
      current = current->right;
    } else {
      return current;
    }
  }

  return nullptr;
}

CartesianTree::~CartesianTree() { delete root; }

} // namespace complex
