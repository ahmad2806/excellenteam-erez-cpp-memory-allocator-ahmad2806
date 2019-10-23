//
// Created by ahmad on 26/11/2018.
//


#include "MemoryAllocator.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


size_t alignment_needed_for(size_t size) {
    //size was multiplied by -1 and then calculate the %, because I needed the complement of the % to add
    return (size * -1) % ALIGNMENT;
}

MemoryAllocator *MemoryAllocator_init(void *memoryPool, size_t size) {//change size
    MemoryAllocator *memAllocator;
//    assert(memoryPool && size);
    memAllocator = malloc(sizeof(MemoryAllocator));

    if (memAllocator) {

        //memAllocator->size = ((size - (size % ALIGNMENT)) - sizeof(size_t)) | (unsigned) ISFREE;// 47 -7 = 40 - 8 (meta) = 32 | 1 (free bit)= 33

        // 37 + (3) = 40 + 8 (meta) = 48 | 1 (free bit)= 49,
        memAllocator->size = (size + alignment_needed_for(size) + sizeof(size_t)) | (unsigned) ISFREE;

        memAllocator->memoryPool = memoryPool;

        //putting at the beginning of the pool the meta of the size
        *((size_t *) memAllocator->memoryPool) = memAllocator->size;
    }

    return memAllocator;
}

char is_free(const size_t *pBlock) {
    return 1 == ((*pBlock) & (size_t) ISFREE);
}

int try_to_allocate(size_t *pCurrentBlock, size_t size) {
//    printf("\nfirst %zu \n", *pCurrentBlock); // for testing
    if (is_free(pCurrentBlock)) {  //check if block is free

        // space is enough for my new block
        // -1 to remove the bit that indicate that free block

        if (--(*pCurrentBlock) >= size) {

            if (*pCurrentBlock != size) {
                // getting the rest to start slice
                size_t *pTheRest = (size_t *) (void *) ((char *) pCurrentBlock + size);
                // adding 1 to indicate that its free space
                *pTheRest = *pCurrentBlock - size + 1;
            }
            *pCurrentBlock = size; // meta-data does not have 1
//            printf("second %zu   %zu\n", *pCurrentBlock, *pTheRest);// for testing
            return 1;
        } else {
            // space is not enough for my new block
            // adding 1 to indicate that its free space
            ++(*pCurrentBlock);
        }
    }
    return 0;
}

void *MemoryAllocator_allocate(MemoryAllocator *allocator, size_t size) {

    size = size + alignment_needed_for(size) + sizeof(size); // 10-> 10 + 6 + 8 (meta) = 24
    size_t *pCurrentBlock = (size_t *) allocator->memoryPool;

//    I casted the memoryPool which contain the address of the pool to char*, because when using the arithmetic of
//    pointers (type* + 5) will increment 5 * sizeof(type), and since size of char is 1 then we still on the right
//    track
    size_t *poolEdge = (size_t *) (void *) ((char *) allocator->memoryPool + allocator->size);

    size_t stepsMove = 0;
    while (pCurrentBlock + 1 < poolEdge) {

        if (try_to_allocate(pCurrentBlock, size)) {
            return allocator->memoryPool;
        }

        stepsMove = *pCurrentBlock;
        if (is_free(pCurrentBlock))
            // this means that the block was free and too small to fit the new block, that's why
            // we should move steps - 1 because in that number there is an extra one indication the free block
            --stepsMove;
//      when casting pCurrentBlock to char*, we still look at old location of pCurrentBlock
//      (we casted it so we can move x bytes forward without making magic formula to davide that size
//      [currentSize in our case]), re cast it to void* before to its origin type so we make sure not to lose data
//      while converting from char* to size_t*
        pCurrentBlock = (size_t *) ((void *) ((char *) pCurrentBlock + stepsMove));
    }
//    printf("space is empty\n"); // for test
    return NULL;
}


void *MemoryAllocator_release(MemoryAllocator *allocator) {
    if (allocator) {
        void *memPool = allocator->memoryPool;
        free(allocator);
        return memPool;
    }
    return NULL;
}


void MemoryAllocator_free(MemoryAllocator *allocator, void *ptr) {
    size_t *poolEdge = (size_t *) (void *) ((char *) allocator->memoryPool + allocator->size);
    size_t blockSize = *((size_t *) ptr);
    if (blockSize <= 0) // if the size of that block is less than 0 then it is a wrong block
        exit(-1);
    // getting the next block to check if it is free
    size_t *nextBlock = ((size_t *) (void *) ((char *) ptr + blockSize));

//    printf("block before%zu\n", *(size_t*)ptr); // for test
//    printf("block %zu\n", *nextBlock); // for test

    if (nextBlock > poolEdge) {
        exit(-1);
    } else if (nextBlock == poolEdge) {
        *((size_t *) ptr) += 1;
    } else {
//        checking if the next block is free, and if so, we add the two blocks, else we just indicate
//        the current block as free
        if (1 == (*nextBlock & (size_t) ISFREE)) {
            *((size_t *) ptr) = *nextBlock + blockSize;
        } else {
            *((size_t *) ptr) += 1;
        }
    }
}

size_t merge_blocks(size_t *pCurrent, const size_t *pNext) {
    printf("merging %zu with %zu\n", *pCurrent, *pNext);
//    no need to change the next block
    *pCurrent += (*pNext) - 1;
    return *pCurrent;
}

size_t *get_next_block(size_t *pCurrentBlock) {
    size_t steps = *pCurrentBlock;
    if (is_free(pCurrentBlock)) {
        --steps;
    }
    return (size_t *) (((char *) (void *) pCurrentBlock) + steps);
}

size_t MemoryAllocator_optimize(MemoryAllocator *allocator) {

    if (!allocator)
        return 0;
    size_t *poolEdge = (size_t *) (void *) ((char *) allocator->memoryPool + allocator->size);
    size_t *pCurrentBlock = allocator->memoryPool;
    size_t *pNextBlock;
    size_t maxFreeBlock = 0;

    size_t newSize = 0;
    while (pCurrentBlock < poolEdge) {
//        printf(" block size %zu  \n", *pCurrentBlock); // testing
        if (is_free(pCurrentBlock)) {
            pNextBlock = get_next_block(pCurrentBlock);

            if (pNextBlock + 1 >= poolEdge) { // check if last block
                if (*pCurrentBlock > maxFreeBlock) // update max if necessary and break
                    maxFreeBlock = newSize;
                break;
            }

            if (is_free(pNextBlock)) {
                // if the next is free so merge and get new size
                size_t sizeAfterMerge = merge_blocks(pCurrentBlock, pNextBlock);
                newSize = sizeAfterMerge;
            } else {
                newSize = *pNextBlock;
            }
            if (newSize > maxFreeBlock)//update max
                maxFreeBlock = newSize;
        }
        pCurrentBlock = get_next_block(pCurrentBlock);
    }
    return maxFreeBlock;
}

