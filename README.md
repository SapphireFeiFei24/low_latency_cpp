# Low Latency CPP

## Goal
* Write high-performance, lock-free, cache-aware C++ code
* Analyze latency sources across CPU, memory and network
* Linux performace tuning

## Core Knowledge Areas for Low Latency C++ Interviews
### 1. C++ Performance and Internals
* Deep understanding of C++ memory model, stack vs heap, and allocation overheads
* Modern C++ features
  * Move semantics, RAII, inline, constexpr, placement new, noexcept, final, override
  * Smart Pointers(`unique_ptr`, `shared_ptr`, `weak_ptr`) -- and when not to use them
  * C++17/20/23 relevant features: std::optional, std::variant, std::chrono, coroutines (if applicable)
* Copy elision, RVO, small object optimization
* Templates & constexpr metaprogramming — compile-time performance tricks
* Avoiding virtual dispatch, using CRTP
#### Practice
* Re-implement parts of STL (vector, ring buffer) from scratch
* Profile allocations and understand cache misses

### 2. Concurrency & Multithreading
* Memory ordering and barriers(std::atomic, memory_prder_relaxed/acquire/release)
* Lock-free data structures(queues,stacks)
* False sharing, NUMA awareness
* Thread affinity and pinning
* Spinlocks vs mutexes, contention handling

#### Practice
* Write lock-free producer-consumer queues
* Benchmark mutex vs spinlock behavior
### 3. OS & System-level Knowledge
* Linux internals: context switching, page faults, interrupts
* Network stack(TCP/UDP), kernel bypass(DPDK, RDMA)
* epoll, mmap, hugepages, syscalls latency
* CPU cache hierarchy(L1/L2/L3), prefetching, cache alignment
* per, strace, ltrace, htop, numactl, taskset
#### Practice
* Measure nanosecond-level latency with std::chrono::steady_clock
* Use perf stat and perf record on your C++ programs

### 4. Low-Latency Techniques
* Zero-copy design(avoiding memcpy, move data by pointer)
* Preallocation & object pools
* Batching & pipelinening
* NUMA locality optimization
* Lock-free ring buffers(used in trading systems)
* Efficient serialization(flatbuffers, Cap'n Proto)

### 5. Networking for Trading Systems
* UDP multicast market data feed handling
* TCP latency vs UDP loss
* Kernel-bypass libraries(Solarflare Onload, DPDK)
* Message queue protocols(e.g. FIX, ITCH, OUCH)

### 6. Algorithmc Efficiency
* Cache-friendly algorithms and data layouts
* Micro-optimization (branch prediction, SIMD)
* Profiling and identifying bottlenecks

#### Practice:
* Write microbenchmarks with Google Benchmark
* Implement fixed-capacity containers or specialized hashmaps

