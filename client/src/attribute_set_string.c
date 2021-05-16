/**
 * @file attribute_set_string.c
 * @brief Sets the attribute value by string
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_EXPORT


#include "rm/attribute.h"

#include "rm/connection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * @brief Sets the value
 * 
 * Calling this function triggers the set command to the station.
 * 
 * @param attr The attribute
 * @param value The string value
 */
void rmAttributeSetString(rmAttribute* attr, const char* value) {
    if(attr->data.s != NULL) {
        if(strcmp(value, attr->data.s) == 0)
            return;
        free(attr->data.s);
    }
    size_t len = strnlen(value, 127);
    attr->data.s = (char*) malloc(len + 1);
    memcpy(attr->data.s, value, len);
    attr->data.s[len] = '\0';
    
    char msg[160];
    sprintf(msg, "set %s \"%s\"\n", attr->name, attr->data.s);
    rmSendMessage(msg);
}
