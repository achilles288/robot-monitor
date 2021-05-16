/**
 * @file connection_spi_default.c
 * @brief SPI connection
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_EXPORT


#include "rm/connection_spi.h"

#ifdef __arm__
#include <string.h>
#endif


/**
 * @brief Sends a message through SPI bus
 * 
 * @param msg Null-terminating string
 */
void rmSPISendMessage(const char* msg) {
    rmSPIBegin();
    #if defined(__AVR)
    char c;
    while((c = *msg++) != '\0')
        rmSPITransfer((uint8_t) c);
    #elif defined(__arm__)
    HAL_SPI_Transmit(rmSPIHandler, (uint8_t*) msg, strlen(msg), 100);
    #endif
    rmSPIEnd();
}

/**
 * @brief Reads a message from SPI bus
 * 
 * @return Unread character. '\0' if there is nothing to read.
 */
char rmSPIRead() {
    static uint8_t c = 0;
    if(c == 0)
        rmSPIBegin();
    #if defined(__AVR)
    c = rmSPITransfer(0xff);
    if(c == 0)
        rmSPIEnd();
    #elif defined(__arm__)
    HAL_StatusTypeDef ret;
    ret = HAL_SPI_Receive(rmSPIHandler, &c, 1, 10);
    if(ret != HAL_OK) {
        rmSPIEnd();
        c = 0;
    }
    #endif
    return (char) c;
}
