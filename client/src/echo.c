/**
 * @file echo.c
 * @brief Prints regular output messages
 * 
 * The 'echo' command is usually to print normal output messages other than
 * attribute changes and functional callbacks.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#include "rm/connection.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>


/**
 * @brief Prints a console ouput message
 * 
 * @param msg A format string that follows the same specifications as format in
 *            printf
 * @param ... Additional arguments
 */
void rmEcho(const char* msg, ...) {
    char buff[128];
    memcpy(buff, "echo ", 5);
    va_list args;
    va_start(args, msg);
    uint8_t len = vsnprintf(buff + 5, 126 - 5, msg, args);
    va_end(args);
    buff[len + 5] = '\n';
    buff[len + 6] = '\0';
    buff[126] = '\n';
    buff[127] = '\0';
    rmSendMessage(buff);
}
