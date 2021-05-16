/**
 * @file connection_spi.h
 * @brief SPI connection
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_CONNECTION_SPI_H__
#define __RM_CONNECTION_SPI_H__ ///< Header guard


#include "connection.h"
#include "pin.h"

#ifndef RM_EXPORT
#ifdef __AVR
#include <avr/io.h>
#include <avr/interrupt.h>
#endif
#endif


#ifdef __cplusplus
extern "C" {
#endif

extern rmPin rmSPISlavePin; ///< Pin to SS of slave module

#if defined(__AVR)
extern uint8_t rmSPCR; ///< SPCR register value
extern uint8_t rmSPSR; ///< SPSR register value
extern volatile uint8_t* rmSPCR_mem; ///< SPCR register
extern volatile uint8_t* rmSPSR_mem; ///< SPSR register
extern volatile uint8_t* rmSPDR_mem; ///< SPDR register
extern uint8_t rmSPIF_mask; ///< SPIF bitmask
#elif defined(__arm__)
typedef struct __SPI_HandleTypeDef SPI_HandleTypeDef;

extern SPI_HandleTypeDef* rmSPIHandler; ///< The SPI handler
#endif

/**
 * @brief Begins the SPI transferring with the slave
 */
static inline void rmSPIBegin() {
    #if defined(__AVR)
    *rmSPCR_mem = rmSPCR;
    *rmSPSR_mem = (*rmSPSR_mem & 0xFE) | rmSPSR;
    *(rmSPISlavePin.io) &= ~(rmSPISlavePin.bitmask);
    #elif defined(__arm__)
    HAL_GPIO_WritePin(rmSPISlavePin.port, rmSPISlavePin.bitmask, 0);
    #endif
}

/**
 * @brief Ends the SPI transferring with the slave
 */
static inline void rmSPIEnd() {
    #if defined(__AVR)
    *(rmSPISlavePin.io) |= rmSPISlavePin.bitmask;
    #elif defined(__arm__)
    HAL_GPIO_WritePin(rmSPISlavePin.port, rmSPISlavePin.bitmask, 0);
    #endif
}

/**
 * @brief Transfer a byte to the SPI bus
 * 
 * @param c The byte to transfer
 * 
 * @return Status or reply byte from the slave device
 */
static inline uint8_t rmSPITransfer(uint8_t c) {
    #if defined(__AVR)
    *rmSPDR_mem = c;
    asm volatile("nop");
    while(!(*rmSPSR_mem & rmSPIF_mask));
    return *rmSPDR_mem;
    #elif defined(__arm__)
    char res = 0;
    HAL_SPI_TransmitReceive(rmSPIHandler, &c, &res, 1, 10);
    return res;
    #endif
}

/**
 * @brief Sends a message through SPI bus
 * 
 * @param msg Null-terminating string
 */
void rmSPISendMessage(const char* msg);

/**
 * @brief Reads a message from SPI bus
 * 
 * @return Unread character. '\0' if there is nothing to read.
 */
char rmSPIRead();


#ifndef RM_EXPORT
#if defined(__AVR)
/**
 * @brief Initializes the SPI connection in master mode
 * 
 * @param clock Clock speed
 * @param bitOrder LSB or MSB
 * @param dataMode SPI data mode
 * @param ss The pin to select the slave device
 */
static inline void rmConnectSPI(uint32_t clock, uint8_t bitOrder,
                                uint8_t dataMode, rmPin ss)
{
    rmFCPU = F_CPU;
    uint32_t clockSetting = F_CPU / 2;
    uint8_t clockDiv = 0;
    while (clockDiv < 6 && clock < clockSetting) {
        clockSetting /= 2;
        clockDiv++;
    }
    if(clockDiv == 6)
        clockDiv = 7;
    clockDiv ^= 0x01;
    
    rmSPCR = _BV(SPE) | _BV(MSTR) | (dataMode & 0x0C) |
             ((clockDiv >> 1) & 0x03);
    if(bitOrder == 0x00)
        rmSPCR |= _BV(DORD);
    rmSPSR = clockDiv & 0x01;
    DDRB |= (1 << 5) | (1 << 3) | (1 << 2);
    *(ss.mode) |= ss.bitmask;
    *(ss.io) |= ss.bitmask;
    rmSPISlavePin = ss;
    rmSPCR_mem = &SPCR;
    rmSPSR_mem = &SPSR;
    rmSPDR_mem = &SPDR;
    rmSPIF_mask = _BV(SPIF);
    
    rmSendMessage = &rmSPISendMessage;
    rmRead = &rmSPIRead;
}
#elif defined(__arm__)
/**
 * @brief Initializes the SPI connection in master mode
 * 
 * @param hspi The SPI port connected to the device
 * @param ss The pin to select the slave device
 */
void rmConnectSPI(SPI_HandleTypeDef* hspi, rmPin ss) {
    rmFCPU = HAL_RCC_GetSysClockFreq();
    rmSPIHandler = hspi;
    rmSPISlavePin = ss;
    HAL_GPIO_WritePin(ss.port, ss.bitmask, 1);
    rmSendMessage = &rmSPISendMessage;
    rmRead = &rmSPIRead;
}
#endif
#endif

#ifdef __cplusplus
}

#ifndef RM_EXPORT
#ifndef Arduino_h
static inline void rmConnectSPI(uint32_t clock, uint8_t bitOrder,
                                uint8_t dataMode, uint8_t ss)
{
    uint8_t port;
    rmPin ssPin;
    port = digitalPinToPort(ss);
    ssPin.mode = (uint8_t*) portModeRegister(port);
    ssPin.io = (uint8_t*) portOutputRegister(port);
    ssPin.bitmask = digitalPinToBitMask(ss);
    rmConnectSPI(clock, bitOrder, dataMode, ssPin);
}
#endif
#endif
#endif

#endif
