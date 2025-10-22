# Deep C++ Performance & Internals

## Goal
* Know how C++ code translates to CPU instructions
* Be able to reason about every byte and every copy
* Recognize hidden costs - temporaries, heap allocations, and function calls
* Write small programs to measure and verify performance claims

## Core Concepts to Master
### 1. Value Categories & Copy/Move Semantics
* lvalue, rvalue, prvalue
* Move vs copy semantics
* Copy elision and RVO (Return Value Optimization)
* When the compiler is forced to allocate or copy

### 2. Stack vs Heap, Object Layout
* Stack allocations are ~1ns, heap can be 100ns–1µs.
* Heap introduces fragmentation & unpredictable latency.
* Know about alignment, padding, and sizeof(struct) rules.

### 3. Function Inlining and Call Overhead
* A non-inlined call costs 3–10ns on modern CPUs due to pipeline & branch misprediction.

### 4. Cache Awareness
* CPU cache lines (typically 64 bytes)
* Cache misses (~100ns penalty)
* Spatial and temporal locality
* False sharing

### 5. Inline Storage & Small Object Optimization