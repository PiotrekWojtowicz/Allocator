#ifndef MY_ALLOC
#define MY_ALLOC

#include <unistd.h>
#include "libmm.h"

#ifndef PRIVATE_CHAR_
#define PRIVATE_CHAR_ static char *
#endif

#ifndef MY_ALLOC_MACROS_
#define MY_ALLOC_MACROS_

//Macros section
#define WSIZE 4 //Word and header/footer size in bytes
#define DSIZE 8 //Dword size double that and we have aliigment
#define CHUNKSIZE (1<<12) //extend heap with the chunk

#define MAX(x, y) ((x) > (y) ? (x) : (y))
//Pack the size and allocated bit into the word
//to be stored in the header/footer
#define PACK(size, alloc_bit) ((size)|(alloc_bit))

//Read/Write Word
#define GET_WORD(p) (*(unsigned int *)(p))
#define PUT_WORD(p, val) (*(unsigned int *)(p) = (val))

//Read size or allocated bit
//Check notes for why we do this binary operations
#define GET_SIZE(p) (GET_WORD(p)&(~0x7)) 
#define GET_A_BIT(p) (GET_WORD(p)&(0x1))

//Get header/footer address given its block pointer
#define HD_ADDRESS(p) ((char *)(p)-WSIZE)
#define FT_ADDRESS(p) ((char *)(p) + GET_SIZE(HD_ADDRESS(p)) - DSIZE)

//Given the block pointer, computer address of next and previous block
#define NEXT_BLOCK_P(p) ((char*)(p) + GET_SIZE(((char*)(p)-WSIZE)))
#define PREV_BLOCK_P(p) ((char*)(p) - GET_SIZE(((char*)(p) - DSIZE)))

//Get the next block size and allocated bit
//given the block pointer
#define GET_NEXT_SIZE(p) GET_SIZE(HD_ADDRESS(NEXT_BLOCK_P(p)))
#define GET_NEXT_A_BIT(p) GET_A_BIT(HD_ADDRESS(NEXT_BLOCK_P(p)))
//and the previous block size
#define GET_PREV_SIZE(p) GET_SIZE(HD_ADDRESS(PREV_BLOCK_P(p)))
#define GET_PREV_A_BIT(p) GET_A_BIT(HD_ADDRESS(PREV_BLOCK_P(p)))

#endif

PRIVATE_CHAR_ heap_listp = NULL;  /* Pointer to first block */  

//Malloc interface for the user
//only these modules shall be
//accessed by the programmer
//==
extern int Alloc_init(void);
extern void* Alloc_malloc(size_t);
extern void Alloc_free(void *ptr);
//==

//Additional realloc and calloc functions
extern void* Mem_realloc(void *ptr, size_t size);
extern void* Mem_calloc(size_t nmemb, size_t size);
extern void* Mem_checkheap(int);
static void* Mem_coalesce(void *ptr);
static void* Find_fit(size_t size);
static void Place(void *bp, size_t size);

//Methods that appears to be helpful
static void *Extend_heap(size_t words);

#endif