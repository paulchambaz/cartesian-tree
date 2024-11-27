#pragma once

namespace complex {
template <typename K, typename P> class BinarySearchTree {
public:
  struct Node {
    K key;
    P priority;
    Node *left;
    Node *right;

    Node(const K &k, const P &p)
        : key(k), priority(p), left(nullptr), right(nullptr) {}
  };

  Node *root_ = nullptr;

  ~BinarySearchTree() {
    while (root_) {
      remove(root_->key);
    }
  }

  void insert(const K &key, const P &priority) {
    Node **curr = &root_;
    while (*curr) {
      if (key < (*curr)->key) {
        curr = &(*curr)->left;
      } else if (key > (*curr)->key) {
        curr = &(*curr)->right;
      } else {
        return;
      }
    }
    *curr = new Node(key, priority);
  }

  bool find(const K &key) const {
    Node *curr = root_;
    while (curr) {
      if (key == curr->key)
        return true;
      curr = (key < curr->key) ? curr->left : curr->right;
    }
    return false;
  }

  void remove(const K &key) {
    Node **curr = &root_;
    Node *to_delete = nullptr;

    while (*curr && (*curr)->key != key) {
      curr = (key < (*curr)->key) ? &(*curr)->left : &(*curr)->right;
    }

    if (!*curr)
      return;

    to_delete = *curr;

    if (!to_delete->left) {
      *curr = to_delete->right;
      delete to_delete;
    } else if (!to_delete->right) {
      *curr = to_delete->left;
      delete to_delete;
    }

    else {
      Node **successor = &to_delete->right;
      while ((*successor)->left) {
        successor = &(*successor)->left;
      }

      to_delete->key = (*successor)->key;
      to_delete->priority = (*successor)->priority;

      Node *temp = *successor;
      *successor = (*successor)->right;
      delete temp;
    }
  }
};

} // namespace complex
