/**
 * @file request.c
 * @brief Sending a message while expecting a response from the station
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#include "rm/request.h"

#include "connection_private.h"
#include "rm/call.h"
#include "time.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>


static bool requested = false;
static void (*respCallback)(char*) = NULL;
static unsigned long reqTime = 0;
static unsigned long reqTimeout = 0;


static void respond(int argc, char *argv[]) {
    if(!requested)
        return;
    if(_rmGetTime() - reqTime < reqTimeout) {
        requested = false;
        return;
    }
    if(argc > 0)
        respCallback(argv[0]);
}


/**
 * @brief Sends a request to the station
 * 
 * Can only handle one request at a time.
 * 
 * @param cmd The command-line
 * @param func The function to call when the response is received. The
 *        parameter of the function is the response message.
 * @param timeout Timeout in milliseconds
 */
void rmSendRequest(const char* cmd, void (*func)(char*), uint16_t timeout) {
    if(requested) {
      if(_rmGetTime() - reqTime < reqTimeout)
          return;
    }
    
    static bool init = false;
    if(!init) {
        rmCreateCall("resp", respond);
        init = true;
    }
    
    respCallback = func;
    reqTimeout = timeout;
    reqTime = _rmGetTime();
    char msg[128];
    snprintf(msg, 128, "$%s\n", cmd);
    _rmSendMessage(msg);
}
