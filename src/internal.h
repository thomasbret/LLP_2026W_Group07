#ifndef INTERNAL_H
#define INTERNAL_H


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


#ifndef HEAP_SIZE
#define HEAP_SIZE (1u << 20)
#endif

#define ALIGNMENT _Alignof(max_align_t)
#define ALIGN_UP(n) (((n) + ALIGNMENT - 1) / ALIGNMENT * ALIGNMENT)
#define MIN_PAYLOAD ALIGNMENT
#define HEADER_SIZE ALIGN_UP(sizeof(block_t))
#define BLOCK_MAGIC 0xA110CA7Eu


typedef struct block {
    size_t          size;       /* payload size in bytes, always a multiple of ALIGNMENT */
    struct block    *next;      /* next block in physical order, NULL for the last block */
    uint32_t        magic;      /* BLOCK_MAGIC */
    bool            is_free;    /* true = free, false = used */
} block_t;


_Static_assert(HEAP_SIZE % ALIGNMENT == 0, "HEAP_SIZE must be a multiple of ALIGNMENT");
_Static_assert(HEAP_SIZE > HEADER_SIZE + MIN_PAYLOAD, "heap must fit a header and a useful payload");
_Static_assert(HEADER_SIZE % ALIGNMENT == 0, "header size must preserve payload alignment");


/* Result codes of heap_check(); 0 means all invariants hold. */
typedef enum {
    HEAP_CHECK_OK = 0,
    HEAP_CHECK_TRUNCATED,     /* leftover space too small for a block */
    HEAP_CHECK_BAD_MAGIC,     /* magic overwritten (or heap_init() not called) */
    HEAP_CHECK_BAD_SIZE,      /* size < MIN_PAYLOAD or not aligned */
    HEAP_CHECK_OVERRUN,       /* block extends past the end of the heap */
    HEAP_CHECK_BAD_LINK,      /* next != address right after the payload */
    HEAP_CHECK_ADJACENT_FREE  /* two neighbouring free blocks (not coalesced) */
} heap_check_result;


/* Access to the heap, implemented in heap.c. */
unsigned char *heap_base(void);  /* first byte of the heap */
unsigned char *heap_end(void);   /* one past the last byte */


/* Pointer conversions. Only call block_from_payload() on a pointer that
 * is already known to be a payload start (see my_free validation). */
static inline void *block_payload(block_t *b) {
    return (unsigned char *)b + HEADER_SIZE;
}
 
static inline block_t *block_from_payload(void *payload) {
    return (block_t *)((unsigned char *)payload - HEADER_SIZE);
}
 
/* Address immediately after a block's payload. */
static inline unsigned char *block_end(const block_t *b) {
    return (unsigned char *)b + HEADER_SIZE + b->size;
}


#endif /* INTERNAL_H */