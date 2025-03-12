#include <print>
#include <thread>
#include <vector>

#include "async/mpscqueue.h"

using namespace chelib::async;

int main() {
  {
    MpscQueue<int> queue;
    std::println("queue isEmpty: {}", queue.isEmpty());
    queue.enqueue(8);
    queue.enqueue(10);
    queue.enqueue(20);
    std::println("after enqueue, queue isEmpty: {}", queue.isEmpty());
    while (!queue.isEmpty()) {
      auto result = queue.dequeue();
      std::println("value: {}", result.value());
    }
    std::println("after dequeue, queue isEmpty: {}", queue.isEmpty());
  }

  {
    MpscQueue<std::string> queue;
    std::println("queue isEmpty: {}", queue.isEmpty());
    queue.enqueue("8");
    queue.enqueue("10");
    queue.enqueue("20");
    std::println("after enqueue, queue isEmpty: {}", queue.isEmpty());
    while (!queue.isEmpty()) {
      auto result = queue.dequeue();
      std::println("value: {}", result.value());
    }
    std::println("after dequeue, queue isEmpty: {}", queue.isEmpty());
  }
  {
    MpscQueue<int> queue;

    std::vector<std::jthread> threads;
    threads.reserve(3);
    for (int i = 0; i < 3; ++i) {
      threads.push_back(std::jthread{[&]() {
        for (int i = 0; i < 50; ++i) {
          queue.enqueue(std::rand());
          std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
      }});
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::jthread consumer{[&]() {
      while (!queue.isEmpty()) {
        std::println("Element: {}", *queue.peek());
        [[maybe_unused]] auto value = queue.dequeue();
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
      }
    }};

    consumer.join();
    for (auto& thread : threads) {
      thread.join();
    }
  }

  return 0;
}