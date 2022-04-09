/**
 * @file connection.c
 * @brief Functions that handle the connection to the station
 * 
 * RX and TX buffering and interrupt handling. The processing of input messages
 * also takes place here.
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#include "rm/connection.h"
#include "connection_private.h"

#include "rm/call.h"
#include "call_private.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>


char rmRxBuffer[RM_RX_BUFFER_SIZE];
uint8_t rmRxHead = 0;
uint8_t rmRxTail = 0;

char rmTxBuffer[RM_TX_BUFFER_SIZE];
uint8_t rmTxHead = 0;
uint8_t rmTxTail = 0;

bool rmTxOn = false;




static char readDefault() {
    if(rmRxTail == rmRxHead) {
        return '\0';
    }
    else {
        char c = rmRxBuffer[rmRxTail];
        rmRxTail = (rmRxTail + 1) % RM_RX_BUFFER_SIZE;
        return c;
    }
}

static void sendMessageDefault(const char* msg) {}

char (*rmRead)() = &readDefault;

void (*rmSendMessage)(const char*) = &sendMessageDefault;




#define PROCESS_DEFAULT   0b00
#define PROCESS_STARTED   0b01
#define PROCESS_SEPERATOR 0b11


/**
 * @brief Reads a message and processes it
 */
void rmProcessMessage() {
    static char cmd[256];
    static char* tokens[8];
    static uint8_t i = 0;
    static uint8_t tokenCount = 0;
    static uint8_t flag = PROCESS_DEFAULT;
    
    char c = rmRead();
    while(c != '\0') {
        if(i == 255)
            c = '\n';
        
        switch(c) {
          case '$':
            i = 0;
            tokenCount = 0;
            flag = PROCESS_STARTED;
            break;
            
          case ' ':
            if(flag & PROCESS_STARTED) {
                cmd[i++] = '\0';
                flag = PROCESS_SEPERATOR;
            }
            break;
            
          case '\n':
            if(flag & PROCESS_STARTED) {
                cmd[i] = '\0';
                rmCall* call = _rmCallGet(cmd);
                if(call != NULL)
                    call->callback(tokenCount, tokens);
                flag = PROCESS_DEFAULT;
                break;
            }
            
          default:
            if(flag & PROCESS_STARTED) {
                if(flag == PROCESS_SEPERATOR) {
                    tokens[tokenCount++] = cmd + i;
                    flag = PROCESS_STARTED;
                }
                cmd[i++] = c;
            }
        }
        c = rmRead();
    }
}


/**
 * @brief Sends a command-line to the station
 * 
 * @param cmd The command (a format string for the arguments should be included
 *            if necessary)
 * @param ... The command-line arguments
 */
void rmSendCommand(const char* cmd, ...) {
    char buff[128];
    va_list args;
    va_start(args, cmd);
    uint8_t len = vsnprintf(buff, 128, cmd, args);
    va_end(args);
    buff[len + 6] = '\n';
    buff[len + 7] = '\0';
    rmSendMessage(buff);
}


/**
 * @brief Prints a console ouput message
 * 
 * @param msg A format string that follows the same specifications as format in
 *            printf
 * @param ... Additional arguments
 */
void rmEcho(const char* msg, ...) {
    char buff[128];
    memcpy(buff, "$echo ", 6);
    va_list args;
    va_start(args, msg);
    uint8_t len = vsnprintf(buff + 6, 128 - 6, msg, args);
    va_end(args);
    buff[len + 6] = '\n';
    buff[len + 7] = '\0';
    rmSendMessage(buff);
}


/**
 * @brief Prints a warning message
 * 
 * @param S A format string that follows the same specifications as format in
 *          printf
 * @param ... Additional arguments
 */
void rmWarn(const char* msg, ...) {
    char buff[128];
    memcpy(buff, "$warn ", 6);
    va_list args;
    va_start(args, msg);
    uint8_t len = vsnprintf(buff + 6, 128 - 6, msg, args);
    va_end(args);
    buff[len + 6] = '\n';
    buff[len + 7] = '\0';
    rmSendMessage(buff);
}


/**
 * @brief Prints an error message
 * 
 * @param S A format string that follows the same specifications as format in
 *          printf
 * @param ... Additional arguments
 */
void rmError(const char* msg, ...) {
    char buff[128];
    memcpy(buff, "$err ", 5);
    va_list args;
    va_start(args, msg);
    uint8_t len = vsnprintf(buff + 5, 128 - 5, msg, args);
    va_end(args);
    buff[len + 5] = '\n';
    buff[len + 6] = '\0';
    rmSendMessage(buff);
}
