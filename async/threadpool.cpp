#include "threadpool.h"

namespace chelib {
int ThreadPool::start(size_t start_threads) {
  if (this->status_ != EStatus::STOP) {
    return -1;
  }

  this->status_ = EStatus::RUNNING;
  start_threads =
      std::clamp(start_threads, this->min_thread_num_, this->max_thread_num_);
  for (size_t i = 0; i < start_threads; ++i) {
    createThread();
  }

  return 0;
}

int ThreadPool::stop() {
  std::unique_lock lock{this->status_wait_mutex_};
  if (this->status_ == EStatus::STOP) {
    return -1;
  }

  this->status_ = EStatus::STOP;
  this->status_wait_cond_.notify_all();
  this->task_cond_.notify_all();

  for (auto &thread : this->threads_) {
    if (thread.thread_->joinable()) {
      thread.thread_->join();
    }
  }

  this->threads_.clear();
  this->cur_thread_num_ = 0;
  this->idle_thread_num_ = 0;
  return 0;
}

int ThreadPool::pause() {
  if (this->status_ == EStatus::RUNNING) {
    this->status_ = EStatus::PAUSE;
  }
  return 0;
}

int ThreadPool::resume() {
  std::scoped_lock status_lock{this->status_wait_mutex_};
  if (this->status_ == EStatus::PAUSE) {
    this->status_ = EStatus::RUNNING;
    this->status_wait_cond_.notify_all();
  }

  return 0;
}

int ThreadPool::wait() const {
  while (true) {
    if (this->status_ == EStatus::STOP ||
        (this->tasks_.empty() &&
         this->idle_thread_num_ == this->cur_thread_num_)) {
      break;
    }
    std::this_thread::yield();
  }
  return 0;
}

bool ThreadPool::createThread() {
  if (this->cur_thread_num_ >= this->max_thread_num_) {
    return false;
  }

  auto thread = std::make_shared<std::thread>([this] {
    bool initial_run = true;
    while (this->status_ != EStatus::STOP) {
      {
        std::unique_lock status_lock{this->status_wait_mutex_};
        this->status_wait_cond_.wait(
            status_lock, [this] { return this->status_ != EStatus::PAUSE; });
      }

      Task task;
      {
        std::unique_lock locker{this->task_mutex_};
        this->task_cond_.wait_for(
            locker, std::chrono::milliseconds(this->max_idle_time_), [this] {
              return this->status_ == EStatus::STOP || !this->tasks_.empty();
            });
        if (this->status_ == EStatus::STOP) {
          return;
        }

        if (this->tasks_.empty()) { // if waiting timeout and tasks is empty
          if (this->cur_thread_num_ > this->min_thread_num_) {
            this->deleteThread(std::this_thread::get_id());
            return;
          }
          continue;
        }
        if (!initial_run) {
          --this->idle_thread_num_;
        }
        task = std::move(this->tasks_.front());
        this->tasks_.pop();
      }
      if (task) {
        task();
        ++this->idle_thread_num_;
        initial_run = false;
      }
    }
  });
  this->addThread(thread);
  return true;
}
} // namespace chelib