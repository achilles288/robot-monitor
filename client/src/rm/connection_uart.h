/**
 * @file connection_uart.h
 * @brief UART connection
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_CONNECTION_UART_H__
#define __RM_CONNECTION_UART_H__ ///< Header guard


#include "connection.h"

#ifdef __arm__
#include "../stm32_hal.h"
#endif

#ifndef RM_EXPORT
#ifdef __AVR
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#endif
#endif


#ifdef __cplusplus
extern "C" {
#endif

#if defined(__AVR)
#ifndef RM_EXPORT

static void rmUARTSendMessage(const char* msg);


/**
 * @brief Initializes the UART connection
 * 
 * @param BAUD UART baud rate
 */
static inline void rmConnectUART(unsigned long baud) {
    rmFCPU = F_CPU;
    uint16_t baud_setting  = (uint16_t) (((F_CPU / (4 * baud)) - 1) / 2);
    UCSR0A = (1 << U2X0);
    if((F_CPU == 16000000UL && baud == 57600) || baud_setting > 4095) {
        UCSR0A = 0;
        baud_setting  = (uint16_t) (((F_CPU / (8 * baud)) - 1) / 2);
    }
    UBRR0H = (uint8_t) (baud_setting >> 8);
    UBRR0L = (uint8_t) baud_setting;
    UCSR0B = _BV(RXEN0) | _BV(RXCIE0) | _BV(TXEN0);
    #if defined(__AVR_ATmega8__)
    UCSR0C = 0x86;
    #else
    UCSR0C = 0x06;
    #endif
    rmSendMessage = &rmUARTSendMessage;
}

/**
 * @brief Declare this macro in order to use the RX and UDRE ISRs
 */
#define RM_UART_ISR \
ISR(USART_RX_vect) { rmRX_ISR(); } \
ISR(USART_UDRE_vect) { rmUDRE_ISR(); }

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

static inline void rmUDRE_ISR() {
    char c = rmTxBuffer[rmTxTail];
    rmTxTail = (rmTxTail + 1) % RM_TX_BUFFER_SIZE;
    UDR0 = c;
    #ifdef MPCM0
    UCSR0A = (UCSR0A & (_BV(U2X0) | _BV(MPCM0))) | _BV(TXC0);
    #else
    UCSR0A &= _BV(U2X0) | _BV(TXC0);
    #endif
    if(rmTxTail == rmTxHead) {
        UCSR0B &= ~_BV(UDRIE0);
    }
}

static void rmUARTSendMessage(const char* msg) {
    char c;
    while((c = *msg++) != '\0') {
        if(rmTxHead == rmTxTail && UCSR0A & _BV(UDRE0)) {
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                UDR0 = c;
                #ifdef MPCM0
                UCSR0A = (UCSR0A & (_BV(U2X0) | _BV(MPCM0))) | _BV(TXC0);
                #else
                UCSR0A &= _BV(U2X0) | _BV(TXC0);
                #endif
            }
            continue;
        }
        
        uint8_t i = (rmTxHead + 1) % RM_TX_BUFFER_SIZE;
        while(i == rmTxTail) {
            if(!(SREG & _BV(SREG_I))) {
                if(UCSR0A & _BV(UDRE0)) {
                    char send = rmTxBuffer[rmTxTail];
                    rmTxTail = (rmTxTail + 1) % RM_TX_BUFFER_SIZE;
                    UDR0 = send;
                    #ifdef MPCM0
                    UCSR0A = (UCSR0A & (_BV(U2X0) | _BV(MPCM0))) | _BV(TXC0);
                    #else
                    UCSR0A &= _BV(U2X0) | _BV(TXC0);
                    #endif
                    break;
                }
            }
            else
                __asm__ __volatile__ ("sei" ::: "memory");
        }
        
        rmTxBuffer[rmTxHead] = c;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            rmTxHead = i;
            UCSR0B |= _BV(UDRIE0);
        }
    }
}
#endif
#elif defined(__arm__)
/**
 * @brief Declare this macro in order to use the RX and UDRE ISRs
 */
#define RM_UART_ISR \
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) { \
    rmUARTRxCheck(); \
} \
\
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) { \
    rmTxOn = false; \
    if(rmTxTail != rmTxHead) \
        rmUARTLoadDMA(); \
}

/**
 * @brief Initializes the UART connection
 * 
 * @param huart UART handler
 * @param hdma_rx DMA RX handler
 * @param hdma_tx DMA TX handler
 */
void rmConnectUART(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_rx,
                   DMA_HandleTypeDef *hdma_tx);

/**
 * @brief Function to be called when using UART DMA with the library
 */
void rmUARTRxCheck();

/**
 * @brief Function to be called when using UART DMA with the library
 */
void rmUARTLoadDMA();
#endif

#ifdef __cplusplus
}
#endif

#endif
