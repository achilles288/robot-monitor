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

#include "rm/connection_buffer.h"

#include <string.h>


volatile uint8_t* rmUDR_mem; ///< The register to send or get message
volatile uint8_t* rmUCSRA_mem; ///< The status register
volatile uint8_t* rmUCSRB_mem; ///< The register handling RX TX bits
volatile uint8_t* rmSREG_mem; ///< The configuration register
uint8_t rmU2X_mask; ///< U2X0 bit mask
uint8_t rmTXC_mask; ///< Bit mask for TX complete status bit
uint8_t rmMPCM0_mask = 0; ///< MPCM0 bit mask
uint8_t rmUDRE_mask; ///< Bit mask for UDR empty status bit
uint8_t rmUDRIE_mask; ///< Bit mask to enable or disable UDRIE

char rmRxBuffer[RM_RX_BUFFER_SIZE]; ///< RX message buffer
uint8_t rmRxHead = 0; ///< RX head
uint8_t rmRxTail = 0; ///< RX tail

char rmTxBuffer[RM_TX_BUFFER_SIZE]; ///< TX message buffer
uint8_t rmTxHead = 0; ///< TX head
uint8_t rmTxTail = 0; ///< TX tail




static __inline__ uint8_t __iCliRetVal() {
    __asm__ __volatile__ ("cli" ::: "memory");
    return 1;
}

static __inline__ void __iRestore(const uint8_t* __s) {
    *rmSREG_mem = *__s;
    __asm__ volatile ("" ::: "memory");
}

#define ATOMIC_BLOCK(type) \
    for(type, __ToDo = __iCliRetVal(); __ToDo ; __ToDo = 0)

#define ATOMIC_RESTORESTATE \
    uint8_t sreg_save __attribute__((__cleanup__(__iRestore))) = *rmSREG_mem




/**
 * @brief Sends a message through a connection
 * 
 * @param msg The message to be sent
 */
void rmSendMessage(const char* msg) {
    if(msg == NULL)
        return;
    const char* ptr = msg;
    char c;
    while((c = *ptr++) != '\0') {
        if(rmTxHead == rmTxTail && *rmUCSRA_mem & rmUDRE_mask) {
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                *rmUDR_mem = c;
                if(rmMPCM0_mask) {
                    uint8_t mask1 = rmU2X_mask | rmMPCM0_mask;
                    *rmUCSRA_mem = (*rmUCSRA_mem & mask1) | rmTXC_mask;
                }
                else
                    *rmUCSRA_mem &= rmU2X_mask | rmTXC_mask;
            }
            continue;
        }
        
        uint8_t i = (rmTxHead + 1) % RM_TX_BUFFER_SIZE;
        while(i == rmTxTail) {
            if(!(*rmSREG_mem & (1 << 7))) {
                if(*rmUCSRA_mem & rmUDRE_mask) {
                    char send = rmTxBuffer[rmTxTail];
                    rmTxTail = (rmTxTail + 1) % RM_TX_BUFFER_SIZE;
                    *rmUDR_mem = send;
                    if(rmMPCM0_mask) {
                        uint8_t mask1 = rmU2X_mask | rmMPCM0_mask;
                        *rmUCSRA_mem = (*rmUCSRA_mem & mask1) | rmTXC_mask;
                    }
                    else
                        *rmUCSRA_mem &= rmU2X_mask | rmTXC_mask;
                    break;
                }
            }
            else
                __asm__ __volatile__ ("sei" ::: "memory");
        }
        
        rmTxBuffer[rmTxHead] = c;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            rmTxHead = i;
            *rmUCSRB_mem |= rmUDRIE_mask;
        }
    }
}

/**
 * @brief Reads a message from the connection
 * 
 * @return An unread character from the received message
 */
char rmRead() {
    if(rmRxTail == rmRxHead) {
        return '\0';
    }
    else {
        char c = rmRxBuffer[rmRxTail];
        rmRxTail = (rmRxTail + 1) % RM_RX_BUFFER_SIZE;
        return c;
    }
}
