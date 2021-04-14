/**
 * @file connection_buffer.h
 * @brief RX and TX buffer arrays
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_CONNECTION_BUFFER_H__
#define __RM_CONNECTION_BUFFER_H__ ///< Header guard


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>


#define RM_RX_BUFFER_SIZE 256 ///< RX buffer size

extern char rmRxBuffer[]; ///< RX buffer message
extern uint8_t rmRxHead; ///< RX head
extern uint8_t rmRxTail; ///< RX tail


#define RM_TX_BUFFER_SIZE 128 ///< TX buffer size

extern char rmTxBuffer[]; ///< TX buffer message
extern uint8_t rmTxHead; ///< TX head
extern uint8_t rmTxTail; ///< TX tail


#ifdef __cplusplus
}
#endif

#endif
