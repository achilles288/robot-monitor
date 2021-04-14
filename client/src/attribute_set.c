/**
 * @file attribute_set.c
 * @brief Sets the attribute value by string
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_EXPORT


#include "rm/attribute.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>


/**
 * @brief Sets the value
 * 
 * @param attr The attribute
 * @param value The string value or blob. The array is copied.
 */
void rmAttributeSetValue(rmAttribute* attr, const char* value) {
    if(attr->type == RM_ATTRIBUTE_BOOL) {
        if(strcmp(value, "false") == 0)
            attr->data.b = false;
        else if(strcmp(value, "true") == 0)
            attr->data.b = true;
    }
    else if(attr->type == RM_ATTRIBUTE_CHAR) {
        attr->data.c = value[0];
    }
    else if(attr->type == RM_ATTRIBUTE_INT) {
        int32_t i = atoi(value);
        if(!isnan(attr->lowerBound.f)) {
            if(i < attr->lowerBound.i)
                i = attr->lowerBound.i;
        }
        if(!isnan(attr->upperBound.f)) {
            if(i > attr->upperBound.i)
                i = attr->upperBound.i;
        }
        attr->data.i = i;
    }
    else if(attr->type == RM_ATTRIBUTE_FLOAT) {
        float f = atof(value);
        if(!isnan(attr->lowerBound.f)) {
            if(f < attr->lowerBound.f)
                f = attr->lowerBound.f;
        }
        if(!isnan(attr->upperBound.f)) {
            if(f > attr->upperBound.f)
                f = attr->upperBound.f;
        }
        attr->data.f = f;
    }
    else if(attr->type == RM_ATTRIBUTE_STRING) {
        if(attr->data.s != NULL) {
            if(strcmp(value, attr->data.s) == 0)
                return;
            free(attr->data.s);
        }
        size_t len = strnlen(value, 127);
        attr->data.s = (char*) malloc(len + 1);
        memcpy(attr->data.s, value, len);
        attr->data.s[len] = '\0';
    }
}
