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
    switch(attr->type) {
      rmAttributeData d;
      case RM_ATTRIBUTE_BOOL:
        if(strcmp(value, "false") == 0)
            attr->data.b = false;
        else if(strcmp(value, "true") == 0)
            attr->data.b = true;
        break;
        
      case RM_ATTRIBUTE_CHAR:
        attr->data.c = value[0];
        break;
        
      case RM_ATTRIBUTE_INT:
        d.i = atoi(value);
        if(!isnan(attr->lowerBound.f)) {
            if(d.i < attr->lowerBound.i)
                d.i = attr->lowerBound.i;
        }
        if(!isnan(attr->upperBound.f)) {
            if(d.i > attr->upperBound.i)
                d.i = attr->upperBound.i;
        }
        attr->data.i = d.i;
        break;
        
      case RM_ATTRIBUTE_FLOAT:
        d.f = atof(value);
        if(!isnan(attr->lowerBound.f)) {
            if(d.f < attr->lowerBound.f)
                d.f = attr->lowerBound.f;
        }
        if(!isnan(attr->upperBound.f)) {
            if(d.f > attr->upperBound.f)
                d.f = attr->upperBound.f;
        }
        attr->data.f = d.f;
        break;
        
      case RM_ATTRIBUTE_STRING:
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
