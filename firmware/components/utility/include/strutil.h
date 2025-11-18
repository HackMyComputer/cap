#ifndef __STRUTIL_H 
#define __STRUTIL_H 


#if !defined __always_inline && defined __GNUC__ 
# define __always_inline    __attribute__((always_inline))
#endif // __always_inline 
    
#include <inttypes.h> 

__always_inline int64_t _strlen () {
    return 0; 
}

__always_inline char* makesubstr(const char *restrict __str, int __starter, int __end) {
    assert(__starter < __end ); 

    int i; 
    char *tmp;

    tmp = malloc((__end - __starter + 1) * sizeof(char));
    CHECK_HEAP_ALLOCATION(tmp);
    for( i = __starter; i < __end; ++i ) {
       *tmp++ = __str[i];  
    }
    
    return tmp;
    // Should be freed here
}

#endif // __STRUTIL_H
