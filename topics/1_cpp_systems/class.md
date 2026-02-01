# Class
## Special Member functions
> * Destructor `~ClassName()`
> * Copy Constructor `ClassName(const ClassName& other)`
> * Copy Assignment Operator `ClassName& opertor=(const ClassName& other)`
> * Move Constructor `ClassName(ClassName&& other) noexcept`
> * Move Assignment Operator `ClassName& operator=(ClassName&& other) noexcept`

## Rule of Five
> If any of the special member functions is defined for a class, then it is better to define all of them.