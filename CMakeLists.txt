cmake_minimum_required(VERSION 4.0)
project(low_latency_cpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(low_latency_cpp
        main.cpp
        topics/2_concurrency_memory_model/multi_thread_tests.h
        topics/2_concurrency_memory_model/thread_pool.h)
