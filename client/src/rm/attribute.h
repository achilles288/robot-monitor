/**
 * @file attribute.h
 * @brief Attributes shown by the client or manipulated by the station
 * 
 * A key value pair structure that can be used in a map structure. This type
 * of data is designed to be portable in communication between two devices.
 * The data here is the real-time data of the client device which can also be
 * overriden by the station. The value may be an integer, a decimal or a text.
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_ATTRIBUTE_H__
#define __RM_ATTRIBUTE_H__ ///< Header guard


#include "string.h"

#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Represents different data types for use withing the library
 */
typedef enum _rmAttributeDataType {
    RM_ATTRIBUTE_BOOL   = 0b00000000, ///< Boolean data type
    RM_ATTRIBUTE_CHAR   = 0b00000001, ///< A single character
    RM_ATTRIBUTE_STRING = 0b00000010, ///< String data type
    RM_ATTRIBUTE_UINT8  = 0b00010000, ///< 8-bit integer data
    RM_ATTRIBUTE_UINT16 = 0b00010001, ///< 16-bit integer data
    RM_ATTRIBUTE_UINT32 = 0b00010010, ///< 32-bit integer data
    RM_ATTRIBUTE_INT8   = 0b00011000, ///< 8-bit integer data
    RM_ATTRIBUTE_INT16  = 0b00011001, ///< 16-bit integer data
    RM_ATTRIBUTE_INT32  = 0b00011010, ///< 32-bit integer data
    RM_ATTRIBUTE_FLOAT  = 0b00011100  ///< Floating point data type
} rmAttributeDataType;


/**
 * @brief Updates the attribute data to the output screen occationally
 * 
 * Stores the key and pointer to the data so that it can send the updated data
 * to the server when needed.
 */
typedef struct _rmOutputAttribute {
    char name[12]; ///< Unique name of the attribute (maximum length is 11)
    void* data; ///< The pointer of the data which the key links with
    rmAttributeDataType type; ///< The data type
} rmOutputAttribute;




/**
 * @brief Creates an input attribute and appends it to the list
 * 
 * @param key Unique name of the attribute (maximum length is 11)
 * @param ptr The pointer of the data which the key links with
 * @param t The data type
 */
void rmCreateInputAttribute(const char* key, void* ptr, rmAttributeDataType t);


/**
 * @brief Sets the boundary to constrain the input data
 * 
 * @param key The attribute name
 * @param lower Lower bound value
 * @param upper Upper bound value
 */
void rmInputAttributeSetBoundaries(const char* key, float lower, float upper);


/**
 * @brief Assigns the callback function which triggers upon data input
 * 
 * @param key The attribute name
 * @param func The callback function
 */
void rmInputAttributeSetOnChange(const char* key, void (*func)());


/**
 * @brief Sends the key and the present data of the attribute to the station
 * 
 * The key and the data is sent as an update command through a connection
 * which is USB or an RF device. This command updates the client's data on the
 * station side.
 * 
 * @param attr The attribute
 */
void rmOutputAttributeUpdate(rmOutputAttribute *attr);


/**
 * @brief Converts the output attribute data to a string
 * 
 * @param attr The attribute
 * 
 * @return String representation of the attribute data. The string returned
 *         must be used or copied before the next function call since it is
 *         allocated in a temporary memory.
 */
char* rmOutputAttributeGetStringData(rmOutputAttribute *attr);


#ifdef __cplusplus
}
#endif

#endif
