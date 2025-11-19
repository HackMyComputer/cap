#ifndef __GNUC__ 
# define __GNUC__ 

#endif // __GNUC__ 

#include <stdint.h>
#include <unistd.h> 
#include <stdbool.h>

#include "esp_log.h"
#include "esp_err.h"

#ifndef _LOG_TAG_SPECIFIER 
#define _LOG_TAG_SPECIFIER TAG 
#endif 

/*const char valid_format_specifier[] = {
    "text/txt", 
    "text/html",
    "stream/flac",
    "stream/mp3",
    "stream/aac", 
    "stream/mp4"
};

typedef struct {
    int64_t *m_data; 
    int64_t m_size; 
    char *filename; 
    char *ftype; 
} data_fmt_t;

static bool analyze_mdata(data_fmt_t *fmt) {*/
    /* Return if the specified file contains valid metadata 
     * according to its format specifier 
     */ 
/*
    return __is_valid_mdata(fmt->m_data);
}

static void find_format(const data_fmt_t *fmt) {
    char fname[256]; // 256 is the max character allowed for a file name; 

    if (fmt->ftype != NULL) {
        fmt->ftype = NULL; 
    }


}*/ 
