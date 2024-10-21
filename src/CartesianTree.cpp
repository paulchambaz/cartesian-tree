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
  Node *current = root;

  while (current != nullptr) {
    if (key == current->key) {
      delete node;
      return false;
    }

    stack.push_back(current);

    if (key < current->key) {
      if (current->left == nullptr) {
        current->left = node;
        break;
      }
      current = current->left;
    } else {
      if (current->right == nullptr) {
        current->right = node;
        break;
      }
      current = current->right;
    }
  }

  stack.push_back(node);

  while (stack.size() >= 2) {
    Node *child = stack.back();
    stack.pop_back();

    Node *parent = stack.back();

    if (parent->priority <= child->priority) {
      break;
    }

    if (parent->left == child) {
      parent->left = child->right;
      child->right = parent;
    } else {
      parent->right = child->left;
      child->left = parent;
    }

    if (stack.size() > 1) {
      Node *grandparent = stack.at(stack.size() - 2);
      if (grandparent->left == parent) {
        grandparent->left = child;
      } else {
        grandparent->right = child;
      }
    } else {
      root = child;
    }

    stack.back() = child;
  }

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
