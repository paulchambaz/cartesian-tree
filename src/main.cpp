#include "CartesianTree.hpp"
#include <iostream>
#include <vector>

using namespace std;
using namespace complex;

void exercise_3() {
  CartesianTree<int> tree;
  // vector<pair<string, int>> list_1 = {
  //     {"A", 5}, {"B", 3}, {"C", 8}, {"D", 2},  {"E", 6},
  //     {"F", 7}, {"G", 9}, {"H", 1}, {"I", 10}, {"J", 12},
  // };
  vector<pair<string, int>> list = {
      {"A", 5},
      {"B", 3},
  };

  for (const auto &element : list) {
    tree.insert(element.first, element.second);
  }

  cout << "Cartesian tree" << endl;
  cout << "Root: " << tree.root_.get() << endl;
  cout << "Left: " << tree.root_.get()->left.get() << endl;
  cout << "Right: " << tree.root_.get()->right.get() << endl;

  // for (const auto &element : tree_1) {
  //   cout << element << endl;
  // }
  // cout << endl;

  // auto *res = tree_1.find("E");
  //
  // if (!res) {
  //   cout << "could not find E in the tree" << endl;
  //   return;
  // }
  //
  // cout << res << endl;
}

int main() { exercise_3(); }
