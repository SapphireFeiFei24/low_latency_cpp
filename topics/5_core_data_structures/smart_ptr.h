#pragma once

#include <atomic>
namespace smart_ptr {
    template<typename T>
    class UniquePtr {
    private:
        T* m_ptr; // The underlying raw pointer

    public:
        // 1. Constructor: Takes ownership of a raw pointer
        explicit UniquePtr(T* ptr = nullptr) : m_ptr(ptr) {}

        // 2. Destructor: Automatically deletes the resource
        ~UniquePtr() {
            delete m_ptr;
        }

        // 3. Disable Copying: unique_ptr must have only ONE owner
        UniquePtr(const UniquePtr&) = delete;
        UniquePtr& operator=(const UniquePtr&) = delete;

        // 4. Enable Moving: Transfer ownership from another pointer
        UniquePtr(UniquePtr&& other) noexcept : m_ptr(other.m_ptr) {
            other.m_ptr = nullptr; // Null the original so it doesn't delete memory
        }

        UniquePtr& operator=(UniquePtr&& other) noexcept {
            if (this != &other) {
                delete m_ptr;        // Delete current resource
                m_ptr = other.m_ptr; // Take new resource
                other.m_ptr = nullptr;
            }
            return *this;
        }

        // 5. Accessors: Allow using it like a real pointer
        T& operator*() const { return *m_ptr; }
        T* operator->() const { return m_ptr; }
        T* get() const { return m_ptr; }
    };

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
        std::atomic<int>* _count; // Strong reference count. Maintain this integer on the heap
        // std::atomic<int>* _count; // Weak reference count
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

        T& operator*() const {
            return *_data;
        }

        T* operator->() const {
            // SharedPtr->member;
            // (SharedPtr.operator->())->member
            return _data;
        }
    };
}