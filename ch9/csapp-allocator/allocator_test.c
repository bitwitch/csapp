#include "memlib.h"
#include "mm.h"
#include <stdio.h>

#define NUM_BLOCKS 1000
#define BLOCK_SIZE 30000

int main() {
    mem_init();
    mm_init();

    char *blocks[NUM_BLOCKS];

    char *bp;
    for (int i=0; i<NUM_BLOCKS; i++) {
        bp = (char *)mm_malloc(BLOCK_SIZE);
        if (bp == NULL) {
            printf("mm_malloc failed.\n");
            return 1;
        }
        printf("allocated %d bytes at %p\n", BLOCK_SIZE, bp);
        blocks[i] = bp;
    }

    for (int i=0; i<NUM_BLOCKS; i++) {
        mm_free(blocks[i]);
    }

    return 0;
}


