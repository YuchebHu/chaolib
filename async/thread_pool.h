#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <list>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>

namespace chaolib {
namespace async {
constexpr size_t default_thread_pool_min_thread_num{1};
constexpr std::chrono::milliseconds default_thread_pool_max_idle_ms{250};

class ThreadPool {
public:
  using Task = std::function<void()>;
  ThreadPool(
      size_t min_threads = default_thread_pool_min_thread_num,
      size_t max_threads = std::thread::hardware_concurrency(),
      std::chrono::milliseconds max_idle_ms = default_thread_pool_max_idle_ms);
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;
  ThreadPool(ThreadPool&&) = delete;
  ThreadPool& operator=(ThreadPool&&) = delete;

  virtual ~ThreadPool();

  void setMinThreadNum(size_t value) { this->min_thread_num_ = value; }

  void setMaxThreadNum(size_t value) { this->max_thread_num_ = value; }

  void setMaxIdleTime(std::chrono::milliseconds ms) {
    this->max_idle_time_ = ms;
  }

  size_t getCurrentThreadNum() const { return this->cur_thread_num_; }

  size_t getIdleThreadNum() const { return this->idle_thread_num_; }

  bool isStarted() const { return this->status_ != EStatus::STOP; }

  bool isStopped() { return this->status_ == EStatus::STOP; }

  int start(size_t start_threads = 0);
  int stop();
  int pause();
  int resume();
  int wait() const;

  template <typename Fn, typename... Args>
  auto submit(Fn&& fn, Args&&... args) {
    if (this->status_ == EStatus::STOP) {
      this->start();
    }
    if (this->idle_thread_num_ <= 0 &&
        this->cur_thread_num_ < this->max_thread_num_) {
      this->createThread();
    }

    using RetType = decltype(fn(args...));
    auto task = std::make_shared<std::packaged_task<RetType()>>(
        [fn = std::forward<Fn>(fn), ... args = std::forward<Args>(args)] {
          return std::invoke(std::move(fn), std::move(args)...);
        });
    std::future<RetType> future = task->get_future();
    {
      std::scoped_lock locker{this->task_mutex_};
      this->tasks_.emplace([task] { (*task)(); });
    }
    this->task_cond_.notify_one();
    return future;
  }

private:
  bool createThread();
  void addThread(const std::shared_ptr<std::thread>& thread);
  void deleteThread(std::thread::id id);

private:
  enum class EStatus : unsigned short {
    STOP = 0,
    RUNNING,
    PAUSE,
  };

  struct ThreadData {
    EStatus status_;
    std::thread::id id_;
    std::shared_ptr<std::thread> thread_;
    std::chrono::steady_clock::time_point start_time_;
    std::chrono::steady_clock::time_point stop_time_;
  };

public:
  size_t min_thread_num_;
  size_t max_thread_num_;
  std::chrono::milliseconds max_idle_time_;

private:
  std::atomic<EStatus> status_{EStatus::STOP};
  std::condition_variable status_wait_cond_{};
  std::mutex status_wait_mutex_{};
  std::atomic<size_t> cur_thread_num_{0};
  std::atomic<size_t> idle_thread_num_{0};
  std::list<ThreadData> threads_{};
  std::mutex thread_mutex_{};
  std::queue<Task> tasks_{};
  std::mutex task_mutex_{};
  std::condition_variable task_cond_{};
};
} // namespace async

using ThreadPool = async::ThreadPool;

} // namespace chaolib

#endif // __THREADPOOL_H__