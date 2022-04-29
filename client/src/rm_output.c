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

#include <string.h>


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
    char msg[128] = "$set ";
    char* str = rmOutputAttributeGetStringData(attr);
    strcpy(msg, attr->name);
    strcpy(msg, " ");
    strcpy(msg, str);
    strcpy(msg, "\n");
    _rmSendMessage(msg);
}


char* rm_itoa(int i);
char* rm_ftoa(float f);

static char charStr[2] = {'x', '\0'};


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
    char* ptr = NULL;
    
    if(attr->type == RM_ATTRIBUTE_BOOL) {
        bool* b = (bool*) attr->data;
        if(*b)
            charStr[0] = '1';
        else
            charStr[0] = '0';
        ptr = charStr;
    }
    
    else if(attr->type == RM_ATTRIBUTE_CHAR) {
        char* c = (char*) attr->data;
        charStr[0] = *c;
        ptr = charStr;
    }
    
    else if(attr->type == RM_ATTRIBUTE_STRING) {
        rmString* str = (rmString*) attr->data;
        ptr = str->data;
    }
    
    else if(attr->type == RM_ATTRIBUTE_FLOAT) {
        float* f = (float*) attr->data;
        ptr = rm_ftoa(*f);
    }
    
    else if(attr->type & 0b00010000) {
        if(attr->type & 0b00001000) {
            if(attr->type == RM_ATTRIBUTE_INT8) {
                int8_t* i = (int8_t*) attr->data;
                ptr = rm_itoa(*i);
            }
            else if(attr->type == RM_ATTRIBUTE_INT16) {
                int16_t* i = (int16_t*) attr->data;
                ptr = rm_itoa(*i);
            }
            else if(attr->type == RM_ATTRIBUTE_INT32) {
                int32_t* i = (int32_t*) attr->data;
                ptr = rm_itoa(*i);
            }
        }
        else {
            if(attr->type == RM_ATTRIBUTE_UINT8) {
                uint8_t* i = (uint8_t*) attr->data;
                ptr = rm_itoa(*i);
            }
            else if(attr->type == RM_ATTRIBUTE_UINT16) {
                uint16_t* i = (uint16_t*) attr->data;
                ptr = rm_itoa(*i);
            }
            else if(attr->type == RM_ATTRIBUTE_UINT32) {
                uint32_t* i = (uint32_t*) attr->data;
                ptr = rm_itoa(*i);
            }
        }
    }
    return ptr;
}
