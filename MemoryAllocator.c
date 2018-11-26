//
// Created by ahmad on 26/11/2018.
//

#include "MemoryAllocator.h"
#include <stdlib.h>

MemoryAllocator *MemoryAllocator_init(void *memoryPool, size_t size) {
    MemoryAllocator *memAllocator;
    memAllocator = (MemoryAllocator *) malloc(sizeof(MemoryAllocator));
    memAllocator->size = size;
    memAllocator->memoryPool = memoryPool;
    return memAllocator;
}

void *MemoryAllocator_release(MemoryAllocator *allocator) {
    size_t current_size = 0;
    while (current_size <= allocator->size) {

        unsigned char *current_block = (unsigned char *) allocator->memoryPool + current_size;
        if (*(current_block) % 2 == 0) {
            *current_block += 1;
        }
        current_size += *current_block - 1;
    }
    return allocator->memoryPool;
}

void *MemoryAllocator_allocate(MemoryAllocator *allocator, size_t size) {
    size_t current_size = 0, valid = 0;
    unsigned char *current_block ;
    while (current_size <= allocator->size) {
        current_block = (unsigned char *) allocator->memoryPool + current_size;
        if (*(current_block) % 2 == 1) {
            if(*current_block >= size){
                unsigned char* the_rest = (current_block + size);
                *current_block = size;
                *the_rest = 0;
                return allocator->memoryPool;

            }
        }
    }
    return NULL;
    //ronny 3zebe
}

void MemoryAllocator_free(MemoryAllocator *allocator, void *ptr) {
    unsigned char* soso=(unsigned char*)ptr;
    unsigned char* tmp_p=(unsigned char*)ptr + *soso;
    if((*tmp_p)%2==1)
        *soso += *tmp_p-1;

    //allocator usage batata

}

size_t MemoryAllocator_optimize(MemoryAllocator *allocator) {
    return 0;
}
