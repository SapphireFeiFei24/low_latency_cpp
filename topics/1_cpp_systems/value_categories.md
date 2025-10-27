# Value Category, Copy/Semantics
> Goal
> * Write zero-copy APIs
> * Control object lifetime precisely
> * Enable RVO (return value optimization)
> * Avoid temporaries and unnecessary destructor calls

## C++ Value Category System
* lvalue
  * Has an identity(can take its address)
  * Usually named objects
  * `int x; x = 5` x is an lvalue
* xvalue
  * Expiring value
  * Object whose resources can be reused
  * typically a cast or return of a temporary
  * `std::move(x)`
* prvalue
  * pure rvalue
  * Temporary value, not associated with a persistent object
  * `42`, `MyClass()`
  * `int z = x + 1`, `x+1` is a prvalue
* glvalue
  * lvalue or xvalue
* rvalue
  * prvalue or xvalue

## Copy vs Move
* Copy constructor/assignment
  * triggerd when the source is an lvalue
  * creates a deep copy(can be expensive)
* Move constructor/assignment
  * triggered when the source is an rvalue
  * Moves resources instead of copying(cheap)
```cpp
#include <iostream>
#include <vector>

struct Data {
    std::vector<int> v;
    Data() { std::cout << "default ctor\n"; }
    Data(const Data&) { std::cout << "copy ctor\n"; }
    Data(Data&&) noexcept { std::cout << "move ctor\n"; }
};

Data make_data() {
    Data d;
    return d;
}

int main() {
    Data a;
    Data b = a;          // copy ctor
    Data c = make_data(); // move or elided
}
```

## Return Value Optimization(RVO) and Copy Elision
The compiler is allowed(and since C++17, required) to elide copies/moves when returning a local variable directly.

### When ROV Doesn't Happen
RVO fails when:
* You return different objects based on branches

## Perfect Forwarding and `std::move`
### `std::move`
> `std::move` is just a cast to `T&&` -- it doesn't move anything by itself
> `&&`: an Rvalue reference declaration (C++11 and later)

It allows the compiler to pick the move constructor.
It tells the compiler: "I won't use it again. Feel free to take it"

### `std::forward`
Used in template functions to preserve value categories

```cpp
template<typename T>
void wrapper(T&& arg) {
    callee(std::forward<T>(arg));
}
```
* Without it, every argument would be treated as lvalue inside the function.
* With `std::forward`, the callee can take advantage of move semantics when appropriate.


#### T&& arg in a template
Normally, `T&&` means an rvalue reference - but in a template, it behaves differently.
> When `T` is a template parameter, T&& is a forwarding reference(also knwon as universal reference).
Rule:
If `T` is a deducted template parameter and the parameter type is `T&&`, then it's a *forwarding reference*.
##### Reference collapsing rules
> If either of the references is an lvalue reference(&), the result is an lvalue reference. 
> Only when both are rvalue references(&&) does the result remain &&.
> * `& &` -> `&`
> * `& &&` -> `&`
> * `&& &` -> `&`
> * `&& &&` -> `&&`

The problem they solve: in C++, you can't have a reference to a reference. But with templates, this can happen indirectly. \
So C++ defines reference collapsing rules to determine what the final type should be.

These rules enable perfect forwarding.

## Copy Elision vs Move Semantics Cost
* Construct on stack, ~1-3 ns, almost free
* Copy large vector, ~O(N), can be 100-500 ns
* Move large vector, ~10-20ns, just swaps pointers -- avoid unnecessary copy
* Heap allocate, 100-100 ns, unpredictable.

