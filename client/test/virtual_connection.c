/**
 * @file virtual_connection.c
 * @brief Virtual port connection for tessting
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#include <connection_private.h>

#include <stddef.h>
#include <string.h>


#define RX2_BUFFER_SIZE 256

static char rx2Buffer[RX2_BUFFER_SIZE];
static uint8_t rx2Count = 0;


static void loadTX() {
    uint8_t i = rmTxTail;
    while(rx2Count < RM_RX_BUFFER_SIZE - 1) {
        if(i == rmTxHead)
            break;
        rx2Buffer[rx2Count] = rmTxBuffer[i];
        i = (i + 1) % RM_TX_BUFFER_SIZE;
        rx2Count++;
    }
    rmTxTail = i;
    rx2Buffer[rx2Count] = '\0';
}


static void sendMessage(const char* msg) {
    char c;
    while((c = *msg++) != '\0') {
        uint8_t i = (rmTxHead + 1) % RM_TX_BUFFER_SIZE;
        while(i == rmTxTail) {
            return;
        }
        rmTxBuffer[rmTxHead] = c;
        rmTxHead = i;
    }
    loadTX();
}


/**
 * @brief Initializes the virtual connection
 */
void rmConnectVirtual() {
    rmSendMessage = &sendMessage;
    rx2Buffer[RX2_BUFFER_SIZE - 1] = '\0';
}


/**
 * @brief Reads the message from the RX buffer for the virtual second device
 * 
 * @return The stored message. NULL if there is no new message.
 */
char* rmVirtualStationRead() {
    if(rx2Count == 0)
        return NULL;
    else {
        rx2Count = 0;
        return rx2Buffer;
    }
}


/**
 * @brief The virtual device sending message
 * 
 * Directly writes data on the client's RX buffer.
 * 
 * @param msg The message to be sent
 */
void rmVirtualStationSendMessage(const char* msg) {
    char c;
    while((c = *msg++) != '\0') {
        uint8_t i = (rmRxHead + 1) % RM_RX_BUFFER_SIZE;
        if(i == rmRxTail)
            return;
        rmRxBuffer[rmRxHead] = c;
        rmRxHead = i;
    }
}
