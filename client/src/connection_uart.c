/**
 * @file connection_uart.c
 * @brief UART connection
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */



#define RM_EXPORT


#include "rm/connection_uart.h"

#ifdef __arm__
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
            if(!txOn)
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
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
    HAL_UART_Receive_DMA(huart, rxDMABuffer, DMA_BUFFER_SIZE);
}

/**
 * @brief Function to be called when using UART DMA with the library
 */
void rmUARTRxCheck() {
    uint8_t len  = DMA_RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(rxDMAHandler);
    if(len == 0)
        return;
    
    char c = UDR0;
    uint8_t i = (rmRxHead + 1) % RM_RX_BUFFER_SIZE;
    if(i != rmRxTail) {
        rmRxBuffer[rmRxHead] = c;
        rmRxHead = i;
    }
    
    memset(rxBuffer, 0, DMA_RX_BUFFER_SIZE);
    HAL_UART_Receive_DMA(&huart1, rxDMABuffer, DMA_RX_BUFFER_SIZE);
}

/**
 * @brief Function to be called when using UART DMA with the library
 */
void rmUARTLoadDMA() {
    rmTxOn = true;
    uint8_t j = rmTxTail;
    for(uint8_t k=0; k<DMA_TX_BUFFER_SIZE; k++) {
        if(j == rmTxHead)
            break;
        txDMABuffer[k] = rmTxBuffer[j];
        j = (j + 1) % RM_TX_BUFFER_SIZE;
        k++;
    }
    HAL_UART_Transmit_DMA(handler, txDMABuffer, DMA_BUFFER_SIZE);
}

#endif
