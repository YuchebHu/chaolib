#ifndef __SPIN_LOCK_H__
#define __SPIN_LOCK_H__
#include <atomic>

namespace chaolib {
namespace async {
class SpinLock {
public:
  void lock() {
    while (this->flag_.test_and_set(std::memory_order_acquire)) {
      ;
    }
  }

  void unlock() { this->flag_.clear(std::memory_order_release); }

private:
  std::atomic_flag flag_ = ATOMIC_FLAG_INIT;
};
} // namespace async
using SpinLock = async::SpinLock;

} // namespace chaolib
#endif // __SPIN_LOCK_H__