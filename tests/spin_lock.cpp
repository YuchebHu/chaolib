#include <iostream>
#include <latch>
#include <syncstream>
#include <thread>

#include "async/spin_lock.h"

#define sync_stream std::osyncstream(std::cout)

using namespace chaolib;

int main() {
  SpinLock spin_lock;
  spin_lock.lock();
  std::latch latch(4);
  for (auto i : {1, 2, 3, 4}) {
    std::thread{[&spin_lock, &latch, i] {
      spin_lock.lock();
      sync_stream << "thread " << i << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      spin_lock.unlock();
      latch.count_down();
    }}.detach();
  }
  spin_lock.unlock();
  latch.wait();

  return 0;
}