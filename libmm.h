#ifndef MY_MM_ALLOC
#define MY_MM_ALLOC
/*Headers */
#include "libmem.h"
#include <unistd.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
/*Define gobal static char pointer
This ensures us that the range of the
variable will not exceed the current file*/
#ifndef PRIVATE_CHAR_
#define PRIVATE_CHAR_ static char *
#endif

#ifndef MAX_HEAP
#define MAX_HEAP (20*(1<<20))  /* 20 MB */
#endif

/*
Why do we even need this interface?
The interface allows us to interleave calls
from our malloc package with the system's
malloc.
*/
/*Malloc wraper*/
void* Malloc(size_t size);
void* Realloc(void *ptr, size_t size);
void* Calloc(size_t nmemb, size_t size);
/*Initialize the memory system*/
void Memory_ini(void);
/*Basic implementation of Sbrk, extending the heap*/
void *Memory_sbrk(int incr);
/*Two additional functions*/
size_t Memory_heapsize();
size_t Memory_pagesize();
struct rusage Memory_Proccess_Info();

#endif