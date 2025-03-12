#ifndef __TIMER_H__
#define __TIMER_H__
#include <functional>
#include <stop_token>

namespace chelib {
namespace async {
void timeout(std::function<void()> func, int time);
std::stop_source setInterval(std::function<void()> func, int interval);
} // namespace async

} // namespace chelib
#endif // __TIMER_H__