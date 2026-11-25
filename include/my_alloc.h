#ifndef MY_ALLOC_H
#define MY_ALLOC_H

#include <stddef.h>

/* Result of my_free(). */
typedef enum {
    MY_FREE_OK = 0,   /* block freed */
    MY_FREE_NULL,     /* ptr was NULL, no-op */
    MY_FREE_INVALID,  /* ptr is not a payload start in the heap, no-op */
    MY_FREE_DOUBLE    /* block was already free, no-op */
} my_free_status;

/* Resets the heap to one free block of HEAP_SIZE - HEADER_SIZE bytes.
 * Call before using the allocator. Discards all current allocations. */
void heap_init(void);
 
/* First-fit allocation. Returns an aligned payload pointer, or NULL for
 * size == 0, a request too large to round safely, or no block that fits. */
void *my_alloc(size_t size);
 
/* Validates ptr, marks its block free and coalesces physical neighbours.
 * Never modifies the heap for a rejected pointer. */
my_free_status my_free(void *ptr);
 
/* Prints every block in physical order (index, offset, payload size,
 * FREE/USED) followed by payload-byte totals, block count and the largest
 * free payload. */
void heap_print(void);
 
/* Walks the heap and verifies all invariants. Returns 0 if OK, nonzero
 * otherwise (see heap_check_result in internal.h for the codes). */
int heap_check(void);

#endif /* MY_ALLOC_H */