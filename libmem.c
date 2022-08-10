#include "libmem.h"

int Alloc_init(void){

    //Try to allocate 4 WORD in memory
    /*
    Aligment  | Prologue header |  P. Footer  | Epilogue header
    */
    if((heap_listp = Memory_sbrk(4*WSIZE)) == (void*)-1)
        return -1;

    PUT_WORD(heap_listp, 0); //Aligment
    PUT_WORD(heap_listp +(1*WSIZE), PACK(DSIZE, 1)); //Prologue header
    PUT_WORD(heap_listp +(2*WSIZE), PACK(DSIZE, 1));//Footer
    PUT_WORD(heap_listp +(3*WSIZE), PACK(0,1));
    //Set the heap list pointer in the between of prologue header and footer
    heap_listp += (2*WSIZE);

    //try to extend heap 
    if(extended_heap(CHUNKSIZE/WSIZE) == NULL)
        return -1;

    return 0;
}

static void *Extend_heap(size_t words){
    //store the memory break pointer
    char *bp;
    //Size of new allocation
    size_t size;

    /*
    Allocate an even number of words 
    */
    size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
    if((long)(bp = Memory_sbrk(size)) == -1)
        return NULL;

    PUT(HD_ADDRESS(bp), PACK(size, 0));
    PUT(FT_ADDRESS(bp), PACK(size, 0)); 
    PUT(HD_ADDRESS(NEXT_BLOCK_P(bp)), PACK(0,1));

    return coalesce(bp);
}

void Alloc_free(void *ptr){
    size_t size = GET_SIZE(HD_ADDRESS(ptr));

    /*
    * Set headers and footers to indicate
    * the block is not allocated
    */
    PUT(HD_ADDRESS(ptr), PACK(size, 0));
    PUT(FT_ADDRESS(ptr), PACK(size, 0));
    //check for possible coalesce
    coalesce(ptr);
}

static void* Mem_coalesce(void *ptr){
    /*
    *  Store data about nearby blocks
    */
    size_t prev_alloc = GET_A_BIT(FT_ADDRESS(PREV_BLOCK_P(ptr)));
    size_t next_alloc = GET_A_BIT(HD_ADDRESS(NEXT_BLOCK_P(ptr)));
    //Get current size
    size_t size = GET_SIZE(HD_ADDRESS(ptr));

    /*
    *  As we can coalesce previous block
    *  or next one, we have 4 cases
    *  1. nothing to do
    *  2. coalesce with next block
    *  3. coalesce with previous block
    *  4. 2 and 3
    */
    if(prev_alloc && next_alloc){   // 1.
        return ptr;
    }
    else if(prev_alloc && !next_alloc) // 2.
    {
        size += GET_SIZE(HD_ADDRESS(NEXT_BLOCK_P(ptr)));
        PUT(HD_ADDRESS(ptr), PACK(size, 0));
        PUT(FT_ADDRESS(ptr), PACK(size, 0));
    }
    else if( !prev_alloc && next_alloc) // 3.
    {
        size += GET_SIZE(HD_ADDRESS(PREV_BLOCK_P(ptr)));
        PUT(FT_ADDRESS(ptr), PACK(size, 0));
        PUT(HD_ADDRESS(PREV_BLOCK_P(ptr)), PACK(size, 0));
        //set the ptr to head of previous block
        ptr = PREV_BLOCK_P(ptr);
    }
    else{ // 4.
        size+=GET_SIZE(HD_ADDRESS(PREV_BLOCK_P(ptr))) + GET_SIZE(FT_ADDRESS(NEXT_BLOCK_P(ptr)));
        PUT(HD_ADDRESS(PREV_BLOCK_P(ptr)), PACK(size, 0));
        PUT(FT_ADDRESS(NEXT_BLOCK_P(ptr)), PACK(size, 0));
        ptr = PREV_BLOCK_P(ptr);
    }

    return ptr;
}

static void *Find_fit(size_t size){
    void *temp_ptr;
    
    for(temp_ptr = heap_listp; GET_SIZE(HD_ADDRESS(temp_ptr)) > 0; temp_ptr = NEXT_BLOCK_P(temp_ptr)){
        //if block is not allocated && size is equal or less than
        //the block is fine
        if(!GET_A_BIT(HD_ADDRESS(temp_ptr)) && (size <= GET_SIZE(HD_ADDRESS(temp_ptr))))
            return temp_ptr;
    }
    return NULL;
}

static void Place(void *bp, size_t size){
    size_t csize = GET_SIZE(HD_ADDRESS(bp));

    if((csize - size) >= (2*DSIZE)){
        PUT(HD_ADDRESS(bp), PACK(size, 1));
        PUT(FT_ADDRESS(bp), PACK(size, 1));
        bp = NEXT_BLOCK_P(bp);
        PUT(HD_ADDRESS(bp), PACK(csize - size, 1));
        PUT(FT_ADDRESS(bp), PACK(csize - size, 1));
    }
    else{
        PUT(HD_ADDRESS(bp), PACK(csize, 1));
        PUT(FT_ADDRESS(bp), PACK(csize, 1));
    }
}

void *Alloc_malloc(size_t size){
    size_t asize; // Adjusted block size
    size_t extendsize; //if no fit
    char* bp;

    if(size == 0)
        return NULL;

    if(size <= DSIZE){
        asize = 2*DSIZE;
    }
    else
        asize = DSIZE * ((size +(DSIZE) + (DSIZE - 1)) / DSIZE);

    //search for free size
    if((bp = Find_fit(asize))!=NULL){
        Place(bp, asize);
        return bp;
    }

    extendsize = MAX(asize, CHUNKSIZE);
    if((bp = Extended_heap(extendsize/WSIZE)) == NULL)
        return NULL;

        Place(bp, asize);
        return bp;
}