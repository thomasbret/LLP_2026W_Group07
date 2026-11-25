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

#endif /* INTERNAL_H */