#include "timer.h"

#include <chrono>
#include <thread>

namespace chaolib::async {
void timeout(std::function<void()> func, int time) {
  std::jthread{[time, func = std::move(func)] {
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
    if (func) {
      func();
    }
  }}.detach();
}

std::stop_source setInterval(std::function<void()> func, int interval) {
  std::jthread thread{
      [interval, func = std::move(func)](std::stop_token stop_token) {
        while (!stop_token.stop_requested()) {
          std::this_thread::sleep_for(std::chrono::milliseconds(interval));
          if (func) {
            func();
          }
        }
      }};
  auto result = thread.get_stop_source();
  thread.detach();
  return result;
}
} // namespace chaolib::async