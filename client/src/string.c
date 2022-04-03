/**
 * @file string.C
 * @brief A string class for use with the library
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#include "rm/string.h"

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
