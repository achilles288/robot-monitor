/**
 * @file attribute.h
 * @brief Attributes shown by the client or manipulated by the station
 * 
 * A key value pair structure that can be used in a map structure. This type
 * of data is designed to be portable in communication between two devices.
 * The data here is the real-time data of the client device which can also be
 * overriden by the station. The value may be a string, a number or a blob.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_ATTRIBUTE_H__
#define __RM_ATTRIBUTE_H__ ///< Header guard


#ifdef __cplusplus
extern "C" {
#endif


#ifndef RM_EXPORT
#include <avr/pgmspace.h>
#endif

#include <stdbool.h>
#include <stdint.h>


#define RM_ATTRIBUTE_BOOL   0 ///< Boolean data type
#define RM_ATTRIBUTE_CHAR   1 ///< A single character
#define RM_ATTRIBUTE_INT    2 ///< Integer data type
#define RM_ATTRIBUTE_FLOAT  3 ///< Floating point data type
#define RM_ATTRIBUTE_STRING 4 ///< String data type
#define RM_ATTRIBUTE_BLOB   5 ///< Binary large object


/**
 * @brief The data type of the rmAttribute's value
 * 
 * Since the stored value has multiple possible data types, this union
 * structure is suitable.
 */
typedef union _rmAttributeData {
    bool b; ///< Boolean data type
    char c; ///< A single character
    int32_t i; ///< Integer data type
    float f; ///< Floating point data type
    char* s; ///< String data type
    uint8_t* ptr; ///< Pointer type
} rmAttributeData;


/**
 * @brief Attributes shown by the client or manipulated by the station
 * 
 * A key value pair structure that can be used in a map structure. This type
 * of data is designed to be portable in communication between two devices.
 * The data here is the real-time data of the client device which can also be
 * overriden by the station. The value may be a string, a number or a blob.
 */
typedef struct _rmAttribute {
    char name[12]; ///< Unique name of the attribute
    rmAttributeData data; ///< The stored data
    int8_t type; ///< The data type
    rmAttributeData lowerBound; ///< The lower bound value
    rmAttributeData upperBound; ///< The upper bound value
    void (*onChange)(); ///< The callback that trigger on value change
} rmAttribute;




/**
 * @brief Creates an attribute and stored in the list
 * 
 * @param key Unique name of the attribute with maximum 11 characters
 * @param t Data type of the value stored
 * 
 * @return The newly created attribute
 */
rmAttribute* _rmCreateAttribute(const char* key, int8_t t);

#ifndef RM_EXPORT
/**
 * @brief Creates an attribute and stored in the list
 * 
 * @param key Unique name of the attribute with maximum 11 characters.
 *            The param must be the string stored in program space.
 * @param t Data type of the value stored
 * 
 * @return The newly created attribute. The returned pointer is not recommended
 *         to be used since the memory location is changing as the new objects
 *         are added. rmGetAttribute() function can be used after adding all
 *         the objects.
 */
static inline rmAttribute* rmCreateAttribute_P(const char* key, int8_t t)
{
    char name[12];
    strncpy_P(name, key, 11);
    name[11] = '\0';
    return _rmCreateAttribute(name, t);
}

/**
 * @brief Creates an attribute and stored in the list
 * 
 * @param T Unique name of the attribute with maximum 11 characters
 * @param K Data type of the value stored
 * 
 * @return The newly created attribute. The returned pointer is not recommended
 *         to be used since the memory location is changing as the new objects
 *         are added. rmGetAttribute() function can be used after adding all
 *         the objects.
 */
#define rmCreateAttribute(K, T) rmCreateAttribute_P(PSTR(K), T)
#endif

/**
 * @brief Looks for an attribute by name
 * 
 * @param key Unique name
 * 
 * @return Requested attribute. Null if the request is unavailable.
 */
rmAttribute* _rmGetAttribute(const char* key);

#ifndef RM_EXPORT
/**
 * @brief Looks for an attribute by name
 * 
 * @param key Unique name
 * 
 * @return Requested attribute. Null if the request is unavailable.
 */
static inline rmAttribute* rmGetAttribute_P(const char* key) {
    char name[12];
    strncpy_P(name, key, 11);
    name[11] = '\0';
    return _rmGetAttribute(name);
}

/**
 * @brief Looks for an attribute by name
 * 
 * @param key Unique name
 * 
 * @return Requested attribute. Null if the request is unavailable.
 */
#define rmGetAttribute(K) rmGetAttribute_P(PSTR(K))
#endif

/**
 * @brief Sets the value
 * 
 * Calling this function triggers the set command to the station.
 * 
 * @param attr The attribute
 * @param value The boolean value
 */
void rmAttributeSetBool(rmAttribute* attr, bool value);

/**
 * @brief Sets the value
 * 
 * Calling this function triggers the set command to the station.
 * 
 * @param attr The attribute
 * @param value The character value
 */
void rmAttributeSetChar(rmAttribute* attr, char value);

/**
 * @brief Sets the value
 * 
 * Calling this function triggers the set command to the station.
 * 
 * @param attr The attribute
 * @param value The integer value
 */
void rmAttributeSetInt(rmAttribute* attr, int32_t value);
    
/**
 * @brief Sets the value
 * 
 * Calling this function triggers the set command to the station.
 * 
 * @param attr The attribute
 * @param value The floating point value
 */
void rmAttributeSetFloat(rmAttribute* attr, float value);

/**
 * @brief Sets the value
 * 
 * Calling this function triggers the set command to the station.
 * 
 * @param attr The attribute
 * @param value The string value or blob. The array is copied.
 */
void rmAttributeSetString(rmAttribute* attr, const char* value);

/**
 * @brief Sets the value
 * 
 * @param attr The attribute
 * @param value The string value or blob. The array is copied.
 */
void rmAttributeSetValue(rmAttribute* attr, const char* value);

/**
 * @brief Sets the boundary
 * 
 * @param attr The attribute
 * @param lower Lower bound value
 * @param upper Upper bound value
 */
static inline void rmAttributeSetBoundary(rmAttribute* attr,
                                          rmAttributeData lower,
                                          rmAttributeData upper)
{
    attr->lowerBound = lower;
    attr->upperBound = upper;
}


#ifdef __cplusplus
}
#endif

#endif
