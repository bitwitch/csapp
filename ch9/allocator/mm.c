/* 
 * Simple, 32-bit and 64-bit clean allocator based on implicit free
 * lists, first-fit placement, and boundary tag coalescing, as described
 * in the CS:APP3e text. Blocks must be aligned to doubleword (8 byte) 
 * boundaries. Minimum block size is 16 bytes. 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mm.h"
#include "memlib.h"

/*
 * If NEXT_FIT defined use next fit search, else use first-fit search 
 */
#define NEXT_FIT

/* Basic constants and macros */
#define WSIZE       4       /* Word and header/footer size (bytes) */
#define DSIZE       8       /* Double word size (bytes) */
#define CHUNKSIZE  (1<<12)  /* Extend heap by this amount (bytes) */

#define MIN_BLOCK_SIZE 16

#define MAX(x, y) ((x) > (y)? (x) : (y))  

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc)) 
#define PACKP(size, prev_alloc, alloc)  ((size) | (prev_alloc << 1) | (alloc))

/* Read and write a word at address p */
#define GET(p)       (*(unsigned int *)(p))
#define PUT(p, val)  (*(unsigned int *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)       (GET(p) & ~0x7)
#define GET_ALLOC(p)      (GET(p) & 0x1)
#define GET_PREV_ALLOC(p) (GET(p) & 0x2)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)       ((char *)(bp) - WSIZE)
#define FTRP(bp)       ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)  ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

/* Global variables */
static char *heap_listp = 0;  /* Pointer to first block */  
#ifdef NEXT_FIT
static char *rover;           /* Next fit rover */
#endif

/* Function prototypes for internal helper routines */
static void *extend_heap(size_t words);
static void place(void *bp, size_t asize);
static void *find_fit(size_t asize);
static void *coalesce(void *bp);
static void printblock(void *bp); 
static void checkheap(int verbose);
static void checkblock(void *bp);

/* 
 * mm_init - Initialize the memory manager 
 */
int mm_init(void) 
{
    heap_listp = mem_sbrk(3*WSIZE);

    if (heap_listp == (char *)-1)
        return -1;

    PUT(heap_listp, 0);
    PUT(heap_listp + (1*WSIZE), PACK(2*WSIZE, 1, 1)); // prologue header
    PUT(heap_listp + (2*WSIZE), PACK(0, 1, 1));        // epilogue header
    
    heap_listp += 2*WSIZE;

#ifdef NEXT_FIT
    rover = heap_listp;
#endif

    if(!extend_heap(CHUNKSIZE/WSIZE))
        return -1;
}

/* 
 * mm_malloc - Allocate a block with at least size bytes of payload 
 */
void *mm_malloc(size_t size) 
{
    if (size == 0)
        return NULL;

    // adjust size to adhere to alignment and size requirements
    if (size <= MIN_BLOCK_SIZE - DSIZE)
        size = MIN_BLOCK_SIZE;
    else {
        size += DSIZE;
        if (size % DSIZE != 0)
            size += DSIZE - (size % DSIZE);
    }

    // find a free block of this size
    void *bp = find_fit(size);

    if (!bp) {
        // request more heap memory
        bp = extend_heap(CHUNKSIZE/WSIZE);
        if (!bp)
            return NULL;
    }

    // allocate this block and potentially split it into a new free block
    place(bp, size);

    return bp;
} 

/* 
 * mm_free - Free a block 
 */
void mm_free(void *bp)
{
    // mark the header and footer as not allocated
    unsigned int size = GET_SIZE(HDRP(bp));
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    coalesce(bp);

    // mark as not allocated in the next block 
}

/*
 * coalesce - Boundary tag coalescing. Return ptr to coalesced block
 */
static void *coalesce(void *bp) 
{
    int prev_alloc = GET_PREV_ALLOC(HDRP(bp));
    int next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    unsigned int current_size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc) {            /* Case 1 */
        return bp;
    }

    else if (prev_alloc && !next_alloc) {      /* Case 2 */
        unsigned int next_size = GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(current_size + next_size, prev_alloc, 0));
        PUT(FTRP(bp), PACK(current_size + next_size, prev_alloc, 0));
    }

    else if (!prev_alloc && next_alloc) {      /* Case 3 */
        void *prev_bp = PREV_BLKP(bp);
        unsigned int prev_size = GET_SIZE(HDRP(prev_bp));
        int prev_prev_alloc = GET_PREV_ALLOC(HDRP(prev_bp));
        PUT(HDRP(prev_bp), PACK(prev_size + current_size, prev_prev_alloc, 0));
        PUT(FTRP(prev_bp), PACK(prev_size + current_size, prev_prev_alloc, 0));
        bp = prev_bp;
    }

    else {                                     /* Case 4 */
        void *prev_bp = PREV_BLKP(bp);
        unsigned int prev_size = GET_SIZE(HDRP(prev_bp));
        unsigned int next_size = GET_SIZE(HDRP(NEXT_BLKP(bp)));
        int prev_prev_alloc = GET_PREV_ALLOC(HDRP(prev_bp));
        PUT(HDRP(prev_bp), PACK(prev_size + current_size + next_size, prev_prev_alloc, 0));
        PUT(FTRP(prev_bp), PACK(prev_size + current_size + next_size, prev_prev_alloc, 0));
        bp = prev_bp;
    }

#ifdef NEXT_FIT
    /* Make sure the rover isn't pointing into the free block */
    /* that we just coalesced */
    if ((rover > (char *)bp) && (rover < NEXT_BLKP(bp))) 
        rover = bp;
#endif

    return bp;
}

/*
 * mm_realloc - Naive implementation of realloc
 */
void *mm_realloc(void *ptr, size_t size)
{

}

/* 
 * mm_checkheap - Check the heap for correctness
 */
void mm_checkheap(int verbose)  
{ 
    checkheap(verbose);
}

/* 
 * The remaining routines are internal helper routines 
 */

/* 
 * extend_heap - Extend heap with free block and return its block pointer
 */
static void *extend_heap(size_t words) 
{
    size_t aligned_size = words % 2 ? (words+1) * WSIZE : words * WSIZE;

    void *free_block = mem_sbrk(aligned_size);

    if (free_block == (void *)-1)
        return NULL;

    int prev_alloc = GET_PREV_ALLOC(HDRP(free_block))

    PUT(HDRP(free_block), PACK(aligned_size, prev_alloc, 0));
    PUT(FTRP(free_block), PACK(aligned_size, prev_alloc, 0));

    // create new epilogue
    PUT(HDRP(NEXT_BLKP(free_block)), PACK(0, 0, 1));

    return coalesce(free_block);
}


/* 
 * find_fit - Find a fit for a block with asize bytes 
 */
static void *find_fit(size_t asize)
{
#ifdef NEXT_FIT

    char *old_rover = rover;
    unsigned block_size; 
    int allocated;

    /* search from rover to end of free list */
    while (1) {
        block_size = GET_SIZE(HDRP(rover));
        allocated = GET_ALLOC(HDRP(rover));
        if (allocated && block_size == 0)
            break;
        if (!allocated && block_size >= asize)
            return rover;
        rover = NEXT_BLKP(rover);
    }

    /* search from beginning of free list to old_rover */
    rover = heap_listp;
    while (1) {
        if (rover >= old_rover)
            return NULL;
        block_size = GET_SIZE(HDRP(rover));
        allocated = GET_ALLOC(HDRP(rover));
        if (!allocated && block_size >= asize)
            return rover;
        rover = NEXT_BLKP(rover);
    }

#else 

    void *bp = heap_listp;
    unsigned block_size; 
    int allocated;

    while (1) {
        block_size = GET_SIZE(HDRP(bp));
        allocated = GET_ALLOC(HDRP(bp));
        if (allocated && block_size == 0)
            return NULL;
        if (!allocated && block_size >= asize)
            return bp;
        bp = NEXT_BLKP(bp);
    }

#endif
}


/* 
 * place - Place block of asize bytes at start of free block bp 
 *         and split if remainder would be at least minimum block size
 */
static void place(void *bp, size_t asize)
{
    size_t free_block_size = GET_SIZE(HDRP(bp));
    size_t remainder = free_block_size - asize;
    int prev_alloc = GET_PREV_ALLOC(HDRP(bp));

    if (remainder >= MIN_BLOCK_SIZE) {
        // allocate part of the block
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
#ifdef NEXT_FIT
        rover = NEXT_BLKP(bp);
#endif

        // split remainder into a new free block
        char *split_block = (char *)bp + asize;
        PUT(HDRP(split_block), PACK(remainder, 0));
        PUT(FTRP(split_block), PACK(remainder, 0));
    } else {
        // allocate the entire free block
        PUT(HDRP(bp), PACK(free_block_size, 1));
        PUT(FTRP(bp), PACK(free_block_size, 1));
#ifdef NEXT_FIT
        rover = NEXT_BLKP(bp);
#endif
    }
}


static void printblock(void *bp) 
{
    size_t hsize, halloc, fsize, falloc;

    /*checkheap(0);*/
    hsize = GET_SIZE(HDRP(bp));
    halloc = GET_ALLOC(HDRP(bp));  
    fsize = GET_SIZE(FTRP(bp));
    falloc = GET_ALLOC(FTRP(bp));  

    if (hsize == 0) {
        printf("%p: EOL\n", bp);
        return;
    }

    printf("%p: header: [%ld:%c] footer: [%ld:%c]\n", bp, 
           hsize, (halloc ? 'a' : 'f'), 
           fsize, (falloc ? 'a' : 'f')); 
}

static void checkblock(void *bp) 
{
    if ((size_t)bp % 8)
        printf("Error: %p is not doubleword aligned\n", bp);
    if (GET(HDRP(bp)) != GET(FTRP(bp)))
        printf("Error: header does not match footer\n");
}

/* 
 * checkheap - Minimal check of the heap for consistency 
 */
void checkheap(int verbose) 
{
    char *bp = heap_listp;

    if (verbose)
        printf("Heap (%p):\n", heap_listp);

    if ((GET_SIZE(HDRP(heap_listp)) != DSIZE) || !GET_ALLOC(HDRP(heap_listp)))
        printf("Bad prologue header\n");
    checkblock(heap_listp);

    for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
        if (verbose) 
            printblock(bp);
        checkblock(bp);
    }

    if (verbose)
        printblock(bp);
    if ((GET_SIZE(HDRP(bp)) != 0) || !(GET_ALLOC(HDRP(bp))))
        printf("Bad epilogue header\n");
}

