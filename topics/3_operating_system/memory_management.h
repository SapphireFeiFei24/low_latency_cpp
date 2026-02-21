#pragma once

#include <memory>
namespace memory {
    int allocate_memory(std::size_t size) {
        std::allocator<int> alloc;
        int* p = alloc.allocate(size); // allocate uninitialized storage

        printf("construction %zu\n", size);
        for (int i = 0; i < static_cast<int>(size); ++i) {
            *(p + i) = i; // need extra construct and destruct for complicated structure
        }

        printf("testing value: %d\n", *(p+5));

        alloc.deallocate(p, size); // deallocate
        return 0;
    }
}
