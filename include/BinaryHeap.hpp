#pragma once

#include <vector>
namespace complex {

template <typename T> class BinaryHeap {
private:
  std::vector<T> heap;

public:
  void push(T value) {
    heap.push_back(value);
    size_t i = heap.size() - 1;
    while (i > 0) {
      size_t parent = (i - 1) / 2;
      if (heap[parent] <= heap[i])
        break;
      std::swap(heap[parent], heap[i]);
      i = parent;
    }
  }

  T pop() {
    T result = heap[0];
    heap[0] = heap.back();
    heap.pop_back();

    if (!heap.empty()) {
      size_t i = 0;
      while (true) {
        size_t smallest = i;
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;

        if (left < heap.size() && heap[left] < heap[smallest])
          smallest = left;
        if (right < heap.size() && heap[right] < heap[smallest])
          smallest = right;

        if (smallest == i)
          break;

        std::swap(heap[i], heap[smallest]);
        i = smallest;
      }
    }
    return result;
  }

  bool empty() const { return heap.empty(); }
  size_t size() const { return heap.size(); }
  T top() const { return heap[0]; }
};

} // namespace complex
