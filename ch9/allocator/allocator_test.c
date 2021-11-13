#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "memlib.h"
#include "mm.h"

#define NUM_BLOCKS 3000
#define MAX_BLOCK_SIZE 2048


void test_malloc() {
    // c standard malloc

    srand(time(0));

    clock_t start, end;
    double elapsed;
    char *blocks[NUM_BLOCKS];
    char *bp;
    char *heap_start; 
    char *heap_end;
    size_t memory_usage;
    size_t total_requested = 0;

    start = clock();

    heap_start = (char *)sbrk(0);

    for (int i=0; i<NUM_BLOCKS; i++) {
        size_t block_size = (rand() % MAX_BLOCK_SIZE) + 1;
        total_requested += block_size;

        bp = (char *)malloc(block_size);
        if (bp == NULL) {
            printf("malloc failed.\n");
            return;
        }
        blocks[i] = bp;
    }

    /*for (int i = 0; i < NUM_BLOCKS; i += (rand() % 3) + 1) {*/
        /*free(blocks[i]);*/
        /*blocks[i] = NULL;*/
    /*}*/

    /*for (int i = 0; i < NUM_BLOCKS; i++) {*/
        /*if (blocks[i] != NULL)*/
            /*continue;*/
        /*size_t block_size = (rand() % MAX_BLOCK_SIZE) + 1;*/
        /*bp = (char *)malloc(block_size);*/
        /*if (bp == NULL) {*/
            /*printf("malloc failed.\n");*/
            /*return 1;*/
        /*}*/
        /*[>printf("allocated %d bytes at %p\n", BLOCK_SIZE, bp);<]*/
        /*blocks[i] = bp;*/
    /*}*/

    heap_end = (char *)sbrk(0);
    memory_usage = heap_end - heap_start;

    for (int i=0; i<NUM_BLOCKS; i++) {
        free(blocks[i]);
        blocks[i] = NULL;
    }

    end = clock();
    elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("\nC Standard Malloc\n");
    printf("===============================\n");
    printf("Execution time:     %f ms\n", elapsed*1000);
    printf("Memory usage:     %ld bytes\n", memory_usage);
    printf("Total requested:  %ld bytes\n", total_requested);
}

void test_custom_malloc() {
    // custom malloc

    srand(time(0));

    clock_t start, end;
    double elapsed;
    char *blocks[NUM_BLOCKS];
    char *bp;
    char *heap_start; 
    char *heap_end;
    size_t memory_usage;
    size_t total_requested = 0;

    start = clock();

    mem_init();
    mm_init();

    heap_start = mem_heap_lo();

    for (int i=0; i<NUM_BLOCKS; i++) {
        size_t block_size = (rand() % MAX_BLOCK_SIZE) + 1;
        total_requested += block_size;

        bp = (char *)mm_malloc(block_size);
        if (bp == NULL) {
            printf("mm_malloc failed.\n");
            return;
        }
        blocks[i] = bp;
    }

    /*for (int i = 0; i < NUM_BLOCKS; i += (rand() % 3) + 1) {*/
        /*free(blocks[i]);*/
        /*blocks[i] = NULL;*/
    /*}*/

    /*for (int i = 0; i < NUM_BLOCKS; i++) {*/
        /*if (blocks[i] != NULL)*/
            /*continue;*/
        /*size_t block_size = (rand() % MAX_BLOCK_SIZE) + 1;*/
        /*bp = (char *)malloc(block_size);*/
        /*if (bp == NULL) {*/
            /*printf("malloc failed.\n");*/
            /*return 1;*/
        /*}*/
        /*[>printf("allocated %d bytes at %p\n", BLOCK_SIZE, bp);<]*/
        /*blocks[i] = bp;*/
    /*}*/

    heap_end = mem_heap_hi();

    memory_usage = heap_end - heap_start;

    for (int i=0; i<NUM_BLOCKS; i++) {
        mm_free(blocks[i]);
        blocks[i] = NULL;
    }

    end = clock();
    elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("\nCustom Malloc\n");
    printf("===============================\n");
    printf("Execution time:     %f ms\n", elapsed*1000);
    printf("Memory usage:     %ld bytes\n", memory_usage);
    printf("Total requested:  %ld bytes\n", total_requested);
}


int main() {

    test_malloc();
    test_custom_malloc();
    return 0;

    mem_init();
    mm_init();

    printf("Initial heap check:\n");
    mm_checkheap(1);

    char *blocks[NUM_BLOCKS];

    srand(time(0));

    char *bp;
    for (int i=0; i<NUM_BLOCKS; i++) {
        size_t block_size = (rand() % 2048) + 1;

        printf("allocating new block: %ld bytes\n", block_size);

        bp = (char *)mm_malloc(block_size);
        if (bp == NULL) {
            printf("mm_malloc failed.\n");
            return 1;
        }
        /*printf("allocated %d bytes at %p\n", BLOCK_SIZE, bp);*/
        blocks[i] = bp;
    }

    printf("Before freeing check:\n");
    mm_checkheap(1);

    for (int i = 0; i < NUM_BLOCKS; i += (rand() % 3) + 1) {
        mm_free(blocks[i]);
        blocks[i] = NULL;
    }

    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (blocks[i] != NULL)
            continue;
        size_t block_size = (rand() % 2048) + 1;
        bp = (char *)mm_malloc(block_size);
        if (bp == NULL) {
            printf("mm_malloc failed.\n");
            return 1;
        }
        /*printf("allocated %d bytes at %p\n", BLOCK_SIZE, bp);*/
        blocks[i] = bp;
    }

    printf("Post heap check:\n");
    mm_checkheap(1);

    return 0;
}


