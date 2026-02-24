# Class
## Special Member functions
> * Destructor `~ClassName()`
> * Copy Constructor `ClassName(const ClassName& other)`
> * Copy Assignment Operator `ClassName& opertor=(const ClassName& other)`
> * Move Constructor `ClassName(ClassName&& other) noexcept`
> * Move Assignment Operator `ClassName& operator=(ClassName&& other) noexcept`

## Rule of Five
> If any of the special member functions is defined for a class, then it is better to define all of them.

## `noexcept` Key words on the move constructor/assignment

### `std::vector` Optimization
> If without `noexcept`, the vector will use copy transfers which guarantees: \
> The original vector will only be destroyed when the new vector is fully constructed during reallocation.
When a std::vector needs to resize, it will move elements to the new memory block only if the move constructor is marked noexcept. If you omit it, the vector will perform a slow copy of every element instead. It does this to maintain the "Strong Exception Guarantee"—if a move throws halfway through a resize, the vector cannot easily roll back.

### `move` semantics should never throw
> Telling the compiler "this operation is safe and cheap"

### compiler optimization
> Compiler generate tighter and more efficient machine code for noexcept, no need to take care of cleanup when an exception is thrown.
