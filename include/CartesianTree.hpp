#pragma once

#include <iostream>
#include <memory>
#include <ostream>
#include <stack>
#include <string>

namespace complex {

template <typename T> class CartesianTree {
public:
  struct Node {

  public:
    std::string key;
    T priority;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    Node *parent;

    Node(const std::string &k, T p)
        : key(k), priority(p), left(nullptr), right(nullptr), parent(nullptr) {}

    friend std::ostream &operator<<(std::ostream &os, const Node *node) {
      if (node) {
        os << node->key << node->priority;
      }
      return os;
    }
  };

  // private:
  std::unique_ptr<Node> root_;
  size_t size_;

  // public:
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

      // Get current node and remove it from stack
      Node *current = stack_.top();
      stack_.pop();

      // Push right child first (so it's processed last)
      if (current->right) {
        stack_.push(current->right.get());
      }

      // Push left child second (so it's processed first)
      if (current->left) {
        stack_.push(current->left.get());
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

  CartesianTree(const CartesianTree &);
  CartesianTree &operator=(const CartesianTree &);
  ~CartesianTree() = default;

  iterator begin() const { return iterator(root_.get()); }

  iterator end() const { return iterator(nullptr); }

  bool is_empty() const { return root_ == nullptr; }
  size_t size() const { return size_; }

  const Node *find(const std::string &key) const {
    Node *current = root_.get();

    while (current) {
      if (key < current->key) {
        current = current->left.get();
      } else if (key > current->key) {
        current = current->right.get();
      } else {
        return current;
      }
    }

    return nullptr;
  }

  bool insert(std::string key, T priority) {
    if (key.empty()) {
      return false;
    }

    if (is_empty()) {
      root_ = std::make_unique<Node>(key, priority);
      size_ = 1;
      return true;
    }

    // find insert position
    std::stack<Node *> path;
    Node *current = root_.get();
    Node *parent = nullptr;
    while (current) {
      if (key == current->key) {
        return false; // duplicate key
      }
      path.push(current);
      parent = current;
      if (key < current->key) {
        if (!current->left) {
          break;
        }
        current = current->left.get();
      } else {
        if (!current->right) {
          break;
        }
        current = current->right.get();
      }
    }

    auto new_node = std::make_unique<Node>(key, priority);

    new_node->parent = parent;
    if (key < parent->key) {
      parent->left = std::move(new_node);
    } else {
      parent->right = std::move(new_node);
    }
    size_++;

    // restore the heap property
    path.push(new_node.get());
    while (!path.empty()) {
      Node *child = path.top();
      path.pop();

      Node *parent = child->parent;
      std::cout << "Child:" << child << std::endl;
      std::cout << "Parent:" << parent << std::endl;

      if (parent->priority <= child->priority) {
        break;
      }
    }

    return true;
  }

  bool remove(const std::string &key);

  friend std::ostream &operator<<(std::ostream &os, const CartesianTree &tree) {
    for (const auto &node : tree) {
      os << &node << std::endl;
    }
    return os;
  }

private:
  std::unique_ptr<Node> remove_root(std::unique_ptr<Node> root);
  void move_node_to_leaf(Node *node);
  bool remove_leaf(Node *node);

  std::unique_ptr<Node> clone_subtree(const Node *node);

  bool is_valid_bst() const;
  bool is_valid_heap() const;
};

} // namespace complex
