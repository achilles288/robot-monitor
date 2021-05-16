/**
 * @file connection_rf24.h
 * @brief Communication with two RF24 modules
 * 
 * Connects the client and station via two RF24 modules.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_CONNECTION_RF24_H__
#define __RM_CONNECTION_RF24_H__ ///< Header guard


#include "connection.h"
#include "connection_spi.h"

#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @breif Configures the RF24 module
 * 
 * @param addr1 The writing pipe address with length of 5 bytes
 * @param addr2 The reading pipe address with length of 5 bytes
 * @param ce The chip enable pin
 * @param csn The chip select not pin or SS pin
 */
void rmRF24Setup(const uint8_t* addr1, const uint8_t* addr2, rmPin ce,
                 rmPin csn);


#ifndef RM_EXPORT
#if defined(__AVR)
/**
 * @brief Initializes the NRF24L01 connection
 * 
 * @param addr1 The writing pipe address with length of 5 bytes
 * @param addr2 The reading pipe address with length of 5 bytes
 * @param ce The chip enable pin
 * @param csn The chip select not pin or SS pin
 */
static inline void rmConnectRF24(const uint8_t* addr1, const uint8_t* addr2,
                                 rmPin ce, rmPin csn)
{
    rmFCPU = F_CPU;
    uint32_t clockSetting = F_CPU / 2;
    uint8_t clockDiv;
    // RF24 SPI speed is 10MHz
    #if F_CPU < 20000000
    clockDiv = 0;
    #elif F_CPU < 40000000
    clockDiv = 1;
    #elif F_CPU < 80000000
    clockDiv = 2;
    #elif F_CPU < 160000000
    clockDiv = 3;
    #elif F_CPU < 320000000
    clockDiv = 4;
    #elif F_CPU < 640000000
    clockDiv = 5;
    #else // F_CPU >= 640000000
    clockDiv = 7;
    #endif
    clockDiv ^= 0x01;
    
    // Data mode is 0x00 and bit order is MSB
    rmSPCR = _BV(SPE) | _BV(MSTR) | ((clockDiv >> 1) & 0x03);
    rmSPSR = clockDiv & 0x01;
    DDRB |= (1 << 5) | (1 << 3) | (1 << 2);
    rmSPCR_mem = &SPCR;
    rmSPSR_mem = &SPSR;
    rmSPDR_mem = &SPDR;
    rmSPIF_mask = _BV(SPIF);
    
    rmRF24Setup(addr1, addr2, ce, csn);
}
#elif defined(__arm__)
/**
 * @brief Initializes the NRF24L01 connection
 * 
 * @paran hspi The SPI port connected to the module
 * @param addr1 The writing pipe address with length of 5 bytes
 * @param addr2 The reading pipe address with length of 5 bytes
 * @param ce The chip enable pin
 * @param csn The chip select not pin or SS pin
 */
static inline void rmConnectRF24(SPI_HandleTypeDef* hspi, const uint8_t* addr1,
                                 const uint8_t* addr2, rmPin ce, rmPin csn)
{
    rmConnectSPI(hspi, csn);
    rmRF24Setup(addr1, addr2, ce, csn);
}
#endif
#endif

#ifdef __cplusplus
}

#ifndef RM_EXPORT
#ifdef Arduino_h
static inline void rmConnectRF24(const uint8_t* addr1, const uint8_t* addr2,
                                 uint8_t ce, uint8_t csn)
{
    uint8_t port;
    rmPin cePin, csnPin;
    port = digitalPinToPort(ce);
    cePin.mode = (uint8_t*) portModeRegister(port);
    cePin.io = (uint8_t*) portOutputRegister(port);
    cePin.bitmask = digitalPinToBitMask(ce);
    port = digitalPinToPort(csn);
    csnPin.mode = (uint8_t*) portModeRegister(port);
    csnPin.io = (uint8_t*) portOutputRegister(port);
    csnPin.bitmask = digitalPinToBitMask(csn);
    rmConnectRF24(addr1, addr2, cePin, csnPin);
}
#endif
#endif
#endif

#endif
