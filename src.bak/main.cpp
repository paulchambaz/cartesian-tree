#include "CartesianTree.h"
#include "Node.h"
#include <iostream>
#include <vector>

using namespace complex;
using namespace std;

void exercice_1() {
  Node *a5 = new Node("A", 5, nullptr, nullptr);
  Node *c8 = new Node("C", 8, nullptr, nullptr);
  Node *b3 = new Node("B", 3, a5, c8);

  Node *g9 = new Node("G", 9, nullptr, nullptr);
  Node *f7 = new Node("F", 7, nullptr, g9);
  Node *e6 = new Node("E", 6, nullptr, f7);

  Node *d2 = new Node("D", 2, b3, e6);

  Node *j12 = new Node("J", 12, nullptr, nullptr);
  Node *i10 = new Node("I", 10, nullptr, j12);

  Node *h1 = new Node("H", 1, d2, i10);

  CartesianTree tree(h1);
}

void exercice_2() {

  Node a5("A", 5, nullptr, nullptr);
  Node c8("C", 8, nullptr, nullptr);
  Node b3("B", 3, &a5, &c8);

  Node g9("G", 9, nullptr, nullptr);
  Node f7("F", 7, nullptr, &g9);
  Node e6("E", 6, nullptr, &f7);

  Node d2("D", 2, &b3, &e6);

  Node j12("J", 12, nullptr, nullptr);
  Node i10("I", 10, nullptr, &j12);

  Node h1("H", 1, &d2, &i10);

  CartesianTree tree(&h1);

  Node *res = tree.find("E");
  cout << res->key << res->priority << endl;
}

void exercice_3() {
  CartesianTree tree_1;
  vector<pair<string, unsigned int>> list_1 = {
      {"A", 5}, {"B", 3}, {"C", 8}, {"D", 2},  {"E", 6},
      {"F", 7}, {"G", 9}, {"H", 1}, {"I", 10}, {"J", 12},
  };

  for (auto &element : list_1) {
    tree_1.insert(element.first, element.second);
  }

  for (auto element : tree_1) {
    cout << element << endl;
  }
  cout << endl;

  CartesianTree tree_2;
  vector<pair<string, unsigned int>> list_2 = {
      {"H", 1}, {"G", 9}, {"A", 5},  {"B", 3},  {"D", 2},
      {"F", 7}, {"C", 8}, {"J", 12}, {"I", 10}, {"E", 6},
  };

  for (auto &element : list_2) {
    tree_2.insert(element.first, element.second);
  }

  for (auto element : tree_2) {
    cout << element << endl;
  }
  cout << endl;

  CartesianTree tree_3;
  vector<pair<string, unsigned int>> list_3 = {
      {"E", 6}, {"H", 1}, {"B", 3},  {"D", 2}, {"C", 8},
      {"F", 7}, {"G", 9}, {"J", 12}, {"A", 5}, {"I", 10},
  };

  for (auto &element : list_3) {
    tree_3.insert(element.first, element.second);
  }

  for (auto element : tree_3) {
    cout << element << endl;
  }
  cout << endl;
}

void exercice_4() {
  CartesianTree tree;
  vector<pair<string, unsigned int>> list = {
      {"A", 5}, {"B", 3}, {"C", 8}, {"D", 2},  {"E", 6},
      {"F", 7}, {"G", 9}, {"H", 1}, {"I", 10}, {"J", 12},
  };

  for (auto &element : list) {
    tree.insert(element.first, element.second);
  }

  for (auto element : tree) {
    cout << element << endl;
  }
  cout << endl;

  // vector<string> to_remove = {"A", "J", "H"};
  vector<string> to_remove = {"A", "J", "D"};
  for (auto &element : to_remove) {
    cout << "Removing: " << element << endl;
    if (tree.remove(element) == false) {
      cout << "There was an error removing an element from the tree" << endl;
    }

    for (auto element : tree) {
      cout << element << endl;
    }
    cout << endl;
  }
}

int main() { exercice_4(); }
