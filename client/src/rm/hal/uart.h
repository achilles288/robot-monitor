/**
 * @file uart.h
 * @brief UART connection
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_UART_H__
#define __RM_UART_H__ ///< Header guard


#include "hal.h"


#ifdef __cplusplus
extern "C" {
#endif


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


#ifdef __cplusplus
}
#endif

#endif
