#include <chrono>
#include <thread>

#include "core/scoped_timer.h"

using namespace chelib;

int main() {
  {
    ScopedTimer timer;
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
  }

  return 0;
}