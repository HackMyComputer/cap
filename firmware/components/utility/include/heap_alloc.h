#ifndef __HEAP_ALLOC_H 
#define __HEAP_ALLOC_H 

#define CHECK_HEAP_ALLOCATION(ptr)  \ 
    do {                            \    
        if (ptr == NULL) {          \
            abort();                \
        }                           \
                                    \
    } while (0)                     
        
#endif // __HEAP_ALLOC_H
