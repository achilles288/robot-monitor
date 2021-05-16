/**
 * @file attribute_set_char.c
 * @brief Sets the attribute value by character
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
 * @param value The character value
 */
void rmAttributeSetChar(rmAttribute* attr, char value) {
    if(attr->data.c == value)
        return;
    attr->data.c = value;
    char msg[19];
    sprintf(msg, "set %s %c\n", attr->name, value);
    rmSendMessage(msg);
}
