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

    /*
    in-dev
    */


}