#ifndef __SCOPED_TIMER_H__
#define __SCOPED_TIMER_H__
#include <chrono>

namespace chelib {
namespace scoped {
class ScopedTimer {
public:
  ScopedTimer();
  ~ScopedTimer();

private:
  std::chrono::time_point<std::chrono::steady_clock> start_;
};
} // namespace scoped
using ScopedTimer = scoped::ScopedTimer;
} // namespace chelib

#endif // __SCOPED_TIMER_H__