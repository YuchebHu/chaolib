#include <print>

#include "async/timer.cpp"

using namespace chelib::async;

int main() {
  timeout([] { std::println("Hello World"); }, 200);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  auto handle = setInterval([] { std::println("Hello Interval"); }, 200);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  auto result = handle.request_stop();
  std::println("stop request: {}", result);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  return 0;
}