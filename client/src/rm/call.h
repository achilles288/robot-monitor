/**
 * @file call.h
 * @brief Calls that invoke on the station's request
 * 
 * The callback functions take the parameters only as an integer that
 * represents number of extra tokens and the array of strings which are those
 * tokens. This is similar to "int main(int argc, char* argv[])". The tokens
 * strings are to be converted into appropriate data types within the callback.
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_CALL_H__
#define __RM_CALL_H__ ///< Header guard


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Creates a named callback and appends it to the list
 * 
 * @param key Unique name of the call (maximum length is 11)
 * @param func The callback function. The callback should have two parameters,
 *        an integer representing the number of extra tokens and the array of
 *        strings.
 */
void rmCreateCall(const char* key, void (*func)(int, char**));


#ifdef __cplusplus
}
#endif

#endif
