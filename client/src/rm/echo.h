/**
 * @file echo.h
 * @brief Prints regular output messages
 * 
 * The 'echo' command is usually to print normal output messages other than
 * attribute changes and functional callbacks.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_ECHO_H__
#define __RM_ECHO_H__ ///< Header guard


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Prints a console ouput message
 * 
 * @param msg A format string that follows the same specifications as format in
 *            printf
 * @param ... Additional arguments
 */
void rmEcho(const char* msg, ...);

/**
 * @brief Prints a warning message
 * 
 * @param S A format string that follows the same specifications as format in
 *          printf
 * @param ... Additional arguments
 */
void rmWarn(const char* msg, ...);

/**
 * @brief Prints an error message
 * 
 * @param S A format string that follows the same specifications as format in
 *          printf
 * @param ... Additional arguments
 */
void rmError(const char* msg, ...);


#ifdef __cplusplus
}
#endif

#endif
