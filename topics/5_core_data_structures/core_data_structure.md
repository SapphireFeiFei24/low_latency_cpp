# Core Data Structures

## Smart Pointers
### Unique Ptr
> Exclusive ownership. Can be transferred via `std::move`
* Move constructible, move assignable
* NOT copy constructible, NOT copy assignable
### Shared Ptr
#### Content
```c++
template <typename T>
ControlBlock {
    T* data_; // or T data_;
    std::allocator<T> alloc;
    int* strong_reference_cnt_; // shared ptr cnt
    int* weak_reference_cnt_; // weak ptr cnt
    
    destroy_data(); // destroy data when number of shared oweners hits zero
    ~ControlBlock(); // release the control block
};

template <typename T>
SharedPtr {
    T* data_;
    void* pointer_to_ctrl_block; 
}
```
#### What happens when reference cnt hits zero
* strong_reference_cnt_ == 0: control block calls the destructor of the managed object.
* weak_reference_cnt_ == 0: deallocate control block
#### Issue: Circular Reference
```c++
struct Node {
    shared_ptr<Node> prev;
    shared_ptr<Node> next;
};

auto a = make_shared<Node>();
auto b = make_shared<Node>();

a->next = b;
b->prev = a;
// Neither ever gets destroyed! ref count never hits 0
```
*Solution: weak ptr*
### Weak Ptr
> Model temporary ownership. Converted to `shared_ptr` to acquire temporary ownership.
> * To solve circular reference.

```c++
struct Node {
    shared_ptr<Node> prev;
    weak_ptr<Node> next;
};

auto a = make_shared<Node>();
auto b = make_shared<Node>();

a->next = b;
b->prev = a;
```

#### Usage
```c++
#include <iostream>
#include <memory>
 
std::weak_ptr<int> gw;
 
void observe()
{
    std::cout << "gw.use_count() == " << gw.use_count() << "; ";
    // we have to make a copy of shared pointer before usage:
    if (std::shared_ptr<int> spt = gw.lock()) // lock return a shared_ptr pointing to the obj
        std::cout << "*spt == " << *spt << '\n';
    else // if expired, return nullptr
        std::cout << "gw is expired\n";
}
 
int main()
{
    {
        auto sp = std::make_shared<int>(42);
        gw = sp;
 
        observe();
    }
 
    observe();
}
```
### `make_shared` vs default constructor (`new`)
#### 1. Performances and Memory
* default constructor
  * Two separate allocation: one for the object and one for the control block
* `make_shared`
  * One single allocation: obj and control block in the same continuous memory
    * Faster, cache locality
#### 2. Safety
* default constructor: risky in expressions
* `make_shared`: exception safe

#### 3. Private Constructor and Customer Deleter
* * default constructor: works
* `make_shared`: not working
