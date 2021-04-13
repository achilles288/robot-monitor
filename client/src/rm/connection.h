/**
 * @file connection.h
 * @brief Functions that handle the connection to the station
 * 
 * USART RX and TX interrupts. Encryption of messages. Passage of messages to
 * data processing
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_CONNECTION_H__
#define __RM_CONNECTION_H__ ///< Header guard


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Sends a message through a connection
 * 
 * @param msg The message to be sent
 */
void rmSendMessage(const char* msg);

/**
 * @brief Reads a message from the connection
 * 
 * @return An unread character from the received message
 */
char rmRead();

/**
 * @brief Reads a message and process the command line
 */
void rmReadMessage();


#ifdef __cplusplus
}
#endif

#endif
