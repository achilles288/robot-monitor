/**
 * @file request.cpp
 * @brief Sending a message while expecting a response from the client
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_EXPORT
#define RM_NO_WX


#include "rm/request.hpp"

#include "rm/client.hpp"

#include <cstring>


/**
 * @brief Constructs a request instance
 * 
 * @param msg The request message
 * @param func The callback function
 * @param t Timeout in milliseconds
 */
rmRequest::rmRequest(const char* msg, void (*func)(const char*), long t) {
    strncpy(message, msg, 127);
    message[127] = '\0';
    callback = func;
    timeout = t;
}

/**
 * @brief Constructs a request instance
 * 
 * @param msg The request message
 * @param func The callback function
 * @param t Timeout in milliseconds
 * @param cli The client instance which the callback has access to
 */
rmRequest::rmRequest(const char* msg, void (*func)(const char*, rmClient*),
                     long t, rmClient* cli)
{
    client = cli;
    strncpy(message, msg, 127);
    message[127] = '\0';
    callback2 = func;
    timeout = t;
}

/**
 * @brief Gets the request message
 * 
 * @return The string of the request
 */
const char* rmRequest::getMessage() const { return message; }

/**
 * @brief Gets the timeout value
 * 
 * @return Timeout in milliseconds
 */
long rmRequest::getTimeout() const { return timeout; }

/**
 * @brief Triggers when a response message is recieved from the client
 */
void rmRequest::onResponse(const char* msg) {
    if(callback != nullptr) {
        callback(msg);
        callback = nullptr;
    }
    else if(callback2 != nullptr && client != nullptr) {
        callback2(msg, client);
        callback2 = nullptr;
        client = nullptr;
    }
    message[0] = '\0';
}


void rmCallbackResp(int argc, char *argv[], rmClient* cli) {
    if(argc < 1)
        return;
    rmRequest req = cli->getPendingRequest();
    req.onResponse(argv[0]);
}
