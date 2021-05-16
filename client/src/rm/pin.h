/**
 * @file pin.h
 * @brief GPIO pin
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_PIN_H__
#define __RM_PIN_H__ ///< Header guard


#include <stdint.h>

#ifdef __arm__
#include "../stm32_hal.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

#if defined(__AVR)
/**
 * @brief GPIO pin
 */
typedef struct _rmPin {
    uint8_t* mode; ///< Port mode register
    uint8_t* io; ///< Port input output register
    uint8_t bitmask; ///< Bitmask but should represent a single bit
} rmPin;
#elif defined(__arm__)
/**
 * @brief GPIO pin
 */
typedef struct _rmPin {
    GPIO_TypeDef* port; ///< GPIO port
    uint16_t bitmask; ///< Bitmask but should represent a single bit
} rmPin;
#endif

#ifdef __cplusplus
}
#endif

#endif
