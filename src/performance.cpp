#include "performance.h"
#include "BinaryHeap.hpp"
#include "BinarySearchTree.hpp"
#include "CartesianTree.hpp"
#include "instant.h"
#include <algorithm>
#include <fstream>
#include <functional>
#include <map>
#include <random>

namespace complex {

std::vector<std::pair<std::string, int>> generate_test_data(
    size_t n, KeyDistribution key_dist,
    PriorityDistribution pri_dist = PriorityDistribution::UNIQUE) {
  std::vector<std::pair<std::string, int>> data;
  data.reserve(n);

  std::random_device rd;
  std::mt19937 gen(rd());

  // Generate priorities based on collision rate
  std::vector<int> priorities;
  priorities.reserve(n);

  switch (pri_dist) {
  case PriorityDistribution::UNIQUE: {
    // Generate n unique priorities
    for (unsigned int i = 0; i < n; i++) {
      priorities.push_back(static_cast<int>(i + 1));
    }
    std::shuffle(priorities.begin(), priorities.end(), gen);
    break;
  }
  case PriorityDistribution::LOW_COLLISION: {
    // Use only 1000 distinct priorities for 0.1% collision rate
    std::uniform_int_distribution<> dist(1, 1000);
    for (size_t i = 0; i < n; i++) {
      priorities.push_back(dist(gen));
    }
    break;
  }
  case PriorityDistribution::MED_COLLISION: {
    // Use only 100 distinct priorities for 1% collision rate
    std::uniform_int_distribution<> dist(1, 100);
    for (size_t i = 0; i < n; i++) {
      priorities.push_back(dist(gen));
    }
    break;
  }
  case PriorityDistribution::HIGH_COLLISION: {
    // Use only 10 distinct priorities for 10% collision rate
    std::uniform_int_distribution<> dist(1, 10);
    for (size_t i = 0; i < n; i++) {
      priorities.push_back(dist(gen));
    }
    break;
  }
  }

  // Generate keys based on distribution type and pair with priorities
  size_t priority_idx = 0;
  switch (key_dist) {
  case KeyDistribution::SORTED_ASC:
    for (size_t i = 0; i < n; i++) {
      std::string key = "key_" + std::to_string(i);
      data.emplace_back(key, priorities[priority_idx++]);
    }
    break;

  case KeyDistribution::SORTED_DESC:
    for (size_t i = 0; i < n; i++) {
      std::string key = "key_" + std::to_string(n - i - 1);
      data.emplace_back(key, priorities[priority_idx++]);
    }
    break;

  case KeyDistribution::ALTERNATING:
    for (size_t i = 0; i < n; i++) {
      size_t val = (i % 2 == 0) ? i / 2 : n - 1 - i / 2;
      std::string key = "key_" + std::to_string(val);
      data.emplace_back(key, priorities[priority_idx++]);
    }
    break;

  case KeyDistribution::RANDOM:
  default:
    for (size_t i = 0; i < n; i++) {
      std::string key = "key_" + std::to_string(i);
      data.emplace_back(key, priorities[priority_idx++]);
    }
    std::shuffle(data.begin(), data.end(), gen);
    break;
  }

  return data;
}

TimingStats calculate_stats(const std::vector<long> &measurements) {
  TimingStats stats;

  // Calculate mean
  double sum = std::accumulate(measurements.begin(), measurements.end(), 0.0);
  stats.mean = sum / static_cast<double>(measurements.size());

  // Calculate standard deviation
  double sq_sum = std::inner_product(measurements.begin(), measurements.end(),
                                     measurements.begin(), 0.0);
  stats.stddev = std::sqrt(sq_sum / static_cast<double>(measurements.size()) -
                           stats.mean * stats.mean);

  // Min and max
  auto [min_it, max_it] =
      std::minmax_element(measurements.begin(), measurements.end());
  stats.min = static_cast<double>(*min_it);
  stats.max = static_cast<double>(*max_it);

  return stats;
}

template <typename Tree>
void compute_tree_metrics(const Tree &tree, BenchmarkResult &result) {
  std::vector<size_t> depths;
  size_t max_depth = 0;
  size_t min_depth = SIZE_MAX;

  // Function to recursively compute depths
  std::function<void(const typename Tree::Node *, size_t)> compute_depths =
      [&](const typename Tree::Node *node, size_t depth) {
        if (!node)
          return;

        // Update metrics for leaf nodes
        if (!node->left && !node->right) {
          depths.push_back(depth);
          max_depth = std::max(max_depth, depth);
          min_depth = std::min(min_depth, depth);
        }

        compute_depths(node->left, depth + 1);
        compute_depths(node->right, depth + 1);
      };

  compute_depths(tree.root_, 0);

  // Compute metrics
  result.max_depth = max_depth;
  result.avg_depth = std::accumulate(depths.begin(), depths.end(), 0.0) /
                     static_cast<double>(depths.size());
  result.balance_factor = min_depth > 0 ? static_cast<double>(min_depth) /
                                              static_cast<double>(max_depth)
                                        : 0.0;

  // Compute depth distribution
  result.depth_distribution.resize(max_depth + 1, 0);
  for (size_t depth : depths) {
    result.depth_distribution[depth]++;
  }
}

BenchmarkResult
benchmark_cartesian_tree(const std::vector<std::pair<std::string, int>> &data,
                         size_t num_runs) {
  BenchmarkResult result;
  result.n = data.size();
  result.structure_name = "CartesianTree";

  std::vector<long> insert_overall, insert_first, insert_last;
  std::vector<long> search_overall, search_nonexistent;
  std::vector<long> remove_times, traversal_times;

  // Generate some non-existent keys for search testing
  std::vector<std::string> nonexistent_keys;
  for (size_t i = 0; i < 100; i++) {
    nonexistent_keys.push_back("nonexistent_" + std::to_string(i));
  }

  for (size_t run = 0; run < num_runs; run++) {
    CartesianTree<std::string, int> tree;

    // First 100 insertions
    auto start = Instant::now();
    for (size_t i = 0; i < 100 && i < data.size(); i++) {
      tree.insert(data[i].first, data[i].second);
    }
    insert_first.push_back(start.elapsed());

    // Middle insertions (not timed)
    for (size_t i = 100; i < data.size() - 100; i++) {
      tree.insert(data[i].first, data[i].second);
    }

    // Last 100 insertions
    start = Instant::now();
    for (size_t i = data.size() - 100; i < data.size(); i++) {
      tree.insert(data[i].first, data[i].second);
    }
    insert_last.push_back(start.elapsed());

    // Record overall insertion time
    start = Instant::now();
    for (const auto &[key, priority] : data) {
      tree.insert(key, priority);
    }
    insert_overall.push_back(start.elapsed());

    // Normal search
    start = Instant::now();
    for (const auto &[key, _] : data) {
      tree.find(key);
    }
    search_overall.push_back(start.elapsed());

    // Non-existent key search
    start = Instant::now();
    for (const auto &key : nonexistent_keys) {
      tree.find(key);
    }
    search_nonexistent.push_back(start.elapsed());

    // Normal operations
    start = Instant::now();
    for (auto it = tree.begin(); it != tree.end(); ++it) {
      (void)*it;
    }
    traversal_times.push_back(start.elapsed());

    start = Instant::now();
    for (const auto &[key, _] : data) {
      tree.remove(key);
    }
    remove_times.push_back(start.elapsed());

    if (run == num_runs - 1) {
      compute_tree_metrics(tree, result);
    }
  }

  // Calculate all statistics
  result.insert_time.overall = calculate_stats(insert_overall);
  result.insert_time.first_100 = calculate_stats(insert_first);
  result.insert_time.last_100 = calculate_stats(insert_last);

  result.search_time.overall = calculate_stats(search_overall);
  result.search_time.nonexistent = calculate_stats(search_nonexistent);

  result.remove_time = calculate_stats(remove_times);
  result.traversal_time = calculate_stats(traversal_times);

  return result;
}

BenchmarkResult
benchmark_map(const std::vector<std::pair<std::string, int>> &data,
              size_t num_runs) {
  BenchmarkResult result;
  result.n = data.size();
  result.structure_name = "std::map";

  std::vector<long> insert_overall, insert_first, insert_last;
  std::vector<long> search_overall, search_nonexistent;
  std::vector<long> remove_times, traversal_times;

  // Generate non-existent keys
  std::vector<std::string> nonexistent_keys;
  for (size_t i = 0; i < 100; i++) {
    nonexistent_keys.push_back("nonexistent_" + std::to_string(i));
  }

  for (size_t run = 0; run < num_runs; run++) {
    std::map<std::string, int> map;

    // First 100 insertions
    auto start = Instant::now();
    for (size_t i = 0; i < 100 && i < data.size(); i++) {
      map.insert({data[i].first, data[i].second});
    }
    insert_first.push_back(start.elapsed());

    // Middle insertions (not timed separately)
    for (size_t i = 100; i < data.size() - 100; i++) {
      map.insert({data[i].first, data[i].second});
    }

    // Last 100 insertions
    start = Instant::now();
    for (size_t i = data.size() - 100; i < data.size(); i++) {
      map.insert({data[i].first, data[i].second});
    }
    insert_last.push_back(start.elapsed());

    // Overall insertion timing
    start = Instant::now();
    for (const auto &[key, priority] : data) {
      map.insert({key, priority});
    }
    insert_overall.push_back(start.elapsed());

    // Normal search
    start = Instant::now();
    for (const auto &[key, _] : data) {
      auto tmp = map.find(key);
      (void)tmp;
    }
    search_overall.push_back(start.elapsed());

    // Non-existent search
    start = Instant::now();
    for (const auto &key : nonexistent_keys) {
      auto tmp = map.find(key);
      (void)tmp;
    }
    search_nonexistent.push_back(start.elapsed());

    // Traversal
    start = Instant::now();
    for (auto it = map.begin(); it != map.end(); ++it) {
      (void)*it;
    }
    traversal_times.push_back(start.elapsed());

    // Removal
    start = Instant::now();
    for (const auto &[key, _] : data) {
      map.erase(key);
    }
    remove_times.push_back(start.elapsed());

    if (run == num_runs - 1) {
      result.max_depth = 0;
      result.avg_depth = 0;
      result.balance_factor = 0;
    }
  }

  // Calculate all statistics
  result.insert_time.overall = calculate_stats(insert_overall);
  result.insert_time.first_100 = calculate_stats(insert_first);
  result.insert_time.last_100 = calculate_stats(insert_last);

  result.search_time.overall = calculate_stats(search_overall);
  result.search_time.nonexistent = calculate_stats(search_nonexistent);

  result.remove_time = calculate_stats(remove_times);
  result.traversal_time = calculate_stats(traversal_times);

  return result;
}

BenchmarkResult
benchmark_unordered_map(const std::vector<std::pair<std::string, int>> &data,
                        size_t num_runs) {
  BenchmarkResult result;
  result.n = data.size();
  result.structure_name = "std::unordered_map";

  std::vector<long> insert_overall, insert_first, insert_last;
  std::vector<long> search_overall, search_nonexistent;
  std::vector<long> remove_times, traversal_times;

  // Generate non-existent keys
  std::vector<std::string> nonexistent_keys;
  for (size_t i = 0; i < 100; i++) {
    nonexistent_keys.push_back("nonexistent_" + std::to_string(i));
  }

  for (size_t run = 0; run < num_runs; run++) {
    std::unordered_map<std::string, int> umap;

    // First 100 insertions
    auto start = Instant::now();
    for (size_t i = 0; i < 100 && i < data.size(); i++) {
      umap.insert({data[i].first, data[i].second});
    }
    insert_first.push_back(start.elapsed());

    // Middle insertions (not timed separately)
    for (size_t i = 100; i < data.size() - 100; i++) {
      umap.insert({data[i].first, data[i].second});
    }

    // Last 100 insertions
    start = Instant::now();
    for (size_t i = data.size() - 100; i < data.size(); i++) {
      umap.insert({data[i].first, data[i].second});
    }
    insert_last.push_back(start.elapsed());

    // Overall insertion timing
    start = Instant::now();
    for (const auto &[key, priority] : data) {
      umap.insert({key, priority});
    }
    insert_overall.push_back(start.elapsed());

    // Normal search
    start = Instant::now();
    for (const auto &[key, _] : data) {
      auto tmp = umap.find(key);
      (void)tmp;
    }
    search_overall.push_back(start.elapsed());

    // Non-existent search
    start = Instant::now();
    for (const auto &key : nonexistent_keys) {
      auto tmp = umap.find(key);
      (void)tmp;
    }
    search_nonexistent.push_back(start.elapsed());

    // Traversal
    start = Instant::now();
    for (auto it = umap.begin(); it != umap.end(); ++it) {
      (void)*it;
    }
    traversal_times.push_back(start.elapsed());

    // Removal
    start = Instant::now();
    for (const auto &[key, _] : data) {
      umap.erase(key);
    }
    remove_times.push_back(start.elapsed());

    if (run == num_runs - 1) {
      result.max_depth = 0;
      result.avg_depth = 0;
      result.balance_factor = 0;
    }
  }

  // Calculate all statistics
  result.insert_time.overall = calculate_stats(insert_overall);
  result.insert_time.first_100 = calculate_stats(insert_first);
  result.insert_time.last_100 = calculate_stats(insert_last);

  result.search_time.overall = calculate_stats(search_overall);
  result.search_time.nonexistent = calculate_stats(search_nonexistent);

  result.remove_time = calculate_stats(remove_times);
  result.traversal_time = calculate_stats(traversal_times);

  return result;
}

BenchmarkResult
benchmark_bst(const std::vector<std::pair<std::string, int>> &data,
              size_t num_runs) {
  BenchmarkResult result;
  result.n = data.size();
  result.structure_name = "BasicBST";

  std::vector<long> insert_overall, insert_first, insert_last;
  std::vector<long> search_overall, search_nonexistent;
  std::vector<long> remove_times, traversal_times;

  // Generate non-existent keys
  std::vector<std::string> nonexistent_keys;
  for (size_t i = 0; i < 100; i++) {
    nonexistent_keys.push_back("nonexistent_" + std::to_string(i));
  }

  for (size_t run = 0; run < num_runs; run++) {
    BinarySearchTree<std::string, int> bst;

    // First 100 insertions
    auto start = Instant::now();
    for (size_t i = 0; i < 100 && i < data.size(); i++) {
      bst.insert(data[i].first, data[i].second);
    }
    insert_first.push_back(start.elapsed());

    // Middle insertions (not timed separately)
    for (size_t i = 100; i < data.size() - 100; i++) {
      bst.insert(data[i].first, data[i].second);
    }

    // Last 100 insertions
    start = Instant::now();
    for (size_t i = data.size() - 100; i < data.size(); i++) {
      bst.insert(data[i].first, data[i].second);
    }
    insert_last.push_back(start.elapsed());

    // Overall insertion timing
    start = Instant::now();
    for (const auto &[key, priority] : data) {
      bst.insert(key, priority);
    }
    insert_overall.push_back(start.elapsed());

    // Normal search
    start = Instant::now();
    for (const auto &[key, _] : data) {
      bst.find(key);
    }
    search_overall.push_back(start.elapsed());

    // Non-existent search
    start = Instant::now();
    for (const auto &key : nonexistent_keys) {
      bst.find(key);
    }
    search_nonexistent.push_back(start.elapsed());

    // Skip traversal as we haven't implemented iterators
    traversal_times.push_back(0);

    // Removal
    start = Instant::now();
    for (const auto &[key, _] : data) {
      bst.remove(key);
    }
    remove_times.push_back(start.elapsed());

    if (run == num_runs - 1) {
      compute_tree_metrics(bst, result);
    }
  }

  // Calculate all statistics
  result.insert_time.overall = calculate_stats(insert_overall);
  result.insert_time.first_100 = calculate_stats(insert_first);
  result.insert_time.last_100 = calculate_stats(insert_last);

  result.search_time.overall = calculate_stats(search_overall);
  result.search_time.nonexistent = calculate_stats(search_nonexistent);

  result.remove_time = calculate_stats(remove_times);
  result.traversal_time = calculate_stats(traversal_times);

  return result;
}

BenchmarkResult
benchmark_heap(const std::vector<std::pair<std::string, int>> &data,
               size_t num_runs) {
  BenchmarkResult result;
  result.n = data.size();
  result.structure_name = "BinaryHeap";

  // Vectors for different timing measurements
  std::vector<long> insert_overall, insert_first, insert_last;
  std::vector<long> search_overall, search_nonexistent;
  std::vector<long> remove_times;
  std::vector<long> traversal_times;

  // Generate non-existent keys
  std::vector<std::string> nonexistent_keys;
  for (size_t i = 0; i < 100; i++) {
    nonexistent_keys.push_back("nonexistent_" + std::to_string(i));
  }

  for (size_t run = 0; run < num_runs; run++) {
    BinaryHeap<std::string, int> heap;

    // First 100 insertions
    auto start = Instant::now();
    for (size_t i = 0; i < 100 && i < data.size(); i++) {
      heap.insert(data[i].first, data[i].second);
    }
    insert_first.push_back(start.elapsed());

    // Middle insertions (not timed separately)
    for (size_t i = 100; i < data.size() - 100; i++) {
      heap.insert(data[i].first, data[i].second);
    }

    // Last 100 insertions
    start = Instant::now();
    for (size_t i = data.size() - 100; i < data.size(); i++) {
      heap.insert(data[i].first, data[i].second);
    }
    insert_last.push_back(start.elapsed());

    // Overall insertion timing
    start = Instant::now();
    for (const auto &[key, priority] : data) {
      heap.insert(key, priority);
    }
    insert_overall.push_back(start.elapsed());

    // Normal search
    start = Instant::now();
    for (const auto &[key, _] : data) {
      heap.find(key);
    }
    search_overall.push_back(start.elapsed());

    // Non-existent search
    start = Instant::now();
    for (const auto &key : nonexistent_keys) {
      heap.find(key);
    }
    search_nonexistent.push_back(start.elapsed());

    // No traversal for heap
    traversal_times.push_back(0);

    // Removal
    start = Instant::now();
    for (const auto &[key, _] : data) {
      heap.remove(key);
    }
    remove_times.push_back(start.elapsed());

    // Heap metrics (only on last run)
    if (run == num_runs - 1) {
      result.max_depth =
          static_cast<size_t>(std::floor(std::log2(data.size()))) + 1;
      result.avg_depth = static_cast<double>(result.max_depth - 1);
      result.balance_factor = 1.0;
    }
  }

  // Calculate all statistics
  result.insert_time.overall = calculate_stats(insert_overall);
  result.insert_time.first_100 = calculate_stats(insert_first);
  result.insert_time.last_100 = calculate_stats(insert_last);

  result.search_time.overall = calculate_stats(search_overall);
  result.search_time.nonexistent = calculate_stats(search_nonexistent);

  result.remove_time = calculate_stats(remove_times);
  result.traversal_time = calculate_stats(traversal_times);

  return result;
}

void run_benchmarks(const std::string &output_file) {
  std::ofstream csv(output_file);
  csv << "n,structure,key_dist,priority_dist,operation,metric,mean,stddev,min,"
         "max,"
      << "max_depth,avg_depth,balance_factor\n";

  std::vector<KeyDistribution> key_distributions = {
      KeyDistribution::RANDOM, KeyDistribution::SORTED_ASC,
      KeyDistribution::SORTED_DESC};
  std::vector<PriorityDistribution> priority_distributions = {
      PriorityDistribution::UNIQUE, PriorityDistribution::LOW_COLLISION,
      PriorityDistribution::MED_COLLISION,
      PriorityDistribution::HIGH_COLLISION};

  for (size_t n = 10000; n < 100000; n += 5000) {
    std::cout << "Testing with n = " << n << std::endl;

    for (auto key_dist : key_distributions) {
      for (auto pri_dist : priority_distributions) {
        auto data = generate_test_data(n, key_dist, pri_dist);
        std::vector<BenchmarkResult> results = {
            benchmark_cartesian_tree(data), benchmark_map(data),
            benchmark_unordered_map(data), benchmark_bst(data),
            benchmark_heap(data)};

        for (const auto &result : results) {
          auto write_timing = [&](const std::string &op,
                                  const std::string &metric,
                                  const TimingStats &stats) {
            csv << result.n << "," << result.structure_name << ","
                << static_cast<int>(key_dist) << ","
                << static_cast<int>(pri_dist) << "," << op << "," << metric
                << "," << stats.mean << "," << stats.stddev << "," << stats.min
                << "," << stats.max << "," << result.max_depth << ","
                << result.avg_depth << "," << result.balance_factor << "\n";
          };

          // Insert timings
          write_timing("insert", "overall", result.insert_time.overall);
          write_timing("insert", "first_100", result.insert_time.first_100);
          write_timing("insert", "last_100", result.insert_time.last_100);

          // Search timings
          write_timing("search", "overall", result.search_time.overall);
          write_timing("search", "nonexistent", result.search_time.nonexistent);

          // Simple operations
          write_timing("remove", "overall", result.remove_time);
          write_timing("traverse", "overall", result.traversal_time);
        }
        csv.flush();
      }
    }
  }
}

} // namespace complex
