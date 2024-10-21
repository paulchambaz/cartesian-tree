#pragma once

#include "Node.h"

namespace complex {

class CartesianTree {
  Node *root;

public:
  CartesianTree() : root(nullptr) {}
  CartesianTree(Node *root) : root(root) {}

  bool insert(std::string key, unsigned int priority);

  Node *find(std::string key);

  bool is_empty() { return root == nullptr; }

  ~CartesianTree();
};

} // namespace complex
