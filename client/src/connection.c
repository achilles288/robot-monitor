/**
 * @file connection.c
 * @brief Functions that handle the connection to the station
 * 
 * USART RX and TX interrupts. Encryption of messages. Passage of messages to
 * data processing
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_EXPORT


#include "rm/connection.h"

#include <string.h>


uint32_t rmFCPU = 16000000U; ///< CPU clock frequency

char rmRxBuffer[RM_RX_BUFFER_SIZE]; ///< RX message buffer
uint8_t rmRxHead = 0; ///< RX head
uint8_t rmRxTail = 0; ///< RX tail

char rmTxBuffer[RM_TX_BUFFER_SIZE]; ///< TX message buffer
uint8_t rmTxHead = 0; ///< TX head
uint8_t rmTxTail = 0; ///< TX tail

bool rmTxOn = false; ///< States if the messages are being transmitted


static void sendMessageDefault(const char* msg) {}

static char readDefault() {
    if(rmRxTail == rmRxHead) {
        return '\0';
    }
    else {
        char c = rmRxBuffer[rmRxTail];
        rmRxTail = (rmRxTail + 1) % RM_RX_BUFFER_SIZE;
        return c;
    }
}

/**
 * @brief Sends a message through a connection
 */
void (*rmSendMessage)(const char*) = &sendMessageDefault;

/**
 * @brief Reads a message from the connection
 */
char (*rmRead)() = &readDefault;
