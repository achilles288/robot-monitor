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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
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
    char msg[128];
    char* str = rmOutputAttributeGetStringData(attr);
    snprintf(msg, 128, "$set %s %s\n", attr->name, str);
    _rmSendMessage(msg);
}


#define BUFFER_SIZE 48

static char strBuffer[BUFFER_SIZE + 1];
static uint8_t strIndex = 0;


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
    char* ptr = &strBuffer[strIndex];
    uint8_t size = BUFFER_SIZE - strIndex;
    if(size < 1) {
        strIndex = 0;
        ptr = &strBuffer[0];
    }
    
    if(attr->type == RM_ATTRIBUTE_BOOL) {
        bool* b = (bool*) attr->data;
        if(*b)
            ptr[0] = '1';
        else
            ptr[0] = '0';
        ptr[1] = '\0';
        strIndex += 2;
    }
    
    else if(attr->type == RM_ATTRIBUTE_CHAR) {
        char* c = (char*) attr->data;
        ptr[0] = *c;
        ptr[1] = '\0';
        strIndex += 2;
    }
    
    else if(attr->type == RM_ATTRIBUTE_STRING) {
        rmString* str = (rmString*) attr->data;
        return str->data;
    }
    
    else if(attr->type == RM_ATTRIBUTE_FLOAT) {
        float* f = (float*) attr->data;
        return rmFloatToString(*f);
    }
    
    else if(attr->type & 0b00010000) {
        if(size < 11) {
            strIndex = 0;
            ptr = &strBuffer[0];
        }
        uint8_t len = 0;
        if(attr->type & 0b00001000) {
            if(attr->type == RM_ATTRIBUTE_INT8) {
                int8_t* i = (int8_t*) attr->data;
                len = sprintf(ptr, "%d", *i);
            }
            else if(attr->type == RM_ATTRIBUTE_INT16) {
                int16_t* i = (int16_t*) attr->data;
                len = sprintf(ptr, "%d", *i);
            }
            else if(attr->type == RM_ATTRIBUTE_INT32) {
                int32_t* i = (int32_t*) attr->data;
                len = sprintf(ptr, "%ld", (long) *i);
            }
        }
        else {
            if(attr->type == RM_ATTRIBUTE_UINT8) {
                uint8_t* i = (uint8_t*) attr->data;
                len = sprintf(ptr, "%u", *i);
            }
            else if(attr->type == RM_ATTRIBUTE_UINT16) {
                uint16_t* i = (uint16_t*) attr->data;
                len = sprintf(ptr, "%u", *i);
            }
            else if(attr->type == RM_ATTRIBUTE_UINT32) {
                uint32_t* i = (uint32_t*) attr->data;
                len = sprintf(ptr, "%lu", (unsigned long) *i);
            }
        }
        strIndex = strIndex + len + 1;
    }
    return ptr;
}


/**
 * @brief Converts the floating point number to a string
 * 
 * @param f The floating point value
 * 
 * @return String representation of the float number. The string returned
 *         must be used or copied before some next function calls since it is
 *         allocated in a temporary memory.
 */
char* rmFloatToString(float f) {
    char* ptr = &strBuffer[strIndex];
    uint8_t size = BUFFER_SIZE - strIndex;
    if(size < 11) {
        strIndex = 0;
        ptr = &strBuffer[0];
    }
    
    uint8_t i = 0;
    if(f < 0) {
        ptr[i++] = '-';
        f = -f;
    }
    uint32_t d = abs((int) f);
    if(f < 0.001f) {
        float q = f;
        int8_t p = 0;
        do {
            q *= 10;
            p--;
        } while(q < 1.0f);
        uint8_t s = (uint8_t) q;
        uint16_t r = (uint16_t) ((q - s + 0.0005f) * 1000);
        i += sprintf(&ptr[i], "%u.%03ue%d", s, r, p);
    }
    else if(d == 0) {
        uint32_t rem = (uint32_t) ((f - d + 0.0000005f) * 1e6);
        i += sprintf(&ptr[i], "0.%06lu", rem);
    }
    else if(d < 1000) {
        uint32_t rem = (uint32_t) ((f - d + 0.0000005f) * 1e6);
        i += sprintf(&ptr[i], "%lu.%06lu", d, rem);
    }
    else if(d < 1000000) {
        uint16_t rem = (uint16_t) ((f - d + 0.0005f) * 1000);
        i += sprintf(&ptr[i], "%lu.%03u", d, rem);
    }
    else if(!isnan(f)) {
        float q = f;
        int8_t p = 0;
        do {
            q /= 10;
            p++;
        } while(q >= 10.0f);
        uint8_t s = (uint8_t) q;
        uint16_t r = (uint16_t) ((q - s + 0.0005f) * 1000);
        i += sprintf(&ptr[i], "%u.%03ue%d", s, r, p);
    }
    else {
        strcpy(&ptr[i], "nan");
        i += 3;
    }
    strIndex = strIndex + i + 1;
    return ptr;
}
