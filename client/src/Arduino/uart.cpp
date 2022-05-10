/**
 * @file uart.cpp
 * @brief UART connection
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#include "../connection_private.h"

#include <HardwareSerial.h>


#ifdef HAVE_HWSERIAL0
static void rmUARTSendMessage(const char* msg) {
    Serial.print(msg);
}
#endif

#ifdef HAVE_HWSERIAL1
static void rmUART1SendMessage(const char* msg) {
    Serial1.print(msg);
}
#endif

#ifdef HAVE_HWSERIAL2
static void rmUART2SendMessage(const char* msg) {
    Serial2.print(msg);
}
#endif

#ifdef HAVE_HWSERIAL3
static void rmUART3SendMessage(const char* msg) {
    Serial3.print(msg);
}
#endif

#ifdef HAVE_HWSERIAL0
static char rmUARTRead() {
    if(Serial.available())
        return Serial.read();
    else
        return '\0';
}
#endif

#ifdef HAVE_HWSERIAL1
static char rmUART1Read() {
    if(Serial1.available())
        return Serial1.read();
    else
        return '\0';
}
#endif

#ifdef HAVE_HWSERIAL2
static char rmUART2Read() {
    if(Serial2.available())
        return Serial2.read();
    else
        return '\0';
}
#endif

#ifdef HAVE_HWSERIAL3
static char rmUART3Read() {
    if(Serial3.available())
        return Serial3.read();
    else
        return '\0';
}
#endif


/**
 * @brief Initializes the UART connection
 * 
 * @param baud UART baud rate
 */
void rmConnectUART(unsigned long baud) {
    #ifdef HAVE_HWSERIAL0
    Serial.begin(baud);
    _rmRead = rmUARTRead;
    _rmSendMessage = rmUARTSendMessage;
    #endif
}

/**
 * @brief Initializes the UART connection
 * 
 * @param baud UART baud rate
 */
void rmConnectUART1(unsigned long baud) {
    #ifdef HAVE_HWSERIAL1
    Serial1.begin(baud);
    _rmRead = rmUART1Read;
    _rmSendMessage = rmUART1SendMessage;
    #endif
}

/**
 * @brief Initializes the UART connection
 * 
 * @param baud UART baud rate
 */
void rmConnectUART2(unsigned long baud) {
    #ifdef HAVE_HWSERIAL2
    Serial2.begin(baud);
    _rmRead = rmUART2Read;
    _rmSendMessage = rmUART2SendMessage;
    #endif
}

/**
 * @brief Initializes the UART connection
 * 
 * @param baud UART baud rate
 */
void rmConnectUART3(unsigned long baud) {
    #ifdef HAVE_HWSERIAL3
    Serial3.begin(baud);
    _rmRead = rmUART3Read;
    _rmSendMessage = rmUART3SendMessage;
    #endif
}
