#include "CartesianTree.h"
#include "Node.h"
#include <cassert>
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

  if (del == root) {
    root = removeRoot(root);
    delete del;
    return true;
  }

  Node *parent = nullptr;
  for (auto node : *this) {
    if (node->left == del || node->right == del) {
      parent = node;
      break;
    }
  }

  while (del->left != nullptr || del->right != nullptr) {
    // if only left child exists
    if (del->right == nullptr) {
      rightRotate(parent, del);
    }
    // if only right child exists
    else if (del->left == nullptr) {
      leftRotate(parent, del);
    }
    // if both children exist, rotate with the higher priority child
    else if (del->left->priority < del->right->priority) {
      leftRotate(parent, del);
    } else {
      rightRotate(parent, del);
    }
  }

  // now del is a leaf node, simply remove it
  if (parent->left == del) {
    parent->left = nullptr;
  } else {
    parent->right = nullptr;
  }
  delete del;

  return true;
}

Node *CartesianTree::removeRoot(Node *root) {
  if (root == nullptr) {
    return nullptr;
  }

  if (root->left == nullptr && root->right == nullptr) {
    return nullptr;
  }

  if (root->left == nullptr) {
    return root->right;
  }
  if (root->right == nullptr) {
    return root->left;
  }

  if (root->left->priority < root->right->priority) {
    return leftRotateRoot(root);
  } else {
    return rightRotateRoot(root);
  }
}

Node *CartesianTree::leftRotateRoot(Node *root) {
  Node *node = root->right;
  Node *T2 = node->left;

  node->left = root;
  root->right = T2;

  return node;
}

Node *CartesianTree::rightRotateRoot(Node *root) {
  Node *node = root->left;
  Node *T2 = node->right;

  node->right = root;
  root->left = T2;

  return node;
}

void CartesianTree::leftRotate(Node *parent, Node *root) {
  Node *node = root->right;
  Node *T2 = node->left;

  node->left = root;
  root->right = T2;

  if (parent->left == root) {
    parent->left = node;
  } else {
    parent->right = node;
  }

  // node->parent = parent;
  // root->parent = node;
  // if (T2) {
  //   T2->parent = x;
  // }
}

void CartesianTree::rightRotate(Node *parent, Node *root) {
  Node *node = root->left;
  Node *T2 = node->right;

  node->right = root;
  root->left = T2;

  if (parent->left == root) {
    parent->left = node;
  } else {
    parent->right = node;
  }

  // node->parent = parent;
  // node->parent = node;
  // if (T2) {
  //   T2->parent = y;
  // }
}

CartesianTree::~CartesianTree() { delete root; }

} // namespace complex
