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
#include "rm/request.hpp"

#include <cstdio>
#include <cstring>


static void responseCallback(rmResponse resp);

    
/**
 * @brief Destructor
 */
rmSync::~rmSync() {
    if(attributes != nullptr)
        delete attributes;
}

/**
 * @brief Gets the attribute count in the table
 * 
 * @return The number of attributes in the sync table
 */
size_t rmSync::getCount() const { return count; }

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
        if(attr != nullptr) {
            rmAttributeData prev = attr->getValue();
            attr->setValue(token);
            if(attr->getValue().i != prev.i) {
                rmAttributeNotifier* noti = attr->getNotifier();
                if(noti != nullptr)
                    noti->onAttributeChange();
            }
        }
        token = strtok(NULL, ",");
    }
}


#define MAX_ATTRIBUTES_PER_SYNC 32


/**
 * @breif Retrive the list of attributes to work in a sync
 * 
 * @param str The string containing the name of every attribute
 * @param cli The client instance
 */
void rmSync::updateList(const char* str, rmClient* cli) {
    char buffer[256];
    strncpy(buffer, str, 255);
    buffer[255] = '\0';
    char *tokens[MAX_ATTRIBUTES_PER_SYNC];
    char *tok = strtok(buffer, ",");
    count = 0;
    
    while(tok != NULL && count < MAX_ATTRIBUTES_PER_SYNC) {
        tokens[count++] = tok;
        tok = strtok(NULL, ",");
    }
    
    if(attributes != nullptr)
        delete attributes;
    attributes = new rmAttribute*[count];
    
    for(uint8_t i=0; i<count; i++)
        attributes[i] = cli->getAttribute(tokens[i]);
}
