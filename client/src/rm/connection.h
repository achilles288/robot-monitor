/**
 * @file connection.h
 * @brief Functions that handle the connection to the station
 * 
 * RX and TX buffering and interrupt handling. The processing of input messages
 * also takes place here.
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_CONNECTION_H__
#define __RM_CONNECTION_H__ ///< Header guard


#if defined(__arm__)
#include "hal/uart.h"
#include "hal/usbd_cdc.h"
#elif defined(Arduino_h)
#include "Arduino/uart.hpp"
#endif

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Reads a message and processes it
 */
void rmProcessMessage();


/**
 * @brief Sends a command-line to the station
 * 
 * @param cmd The command (a format string for the arguments should be included
 *            if necessary)
 * @param ... The command-line arguments
 */
void rmSendCommand(const char* cmd, ...);


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
