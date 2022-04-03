/**
 * @file output.c
 * @brief Attribute to string conversion functions for the output messages
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#include "rm/attribute.h"

#include "connection_private.h"

#include <stdio.h>


/**
 * @brief Sends the key and the present data of the attribute to the station
 * 
 * The key and the data is sent as an update command through a connection
 * which is USB or an RF device. This command updates the client's data on the
 * station side.
 * 
 * @param attr The attribute
 */
void rmOutputAttributeUpdate(rmOutputAttribute *attr) {
    char msg[128];
    char* str = rmOutputAttributeGetStringData(attr);
    snprintf(msg, 128, "$set %s %s\n", attr->name, str);
    rmSendMessage(msg);
}


static char buffer[20];


/**
 * @brief Converts the output attribute data to a string
 * 
 * @param attr The attribute
 * 
 * @return String representation of the attribute data. The string returned
 *         must be used or copied before the next function call since it is
 *         allocated in a temporary memory.
 */
char* rmOutputAttributeGetStringData(rmOutputAttribute *attr) {
    if(attr->type == RM_ATTRIBUTE_BOOL) {
        bool* b = (bool*) attr->data;
        if(*b)
            buffer[0] = '1';
        else
            buffer[0] = '0';
        buffer[1] = '\0';
    }
    
    else if(attr->type == RM_ATTRIBUTE_CHAR) {
        char* c = (char*) attr->data;
        buffer[0] = *c;
        buffer[1] = '\0';
    }
    
    else if(attr->type == RM_ATTRIBUTE_STRING) {
        rmString* str = (rmString*) attr->data;
        return str->data;
    }
    
    else if(attr->type == RM_ATTRIBUTE_FLOAT) {
        float* f = (float*) attr->data;
        int d = (int) *f;
        uint16_t rem;
        if(d >= 1000) {
            rem = (int) ((*f - d) * 1000);
            if(rem < 0)
                rem *= -1;
            sprintf(buffer, "%d.%03u", d, rem);
        }
        else {
            rem = (int) ((*f - d) * 1000000);
            if(rem < 0)
                rem *= -1;
            sprintf(buffer, "%d.%06u", d, rem);
        }
    }
    
    else if(attr->type & 0b00010000) {
        if(attr->type & 0b00001000) {
            if(attr->type == RM_ATTRIBUTE_INT8) {
                int8_t* i = (int8_t*) attr->data;
                sprintf(buffer, "%d", *i);
            }
            else if(attr->type == RM_ATTRIBUTE_INT16) {
                int16_t* i = (int16_t*) attr->data;
                sprintf(buffer, "%d", *i);
            }
            else if(attr->type == RM_ATTRIBUTE_INT32) {
                int32_t* i = (int32_t*) attr->data;
                sprintf(buffer, "%ld", (long) *i);
            }
        }
        else {
            if(attr->type == RM_ATTRIBUTE_UINT8) {
                uint8_t* i = (uint8_t*) attr->data;
                sprintf(buffer, "%u", *i);
            }
            else if(attr->type == RM_ATTRIBUTE_UINT16) {
                uint16_t* i = (uint16_t*) attr->data;
                sprintf(buffer, "%u", *i);
            }
            else if(attr->type == RM_ATTRIBUTE_UINT32) {
                uint32_t* i = (uint32_t*) attr->data;
                sprintf(buffer, "%lu", (unsigned long) *i);
            }
        }
    }
    return buffer;
}
