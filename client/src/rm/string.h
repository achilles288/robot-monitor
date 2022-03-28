/**
 * @file string.h
 * @brief A string class for use with the library
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_STRING_H__
#define __RM_STRING_H__ ///< Header guard


#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief A string class for use with the library
 */
typedef struct _rmString {
    char* data; ///< Array of characters with user-defined size
    uint8_t size; ///< Array size
} rmString;


/**
 * @brief Creates a string with user-defined size
 * 
 * @param len Maximum number of characters to be stored
 * 
 * @return A string object
 */
rmString rmCreateString(uint8_t len);


#ifdef __cplusplus
}
#endif

#endif
