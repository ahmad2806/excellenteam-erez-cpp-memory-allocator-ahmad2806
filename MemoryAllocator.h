//
// Created by ahmad on 26/11/2018.
//

#ifndef MEMORYALLOCATOR_MEMORYALLOCATOR_H
#define MEMORYALLOCATOR_MEMORYALLOCATOR_H

#define ALIGNMENT 8
#define ISFREE 1

#include <stdint.h>
#include <stdlib.h>

typedef struct MemoryAllocator{
    void* memoryPool;
    size_t size;
}MemoryAllocator;

/* memoryPool is a ptr to an already-existing large memory block */
MemoryAllocator* MemoryAllocator_init(void* memoryPool, size_t size);

/* return a ptr to the memoryPool */
void* MemoryAllocator_release(MemoryAllocator* allocator);
void* MemoryAllocator_allocate(MemoryAllocator* allocator, size_t size);


/* Merge the next adjacent block if free */
void MemoryAllocator_free(MemoryAllocator* allocator, void* ptr);


/* Merges all adjacent free blocks, and returns the size of largest free block */
size_t MemoryAllocator_optimize(MemoryAllocator* allocator);

#endif //MEMORYALLOCATOR_MEMORYALLOCATOR_H
