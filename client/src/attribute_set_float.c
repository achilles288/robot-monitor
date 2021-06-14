/**
 * @file attribute_set_float.c
 * @brief Sets the attribute value by floating point number
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
 * @param value The floating point value
 */
void rmAttributeSetFloat(rmAttribute* attr, float value) {
    float prev = attr->data.f;
    attr->data.f = value;
    if(!isnan(attr->lowerBound.f)) {
        if(attr->data.f < attr->lowerBound.f)
            attr->data.f = attr->lowerBound.f;
    }
    if(!isnan(attr->upperBound.f)) {
        if(attr->data.f > attr->upperBound.f)
            attr->data.f = attr->upperBound.f;
    }
    
    if(attr->data.f == prev)
        return;
    
    char msg[33];
    int d = (int) value;
    int rem = (int) ((value - d) * 1000);
    if(rem < 0)
        rem *= -1;
    sprintf(msg, "set %s %d.%03u\n", attr->name, d, rem);
    rmSendMessage(msg);
}
