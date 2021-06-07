/**
 * @file hal.h
 * @brief Required declarations from stm32 HAL library
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_HAL_H__
#define __RM_HAL_H__ ///< Header guard


#ifndef __STM32F1xx_HAL_H
#define __STM32F1xx_HAL_H


#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

#define __IO volatile

typedef enum {
    HAL_OK      = 0x00U,
    HAL_ERROR   = 0x01U,
    HAL_BUSY    = 0x02U,
    HAL_TIMEOUT = 0x03U
} HAL_StatusTypeDef;

typedef enum {
    HAL_UNLOCKED = 0x00U,
    HAL_LOCKED   = 0x01U
} HAL_LockTypeDef;

typedef struct {
    __IO uint32_t ISR;
    __IO uint32_t IFCR;
} DMA_TypeDef;

typedef struct {
    __IO uint32_t CCR;
    __IO uint32_t CNDTR;
    __IO uint32_t CPAR;
    __IO uint32_t CMAR;
} DMA_Channel_TypeDef;

typedef struct {
    uint32_t Direction;
    uint32_t PeriphInc;
    uint32_t MemInc;
    uint32_t PeriphDataAlignment;
    uint32_t MemDataAlignment;
    uint32_t Mode;
    uint32_t Priority;
} DMA_InitTypeDef;

typedef enum {
    HAL_DMA_STATE_RESET   = 0x00U,
    HAL_DMA_STATE_READY   = 0x01U,
    HAL_DMA_STATE_BUSY    = 0x02U,
    HAL_DMA_STATE_TIMEOUT = 0x03U
} HAL_DMA_StateTypeDef;

typedef struct __DMA_HandleTypeDef {
    DMA_Channel_TypeDef *Instance;
    DMA_InitTypeDef Init;
    HAL_LockTypeDef Lock;
    HAL_DMA_StateTypeDef State;
    void *Parent;
    void (*XferCpltCallback)( struct __DMA_HandleTypeDef * hdma);
    void (*XferHalfCpltCallback)( struct __DMA_HandleTypeDef * hdma);
    void (*XferErrorCallback)( struct __DMA_HandleTypeDef * hdma);
    void (* XferAbortCallback)( struct __DMA_HandleTypeDef * hdma);
    __IO uint32_t ErrorCode;
    DMA_TypeDef *DmaBaseAddress;
    uint32_t ChannelIndex;
} DMA_HandleTypeDef;

typedef struct {
    __IO uint32_t SR;
    __IO uint32_t DR;
    __IO uint32_t BRR;
    __IO uint32_t CR1;
    __IO uint32_t CR2;
    __IO uint32_t CR3;
    __IO uint32_t GTPR;
} USART_TypeDef;

typedef struct {
    uint32_t BaudRate;
    uint32_t WordLength;
    uint32_t StopBits;
    uint32_t Parity;
    uint32_t Mode;
    uint32_t HwFlowCtl;
    uint32_t OverSampling;
} UART_InitTypeDef;

typedef enum {
    HAL_UART_STATE_RESET      = 0x00U,
    HAL_UART_STATE_READY      = 0x20U,
    HAL_UART_STATE_BUSY       = 0x24U,
    HAL_UART_STATE_BUSY_TX    = 0x21U,
    HAL_UART_STATE_BUSY_RX    = 0x22U,
    HAL_UART_STATE_BUSY_TX_RX = 0x23U,
    HAL_UART_STATE_TIMEOUT    = 0xA0U,
    HAL_UART_STATE_ERROR      = 0xE0U
} HAL_UART_StateTypeDef;

typedef struct __UART_HandleTypeDef {
    USART_TypeDef *Instance;
    UART_InitTypeDef Init;
    uint8_t *pTxBuffPtr;
    uint16_t TxXferSize;
    __IO uint16_t TxXferCount;
    uint8_t *pRxBuffPtr;
    uint16_t RxXferSize;
    __IO uint16_t RxXferCount;
    DMA_HandleTypeDef *hdmatx;
    DMA_HandleTypeDef *hdmarx;
    HAL_LockTypeDef Lock;
    __IO HAL_UART_StateTypeDef gState;
    __IO HAL_UART_StateTypeDef RxState;
    __IO uint32_t ErrorCode;
} UART_HandleTypeDef;

struct __SPI_HandleTypeDef;
typedef struct __SPI_HandleTypeDef SPI_HandleTypeDef;

typedef struct {
    __IO uint32_t CRL;
    __IO uint32_t CRH;
    __IO uint32_t IDR;
    __IO uint32_t ODR;
    __IO uint32_t BSRR;
    __IO uint32_t BRR;
    __IO uint32_t LCKR;
} GPIO_TypeDef;

typedef enum {
    GPIO_PIN_RESET = 0u,
    GPIO_PIN_SET
} GPIO_PinState;

void HAL_Delay(uint32_t Delay);
void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,
                       GPIO_PinState PinState);
uint32_t HAL_RCC_GetSysClockFreq();
HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData,
                                   uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData,
                                  uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_TransmitReceive(SPI_HandleTypeDef *hspi,
                                          uint8_t *pTxData, uint8_t *pRxData,
                                          uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_UART_Transmit_DMA(UART_HandleTypeDef *huart,
                                        uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UART_Receive_DMA(UART_HandleTypeDef *huart,
                                       uint8_t *pData, uint16_t Size);

#define UART_CR1_REG_INDEX 1U
#define UART_CR2_REG_INDEX 2U
#define USART_CR1_IDLEIE_Pos (4U)                               
#define USART_CR1_IDLEIE_Msk (0x1UL << USART_CR1_IDLEIE_Pos)
#define USART_CR1_IDLEIE USART_CR1_IDLEIE_Msk              
#define UART_IT_IDLE ((uint32_t)(UART_CR1_REG_INDEX << 28U | USART_CR1_IDLEIE))
#define UART_IT_MASK 0x0000FFFFU

#define __HAL_UART_ENABLE_IT(__HANDLE__, __INTERRUPT__) \
    ((((__INTERRUPT__) >> 28U) == UART_CR1_REG_INDEX)? ((__HANDLE__)->Instance->CR1 |= ((__INTERRUPT__) & UART_IT_MASK)): \
    (((__INTERRUPT__) >> 28U) == UART_CR2_REG_INDEX)? ((__HANDLE__)->Instance->CR2 |= ((__INTERRUPT__) & UART_IT_MASK)): \
    ((__HANDLE__)->Instance->CR3 |= ((__INTERRUPT__) & UART_IT_MASK)))

#define __HAL_DMA_GET_COUNTER(__HANDLE__) \
    ((__HANDLE__)->Instance->CNDTR)

#ifdef __cplusplus
}
#endif

#endif
#endif
