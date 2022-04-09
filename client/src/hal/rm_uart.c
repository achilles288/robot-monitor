/**
 * @file uart.c
 * @brief UART connection
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#ifdef __arm__

#include "../rm/hal/uart.h"

#include "../connection_private.h"

#include <string.h>


#define DMA_RX_BUFFER_SIZE 128
#define DMA_TX_BUFFER_SIZE 128

static UART_HandleTypeDef *handler;
static DMA_HandleTypeDef *rxDMAHandler;
static DMA_HandleTypeDef *txDMAHandler;
static uint8_t rxDMABuffer[DMA_RX_BUFFER_SIZE];
static uint8_t txDMABuffer[DMA_TX_BUFFER_SIZE];


static void rmUARTSendMessage(const char* msg) {
    char c;
    while((c = *msg++) != '\0') {
        uint8_t i = (rmTxHead + 1) % RM_TX_BUFFER_SIZE;
        
        if(i == rmTxTail) {
            if(!rmTxOn)
                rmUARTLoadDMA();
        }
        
        while(i == rmTxTail) {}
        rmTxBuffer[rmTxHead] = c;
        rmTxHead = i;
    }
}

/**
 * @brief Initializes the UART connection
 * 
 * @param huart UART handler
 * @param hdma_rx DMA RX handler
 * @param hdma_tx DMA TX handler
 */
void rmConnectUART(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_rx,
                   DMA_HandleTypeDef *hdma_tx)
{
    handler = huart;
    rxDMAHandler = hdma_rx;
    txDMAHandler = hdma_tx;
    rmSendMessage = &rmUARTSendMessage;
    memset(rmRxBuffer, 0, DMA_RX_BUFFER_SIZE);
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
    HAL_UART_Receive_DMA(huart, rxDMABuffer, DMA_RX_BUFFER_SIZE);
}

/**
 * @brief Function to be called when using UART DMA with the library
 */
void rmUARTRxCheck() {
    uint8_t len  = DMA_RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(rxDMAHandler);
    if(rxOn)
        return;
    if(len == 0)
        return;
    rxOn = true;
    
    // Reads the buffer
    HAL_UART_DMAStop(handler);
    for(uint8_t i=0; i<len; i++) {
        uint8_t j = (rmRxHead + 1) % RM_RX_BUFFER_SIZE;
        if(j == rmRxTail)
            break;
        rmRxBuffer[rmRxHead] = rmRxDMABuffer[i];
        rmRxHead = j;
    }
    
    // Returns everything to normal
    rmRxOn = false;
    HAL_UART_Receive_DMA(handler, rmRxDMABuffer, DMA_RX_BUFFER_SIZE);
}

/**
 * @brief Function to be called when using UART DMA with the library
 */
void rmUARTLoadDMA() {
    rmTxOn = false;
    if(rmTxTail == rmTxHead || rmRxOn)
        return;
    uint8_t i = rmTxTail;
    uint8_t count = 0;
    while(count < DMA_TX_BUFFER_SIZE) {
        if(i == rmTxHead)
            break;
        txDMABuffer[count] = rmTxBuffer[i];
        i = (i + 1) % RM_TX_BUFFER_SIZE;
        count++;
    }
    HAL_UART_Transmit_DMA(handler, txDMABuffer, count);
    txTail = i;
    rmTxOn = true;
}

#endif