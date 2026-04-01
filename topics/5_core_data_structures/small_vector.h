#pragma once

#include <array>
#include <locale>
#include <memory>
/***
 * Goal: using stack memory for vector when size is small
 * and heap memory when large
 * Free existing rules: only in assignment operators, NOT in constructors
 ***/
namespace small_vector {
    template <typename T, size_t THRES>
    class SmallVector {
        T* _heap_data;
        std::array<int, THRES> _stack_data = {0};
        int _capacity;
        int _cnt;
        std::allocator<T> _alloc;
        bool using_heap() const {
            return _capacity > THRES;
        }
        // friend void swap(SmallVector&a, SmallVector& b) {
           // Risky, TODO: handle heap mode and stack mode mixed situation. Use classic const reference is better
        //     if (b.using_heap()) {
        //         std::swap(a._heap_data, b._heap_data);
        //     } else {
        //         std::swap(a._stack_data, b._stack_data);
        //     }
        //     std::swap(a._capacity, b._capacity);
        //     std::swap(a._cnt, b._cnt);
        //
        // }

    public:
        SmallVector(int capacity) : _heap_data(nullptr), _capacity(capacity), _cnt(0) {}
        ~SmallVector() {
            if (_capacity > THRES && _heap_data) {
                _alloc.deallocate(_heap_data, _capacity);
            }
        }
        SmallVector(const SmallVector& other) {
            _capacity = other._capacity;
            _cnt = other._cnt;
            if (_capacity <= THRES) {
                // small mode
                std::copy(other._stack_data.begin(), other._stack_data.begin() + other._cnt, _stack_data.begin());
            } else {
                _heap_data = _alloc.allocate(other._capacity);
                std::copy(other._heap_data, other._heap_data + other._cnt, _heap_data);
            }
        }
        SmallVector& operator=(const SmallVector& other) {
            if (this == &other) {
                // must handle self assignment
                return *this;
            }
            // Free existing heap memory if needed
            if (_capacity > THRES && _heap_data) {
                _alloc.deallocate(_heap_data, _capacity);
            }

            _capacity = other._capacity;
            _cnt = other._cnt;

            if (other._capacity <= THRES) {
                std::copy(other._stack_data.begin(), other._stack_data.begin() + other._cnt, _stack_data.begin());
            } else {
                _heap_data = _alloc.allocate(other._capacity);
                std::copy(other._heap_data, other._heap_data+other._cnt, _heap_data);
            }
            return *this;
        }

        // SmallVector& operator=(const SmallVector& other) {
        //     // pass by value, copy and swap idio
        //     swap(*this, other);
        //     return *this;
        // }
        SmallVector(SmallVector&& other) noexcept :
        _heap_data (nullptr),
        _capacity (other._capacity),
        _cnt (other._cnt){
            // no deallocate in constructors

            if (other._capacity <= THRES) {
                _stack_data = std::move(other._stack_data);
            } else {
                _heap_data = other._heap_data;
                other._heap_data = nullptr;
            }
            // reset other
            other._capacity = 0;
            other._cnt = 0;
        }

        SmallVector& operator=(SmallVector&& other) noexcept {
            if (this == &other) {
                return *this;
            }
            // reset curr
            if (using_heap() && _heap_data) {
                _alloc.deallocate(_heap_data, _capacity);
            }
            _capacity = other._capacity;
            _cnt = other._cnt;

            if (other._capacity <= THRES) {
                _stack_data = std::move(other._stack_data);
            } else {
                _heap_data = other._heap_data;
                other._heap_data = nullptr;
            }
            // reset other
            other._capacity = 0;
            other._cnt = 0;
            return *this;
        }

        void push_back(T value) {
            if (_cnt == _capacity) {
                int new_capacity = std::max(1, 2 * _capacity);
                // resize
                if (using_heap()) {
                    auto new_buffer = _alloc.allocate(new_capacity);
                    std::copy(_heap_data, _heap_data + _cnt, new_buffer);
                    _alloc.deallocate(_heap_data, _capacity);
                    _heap_data = new_buffer;
                } else if (_capacity == THRES) {
                    // move to heap
                    _heap_data = _alloc.allocate(new_capacity);
                    std::copy(_stack_data.begin(), _stack_data.end(), _heap_data);
                }
                _capacity = new_capacity;
            }
            // Set value
            if (using_heap()) {
                *(_heap_data + _cnt++) = value;
            } else {
                _stack_data[_cnt++] = value;
            }
        }
        void pop_back() {
            if (_cnt == 0) {
                throw std::out_of_range("pop_back");
            }
            --_cnt;
            // TODO: call destructor if templated version
        }

        int* data() {
            if (using_heap()) {
                return _heap_data;
            }
            return _stack_data.data();
        }
        class iterator {
            T *ptr;
        public:
           iterator(T *ptr_) : ptr(ptr_) {}
            T& operator*() {return *ptr;}
            iterator& operator++() {++ptr; return *this;}
            iterator operator++(int) {iterator tmp = *this; ptr++; return tmp;}


        };
        iterator begin() {
            if (using_heap()) {
                return iterator(_heap_data);
            }
            return iterator(_stack_data.begin());
        }
        iterator end() {
            if (using_heap()) {
                return iterator(_heap_data + _cnt);
            }
            return iterator(_stack_data.end());
        }
    };
}