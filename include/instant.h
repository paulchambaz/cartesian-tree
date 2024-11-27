#pragma once
#include <chrono>

namespace complex {

class Instant {
private:
  std::chrono::time_point<std::chrono::steady_clock> start;

public:
  Instant(std::chrono::time_point<std::chrono::steady_clock> s) : start(s) {}
  static Instant now() { return Instant(std::chrono::steady_clock::now()); }
  long elapsed() const {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(now - start)
        .count();
  }
};
} // namespace complex
