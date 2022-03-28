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


/**
 * @brief Creates a string with user-defined size
 * 
 * @param len Maximum number of characters to be stored
 * 
 * @return A string object
 */
rmString rmCreateString(uint8_t len) {
    rmString str;
    str.data = (char*) malloc(len + 1);
    str.data[len] = '\0';
    str.size = len;
    return str;
}
