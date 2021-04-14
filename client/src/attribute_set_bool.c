/**
 * @file attribute_set_bool.c
 * @brief Sets the attribute value by boolean
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_EXPORT


#include "rm/attribute.h"

#include "rm/connection.h"

#include <stdio.h>


/**
 * @brief Sets the value
 * 
 * Calling this function triggers the set command to the station.
 * 
 * @param attr The attribute
 * @param value The boolean value
 */
void rmAttributeSetBool(rmAttribute* attr, bool value) {
    if(attr->data.b == value)
        return;
    attr->data.b = value;
    char msg[23];
    if(value)
        snprintf(msg, 22, "set %s true\n", attr->name);
    else
        snprintf(msg, 22, "set %s false\n", attr->name);
    msg[22] = '\0';
    rmSendMessage(msg);
}
