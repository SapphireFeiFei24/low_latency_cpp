#pragma once

#include <random>
#include <thread>
#include "concurrent_lru_cache.h"
#include "thread_pool.h"

void multi_thread_test() {
    concurrency::ConcurrentLRUCache<int, int> cache(1000);
    const size_t NUM_THREADS = 10;
    const int OPS_PER_THREAD = 100000;
    auto start = std::chrono::high_resolution_clock::now();
    auto worker = [&](int tid) {
        std::mt19937 rng(tid + 1);
        std::uniform_int_distribution<int> dist(0, 2000);

        for (int i = 0; i < OPS_PER_THREAD; ++i) {
            int key = dist(rng);
            if (i % 3 == 0) {
                // Write
                cache.put(key, key + 100);
            } else {
                // Read
                int out;
                cache.get(key, out);
            }
        }
    };
    std::vector<std::thread> threads;
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.push_back(std::thread(worker, i));
    }
    for (auto &t: threads) {
        t.join();
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Multithread test done in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";
}

void thread_pool_test() {
    concurrency::ConcurrentLRUCache<int, int> cache(2000);
    concurrency::ThreadPool pool(8);

    const int TASKS = 200000;

    std::mt19937 rng(123);
    std::uniform_int_distribution<int> dist(0, 5000);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < TASKS; i++) {
        int key = dist(rng);

        pool.enqueue([key, &cache, i] {
            if (i % 4 == 0) {
                cache.put(key, key * 2);
            } else {
                int v;
                cache.get(key, v);
            }
        });
    }

    // ThreadPool destructor waits for all tasks
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Thread pool test done in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";
}

