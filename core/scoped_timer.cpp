#include "scoped_timer.h"

#include <print>

namespace chaolib {
namespace scoped {
ScopedTimer::ScopedTimer()
    : start_(std::chrono::high_resolution_clock::now()) {}

ScopedTimer::~ScopedTimer() {
  auto end = std::chrono::high_resolution_clock::now();
  std::println(
      "scoped code cost: {}s.",
      std::chrono::duration<double>(end - this->start_).count());
}
} // namespace scoped
} // namespace chaolib