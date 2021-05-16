/**
 * @file attribute_set_int.c
 * @brief Sets the attribute value by integer
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_EXPORT


#include "rm/attribute.h"

#include "rm/connection.h"

#include <math.h>
#include <stdio.h>


/**
 * @brief Sets the value
 * 
 * Calling this function triggers the set command to the station.
 * 
 * @param attr The attribute
 * @param value The integer value
 */
void rmAttributeSetInt(rmAttribute* attr, int32_t value) {
    int32_t prev = attr->data.i;
    attr->data.i = value;
    if(!isnan(attr->lowerBound.f)) {
        if(attr->data.i < attr->lowerBound.i)
            attr->data.i = attr->lowerBound.i;
    }
    if(!isnan(attr->upperBound.f)) {
        if(attr->data.i > attr->upperBound.i)
            attr->data.i = attr->upperBound.i;
    }
    
    if(attr->data.i == prev)
        return;
    
    char msg[29];
    sprintf(msg, "set %s %ld\n", attr->name, attr->data.i);
    rmSendMessage(msg);
}
