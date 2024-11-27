#pragma once

#include <iostream>
#include <ostream>
#include <stack>
#include <string>

namespace complex {

template <typename K, typename P> class CartesianTree {
public:
  struct Node {

  public:
    K key;
    P priority;

    Node *left;
    Node *right;

    Node(const K &k, P p, Node *left, Node *right)
        : key(k), priority(p), left(left), right(right) {}

    ~Node() {
      delete left;
      delete right;
    }

    friend std::ostream &operator<<(std::ostream &os, const Node *node) {
      if (node) {
        os << node->key << node->priority;
      }
      return os;
    }
  };

  Node *root_;
  size_t size_;

  class iterator {
  private:
    std::stack<Node *> stack_;

  public:
    explicit iterator(Node *node) {
      if (node) {
        stack_.push(node);
      }
    }

    Node *operator*() const {
      if (stack_.empty()) {
        return nullptr;
      }
      return stack_.top();
    }

    iterator &operator++() {
      if (stack_.empty()) {
        return *this;
      }

      Node *current = stack_.top();
      stack_.pop();

      if (current->right) {
        stack_.push(current->right);
      }

      if (current->left) {
        stack_.push(current->left);
      }

      return *this;
    }

    bool operator!=(const iterator &other) const {
      // Empty stacks are equal
      if (stack_.empty() && other.stack_.empty()) {
        return false;
      }

      // One empty stack means iterators are different
      if (stack_.empty() || other.stack_.empty()) {
        return true;
      }

      // Compare top nodes
      return stack_.top() != other.stack_.top();
    }
  };

  CartesianTree() : root_(nullptr), size_(0) {}
  CartesianTree(Node *root) : root_(root), size_(0) {}

  ~CartesianTree() { delete root_; };

  iterator begin() const { return iterator(root_); }
  iterator end() const { return iterator(nullptr); }

  bool is_empty() const { return root_ == nullptr; }
  size_t size() const { return size_; }

  const Node *find(const std::string &key) const {
    Node *current = root_;

    while (current) {
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

  bool insert(K key, P priority) {

    if (is_empty()) {
      root_ = new Node(key, priority, nullptr, nullptr);
      return true;
    }

    auto [parent, path] = find_insert_node(root_, key);
    if (parent == nullptr) { // Key already exists
      return false;
    }

    Node *new_node = new Node(key, priority, nullptr, nullptr);

    insert_node(parent, new_node, key);
    path.push(new_node);
    restore_heap(root_, path);

    return true;
  }

  bool remove(const std::string &key) {
    if (is_empty()) {
      return false;
    }

    // Find the node to remove and track the path to it
    Node *current = root_;
    Node *parent = nullptr;
    bool is_left_child = false;

    while (current && current->key != key) {
      parent = current;
      if (key < current->key) {
        current = current->left;
        is_left_child = true;
      } else {
        current = current->right;
        is_left_child = false;
      }
    }

    // If key not found
    if (!current) {
      return false;
    }

    // Rotate the node down until it becomes a leaf
    while (current->left || current->right) {
      // If only right child exists or right child has lower priority
      if (!current->left || (current->right && current->right->priority <
                                                   current->left->priority)) {
        if (parent) {
          if (is_left_child) {
            parent->left = current->right;
          } else {
            parent->right = current->right;
          }
        } else {
          root_ = current->right;
        }
        Node *right = current->right;
        current->right = right->left;
        right->left = current;
        parent = right;
        is_left_child = true;
      }
      // If only left child exists or left child has lower priority
      else {
        if (parent) {
          if (is_left_child) {
            parent->left = current->left;
          } else {
            parent->right = current->left;
          }
        } else {
          root_ = current->left;
        }
        Node *left = current->left;
        current->left = left->right;
        left->right = current;
        parent = left;
        is_left_child = false;
      }
    }

    // Now current is a leaf node, we can delete it
    if (parent) {
      if (is_left_child) {
        parent->left = nullptr;
      } else {
        parent->right = nullptr;
      }
    } else {
      root_ = nullptr;
    }

    // Important: null out pointers before deletion to prevent recursive
    // deletion
    current->left = nullptr;
    current->right = nullptr;
    delete current;
    size_--;

    return true;
  }

  friend std::ostream &operator<<(std::ostream &os, const CartesianTree &tree) {
    for (const auto &node : tree) {
      os << &node << std::endl;
    }
    return os;
  }

private:
  std::pair<Node *, std::stack<Node *>>
  find_insert_node(Node *root, const std::string &key) {
    std::stack<Node *> path;
    Node *current = root;
    Node *parent = nullptr;

    while (current != nullptr) {
      if (key == current->key) {
        return {nullptr, path}; // Key already exists
      }
      path.push(current);
      parent = current;
      if (key < current->key) {
        current = current->left;
      } else {
        current = current->right;
      }
    }
    return {parent, path};
  }

  void insert_node(Node *parent, Node *new_node, const std::string &key) {
    if (parent == nullptr) {
      return;
    }
    if (key < parent->key) {
      parent->left = new_node;
    } else {
      parent->right = new_node;
    }
  }

  void rotate_right(Node *&root, Node *parent, Node *child) {
    parent->left = child->right;
    child->right = parent;
    if (parent == root) {
      root = child;
    }
  }

  void rotate_left(Node *&root, Node *parent, Node *child) {
    parent->right = child->left;
    child->left = parent;
    if (parent == root) {
      root = child;
    }
  }

  void restore_heap(Node *&root, std::stack<Node *> &path) {
    while (path.size() >= 2) {
      Node *child = path.top();
      path.pop();
      Node *parent = path.top();
      path.pop();

      if (parent->priority <= child->priority) {
        path.push(parent);
        path.push(child);
        break;
      }

      if (parent->left == child) {
        rotate_right(root, parent, child);
      } else {
        rotate_left(root, parent, child);
      }

      if (!path.empty()) {
        Node *grandparent = path.top();
        if (grandparent->left == parent) {
          grandparent->left = child;
        } else {
          grandparent->right = child;
        }
      }

      path.push(child);
    }
  }
};

} // namespace complex
