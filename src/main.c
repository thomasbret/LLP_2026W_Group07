#include <stdio.h>
#include "my_alloc.h"


int main() {
    heap_init();
    int status = heap_check();
    printf("Return status of heap_check() is %d.\n", status);
    return 0;
}