#pragma once
#include <vector>
#include <cstddef>

class my_memory_pool {
    public:
        my_memory_pool();
        ~my_memory_pool();
        void* allocate();
        void deallocate(void* block);
        void init(size_t blockSize, size_t numBlocks);
        void clear();
        my_memory_pool(const my_memory_pool&) = delete;
        my_memory_pool& operator=(const my_memory_pool&) = delete;
        size_t used_size;
    private:
        size_t blockSize_;
        size_t poolSize_;
        size_t numBlocks_;
        char* pool_;
        std::vector<char*> freeList_;
};
