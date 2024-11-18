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

bool CartesianTree::remove(std::string key) {
  Node *del = find(key);
  if (del == nullptr) {
    return false;
  }

  Node **parent_del = nullptr;
  for (auto element : *this) {
    if (element->left == del) {
      parent_del = &element;
    }
    if (element->right == del) {
      parent_del = &element;
    }
  }

  Node *child = nullptr;
  while (del->right != nullptr && del->left != nullptr) {
    std::cout << "We still need to perform an inversion" << std::endl;
    if (del->right == nullptr && del->left != nullptr) {
      // we switch with left when right does not exist
      child = del->left;
      del->left = child->left;

      child->left = del;
      child->right = nullptr;

      if (parent_del) {
        *parent_del = child;
      }

      parent_del = &child;
    } else if (del->right != nullptr && del->left == nullptr) {
      // we switch with right when left does not exist
      child = del->right;
      del->right = child->right;

      child->right = del;
      child->left = nullptr;

      if (parent_del) {
        *parent_del = child;
      }

      parent_del = &child;
    } else {
      if (del->left->priority < del->right->priority) {
        // we switch with left when right does exist
        child = del->left;
        del->left = child->left;
        del->right = child->right;

        child->left = del;
        child->right = del->right;

        if (parent_del) {
          *parent_del = child;
        }

        parent_del = &child;
      } else {
        // we switch with right when left does exist
        child = del->right;
        del->right = child->right;
        del->left = child->left;

        child->right = del;
        child->left = del->left;

        if (parent_del) {
          *parent_del = child;
        }

        parent_del = &child;
      }
    }
  }

  Node *parent = (child) ? child : *parent_del;

  std::cout << "The parent of " << del << " is " << parent << std::endl;

  if (parent && parent->left == del) {
    parent->left = nullptr;
  }
  if (parent && parent->right == del) {
    parent->right = nullptr;
  }
  delete del;

  return true;
}

CartesianTree::~CartesianTree() { delete root; }

} // namespace complex
