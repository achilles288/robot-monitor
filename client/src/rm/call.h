/**
 * @file call.h
 * @brief Calls that invoke on the station's request
 * 
 * The callback functions take the parameters only as an integer that
 * represents number of extra tokens and the array of strings which are those
 * tokens. This is similar to "int main(int argc, char* argv[])". The tokens
 * strings are to be converted into appropriate data types within the callback.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_CALL_H__
#define __RM_CALL_H__ ///< Header guard


#ifndef RM_EXPORT
#ifdef __AVR
#include <avr/pgmspace.h>
#endif
#endif


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calls that invoke on the station's request
 * 
 * The callback functions take the parameters only as an integer that
 * represents number of extra tokens and the array of strings which are those
 * tokens. The tokens strings are to be converted into appropriate data types
 * within the callback.
 */
typedef struct _rmCall {
    char name[12]; ///< Name of the call or command
    void (*callback)(int, char**); ///< Function triggers when command detected
} rmCall;


/**
 * @brief Creates a call in the map structure
 * 
 * @param key Unique name of the attribute with maximum 11 characters
 * @param func The callback function. The callback should have two parameters,
 *        an integer representing the number of extra tokens and the array of
 *        strings.
 * 
 * @return The newly created call
 */
rmCall* _rmCreateCall(const char* key, void (*func)(int, char**));

#ifndef RM_EXPORT
#ifdef __AVR
/**
 * @brief Creates a call in the map structure
 * 
 * @param key Unique name of the attribute with maximum 11 characters.
 *            The param must be the string stored in program space.
 * @param func The callback function. The callback should have two parameters,
 *        an integer representing the number of extra tokens and the array of
 *        strings.
 * 
 * @return The newly created call. The returned pointer is not recommended to
 *         be used since the memory location is changing as the new objects are
 *         added. rmGetAttribute() function can be used after adding all the
 *         objects.
 */
static rmCall* rmCreateCall_P(const char* key, void (*func)(int,char**))
{
    char name[12];
    strncpy_P(name, key, 11);
    name[11] = '\0';
    return _rmCreateCall(name, func);
}

/**
 * @brief Creates a call in the map structure
 * 
 * @param key Unique name of the attribute with maximum 11 characters.
 *            The param must be the string stored in program space.
 * @param func The callback function. The callback should have two parameters,
 *        an integer representing the number of extra tokens and the array of
 *        strings.
 * 
 * @return The newly created call. The returned pointer is not recommended to
 *         be used since the memory location is changing as the new objects are
 *         added. rmGetAttribute() function can be used after adding all the
 *         objects.
 */
#endif

#ifdef __AVR
#define rmCreateCall(K, F) rmCreateCall_P(PSTR(K), F)
#else
#define rmCreateCall(K, F) _rmCreateCall(K, F)
#endif
#endif

/**
 * @brief Looks for a call by name
 * 
 * @param key Unique name
 * 
 * @return Requested call. Null if the request is unavailable.
 */
rmCall* _rmGetCall(const char* key);

#ifndef RM_EXPORT
#ifdef __AVR
/**
 * @brief Looks for a call by name
 * 
 * @param key Unique name
 * 
 * @return Requested call. Null if the request is unavailable.
 */
static rmCall* rmGetCall_P(const char* key) {
    char name[12];
    strncpy_P(name, key, 11);
    name[11] = '\0';
    return _rmGetCall(name);
}
#endif

#ifdef __AVR
#define rmGetCall(K) rmGetCall_P(PSTR(K))
#else
#define rmGetCall(K) _rmGetCall(K)
#endif
#endif

/**
 * @brief Clears all the attributes and calls created
 */
void rmClearData();

#ifdef __cplusplus
}
#endif

#endif