#include "libmm.h"

/*Private global variables*/
/*Pointer the begining of our memory heap*/
PRIVATE_CHAR_ Memory_heap = NULL;
/*Memory break pointer */
PRIVATE_CHAR_ Memory_brk = NULL;
/*Max legal heap address*/
PRIVATE_CHAR_ Memory_max_addr= NULL;

void *Malloc(size_t size){
    //store the output
    void *p = NULL;
    //in case of failure
    if((p = malloc(size)) == NULL){
        perror("Malloc wraper error");
    }
    return p;
}

void* Realloc(void *ptr, size_t size){
    //store the output
    void *p = NULL;
    if((p = realloc(ptr, size))){
        perror("Realloc wraper error");
    }
    return p;
}

void* Calloc(size_t nmemb, size_t size){
    //store the output
    void *p = NULL;
    if((p = calloc(nmemb, size))){
        perror("Realloc wraper error");
    }
    return p;
}

void Memory_inti(void){
    //Allocate max memory
    Memory_heap = (char* )Malloc(MAX_HEAP);
    //Set the brk pointer to the begining
    Memory_brk = (char *)Memory_heap;
    //Set the max addr pointer to the end of the heap
    Memory_max_addr = (char*)(Memory_heap + MAX_HEAP);
}

void *Memory_sbrk(int incr){
    //Store the old_brk pointer
    char *old_brk = Memory_brk;
    
    if((incr < 0) || ((Memory_brk + incr)>Memory_max_addr)){
        errno = ENOMEM; //cant shrunk or exceed memory
        perror("Error: Run out of memory");
        exit(EXIT_FAILURE); // no error handling in here
    } 

    //Push up the memory break pointer 
    Memory_brk = Memory_brk + incr;
    return (void*)old_brk;
}

size_t Memory_heapsize(){
    //Return the brk - (start of the) heap
    return (size_t)((void*)Memory_brk - (void*)Memory_heap);
}

size_t Memory_pagesize(){
    //Returns the pagesize
    return (size_t)getpagesize();
}

struct rusage Memory_Proccess_Info(){
    //basic info about the proccess
    struct rusage my_usage;
    getrusage(RUSAGE_SELF, &my_usage);
    return my_usage;
}