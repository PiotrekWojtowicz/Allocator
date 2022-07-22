#ifndef MY_MM_ALLOC
#define MY_MM_ALLOC
#include "libmem.h"
#include <unistd.h>
#include <sys/resource.h>
/*Define gobal static char pointer
This ensures us that the range of the
variable will not exceed the current file*/
#ifndef PRIVATE_CHAR_
#define PRIVATE_CHAR_ static char *
#endif

/*Private global variables*/
/*Pointer the begining of our memory heap*/
PRIVATE_CHAR_ Memory_heap;
/*Memory break pointer */
PRIVATE_CHAR_ Memory_brk;
/*Max legal heap address*/
PRIVATE_CHAR_ Memory_max_addr;

/*
Why do we even need this interface?
The interface allows us to interleave calls
from our malloc package with the system's
malloc.
*/
/*Malloc wraper*/
void* Malloc(size_t size);
/*Initialize the memory system*/
void Memory_ini(void);
/*Basic implementation of Sbrk, extending the heap*/
void *Memory_sbrk(int incr);
/*Two additional functions*/
size_t Memory_heapsize();
size_t Memory_pagesize();
struct rusage Memory_Proccess_Info();

#endif