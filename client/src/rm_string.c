/**
 * @file string.C
 * @brief A string class for use with the library
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#include "rm/string.h"

#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


/**
 * @brief Creates a string with user-defined size
 * 
 * @param str Initial string value
 * @param len Maximum number of characters to be stored
 * 
 * @return A string object
 */
rmString rmCreateString(const char* str, uint8_t len) {
    rmString s;
    s.data = (char*) malloc(len + 1);
    s.data[len] = '\0';
    s.size = len;
    strncpy(s.data, str, len);
    return s;
}


static uint8_t ilen;


char* rm_itoa(int i) {
    static char buffer[12];
    uint8_t n = 10;
    bool neg = false;
    
    if(i < 0) {
        neg = true;
        i = -i;
    }
    
    do {
        buffer[n--] = '0' + (i % 10);
        i /= 10;
    } while(i);
    
    if(neg)
        buffer[n] = '-';
    else
        n++;
    
    buffer[11] = '\0';
    ilen = 11 - n;
    return &buffer[n];
}


static char fbuffer[16];
static uint8_t flen;


char* rm_ftoa(float f) {
    flen = 0;
    if(f < 0) {
        fbuffer[flen++] = '-';
        f = -f;
    }
    if(f > 4294967295 || isnan(f)) {
        memcpy(&fbuffer[flen], "nan", 3);
        flen += 3;
    }
    else {
        uint32_t d = (uint32_t) f;
        uint16_t rem = (uint16_t) ((f - d + 0.0005f) * 1000);
        char* ptr = rm_itoa(d);
        memcpy(&fbuffer[flen], ptr, ilen);
        flen += ilen;
        fbuffer[flen++] = '.';
        for(uint8_t i=3; i--; ) {
            fbuffer[flen + i] = '0' + (rem % 10);
            rem /= 10;
        }
        flen += 3;
    }
    fbuffer[flen] = '\0';
    return fbuffer;
}


int rm_sprintf(char* buf, const char* fmt, va_list va) {
    uint8_t len = 0;
    const char* fend = fmt + strlen(fmt);
    while(*fmt) {
        if(*fmt == '%') {
            if(fend - fmt == 1) {
                buf[len++] = *(fmt++);
                continue;
            }
            fmt++;
            if(*fmt == 'c') {
                buf[len++] = (char) va_arg(va, int);
            }
            else if(*fmt == 'd') {
                int i = va_arg(va, int);
                char* ptr = rm_itoa(i);
                memcpy(&buf[len], ptr, ilen);
                len += ilen;
            }
            else if(*fmt == 'f') {
                float f = va_arg(va, double);
                rm_ftoa(f);
                memcpy(&buf[len], fbuffer, flen);
                len += flen;
            }
            else if(*fmt == 's') {
                char* s = va_arg(va, char*);
                uint8_t n = strlen(s);
                memcpy(&buf[len], s, n);
                len += n;
            }
            else {
                buf[len++] = '%';
                buf[len++] = *fmt;
            }
            fmt++;
        }
        else
            buf[len++] = *(fmt++);
    }
    buf[len] = '\0';
    return len;
}
