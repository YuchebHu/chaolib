#include <iostream>

#include "async/threadpool.h"

using namespace chelib::async;

void threadPoolTest(size_t minThread, size_t maxThread, size_t taskNum) {
  std::cout << std::format("ThreadPool({}, {}) start! Task num is {}\n",
                           minThread, maxThread, taskNum);
  ThreadPool threadPool{minThread, maxThread};
  auto start = std::chrono::high_resolution_clock::now();
  threadPool.start();
  for (size_t i = 0; i < taskNum; ++i) {
    threadPool.submit(
        []() { std::this_thread::sleep_for(std::chrono::milliseconds(10)); });
  }
  threadPool.wait();
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "cost: " << std::chrono::duration<double>(end - start).count()
            << std::endl
            << std::endl;
  ;
}

int main() {
  for (auto taskNum : {100, 200, 400, 800, 1600, 3200, 6400}) {
    threadPoolTest(2, 4, taskNum);
    threadPoolTest(2, 8, taskNum);
    threadPoolTest(2, 15, taskNum);
  }
  return 0;
}