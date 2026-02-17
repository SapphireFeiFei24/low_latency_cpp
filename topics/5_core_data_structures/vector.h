#pragma once

#include <vector>
/***
 * Contiguous dynamic array
 * Random access O(1)
 * push_back, amortized O(1)
 * insert middle/erase middle O(n)
 *
 * Good: continuous memory, cache friendly
 * Trap: Reallocation invalidates: iterators, references, pointers --> preallocate large size
 ***/

template <class T, class A = std::allocator<T>>
class Vector {
private:
    T* data_;         // Points to the first element
    std::size_t size_;     // Current number of elements
    std::size_t capacity_; // Total allocated storage size
    A allocator_;      // The memory allocator

public:
    // Constructors, destructors, public member functions (push_back, pop_back, etc.)
    void push_back(const T& value) {
        if (size_ == capacity_) {
            // Reallocation needed
            reserve(capacity_ == 0 ? 1 : capacity_ * 2); // Common strategy is doubling
        }
        // Construct the new element in the next available raw memory location
        std::construct_at(data_ + size_, value);
        size_++;
    }

    void reserve(unsigned int new_capacity) {
        if (new_capacity <= capacity_) return;

        // Allocate a new, larger block of raw memory
        T* new_buffer = allocator_.allocate(new_capacity);

        // Move (or copy in older C++ versions) elements from the old buffer to the new one
        try {
            std::uninitialized_move(data_, data_ + size_, new_buffer);
        } catch (...) {
            allocator_.deallocate(new_buffer, new_capacity);
            throw; // ensure exception safety
        }

        // Destroy elements and deallocate the old memory block
        for (size_t i = 0; i < size_; ++i) {
            allocator_.destroy(data_ + i);
        }
        allocator_.deallocate(data_, capacity_);

        data_ = new_buffer;
        capacity_ = new_capacity;
    }

    void pop_back() {
        if (size_ > 0) {
            --size_; // Simply decrement size; memory is reused later
        }
    }

    T& operator[](size_t index) { return _data[index]; } // Read/Write access
};

void use_vector () {
    std::vector<int> vec;
    vec.resize(10);
    vec.reserve(10);
    vec.emplace_back(10);
    vec.pop_back();
    vec.begin();
    vec.end();
}

