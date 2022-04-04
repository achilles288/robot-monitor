/**
 * @file request.h
 * @brief Sending a message while expecting a response from the station
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_REQUEST_H__
#define __RM_REQUEST_H__ ///< Header guard


#include <stdint.h>


/**
 * @brief Sends a request to the station
 * 
 * Can only handle one request at a time.
 * 
 * @param cmd The command-line
 * @param func The function to call when the response is received. The
 *        parameter of the function is the response message.
 * @param timeout Timeout in milliseconds
 */
void rmSendRequest(const char* cmd, void (*func)(char*), uint16_t timeout);


#ifdef __cplusplus
}
#endif

#endif
