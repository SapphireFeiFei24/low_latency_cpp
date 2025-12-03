#pragma once

#include <condition_variable>
#include <queue>
#include <vector>
#include <thread>
namespace concurrency {

    class ThreadPool {
        public:
        explicit ThreadPool(size_t capacity): capacity_(capacity) {
            for (size_t i = 0; i < capacity_; ++i) {
                workers_.emplace_back([this]() {
                    while (true) {
                        std::function<void()> task;
                        // loop until stop
                        {
                            std::unique_lock<std::mutex> wlock(mutex_);
                            condition_.wait(wlock, [this]() {
                                return stop_ || !tasks_.empty();
                            });

                            if (stop_ && tasks_.empty()) {
                                break;
                            }

                            task = std::move(tasks_.front());
                            tasks_.pop();
                        }
                        task();
                    }
                });
            }
        }

        ~ThreadPool() {
            {
                std::unique_lock<std::mutex> wlock(mutex_);
                stop_ = true;
            }
            condition_.notify_all();
            for (std::thread &worker: workers_) {
                worker.join();
            }
        }

        template <typename F>
        void enqueue(F&& f) {
            {
                std::unique_lock<std::mutex> wlock(mutex_);
                tasks_.emplace(std::forward<F>(f));
            }
            condition_.notify_one();
        }
        private:
        size_t capacity_;
        std::mutex mutex_;
        std::condition_variable condition_;
        std::vector<std::thread> workers_;
        std::queue<std::function<void()>> tasks_;
        bool stop_ = false;

    };

}
