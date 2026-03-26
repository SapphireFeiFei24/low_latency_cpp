# Class & Struct
> * Class: private by default
> * Struct: public by default
## Special Member functions
> * Destructor `~ClassName()`
> * Copy Constructor `ClassName(const ClassName& other)`
> * Copy Assignment Operator `ClassName& opertor=(const ClassName& other)`
> * Move Constructor `ClassName(ClassName&& other) noexcept`
> * Move Assignment Operator `ClassName& operator=(ClassName&& other) noexcept`

### Constructor vs Assignment Operator
* Same: All use the other object to create a new object
* Difference:
  * Constructor: initialize a new object from existing one
    * Allocate new resource
  * Assignment Operator: update an already existing object
    * Reuse the current resource
    * Requires careful handling of self-assignment and proper resource management.
## Rule of Five
> If any of the special member functions is defined for a class, then it is better to define all of them.

## `noexcept` Key words on the move constructor/assignment

### `std::vector` Optimization
> If without `noexcept`, the vector will use copy transfers which guarantees: \
> The original vector will only be destroyed when the new vector is fully constructed during reallocation.
When a std::vector needs to resize, it will move elements to the new memory block only if the move constructor is marked noexcept.
* If you omit it, the vector will perform a slow copy of every element instead.
  * `memcpy` is faster than slow element copy: memcpy is highly optimized, often copy by chunks instead of bytes
* It does this to maintain the "Strong Exception Guarantee"—if a move throws halfway through a resize, the vector cannot easily roll back.

### `move` semantics should never throw
> Telling the compiler "this operation is safe and cheap"

### compiler optimization
> Compiler generate tighter and more efficient machine code for noexcept, no need to take care of cleanup when an exception is thrown.

## Virtual Function
> * One virtual table per class.
>   * A static array of functions pointers is created for each class that has or inherits virtual functions.
>   * Each entry in the vtable points to the most derived implementation of a virtual function accessible by that class.
> * One virtual ptr(pointing to virtual table) per object.
> * Slower than nonvirtual: involves pointer look up

### Virtual Table Construction
> * Constructed at runtime.
> * Add .rodata under data segment

### How the Call is Resolved
1. vptr->vtable: The program follows the object's VPtr to find the correct VTable.
2. function addr->offset: It looks up the function's address at a fixed offset (index) within that table.
3. Address->code: It jumps to that address to execute the most-derived version of the function.

## Virtual inheritance
* Solve diamond shape inheritance
* Ensure the derived class contains a single shared instance of a common base class
* Unlike normal inherited class which stores in the beginning of the object, **virtual inherited object stored at the end of the object**

## Memory Layout
### Alignment - Paddings
> CPUs fetch memory in aligned chunks (words or cache lines).
If data crosses boundaries, the CPU may need multiple memory accesses.
* [Sequential Allocation] Members are allocated in the order they declared.
* [Individual Member Alignment] Variable x must start at the offset that's a multiple of the sizeof(x)
* [Struct/Class Alignment] Struct size must be multiple of the maximum struct alignment of all of its individual members.
* To minimize paddings: start with the largest variables
```c++
struct A {
    char c;  // start at multiple of 1
    int a;   // start at multiple of 4
    char c2; // start at multiple of 1
    char c3; // start at multiple of 1
};
```
Memory layout (Total size is 12 bytes):
```text
offset 0: c
offset 1: padding
offset 2: padding
offset 3: padding
offset 4: a
offset 8: c2
offset 9: c3
offset 10: padding
offset 11: padding
```
### Virtual Function `vptr`
> If a class has virtual functions, a vptr is inserted. Usually in the beginning of the object.
```c++
class Base {
public:
    virtual void foo();
    int x;
};
```

```text
0   vptr (8 bytes) // point to vtable
8   x (4 bytes)
12  padding
```
### Inheritance
> Base class memory comes first

### Virtual Inheritance
> Virtual inherited class memory is storeed at the end of the object.

### False Sharing
> Two (or more) cores modify the different variables on the same cache line. \
> Causing cache misses. \
> **Solution: Add Paddings**
```c++
struct A {
    char c;  
    char c2; // on the same cache line at c
};
```