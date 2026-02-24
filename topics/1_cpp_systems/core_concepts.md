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

### 6. Object Model & Memory Layout
> One vtable per class, One vptr per object, sizeof(object) = member variables(including vptr)
* Four Principles of OOP: Encapsulation, Abstraction, Inheritance, Polymorphism
* Low-level Layout
  * non-static data members: directly inside the object
  * static data members: outside the individual class objects(in static data segment BSS or Data, allocated when program begins, freed only when program terminates), with only one copy existing for entire class
  * member functions: outside the object for both static and non-static(Code section), avoid redundant copies
  * virtual functions & vptr: 
    * code: code section
    * virtual table: 
      * (read-only) data section, loaded into data segment at runtime; 
      * **one vtable per class**
      * Layout(compiler dependent)
        * Offset to top: used for adjusting thee this pointer in scenarios involving multiple or virtual inheritance
        * Run-Time Type Information(RTTI) Pointer
          * Pointer to `type_info` object for the class, used for dynamic casting and `typeid`
        * Pointers to Virtual Functions
          * Point to the most derived implementation of a virtual function
    * virtual ptr: 
      * stored as a hidden data member within the object memory layout(compile time)
      * constructed and initialized at compile time
      * **one vptr per object, one vptr for each if multiple inheritance**
  * Virtual Inheritance
    * Solve diamond shape inheritance
    * Ensgture the derived class contains a single shared instance of a common base class