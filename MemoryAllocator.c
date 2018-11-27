//
// Created by ahmad on 26/11/2018.
//

#include "MemoryAllocator.h"
#include <stdlib.h>


MemoryAllocator *MemoryAllocator_init(void *memoryPool, size_t size) {//change size
    MemoryAllocator *memAllocator;
    memAllocator = (MemoryAllocator *) malloc(sizeof(MemoryAllocator));

    memAllocator->size = size - ((size_t)&(*memAllocator) % ALIGNMENT);
    //if the os already created that chunk
    // !! alignment is already been taken in calculation

    memAllocator->size = (memAllocator->size % ALIGNMENT) + memAllocator->size;
    memAllocator->memoryPool = memoryPool;
    return memAllocator;
}

void *MemoryAllocator_release(MemoryAllocator *allocator) {
    void *return_me_please = allocator->memoryPool;
    free(allocator);
    return return_me_please;

}

void *MemoryAllocator_allocate(MemoryAllocator *allocator, size_t size) {
    size = (size % ALIGNMENT) + size;
    size_t current_size = 0;
    size_t *p_current_block = &current_size;
    while (current_size <= allocator->size) {
        p_current_block = (size_t *) allocator->memoryPool + *p_current_block;
        if (*(p_current_block) % 2 == 1) {
            if (*p_current_block >= size) {
                size_t *the_rest = (p_current_block + size);
                *the_rest = *p_current_block - size;
                *p_current_block = size;
                return allocator->memoryPool;
            }
        }
    }
    return NULL;
}


void MemoryAllocator_free(MemoryAllocator *allocator, void *ptr) {
    size_t *p_fofo;
    p_fofo = (size_t *) ptr;
    size_t *p_kuku;
    p_kuku = (size_t *) ptr + *p_fofo;
    if ((*p_kuku) % 2 == 1)
        *p_fofo += *p_kuku - 1;
    //allocator usage batata
}

size_t MemoryAllocator_optimize(MemoryAllocator *allocator) {
    size_t current_size = 0, biggest_block = 0;
    size_t *p_current_block = &current_size;
    while (current_size <= allocator->size) {
        p_current_block = (size_t *) allocator->memoryPool + *p_current_block;
        if (*(p_current_block) % 2 == 1) {
            MemoryAllocator_free(allocator, p_current_block);
            if (*p_current_block > biggest_block)
                biggest_block = *p_current_block;
        }
    }
    return biggest_block;
}
