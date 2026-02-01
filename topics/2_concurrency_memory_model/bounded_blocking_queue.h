#pragma once

#include <mutex>
#include <queue>
#include <shared_mutex>

namespace concurrency {
    template<typename T>
    class BoundedBlockingQueue {
        public:
        explicit BoundedBlockingQueue(size_t capacity): capacity_(capacity) {
        }
        void push(const T& value) {

            std::unique_lock<std::mutex> lock(mutex_);
            not_full_.wait(lock, [&] {
                // wait: in a single atomic step, it releases the mutex
                // when unblocked (via notification or spurious wakeup)
                    // it reacquires the lock beofre returning
                return queue_.size() < capacity_ && !closed_;
            });
            queue_.push(value);
            not_empty_.notify_one();
        }

        T pop() {
            std::unique_lock<std::mutex> lock(mutex_);
            not_empty_.wait(lock, [&] {
                return !queue_.empty();
            });
            auto value = queue_.front();
            queue_.pop();
            not_full_.notify_one();
            return value;
        }

        bool try_push(const T& value) {
            std::unique_lock<std::mutex> lock(mutex_);
            if (closed_ || queue_.size() >= capacity_) {
                return false;
            }
            queue_.push(value);
            not_full_.notify_one();
            return true;
        }

        bool try_pop(T& value) {
            std::unique_lock<std::mutex> lock(mutex_);
            if (queue_.empty()) {
                return false;
            }
            value = queue_.front();
            queue_.pop();
            not_full_.notify_one();
            return true;
        }

        void close() {
            std::unique_lock<std::mutex> lock(mutex_);
            closed_ = true;
            not_empty_.notify_all();
            not_full_.notify_all();
        }

        bool is_closed() {
            std::unique_lock<std::mutex> lock(mutex_);
            return closed_;
        }
        private:
        size_t capacity_;
        std::mutex mutex_;
        std::queue<T> queue_;
        std::condition_variable not_full_;
        std::condition_variable not_empty_;
        bool closed_ = false;
    };
}
