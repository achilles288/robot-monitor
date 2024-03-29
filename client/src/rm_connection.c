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

bool rmRxOn = false;
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

char (*_rmRead)() = &readDefault;

void (*_rmSendMessage)(const char*) = &sendMessageDefault;

void (*_rmConnectionIdle)() = NULL;




#define PROCESS_DEFAULT   0b00
#define PROCESS_STARTED   0b01
#define PROCESS_SEPERATOR 0b11


/**
 * @brief Reads a message and processes it
 */
void rmProcessMessage() {
    if(_rmConnectionIdle != NULL)
        _rmConnectionIdle();
    
    static char cmd[256];
    static char* tokens[8];
    static uint8_t i = 0;
    static uint8_t tokenCount = 0;
    static uint8_t flag = PROCESS_DEFAULT;
    
    char c = _rmRead();
    while(c != '\0') {
        if(i == 255)
            c = '\n';
        
        if(flag & PROCESS_STARTED) {
            rmCall* call;
            switch(c) {
              case ' ':
                cmd[i++] = '\0';
                flag = PROCESS_SEPERATOR;
                break;
                
              case '\n':
                cmd[i] = '\0';
                call = _rmCallGet(cmd);
                if(call != NULL)
                    call->callback(tokenCount, tokens);
                flag = PROCESS_DEFAULT;
                break;
                
              default:
                if(flag == PROCESS_SEPERATOR) {
                    if(tokenCount == 8)
                        break;
                    tokens[tokenCount++] = &cmd[i];
                    flag = PROCESS_STARTED;
                }
                cmd[i++] = c;
            }
        }
        else if(c == '$') {
            i = 0;
            tokenCount = 0;
            flag = PROCESS_STARTED;
        }
        c = _rmRead();
    }
}


int rm_sprintf(char* buf, const char* fmt, va_list va);


/**
 * @brief Sends a command-line to the station
 * 
 * @param cmd The command (a format string for the arguments should be included
 *            if necessary)
 * @param ... The command-line arguments
 */
void rmSendCommand(const char* cmd, ...) {
    _rmSendMessage("$");
    va_list args;
    va_start(args, cmd);
    char buff[256];
    rm_sprintf(buff, cmd, args);
    _rmSendMessage(buff);
    va_end(args);
    _rmSendMessage("\n");
}


/**
 * @brief Prints a console ouput message
 * 
 * @param msg A format string that follows the same specifications as format in
 *            printf
 * @param ... Additional arguments
 */
void rmEcho(const char* msg, ...) {
    _rmSendMessage("$echo ");
    va_list args;
    va_start(args, msg);
    char buff[256];
    rm_sprintf(buff, msg, args);
    _rmSendMessage(buff);
    va_end(args);
    _rmSendMessage("\n");
}


/**
 * @brief Prints a warning message
 * 
 * @param S A format string that follows the same specifications as format in
 *          printf
 * @param ... Additional arguments
 */
void rmWarn(const char* msg, ...) {
    _rmSendMessage("$warn ");
    va_list args;
    va_start(args, msg);
    char buff[256];
    rm_sprintf(buff, msg, args);
    _rmSendMessage(buff);
    va_end(args);
    _rmSendMessage("\n");
}


/**
 * @brief Prints an error message
 * 
 * @param S A format string that follows the same specifications as format in
 *          printf
 * @param ... Additional arguments
 */
void rmError(const char* msg, ...) {
    _rmSendMessage("$err ");
    va_list args;
    va_start(args, msg);
    char buff[256];
    rm_sprintf(buff, msg, args);
    _rmSendMessage(buff);
    va_end(args);
    _rmSendMessage("\n");
}
