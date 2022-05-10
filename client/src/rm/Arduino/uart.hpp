/**
 * @file uart.hpp
 * @brief UART connection
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_UART_H__
#define __RM_UART_H__ ///< Header guard


/**
 * @brief Initializes the UART connection
 * 
 * @param baud UART baud rate
 */
void rmConnectUART(unsigned long baud);

/**
 * @brief Initializes the UART connection
 * 
 * @param baud UART baud rate
 */
void rmConnectUART1(unsigned long baud);

/**
 * @brief Initializes the UART connection
 * 
 * @param baud UART baud rate
 */
void rmConnectUART2(unsigned long baud);

/**
 * @brief Initializes the UART connection
 * 
 * @param baud UART baud rate
 */
void rmConnectUART3(unsigned long baud);

#endif
