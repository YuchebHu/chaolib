#include <iostream>

#include "async/thread_pool.h"
#include "core/scoped_timer.h"

using namespace chelib;

void threadPoolTest(size_t minThread, size_t maxThread, size_t taskNum) {
  std::cout << std::format("ThreadPool({}, {}) start! Task num is {}\n",
                           minThread, maxThread, taskNum);
  ThreadPool threadPool{minThread, maxThread};
  ScopedTimer timer;
  threadPool.start();
  for (size_t i = 0; i < taskNum; ++i) {
    threadPool.submit(
        []() { std::this_thread::sleep_for(std::chrono::milliseconds(10)); });
  }
  threadPool.wait();
}

int main() {
  for (auto taskNum : {100, 200, 400, 800, 1600, 3200, 6400}) {
    threadPoolTest(2, 4, taskNum);
    threadPoolTest(2, 8, taskNum);
    threadPoolTest(2, 15, taskNum);
  }
  return 0;
}