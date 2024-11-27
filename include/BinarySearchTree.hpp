#pragma once

namespace complex {

template <typename K> class BinarySearchTree {
public:
  struct Node {
    K key;
    Node *left;
    Node *right;
    Node(const K &k) : key(k), left(nullptr), right(nullptr) {}
  };
  
  Node *root_ = nullptr;
  
  ~BinarySearchTree() {
    while (root_) {
      remove(root_->key);
    }
  }

  void insert(const K &key) {
    Node **curr = &root_;
    while (*curr) {
      if (key < (*curr)->key) {
        curr = &(*curr)->left;
      } else if (key > (*curr)->key) {
        curr = &(*curr)->right;
      } else {
        return;  // Key already exists
      }
    }
    *curr = new Node(key);
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
    
    // Find the node to delete
    while (*curr && (*curr)->key != key) {
      curr = (key < (*curr)->key) ? &(*curr)->left : &(*curr)->right;
    }
    
    if (!*curr) return;  // Key not found
    
    to_delete = *curr;
    
    // Case 1: No left child
    if (!to_delete->left) {
      *curr = to_delete->right;
      delete to_delete;
    }
    // Case 2: No right child
    else if (!to_delete->right) {
      *curr = to_delete->left;
      delete to_delete;
    }
    // Case 3: Two children
    else {
      // Find successor (smallest key in right subtree)
      Node **successor = &to_delete->right;
      while ((*successor)->left) {
        successor = &(*successor)->left;
      }
      
      // Copy successor's key
      to_delete->key = (*successor)->key;
      
      // Delete successor
      Node *temp = *successor;
      *successor = (*successor)->right;
      delete temp;
    }
  }
};

} // namespace complex
