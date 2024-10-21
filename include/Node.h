#pragma once

#include <string>
namespace complex {

class Node {
public:
  std::string key;
  unsigned int priority;

  Node *left;
  Node *right;

  Node(std::string key, unsigned int priority)
      : key(key), priority(priority), left(nullptr), right(nullptr) {}
  Node(std::string key, unsigned int priority, Node *left, Node *right)
      : key(key), priority(priority), left(left), right(right) {}

  ~Node();
};

} // namespace complex
