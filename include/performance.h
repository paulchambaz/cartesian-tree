#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace complex {

#define NUM_RUNS 50

enum class KeyDistribution { RANDOM, SORTED_ASC, SORTED_DESC, ALTERNATING };

enum class PriorityDistribution {
  UNIQUE,
  LOW_COLLISION,
  MED_COLLISION,
  HIGH_COLLISION
};

// Basic timing statistics
struct TimingStats {
  double mean;
  double stddev;
  double min;
  double max;
};

struct InsertTimingStats {
  TimingStats overall;
  TimingStats first_100;
  TimingStats last_100;
};

struct SearchTimingStats {
  TimingStats overall;
  TimingStats nonexistent;
};

struct BenchmarkResult {
  size_t n;
  std::string structure_name;
  InsertTimingStats insert_time;
  SearchTimingStats search_time;
  TimingStats remove_time;
  TimingStats traversal_time;
  size_t max_depth;
  double avg_depth;
  double balance_factor;
  std::vector<size_t> depth_distribution;
};

// Data generation
std::vector<std::pair<std::string, int>>
generate_test_data(size_t n, KeyDistribution dist = KeyDistribution::RANDOM);

// Benchmark functions
BenchmarkResult
benchmark_cartesian_tree(const std::vector<std::pair<std::string, int>> &data,
                         size_t num_runs = NUM_RUNS);
BenchmarkResult
benchmark_map(const std::vector<std::pair<std::string, int>> &data,
              size_t num_runs = NUM_RUNS);
BenchmarkResult
benchmark_unordered_map(const std::vector<std::pair<std::string, int>> &data,
                        size_t num_runs = NUM_RUNS);

BenchmarkResult
benchmark_bst(const std::vector<std::pair<std::string, int>> &data,
              size_t num_runs = NUM_RUNS);

BenchmarkResult
benchmark_heap(const std::vector<std::pair<std::string, int>> &data,
               size_t num_runs = NUM_RUNS);

void run_benchmarks(const std::string &output_file);

} // namespace complex
