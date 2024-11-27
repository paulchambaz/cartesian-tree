#pragma once

#include <unordered_map>
#include <vector>
namespace complex {

template <typename K, typename P> class BinaryHeap {
public:
  struct HeapNode {
    K key;
    P priority;

    HeapNode(const K &k, const P &p) : key(k), priority(p) {}
  };

  std::vector<HeapNode> heap;
  std::unordered_map<K, size_t> key_to_index;

  void insert(const K &key, const P &priority) {
    heap.emplace_back(key, priority);
    size_t index = heap.size() - 1;
    key_to_index[key] = index;

    // Sift up
    while (index > 0) {
      size_t parent = (index - 1) / 2;
      if (heap[parent].priority <= heap[index].priority)
        break;

      std::swap(heap[parent], heap[index]);
      key_to_index[heap[parent].key] = parent;
      key_to_index[heap[index].key] = index;
      index = parent;
    }
  }

  bool find(const K &key) const { return key_to_index.count(key) > 0; }

  void remove(const K &key) {
    auto it = key_to_index.find(key);
    if (it == key_to_index.end())
      return;

    size_t index = it->second;
    size_t last = heap.size() - 1;

    // If not the last element, swap with last and sift
    if (index != last) {
      std::swap(heap[index], heap[last]);
      key_to_index[heap[index].key] = index;
      heap.pop_back();
      key_to_index.erase(key);

      if (heap.empty())
        return;

      // Sift down
      while (true) {
        size_t smallest = index;
        size_t left = 2 * index + 1;
        size_t right = 2 * index + 2;

        if (left < heap.size() &&
            heap[left].priority < heap[smallest].priority) {
          smallest = left;
        }
        if (right < heap.size() &&
            heap[right].priority < heap[smallest].priority) {
          smallest = right;
        }

        if (smallest == index)
          break;

        std::swap(heap[index], heap[smallest]);
        key_to_index[heap[index].key] = index;
        key_to_index[heap[smallest].key] = smallest;
        index = smallest;
      }
    } else {
      heap.pop_back();
      key_to_index.erase(key);
    }
  }
};
} // namespace complex
