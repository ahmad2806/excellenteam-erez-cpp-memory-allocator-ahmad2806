## excellenteam-erez-cpp-memory-allocator-ahmad2806
## Memory allocation service

The goal of this exercise is to create memory allocator in C which is using a big chunk of memory pool which is set at initialization.

## Algorithm
### 1.Each block keeps its size and indication whether it is free or occupied in a compressed manner (LSB).
### 2.When allocation is requested, I find the first freeblock and do the following:-
Check if it is big enough. If it is, and it is the exact samesize, mark it as allocated and return it. 
Otherwise, split it to the requested size + the rest, and mark the first block as allocated and the second as free.

-If it is not big enough, check if the next block is free. If it is, merge them and check the size again.
-If it is not big enough, look for the next free block.-If you reached the end of the memory pool, return NULL.

## Restrictions:
I Assumed that the memory pool is aligned to the platform-specific alignment width (like any allocated memory provided by malloc).Each allocation must be aligned in the same manner.The metadata holding the size and the free bit indication should be aligned as well.
Thus, if I asked to allocate 33 bytes and my platform alignment width is 8 bytes, then I will have a total of 48-bytesblock (8 for the metadata, and 40 for the actual allocated data).
