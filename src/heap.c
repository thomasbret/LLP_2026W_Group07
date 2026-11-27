#include "internal.h"
#include "my_alloc.h"


/* The heap itself. The union gives the byte array the alignment of
 * max_align_t without adding any bytes; `alignment` is never accessed. */
static union {
    max_align_t alignment;
    unsigned char bytes[HEAP_SIZE];
} heap_storage;


unsigned char *heap_base(void) {
    return heap_storage.bytes;
}

unsigned char *heap_end(void) {
    return heap_storage.bytes + HEAP_SIZE;
}

void heap_init(void) {

    block_t *first = (block_t *)heap_base();

    first->size     = HEAP_SIZE - HEADER_SIZE;
    first->next     = NULL;
    first->magic    = BLOCK_MAGIC;
    first->is_free  = true;

}

int heap_check(void) {
    
    unsigned char *const end = heap_end();
    unsigned char *cur = heap_base();   /* invariant 1: first block at heap[0] */
    bool prev_free = false;
 
    while (cur != end) {
        size_t remaining = (size_t)(end - cur);
 
        /* Invariant 5: a whole header plus minimal payload must still fit. */
        if (remaining < HEADER_SIZE + MIN_PAYLOAD)
            return HEAP_CHECK_TRUNCATED;
 
        const block_t *b = (const block_t *)cur;
 
        /* Invariant 4 */
        if (b->magic != BLOCK_MAGIC)
            return HEAP_CHECK_BAD_MAGIC;
 
        /* Invariant 2 */
        if (b->size < MIN_PAYLOAD || b->size % ALIGNMENT != 0)
            return HEAP_CHECK_BAD_SIZE;
 
        /* Invariant 5: check the size BEFORE computing the next address. */
        if (b->size > remaining - HEADER_SIZE)
            return HEAP_CHECK_OVERRUN;
 
        unsigned char *next_addr = block_end(b);
 
        /* Invariant 3: last block has next == NULL, others point right
         * behind their payload. */
        if (next_addr == end) {
            if (b->next != NULL)
                return HEAP_CHECK_BAD_LINK;
        } else if (b->next != (const block_t *)next_addr) {
            return HEAP_CHECK_BAD_LINK;
        }
 
        /* Invariant 6: no two adjacent free blocks. */
        if (prev_free && b->is_free)
            return HEAP_CHECK_ADJACENT_FREE;
 
        prev_free = b->is_free;
        cur = next_addr;
    }
 
    /* Invariant 7 holds by construction: every step advanced by exactly
     * HEADER_SIZE + size and the loop ended exactly at heap_end(). */
    return HEAP_CHECK_OK;

}