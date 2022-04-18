/**
 * @file sync.cpp
 * @brief Handles the data synchronization between client and station
 * 
 * The data to be updated on the station are the output attributes. This module
 * updates a good number of attributes at the same time in the most efficient
 * way.
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_EXPORT
#define RM_NO_WX


#include "rm/sync.hpp"

#include "rm/attribute.hpp"
#include "rm/client.hpp"

#include <cstdio>
#include <cstring>


static void responseCallback(const char* msg, rmClient* cli);


/**
 * @brief Constructs the sync table with an index
 * 
 * @param i ID or index which is between 0 and 9
 */
rmSync::rmSync(uint8_t i) :id(i) {}

    
/**
 * @brief Destructor
 */
rmSync::~rmSync() {
    if(attributes != nullptr)
        delete attributes;
}


/**
 * @breif Retrive the list of attributes to work in a sync
 * 
 * @param cli The client instance
 */
void rmSync::updateList(rmClient* cli) {
    char msg[7];
    snprintf(msg, 6, "lsa %d", id);
    msg[6] = '\0';
    rmRequest req = rmRequest(msg, responseCallback, 1000, cli);
    cli->sendRequest(req);
}

/**
 * @breif Updates the attribute values
 * 
 * @param str The string representing the values of the list
 */
void rmSync::onSync(const char* str) {
    size_t i = 0;
    char buffer[256];
    strncpy(buffer, str, 255);
    buffer[255] = '\0';
    char *token = strtok(buffer, ",");
    
    while(token != NULL && i < count) {
        rmAttribute* attr = attributes[i++];
        if(attr != nullptr)
            attr->setValue(*token);
        token = strtok(NULL, ",");
    }
}


#define MAX_ATTRIBUTES_PER_SYNC 32


static void responseCallback(const char* msg, rmClient* cli) {
    /*char *tokens[32];
    char *tok = strtok(msg, ",");
    count = 0;
    
    while(token != NULL) {
        tokens[count++] = tok;
        tok = strtok(NULL, ",");
    }
    
    if(attributes != nullptr)
        delete attributes;
    attributes = new rmAttribute*[count];
    
    for(uint8_t i=0; i<count; i++)
        attributes[i] = cli->getAttribute(tokens[i]);*/
}
