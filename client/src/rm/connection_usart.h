/**
 * @file connection_usart.h
 * @brief USART connection
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_CONNECTION_USART_H__
#define __RM_CONNECTION_USART_H__ ///< Header guard


#ifdef __cplusplus
extern "C" {
#endif


#include "connection.h"
#include "connection_buffer.h"

#ifndef RM_EXPORT
#include <avr/io.h>
#include <avr/interrupt.h>
#endif


#ifndef RM_EXPORT

extern volatile uint8_t* rmUDR_mem; ///< The register to send or get message
extern volatile uint8_t* rmUCSRA_mem; ///< The status register
extern volatile uint8_t* rmUCSRB_mem; ///< The register handling RX TX bits
extern volatile uint8_t* rmSREG_mem; ///< The configuration register
extern uint8_t rmU2X_mask; ///< U2X bit mask
extern uint8_t rmTXC_mask; ///< Bit mask for TX complete status bit
extern uint8_t rmMPCM0_mask; ///< MPCM0 bit mask
extern uint8_t rmUDRE_mask; ///< Bit mask for UDR empty status bit
extern uint8_t rmUDRIE_mask; ///< Bit mask to enable or disable UDRIE


/**
 * @brief Initializes the USART connection
 * 
 * @param BAUD USART baud rate
 */
static inline void rmConnectUSART(unsigned long baud) {
    uint16_t baud_setting  = (uint16_t) (((F_CPU / (4 * baud)) - 1) / 2);
    UCSR0A = (1 << U2X0);
    if((F_CPU == 16000000UL && baud == 57600) || baud_setting > 4095) {
        UCSR0A = 0;
        baud_setting  = (uint16_t) (((F_CPU / (8 * baud)) - 1) / 2);
    }
    UBRR0H = (uint8_t) (baud_setting >> 8);
    UBRR0L = (uint8_t) baud_setting;
    UCSR0B = (1 << RXEN0) | (1 << RXCIE0) | (1 << TXEN0);
    #if defined(__AVR_ATmega8__)
    UCSR0C = 0x86;
    #else
    UCSR0C = 0x06;
    #endif
    
    rmUDR_mem = &UDR0;
    rmUCSRA_mem = &UCSR0A;
    rmUCSRB_mem = &UCSR0B;
    rmSREG_mem = &SREG;
    rmU2X_mask = (1 << U2X0);
    rmTXC_mask = (1 << TXC0);
    #ifdef MPCM0
    rmMPCM0_mask = (1 << MPCM0);
    #endif
    rmUDRE_mask = (1 << UDRE0);
    rmUDRIE_mask = (1 << UDRIE0);
}

/**
 * @brief Declare this macro in order to use the RX and UDRE ISRs
 */
#define RM_USART_ISR \
ISR(USART_RX_vect) { rmRX_ISR(); } \
ISR(USART_UDRE_vect) { rmUDRE_ISR(); }

/**
 * @brief USART RX internal service routine
 * 
 * Add this line to a source code and the USART RX ISR is deployed
 */
static inline void rmRX_ISR() {
    if(UCSR0A & ~(1 << UPE0)) {
        char c = UDR0;
        uint8_t i = (rmRxHead + 1) % RM_RX_BUFFER_SIZE;
        if(i != rmRxTail) {
            rmRxBuffer[rmRxHead] = c;
            rmRxHead = i;
        }
        else
            UDR0;
    }
}

/**
 * @brief USART TX data empty internal service routine
 * 
 * Add this line to a source code and the USART UDRE ISR is deployed
 */
static inline void rmUDRE_ISR() {
    char c = rmTxBuffer[rmTxTail];
    rmTxTail = (rmTxTail + 1) % RM_TX_BUFFER_SIZE;
    UDR0 = c;
    #ifdef MPCM0
    UCSR0A = (UCSR0A & ((1 << U2X0) | (1 << MPCM0))) | (1 << TXC0);
    #else
    UCSR0A &= (1 << U2X0) | (1 << TXC0);
    #endif
    if(rmTxTail == rmTxHead) {
        UCSR0B &= ~(1 << UDRIE0);
    }
}

#endif


#ifdef __cplusplus
}
#endif

#endif
