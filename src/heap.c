#include "internal.h"
#include "my_alloc.h"

static union {
    max_align_t alignment;
    unsigned char bytes[HEAP_SIZE];
} heap_storage;

unsigned char *heap_base(void) {
    return heap_storage.bytes;
}

void heap_init(void) {

    block_t *first = (block_t *)heap_base();

    first->size     = HEAP_SIZE - HEADER_SIZE;
    first->next     = NULL;
    first->magic    = BLOCK_MAGIC;
    first->is_free  = true;

}
