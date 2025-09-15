#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>
#include "my_memory.h"
 
my_memory_pool::my_memory_pool(){}

void my_memory_pool::init(size_t blockSize, size_t numBlocks){
    blockSize_ = blockSize;
    numBlocks_ = numBlocks;
    poolSize_ = blockSize_ * numBlocks_;
    used_size = 0;
    pool_ = static_cast<char*>(std::malloc(poolSize_));
    if (!pool_) {
        throw std::bad_alloc();
    }
    for (size_t i = 0; i < numBlocks_; ++i) {
        freeList_.push_back(pool_ + i * blockSize_);
    }
    memset(pool_, 0, poolSize_);
}

void my_memory_pool::clear(){
    used_size = 0;
    freeList_.clear();
    for (size_t i = 0; i < numBlocks_; ++i) {
        freeList_.push_back(pool_ + i * blockSize_);
    }
}

my_memory_pool::~my_memory_pool() {
    std::free(pool_);
}

void* my_memory_pool::allocate() {
    if (freeList_.empty()) {
        throw std::bad_alloc();
    }
    void* block = freeList_.back();
    freeList_.pop_back();
    used_size += 1;
    return block;
}

void my_memory_pool::deallocate(void* block) {
    used_size -= 1;
    freeList_.push_back(static_cast<char*>(block));
}