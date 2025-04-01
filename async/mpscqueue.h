#ifndef __MPSCQUEUE_H__
#define __MPSCQUEUE_H__

#include <atomic>
#include <optional>

namespace chaolib {
namespace async {
template <typename T> class MpscQueue {
public:
  MpscQueue() {
    Node* node = new Node;
    this->head_.store(node, std::memory_order_relaxed);
    this->tail_ = node;
  }
  ~MpscQueue() {
    auto next = this->tail_->next_.load(std::memory_order_relaxed);
    delete this->tail_;
    while (next != nullptr) {
      auto node = next;
      next = node->next_.load(std::memory_order_relaxed);
      delete node;
    }
  }

  template <typename... Args> void enqueue(Args&&... args) {
    Node* next = new Node;
    next->value_ = T{std::forward<Args>(args)...};
    auto prev = this->head_.exchange(next, std::memory_order_acq_rel);
    prev->next_.store(next, std::memory_order_release);
  }

  std::optional<T> dequeue() {
    Node* next = this->tail_->next_.load(std::memory_order_acquire);
    if (!next) {
      return std::nullopt;
    }
    T result = std::move(next->value_);
    delete this->tail_;
    this->tail_ = next;

    return result;
  }

  bool dequeue(T& element) {
    auto result = this->dequeue();
    if (result) {
      element = std::move(result.value());
      return true;
    }
    return false;
  }

  T* peek() const {
    auto peek = this->tail_->next_.load(std::memory_order_acquire);
    if (!peek) {
      return nullptr;
    }
    return &peek->value_;
  }

  bool isEmpty() const {
    return this->tail_->next_.load(std::memory_order_acquire) == nullptr;
  }

private:
  struct Node {
    std::atomic<Node*> next_{nullptr};
    T value_;
  };
  std::atomic<Node*> head_{nullptr};
  Node* tail_{nullptr};
};
} // namespace async

template <typename T> using MpscQueue = async::MpscQueue<T>;

} // namespace chaolib

#endif // __MPSCQUEUE_H__