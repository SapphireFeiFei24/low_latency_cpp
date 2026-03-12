#pragma once
#include <vector>

namespace memory_pool {
    /**
     * @brief Continuous block. Alloc Dealloc in Seq
     * @tparam T
     * @tparam N
     */
    template<typename T, size_t N>
    class MemoryPool {
    private:
        alignas(T) char buffer[N * sizeof(T)];
        std::vector<T*> free_list;

    public:
        MemoryPool() {
            for (size_t i = 0; i < N; i++) {
                free_list.push_back(reinterpret_cast<T*>(buffer + i * sizeof(T)));
            }
        }

        template<typename... Args>
        T* allocate(Args&&... args) {
            if (free_list.empty()) return nullptr;

            T* ptr = free_list.back();
            free_list.pop_back();

            // constructed obj in the pre allocated memory
            // new(ptr): placement new
            return new(ptr) T(std::forward<Args>(args)...); // placement new
        }

        void deallocate(T* ptr) {
            ptr->~T();
            free_list.push_back(ptr);
        }
    };

    /**
     * @brief Mempool that supports random allocate and random free
     */

    template<typename T>
    class FreeListPool {
    private:

        union Node {
            T object;
            Node* next;
        };

        Node* memory;
        Node* free_list;
        size_t capacity;

    public:

        FreeListPool(size_t n) : capacity(n) {

            memory = static_cast<Node*>(::operator new(sizeof(Node) * capacity));

            free_list = &memory[0];

            for (size_t i = 0; i < capacity - 1; ++i) {
                memory[i].next = &memory[i + 1];
            }

            memory[capacity - 1].next = nullptr;
        }

        ~FreeListPool() {
            ::operator delete(memory);
        }

        template<typename... Args>
        T* allocate(Args&&... args) {

            if (!free_list)
                throw std::bad_alloc();

            Node* node = free_list;
            free_list = free_list->next;

            return new (&node->object) T(std::forward<Args>(args)...);
        }

        void deallocate(T* obj) {

            obj->~T(); // must call it explicitely

            Node* node = reinterpret_cast<Node*>(obj);

            node->next = free_list;
            free_list = node;
        }
    };
}