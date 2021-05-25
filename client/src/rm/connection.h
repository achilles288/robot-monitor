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


#include <stdbool.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

#define RM_RX_BUFFER_SIZE 256 ///< RX buffer size
#define RM_TX_BUFFER_SIZE 256 ///< TX buffer size

extern uint32_t rmFCPU; ///< CPU clock frequency

extern char rmRxBuffer[]; ///< RX buffer message
extern uint8_t rmRxHead; ///< RX head
extern uint8_t rmRxTail; ///< RX tail

extern char rmTxBuffer[]; ///< TX buffer message
extern uint8_t rmTxHead; ///< TX head
extern uint8_t rmTxTail; ///< TX tail

extern bool rmTxOn; ///< States if the messages are being transmitted


/**
 * @brief Sends a message through a connection
 */
extern void (*rmSendMessage)(const char*);

/**
 * @brief Reads a message from the connection
 */
extern char (*rmRead)();

/**
 * @brief Reads a message and process the command line
 */
void rmReadMessage();


#ifdef __cplusplus
}
#endif

#endif
