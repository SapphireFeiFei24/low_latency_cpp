#pragma once

#include <atomic>
namespace smart_ptr {
    /***
     * Both data and reference count are maintained on the heap.
     * Reason:
     *  Stack is tied to the lifetime of the object. Destroyed during object destruction
     *  Shared access across shared ptrs
     * noexcept on move semantics for vector reallocation optimization
     ***/
    template <typename T>
    class SharedPtr {
        T* _data;
        std::atomic<int>* _count; // Maintain this integer on the heap
        void release() {
            // helper function: decrease count by one
            if (_count != nullptr) {
                --(*_count);
                if (*_count == 0) {
                    // no more reference to the resource
                    // reset all the pointers
                    delete _data;
                    delete _count;
                    _data = nullptr;
                    _count = nullptr;
                }
            }
        }
    public:
        explicit SharedPtr(T* ptr = nullptr) {
            // use explicit to avoid unwanted auto type conversion
            if (ptr) {
                _data = ptr;
                _count = new std::atomic<int>(1);
            }
        }
        SharedPtr(const SharedPtr& other) {
            _data = other._data;
            _count = other._count;
            if (_count) {
                ++(*_count);
            }
        }
        SharedPtr& operator=(const SharedPtr& other) {
            // release old, take new
            if (this != &other) { // check the avoid self referencing
                release();
                _data = other._data;
                _count = other._count;
                if (_count) {
                    ++(*_count);
                }
            }

            return *this;
        }

        SharedPtr(SharedPtr&& other) noexcept {
            if (this != &ptr) {
                release();

                // transfer ownership
                _data = other._data;
                _count = other._count;

                // nullify other manually
                other._data = nullptr;
                other._count = nullptr;
            }
        }
        SharedPtr& operator=(SharedPtr&& other) noexcept{
            if (this != &other) {
                release();
                _data = other._data;
                _count = other._count;
                other._data = nullptr;
                other._count = 0;
            }
            return *this;
        }
    };
}