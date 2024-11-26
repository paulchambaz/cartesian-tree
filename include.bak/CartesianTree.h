#pragma once

#include "Node.h"
#include <stack>

namespace complex {

class CartesianTree {
  Node *removeRoot(Node *root);
  Node *leftRotateRoot(Node *root);
  Node *rightRotateRoot(Node *root);
  void leftRotate(Node *parent, Node *root);
  void rightRotate(Node *parent, Node *root);

public:
  Node *root;

  class iterator {
    std::stack<Node *> stack;

  public:
    iterator() {}
    iterator(Node *node) {
      if (node)
        stack.push(node);
    }

    Node *operator*() const { return stack.empty() ? nullptr : stack.top(); }

    iterator &operator++() {
      if (!stack.empty()) {
        Node *node = stack.top();
        stack.pop();
        if (node->right)
          stack.push(node->right);
        if (node->left)
          stack.push(node->left);
      }

      return *this;
    }

    bool operator!=(const iterator &other) const {
      return !stack.empty() || !other.stack.empty();
    }
  };

  CartesianTree() : root(nullptr) {}
  CartesianTree(Node *root) : root(root) {}

  bool insert(std::string key, unsigned int priority);

  bool remove(std::string key);

  Node *find(std::string key);

  bool is_empty() { return root == nullptr; }

  iterator begin() const { return iterator(root); }
  iterator end() const { return iterator(); }

  // TODO: ecrire un operateur == qui compare qu'on a bien les memes noeuds aux
  // memes endroits

  ~CartesianTree();
};

} // namespace complex
