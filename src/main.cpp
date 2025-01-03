#include "BinaryHeap.hpp"
#include "BinarySearchTree.hpp"
#include "CartesianTree.hpp"
#include "performance.h"
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;
using namespace complex;

void exercise_1() {
  cout << "\n\nExercise 1\n" << endl;

  auto *a5 = new CartesianTree<string, int>::Node("A", 5, nullptr, nullptr);
  auto *c8 = new CartesianTree<string, int>::Node("C", 8, nullptr, nullptr);
  auto *b3 = new CartesianTree<string, int>::Node("B", 3, a5, c8);

  auto *g9 = new CartesianTree<string, int>::Node("G", 9, nullptr, nullptr);
  auto *f7 = new CartesianTree<string, int>::Node("F", 7, nullptr, g9);
  auto *e6 = new CartesianTree<string, int>::Node("E", 6, nullptr, f7);

  auto *d2 = new CartesianTree<string, int>::Node("D", 2, b3, e6);

  auto *j12 = new CartesianTree<string, int>::Node("J", 12, nullptr, nullptr);
  auto *i10 = new CartesianTree<string, int>::Node("I", 10, nullptr, j12);

  auto *h1 = new CartesianTree<string, int>::Node("H", 1, d2, i10);

  CartesianTree<string, int> tree(h1);
}

void exercise_2() {
  cout << "\n\nExercise 2\n" << endl;

  // Create the tree nodes as before
  auto *a5 = new CartesianTree<string, int>::Node("A", 5, nullptr, nullptr);
  auto *c8 = new CartesianTree<string, int>::Node("C", 8, nullptr, nullptr);
  auto *b3 = new CartesianTree<string, int>::Node("B", 3, a5, c8);
  auto *g9 = new CartesianTree<string, int>::Node("G", 9, nullptr, nullptr);
  auto *f7 = new CartesianTree<string, int>::Node("F", 7, nullptr, g9);
  auto *e6 = new CartesianTree<string, int>::Node("E", 6, nullptr, f7);
  auto *d2 = new CartesianTree<string, int>::Node("D", 2, b3, e6);
  auto *j12 = new CartesianTree<string, int>::Node("J", 12, nullptr, nullptr);
  auto *i10 = new CartesianTree<string, int>::Node("I", 10, nullptr, j12);
  auto *h1 = new CartesianTree<string, int>::Node("H", 1, d2, i10);
  CartesianTree<string, int> tree(h1);

  // Test finding each node and verify its properties
  std::vector<std::pair<std::string, int>> expected = {
      {"A", 5}, {"B", 3}, {"C", 8}, {"D", 2},  {"E", 6},
      {"F", 7}, {"G", 9}, {"H", 1}, {"I", 10}, {"J", 12}};

  for (const auto &[key, priority] : expected) {
    auto *res = tree.find(key);
    assert(res && "Node should exist");
    assert(res->key == key && "Key should match");
    assert(res->priority == priority && "Priority should match");
    std::cout << "Verified: " << res->key << res->priority << std::endl;
  }

  // Test non-existent node
  auto *nonexistent = tree.find("X");
  assert(nonexistent == nullptr && "Non-existent node should return nullptr");

  std::cout << "All tests passed!" << std::endl;
}

void exercise_3() {
  cout << "\n\nExercise 3\n" << endl;

  // First tree
  CartesianTree<string, int> tree_1;
  vector<pair<string, int>> list_1 = {
      {"A", 5}, {"B", 3}, {"C", 8}, {"D", 2},  {"E", 6},
      {"F", 7}, {"G", 9}, {"H", 1}, {"I", 10}, {"J", 12},
  };
  for (const auto &element : list_1) {
    bool success = tree_1.insert(element.first, element.second);
    assert(success && "Insert should succeed for tree_1");
  }

  // Second tree
  CartesianTree<string, int> tree_2;
  vector<pair<string, int>> list_2 = {
      {"H", 1}, {"G", 9}, {"A", 5},  {"B", 3},  {"D", 2},
      {"F", 7}, {"C", 8}, {"J", 12}, {"I", 10}, {"E", 6},
  };
  for (const auto &element : list_2) {
    bool success = tree_2.insert(element.first, element.second);
    assert(success && "Insert should succeed for tree_2");
  }

  // Third tree
  CartesianTree<string, int> tree_3;
  vector<pair<string, int>> list_3 = {
      {"E", 6}, {"H", 1}, {"B", 3},  {"D", 2}, {"C", 8},
      {"F", 7}, {"G", 9}, {"J", 12}, {"A", 5}, {"I", 10},
  };
  for (const auto &element : list_3) {
    bool success = tree_3.insert(element.first, element.second);
    assert(success && "Insert should succeed for tree_3");
  }

  // Expected final state (should be the same for all trees)
  std::vector<std::pair<std::string, int>> expected = {
      {"H", 1}, {"D", 2}, {"B", 3}, {"A", 5},  {"C", 8},
      {"E", 6}, {"F", 7}, {"G", 9}, {"I", 10}, {"J", 12}};

  // Verify tree_1
  std::vector<std::pair<std::string, int>> result_1;
  for (const auto &node : tree_1) {
    if (node) {
      result_1.push_back({node->key, node->priority});
    }
  }

  // Verify tree_2
  std::vector<std::pair<std::string, int>> result_2;
  for (const auto &node : tree_2) {
    if (node) {
      result_2.push_back({node->key, node->priority});
    }
  }

  // Verify tree_3
  std::vector<std::pair<std::string, int>> result_3;
  for (const auto &node : tree_3) {
    if (node) {
      result_3.push_back({node->key, node->priority});
    }
  }

  // Assertions for all trees
  assert(result_1.size() == expected.size() &&
         "Tree 1 size should match expected");
  assert(result_2.size() == expected.size() &&
         "Tree 2 size should match expected");
  assert(result_3.size() == expected.size() &&
         "Tree 3 size should match expected");

  // Verify each tree matches expected traversal order
  for (size_t i = 0; i < expected.size(); i++) {
    // Tree 1 verification
    assert(result_1[i].first == expected[i].first &&
           result_1[i].second == expected[i].second && "Tree 1 node mismatch");
    std::cout << "Tree 1 Verified: " << result_1[i].first << result_1[i].second
              << std::endl;

    // Tree 2 verification
    assert(result_2[i].first == expected[i].first &&
           result_2[i].second == expected[i].second && "Tree 2 node mismatch");
    std::cout << "Tree 2 Verified: " << result_2[i].first << result_2[i].second
              << std::endl;

    // Tree 3 verification
    assert(result_3[i].first == expected[i].first &&
           result_3[i].second == expected[i].second && "Tree 3 node mismatch");
    std::cout << "Tree 3 Verified: " << result_3[i].first << result_3[i].second
              << std::endl;
  }

  // Print final sequences
  std::cout << "\nTree 1 final sequence:\n";
  for (const auto &pair : result_1) {
    std::cout << pair.first << pair.second << " ";
  }
  std::cout << std::endl;

  std::cout << "Tree 2 final sequence:\n";
  for (const auto &pair : result_2) {
    std::cout << pair.first << pair.second << " ";
  }
  std::cout << std::endl;

  std::cout << "Tree 3 final sequence:\n";
  for (const auto &pair : result_3) {
    std::cout << pair.first << pair.second << " ";
  }
  std::cout << std::endl;

  std::cout << "All trees verified successfully!" << std::endl;
}

void exercise_4() {
  cout << "\n\nExercise 4\n" << endl;

  // First tree
  CartesianTree<string, int> tree;
  vector<pair<string, int>> list_1 = {
      {"A", 5}, {"B", 3}, {"C", 8}, {"D", 2},  {"E", 6},
      {"F", 7}, {"G", 9}, {"H", 1}, {"I", 10}, {"J", 12},
  };
  for (const auto &element : list_1) {
    bool success = tree.insert(element.first, element.second);
    assert(success && "Insert should succeed for tree_1");
  }

  // Function to verify tree structure after each removal
  auto verify_tree = [](const CartesianTree<string, int> &tree,
                        const vector<pair<string, int>> &expected) {
    vector<pair<string, int>> result;
    for (const auto &node : tree) {
      if (node) {
        result.push_back({node->key, node->priority});
      }
    }

    assert(result.size() == expected.size() &&
           "Tree size should match expected");
    for (size_t i = 0; i < expected.size(); i++) {
      assert(result[i].first == expected[i].first &&
             result[i].second == expected[i].second &&
             "Node mismatch after removal");
    }

    cout << "Current tree state: ";
    for (const auto &pair : result) {
      cout << pair.first << pair.second << " ";
    }
    cout << endl;
  };

  // Test 1: Remove leaf nodes
  cout << "\nTest 1: Removing leaf nodes" << endl;
  assert(tree.remove("A") && "Should successfully remove leaf node A");
  vector<pair<string, int>> expected_1 = {{"H", 1}, {"D", 2},  {"B", 3},
                                          {"C", 8}, {"E", 6},  {"F", 7},
                                          {"G", 9}, {"I", 10}, {"J", 12}};
  verify_tree(tree, expected_1);

  // Test 2: Remove nodes with one child
  cout << "\nTest 2: Removing nodes with one child" << endl;
  assert(tree.remove("I") &&
         "Should successfully remove node I with one child");
  vector<pair<string, int>> expected_2 = {{"H", 1}, {"D", 2}, {"B", 3},
                                          {"C", 8}, {"E", 6}, {"F", 7},
                                          {"G", 9}, {"J", 12}};
  verify_tree(tree, expected_2);

  // Test 3: Remove nodes with two children
  cout << "\nTest 3: Removing nodes with two children" << endl;
  assert(tree.remove("E") &&
         "Should successfully remove node E with two children");
  vector<pair<string, int>> expected_3 = {
      {"H", 1}, {"D", 2}, {"B", 3}, {"C", 8}, {"F", 7}, {"G", 9}, {"J", 12}};
  verify_tree(tree, expected_3);

  // Test 4: Remove root node
  cout << "\nTest 4: Removing root node" << endl;
  assert(tree.remove("H") && "Should successfully remove root node H");
  vector<pair<string, int>> expected_4 = {{"D", 2}, {"B", 3}, {"C", 8},
                                          {"F", 7}, {"G", 9}, {"J", 12}};
  verify_tree(tree, expected_4);

  // Test 5: Try to remove non-existent node
  cout << "\nTest 5: Attempting to remove non-existent node" << endl;
  assert(!tree.remove("X") && "Should fail to remove non-existent node");
  verify_tree(tree, expected_4); // Tree should remain unchanged

  // Test 6: Remove remaining nodes in random order
  cout << "\nTest 6: Removing remaining nodes in random order" << endl;
  assert(tree.remove("C") && "Should remove C");
  assert(tree.remove("G") && "Should remove G");
  assert(tree.remove("B") && "Should remove B");
  assert(tree.remove("F") && "Should remove F");
  assert(tree.remove("J") && "Should remove J");
  assert(tree.remove("D") && "Should remove D");

  // Verify empty tree
  vector<pair<string, int>> expected_empty;
  verify_tree(tree, expected_empty);

  // Test 7: Try to remove from empty tree
  cout << "\nTest 7: Attempting to remove from empty tree" << endl;
  assert(!tree.remove("A") && "Should fail to remove from empty tree");

  cout << "All remove operation tests passed!" << endl;
}

void test_bst() {
  cout << "\nTesting Binary Search Tree\n" << endl;
  BinarySearchTree<int> bst;

  // Test 1: Insertion and Find
  cout << "Test 1: Insertion and Find" << endl;
  bst.insert(5);
  bst.insert(3);
  bst.insert(7);
  bst.insert(1);
  bst.insert(9);

  assert(bst.find(5) && "Should find 5");
  assert(bst.find(3) && "Should find 3");
  assert(bst.find(7) && "Should find 7");
  assert(bst.find(1) && "Should find 1");
  assert(bst.find(9) && "Should find 9");
  assert(!bst.find(4) && "Should not find 4");
  cout << "Insertion and find tests passed!" << endl;

  // Test 2: Duplicate Insertion
  cout << "\nTest 2: Duplicate Insertion" << endl;
  bst.insert(5); // Duplicate insertion
  assert(bst.find(5) && "Should still find 5");
  cout << "Duplicate insertion test passed!" << endl;

  // Test 3: Removal
  cout << "\nTest 3: Removal" << endl;
  // Test removing leaf node
  bst.remove(1);
  assert(!bst.find(1) && "Should not find 1 after removal");

  // Test removing node with one child
  bst.insert(8);
  bst.remove(9);
  assert(!bst.find(9) && "Should not find 9 after removal");
  assert(bst.find(8) && "Should still find 8");

  // Test removing node with two children
  bst.remove(7);
  assert(!bst.find(7) && "Should not find 7 after removal");
  assert(bst.find(8) && "Should still find 8");

  cout << "Removal tests passed!" << endl;

  // Test 4: Removing root
  cout << "\nTest 4: Removing Root" << endl;
  bst.remove(5);
  assert(!bst.find(5) && "Should not find 5 after removal");
  assert(bst.find(3) && "Should still find 3");
  assert(bst.find(8) && "Should still find 8");
  cout << "Root removal test passed!" << endl;
}

void test_binary_heap() {
  cout << "\nTesting Binary Heap\n" << endl;
  BinaryHeap<int> heap;

  // Test 1: Push and Top
  cout << "Test 1: Push and Top" << endl;
  heap.push(5);
  assert(heap.top() == 5 && "Top should be 5");
  heap.push(3);
  assert(heap.top() == 3 && "Top should be 3");
  heap.push(7);
  assert(heap.top() == 3 && "Top should still be 3");
  heap.push(1);
  assert(heap.top() == 1 && "Top should be 1");
  cout << "Push and top tests passed!" << endl;

  // Test 2: Size and Empty
  cout << "\nTest 2: Size and Empty" << endl;
  assert(heap.size() == 4 && "Size should be 4");
  assert(!heap.empty() && "Heap should not be empty");
  cout << "Size and empty tests passed!" << endl;

  // Test 3: Pop
  cout << "\nTest 3: Pop" << endl;
  assert(heap.pop() == 1 && "Should pop 1");
  assert(heap.top() == 3 && "Top should now be 3");
  assert(heap.pop() == 3 && "Should pop 3");
  assert(heap.top() == 5 && "Top should now be 5");
  assert(heap.size() == 2 && "Size should be 2");
  cout << "Pop tests passed!" << endl;

  // Test 4: Order Verification
  cout << "\nTest 4: Order Verification" << endl;
  BinaryHeap<int> ordered_heap;
  vector<int> values = {9, 5, 2, 7, 1, 8, 3, 6, 4};
  for (int val : values) {
    ordered_heap.push(val);
  }

  vector<int> popped;
  while (!ordered_heap.empty()) {
    popped.push_back(ordered_heap.pop());
  }

  for (size_t i = 1; i < popped.size(); i++) {
    assert(popped[i - 1] <= popped[i] &&
           "Elements should be in ascending order");
  }
  cout << "Order verification test passed!" << endl;

  // Test 5: Empty Heap Operations
  cout << "\nTest 5: Empty Heap Operations" << endl;
  BinaryHeap<int> empty_heap;
  assert(empty_heap.empty() && "New heap should be empty");
  assert(empty_heap.size() == 0 && "New heap should have size 0");
  cout << "Empty heap tests passed!" << endl;
}

int main() {
  exercise_1();
  exercise_2();
  exercise_3();
  exercise_4();

  test_bst();
  test_binary_heap();

  cout << "\n\nRunning benchmarks\n" << endl;

  // run_all_benchmarks();
}
