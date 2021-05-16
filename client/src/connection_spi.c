/**
 * @file connection_spi.c
 * @brief SPI connection
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_EXPORT


#include "rm/connection_spi.h"


rmPin rmSPISlavePin; ///< Pin to SS of slave module

#if defined(__AVR)
uint8_t rmSPCR; ///< SPCR register value
uint8_t rmSPSR; ///< SPSR register value
volatile uint8_t* rmSPCR_mem; ///< SPCR register
volatile uint8_t* rmSPSR_mem; ///< SPSR register
volatile uint8_t* rmSPDR_mem; ///< SPDR register
uint8_t rmSPIF_mask; ///< SPIF bitmask
#elif defined(__arm__)
SPI_HandleTypeDef* rmSPIHandler; ///< The SPI handler
#endif
