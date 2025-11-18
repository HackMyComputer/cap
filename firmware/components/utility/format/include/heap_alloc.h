#ifndef __HEAP_ALLOC_H 
#define __HEAP_ALLOC_H 

#define HEAP_SIZE   16384
#define CHECK_HEAP_ALLOCATION(ptr)  \
    do {                            \
        if ((ptr) == NULL)          \
            abort();                \
    } while (0)         

#endif 
