#ifndef __STRUTIL_H 
#define __STRUTIL_H 

#if !defined __always_inline && defined __GNUC__ 
# define __always_inline    __attribute__((always_inline))
#endif // __always_inline 
    
#include <inttypes.h> 
 
#include "heap_alloc.h"

// Sometimes glibc strlen sucks 
__always_inline int64_t _strlen (const char* __str) {
    int64_t s = 0; 

    while (*__str++) s++; 
    return s; 
}

__always_inline char* make_substr(const char *restrict __str, int __starter, int __end) {
    assert(__starter <= __end ); 

    int i, j; 
    char *tmp;

    tmp = malloc((__end - __starter + 1) * sizeof(char));
    CHECK_HEAP_ALLOCATION(tmp);
    for(j = 0, i = __starter; i < __end; ++i, ++j) {
       tmp[j] = __str[i];  
    }
   
    tmp[j] = '\0';
    return tmp;
    // Should be freed here
}

#endif // __STRUTIL_H
