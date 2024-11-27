#include "performance.h"
#include "BinaryHeap.hpp"
#include "BinarySearchTree.hpp"
#include "CartesianTree.hpp"
#include "instant.h"
#include <algorithm>
#include <fstream>
#include <functional>
#include <random>

namespace complex {

std::vector<std::pair<std::string, int>>
generate_cartesian_data(size_t n, double collision_rate) {
  std::cout << "Generating Cartesian data with n=" << n
            << ", collision_rate=" << collision_rate << std::endl;

  std::vector<std::pair<std::string, int>> data;
  data.reserve(n);

  std::random_device rd;
  std::mt19937 gen(rd());

  size_t distinct_priorities;
  if (collision_rate == 0.0) {
    distinct_priorities = n;
  } else if (collision_rate == 0.01) {
    distinct_priorities = n / 100;
  } else if (collision_rate == 0.1) {
    distinct_priorities = n / 10;
  } else {
    throw std::invalid_argument("Invalid collision rate");
  }

  std::cout << "Using " << distinct_priorities << " distinct priorities"
            << std::endl;

  std::uniform_int_distribution<int> dist(
      1, static_cast<int>(distinct_priorities));

  for (size_t i = 0; i < n; i++) {
    std::string key = "key_" + std::to_string(i);
    int priority = dist(gen);
    data.emplace_back(key, priority);
  }

  std::shuffle(data.begin(), data.end(), gen);
  std::cout << "Generated " << data.size() << " data points" << std::endl;
  return data;
}

std::vector<std::string> generate_nonexistent_keys(size_t n) {
  std::cout << "Generating nonexistent keys relative to n=" << n << std::endl;

  std::vector<std::string> keys;
  keys.reserve(100);

  for (size_t i = 0; i < 100; i++) {
    keys.push_back("nonexistent_" + std::to_string(n + i));
  }

  std::cout << "Generated " << keys.size() << " nonexistent keys" << std::endl;
  return keys;
}

std::vector<std::string> generate_bst_data(size_t n,
                                           const std::string &pattern) {
  std::cout << "Generating BST data with n=" << n << ", pattern=" << pattern
            << std::endl;

  std::vector<std::string> data;
  data.reserve(n);

  for (size_t i = 0; i < n; i++) {
    data.push_back("key_" + std::to_string(i));
  }

  if (pattern == "random") {
    std::cout << "Shuffling data randomly" << std::endl;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(data.begin(), data.end(), gen);
  } else if (pattern == "descending") {
    std::cout << "Reversing data for descending pattern" << std::endl;
    std::reverse(data.begin(), data.end());
  }

  std::cout << "Generated " << data.size() << " BST data points" << std::endl;
  return data;
}

std::vector<int> generate_heap_data(size_t n) {
  std::cout << "Generating heap data with n=" << n << std::endl;

  std::vector<int> data;
  data.reserve(n);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(1, static_cast<int>(n * 2));

  for (size_t i = 0; i < n; i++) {
    data.push_back(dist(gen));
  }

  std::cout << "Generated " << data.size() << " heap data points" << std::endl;
  return data;
}

template <typename K, typename P>
CartesianMetrics compute_cartesian_metrics(const CartesianTree<K, P> &tree) {
  CartesianMetrics metrics{0, 0, 0.0};
  if (!tree.root_) {
    return metrics;
  }

  size_t total_depth = 0;
  size_t node_count = 0;

  // Calculate height using the same approach as BST metrics
  std::function<size_t(typename CartesianTree<K, P>::Node *)> calculate_height =
      [&](typename CartesianTree<K, P>::Node *node) -> size_t {
    if (!node)
      return 0;
    return 1 + std::max(calculate_height(node->left),
                        calculate_height(node->right));
  };

  // Calculate depths using the same approach as BST metrics
  std::function<void(typename CartesianTree<K, P>::Node *, size_t)>
      calculate_depths =
          [&](typename CartesianTree<K, P>::Node *node, size_t depth) {
            if (!node)
              return;
            total_depth += depth;
            node_count++;
            calculate_depths(node->left, depth + 1);
            calculate_depths(node->right, depth + 1);
          };

  // Calculate balance using the same approach as BST metrics
  std::function<double(typename CartesianTree<K, P>::Node *)>
      calculate_balance =
          [&](typename CartesianTree<K, P>::Node *node) -> double {
    if (!node)
      return 0.0;
    double left_height = static_cast<double>(calculate_height(node->left));
    double right_height = static_cast<double>(calculate_height(node->right));
    return left_height / (left_height + right_height);
  };

  // Calculate metrics using the same order and approach as BST metrics
  metrics.height = calculate_height(tree.root_) - 1;
  calculate_depths(tree.root_, 0);
  metrics.avg_depth = total_depth / node_count;
  metrics.balance_factor = calculate_balance(tree.root_);

  return metrics;
}

template <typename K>
BSTMetrics compute_bst_metrics(const BinarySearchTree<K> &tree) {
  BSTMetrics metrics{0, 0, 0.0};
  if (!tree.root_) {
    return metrics;
  }

  size_t total_depth = 0;
  size_t node_count = 0;

  std::function<size_t(typename BinarySearchTree<K>::Node *)> calculate_height =
      [&](typename BinarySearchTree<K>::Node *node) -> size_t {
    if (!node)
      return 0;
    return 1 + std::max(calculate_height(node->left),
                        calculate_height(node->right));
  };

  std::function<void(typename BinarySearchTree<K>::Node *, size_t)>
      calculate_depths =
          [&](typename BinarySearchTree<K>::Node *node, size_t depth) {
            if (!node)
              return;

            total_depth += depth;
            node_count++;

            calculate_depths(node->left, depth + 1);
            calculate_depths(node->right, depth + 1);
          };

  std::function<double(typename BinarySearchTree<K>::Node *)>
      calculate_balance =
          [&](typename BinarySearchTree<K>::Node *node) -> double {
    if (!node)
      return 0.0;

    double left_height = static_cast<double>(calculate_height(node->left));
    double right_height = static_cast<double>(calculate_height(node->right));

    return left_height / (left_height + right_height);
  };

  metrics.height = calculate_height(tree.root_) - 1;
  calculate_depths(tree.root_, 0);
  metrics.avg_depth = total_depth / node_count;
  metrics.balance_factor = calculate_balance(tree.root_);

  return metrics;
}

void benchmark_cartesian_tree(const std::string &operation_file,
                              const std::string &structure_file) {
  std::cout << "\n=== Starting Cartesian Tree Benchmarks ===" << std::endl;
  std::cout << "Writing to files: " << operation_file << " and "
            << structure_file << std::endl;
  std::ofstream op_out(operation_file);
  std::ofstream struct_out(structure_file);
  op_out << "n,collision_rate,operation,time_us\n";
  struct_out << "n,collision_rate,height,avg_depth,balance_factor\n";
  std::vector<size_t> sizes = {1000, 5000, 10000, 50000, 100000};
  std::vector<double> collision_rates = {0.0, 0.01, 0.1};
  for (size_t n : sizes) {
    std::cout << "\nTesting size n=" << n << std::endl;
    for (double rate : collision_rates) {
      std::cout << "Testing collision rate=" << rate << std::endl;
      auto data = generate_cartesian_data(n, rate);
      auto nonexistent_keys = generate_nonexistent_keys(n);

      std::cout << "Building Cartesian tree..." << std::endl;
      auto start = Instant::now();
      CartesianTree<std::string, int> tree;
      for (const auto &[key, priority] : data) {
        tree.insert(key, priority);
      }
      long insert_time = start.elapsed() / static_cast<long>(n);
      std::cout << "Average insert time: " << insert_time << "us" << std::endl;

      auto metrics = compute_cartesian_metrics(tree);

      // Measure search time for existing keys
      start = Instant::now();
      for (const auto &[key, _] : data) {
        tree.find(key);
      }
      long search_time = start.elapsed() / static_cast<long>(n);
      std::cout << "Average search time: " << search_time << "us" << std::endl;

      // Measure search time for non-existent keys
      start = Instant::now();
      for (const auto &key : nonexistent_keys) {
        tree.find(key);
      }
      long failed_search_time = start.elapsed() / static_cast<long>(n);
      std::cout << "Average failed search time: " << failed_search_time << "us"
                << std::endl;

      // Measure remove time
      start = Instant::now();
      for (const auto &[key, _] : data) {
        tree.remove(key);
      }
      long remove_time = start.elapsed() / static_cast<long>(n);
      std::cout << "Average remove time: " << remove_time << "us" << std::endl;

      op_out << n << "," << rate << ",insert," << insert_time << "\n"
             << n << "," << rate << ",search," << search_time << "\n"
             << n << "," << rate << ",failed_search," << failed_search_time
             << "\n"
             << n << "," << rate << ",remove," << remove_time << "\n";

      struct_out << n << "," << rate << "," << metrics.height << ","
                 << metrics.avg_depth << "," << metrics.balance_factor << "\n";
    }
  }
  std::cout << "Cartesian tree benchmarks completed" << std::endl;
}

void benchmark_bst(const std::string &operation_file,
                   const std::string &structure_file) {
  std::cout << "\n=== Starting BST Benchmarks ===" << std::endl;
  std::cout << "Writing to files: " << operation_file << " and "
            << structure_file << std::endl;

  std::ofstream op_out(operation_file);
  std::ofstream struct_out(structure_file);

  op_out << "n,pattern,operation,time_us\n";
  struct_out << "n,pattern,height,avg_depth,balance_factor\n";

  std::vector<size_t> sizes = {1000, 5000, 10000, 50000, 100000};
  std::vector<std::string> patterns = {"random", "ascending", "descending"};

  for (size_t n : sizes) {
    std::cout << "\nTesting size n=" << n << std::endl;

    for (const auto &pattern : patterns) {
      std::cout << "Testing pattern=" << pattern << std::endl;

      auto data = generate_bst_data(n, pattern);
      auto nonexistent_keys = generate_nonexistent_keys(n);

      std::cout << "Building BST..." << std::endl;

      auto start = Instant::now();
      BinarySearchTree<std::string> tree;
      for (const auto &key : data) {
        tree.insert(key);
      }
      long insert_time = start.elapsed() / static_cast<long>(n);
      std::cout << "Average insert time: " << insert_time << "us" << std::endl;

      auto metrics = compute_bst_metrics(tree);

      start = Instant::now();
      for (const auto &key : data) {
        tree.find(key);
      }
      long search_time = start.elapsed() / static_cast<long>(n);
      std::cout << "Average search time: " << search_time << "us" << std::endl;

      start = Instant::now();
      for (const auto &key : nonexistent_keys) {
        tree.find(key);
      }
      long failed_search_time = start.elapsed() / static_cast<long>(n);
      std::cout << "Average failed search time: " << failed_search_time << "us"
                << std::endl;

      start = Instant::now();
      for (const auto &key : data) {
        tree.remove(key);
      }
      long remove_time = start.elapsed() / static_cast<long>(n);
      std::cout << "Average remove time: " << remove_time << "us" << std::endl;

      op_out << n << "," << pattern << ",insert," << insert_time << "\n"
             << n << "," << pattern << ",search," << search_time << "\n"
             << n << "," << pattern << ",failed_search," << failed_search_time
             << "\n"
             << n << "," << pattern << ",remove," << remove_time << "\n";

      struct_out << n << "," << pattern << "," << metrics.height << ","
                 << metrics.avg_depth << "," << metrics.balance_factor << "\n";
    }
  }

  std::cout << "BST benchmarks completed" << std::endl;
}

void benchmark_heap(const std::string &operation_file) {
  std::cout << "\n=== Starting Heap Benchmarks ===" << std::endl;
  std::cout << "Writing to file: " << operation_file << std::endl;

  std::ofstream op_out(operation_file);
  op_out << "n,operation,time_us\n";

  std::vector<size_t> sizes = {1000, 5000, 10000, 50000, 100000};

  for (size_t n : sizes) {
    std::cout << "\nTesting size n=" << n << std::endl;

    auto data = generate_heap_data(n);

    std::cout << "Building heap..." << std::endl;
    auto start = Instant::now();
    BinaryHeap<int> heap;
    for (int value : data) {
      heap.push(value);
    }
    long push_time = start.elapsed() / static_cast<long>(n);
    std::cout << "Average push time: " << push_time << "us" << std::endl;

    std::cout << "Performing pop operations..." << std::endl;
    start = Instant::now();
    while (!heap.empty()) {
      heap.pop();
    }
    long pop_time = start.elapsed() / static_cast<long>(n);
    std::cout << "Average pop time: " << pop_time << "us" << std::endl;

    op_out << n << ",push," << push_time << "\n"
           << n << ",pop," << pop_time << "\n";
  }

  std::cout << "Heap benchmarks completed" << std::endl;
}

void run_all_benchmarks() {
  std::cout << "\n====== Starting All Benchmarks ======" << std::endl;

  std::cout << "\nRunning Cartesian Tree benchmarks..." << std::endl;
  benchmark_cartesian_tree("paper/data/cartesian_operations.csv",
                           "paper/data/cartesian_structure.csv");

  std::cout << "\nRunning BST benchmarks..." << std::endl;
  benchmark_bst("paper/data/bst_operations.csv",
                "paper/data/bst_structure.csv");

  std::cout << "\nRunning Heap benchmarks..." << std::endl;
  benchmark_heap("paper/data/heap_operations.csv");

  std::cout << "\n====== All Benchmarks Completed ======" << std::endl;
}

} // namespace complex
