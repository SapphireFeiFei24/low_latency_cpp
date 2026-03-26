#pragma once
#include <atomic>
#include <array>
#include <iostream>
#include <optional>
#include <thread>

namespace lockfree {
    /**
    * @brief Lock Free Queue
    * Requirement:
    *   1 Producer, 1 Consumer. Different speed. Avoid locking on every operation
    * Technical Decision:
    *   Ring buffer + Atomic indices
    * Details:
    *   Align or padding to avoid false sharing between head and tail
    */
    template <typename T, size_t N>
    class SPSCQueue {
        // Consumer owns this cache line
        alignas(64) std::atomic<size_t> head; // ensure head starts on 64 boundary
        size_t cached_tail{0};

        // Producer owns this cache line
        alignas(64) std::atomic<size_t> tail; // ensure tail starts on 64 boundary
        size_t cached_head{0};

        // Data buffer
        alignas(64) std::array<T, N> data;
    public:
        bool enqueue(const T& item) {
            const size_t curr_tail = tail.load(std::memory_order_relaxed); // only single thread(itself) is writing it. no need ensure seq
            const size_t next_tail = curr_tail + 1;

            // Check if full - use cached_head to avoid atomic load every time
            // cached_head is inside the same cache line with curr_tail
            if (curr_tail - cached_head > N) {
                cached_tail = head.load(std::memory_order_acquire); // ensure all the previous writes visible
                // check with the latest data again
                if (curr_tail - cached_tail > N) {
                    return false; // truly full
                }
            }
            data[curr_tail % N] = item;
            tail.store(next_tail, std::memory_order_release);
            return true;
        }

        std::optional<T> dequeue() {
            const size_t curr_head = head.load(std::memory_order_relaxed); // only single thread is writing
            const size_t next_head = curr_head + 1;

            if (cached_tail == curr_head) {
                cached_tail = head.load(std::memory_order_acquire);
                if (cached_tail == next_head) {
                    return std::nullopt; // truly empty
                }
            }
            T& item = data[curr_head % N];
            head.store(next_head, std::memory_order_release);
            return item;
        }
    };

    /**
     * @brif Use lock free queue
     * yield() is the simplest choice. In low-latency systems you'd spin a few times first, then yield, then sleep — trading CPU for latency.
     * yield leads to context switch out of the current thread
     */
    void lock_free_queue() {
        SPSCQueue<int, 64> q;
        std::thread producer([&]() {
            for (int i = 0; i < 10; i++) {
                // can add a few spin before yield
                while (!q.enqueue(i)) {
                    std::this_thread::yield(); // queue full, back off
                }
                std::cout << "Produced: " << i << "\n";
            }
        });
        std::thread consumer([&]() {
        int received = 0;
        while (received < 10) {
            auto item = q.dequeue();
            if (item) {
                std::cout << "Consumed: " << *item << "\n";
                received++;
            } else {
                std::this_thread::yield(); // queue empty, back off
            }
        }
        });

        producer.join();
        consumer.join();
    }


}
