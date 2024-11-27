#pragma once

#include <cstddef>

namespace complex {

#define NUM_RUNS 50

struct CartesianMetrics {
  size_t height;
  size_t avg_depth;
  double balance_height_factor;
  double balance_size_factor;
};

struct BSTMetrics {
  size_t height;
  size_t avg_depth;
  double balance_height_factor;
  double balance_size_factor;
};

struct OperationMetrics {
  long insert_time;
  long search_time;
  long failed_search_time;
  long delete_time;
  long traversal_time;
};

void run_all_benchmarks();

} // namespace complex
