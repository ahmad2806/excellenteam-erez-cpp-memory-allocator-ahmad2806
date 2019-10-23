#include <stdio.h>
#include "MemoryAllocator.h"


void check_allocate_memory(MemoryAllocator *mA) {
//    uncomment the print to see the results
    MemoryAllocator_allocate(mA, 10);
    MemoryAllocator_allocate(mA, 20);
    MemoryAllocator_allocate(mA, 30);
    MemoryAllocator_allocate(mA, 40);
    MemoryAllocator_allocate(mA, 500);
    MemoryAllocator_allocate(mA, 500);
}

void check_free_memory(MemoryAllocator *mA) {
    //    uncomment the print to see the results
    MemoryAllocator_allocate(mA, 10);
    MemoryAllocator_allocate(mA, 20);
    MemoryAllocator_free(mA, mA->memoryPool);
}

void check_optimize_memory(MemoryAllocator *mA) {
    //    uncomment the print to see the results
    MemoryAllocator_allocate(mA, 10);
    MemoryAllocator_allocate(mA, 20);
    MemoryAllocator_allocate(mA, 30);
    MemoryAllocator_allocate(mA, 40);
    MemoryAllocator_allocate(mA, 66);

    MemoryAllocator_free(mA, mA->memoryPool + 24);
    MemoryAllocator_free(mA, mA->memoryPool + 24 + 32);
    MemoryAllocator_allocate(mA, 64);
    printf("maxBlock = %zu \n", MemoryAllocator_optimize(mA));
}

int main() {


    size_t size = 800;
    void *memoryPool = NULL;
    memoryPool = malloc(size + sizeof(size)); // allocating memory



    MemoryAllocator *mA = MemoryAllocator_init(memoryPool, size);
    check_allocate_memory(mA);
    check_free_memory(mA);
    check_optimize_memory(mA);

    memoryPool = MemoryAllocator_release(mA);
    free(memoryPool);
    return 0;
}



