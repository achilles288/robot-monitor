/**
 * @file virtual_connection.h
 * @brief Virtual port connection for tessting
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_VIRTUAL_H__
#define __RM_VIRTUAL_H__ ///< Header guard

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Initializes the virtual connection
 */
void rmConnectVirtual();

/**
 * @brief Reads the message from the RX buffer for the virtual second device
 * 
 * @return The stored message. NULL if there is no new message.
 */
char* rmVirtualStationRead();

/**
 * @brief The virtual device sending message
 * 
 * Directly writes data on the client's RX buffer.
 * 
 * @param msg The message to be sent
 */
void rmVirtualStationSendMessage(const char* msg);


#ifdef __cplusplus
}
#endif

#endif
