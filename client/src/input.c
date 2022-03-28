/**
 * @file input.c
 * @brief Handles the inputs from the station and the data storage for it
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#include "rm/attribute.h"

#include "rm/call.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>


typedef struct _rmInputAttribute {
    char name[12];
    void *data;
    int8_t type;
    float lowerBound;
    float upperBound;
    void (*onChange)();
} rmInputAttribute;


static rmInputAttribute* attributes = NULL;
static uint8_t count = 0;


static int8_t cmp = 0;


static uint8_t binarySearch(int8_t low, int8_t high, const char* key) {
    while(true) {
        if(low >= high) {
            cmp = strcmp(key, attributes[low].name);
            if(cmp > 0)
                return low + 1;
            else
                return low;
        }
        int8_t mid = low + (high - low) / 2;
        cmp = strcmp(key, attributes[mid].name);
        if(cmp == 0)
            return mid;
        else if(cmp < 0)
            high = mid - 1;
        else
            low = mid + 1;
    }
}


static rmInputAttribute* _rmInputAttributeGet(const char* key) {
    uint8_t pos = binarySearch(0, count - 1, key);
    if(cmp == 0)
        return &attributes[pos];
    return NULL;
}


static void callbackSet(int argc, char* argv[]);




/**
 * @brief Creates an input attribute and appends it to the list
 * 
 * @param key Unique name of the attribute (maximum length is 11)
 * @param ptr The pointer of the data which the key links with
 * @param t The data type
 */
void rmCreateInputAttribute(const char* key, void* ptr, int8_t t) {
    rmInputAttribute attr;
    strncpy(attr.name, key, 11);
    attr.name[11] = '\0';
    attr.data = ptr;
    attr.type = t;
    attr.lowerBound = NAN;
    attr.upperBound = NAN;
    attr.onChange = NULL;
    
    uint8_t pos = 0;
    if(count > 0)
        pos = binarySearch(0, count - 1, attr.name);
    
    size_t size = sizeof(rmInputAttribute) * (count + 1);
    rmInputAttribute* newArr = (rmInputAttribute*) malloc(size);
    for(uint8_t i=0; i<pos; i++) {
        newArr[i] = attributes[i];
    }
    for(uint8_t i=pos; i<count; i++) {
        newArr[i + 1] = attributes[i];
    }
    newArr[pos] = attr;
    count++;
    
    if(attributes != NULL)
        free(attributes);
    attributes = newArr;
    
    static bool init = false;
    if(!init)
        rmCreateCall("set", callbackSet);
}




static void attributeSetValue(rmInputAttribute* attr, const char* str) {
    bool changed = false;
    
    if(attr->type == RM_ATTRIBUTE_BOOL) {
        bool* b = (bool*) attr->data;
        if(str[0] == '0') {
            if(*b == true) {
                *b = false;
                changed = true;
            }
        }
        else if(str[0] == '1') {
            if(*b == false) {
                *b = true;
                changed = true;
            }
        }
    }
    
    else if(attr->type == RM_ATTRIBUTE_CHAR) {
        char* c = (char*) attr->data;
        if(str[0] != *c) {
            *c = str[0];
            changed = true;
        }
    }
    
    else if(attr->type == RM_ATTRIBUTE_STRING) {
        rmString* str1 = (rmString*) attr->data;
        if(strcmp(str, str1->data) != 0) {
            uint8_t maxLen = str1->size;
            strncpy(str1->data, str, maxLen);
            changed = true;
        }
    }
    
    else if(attr->type == RM_ATTRIBUTE_FLOAT) {
        float val = atof(str);
        if(!isnan(attr->lowerBound)) {
            if(val < attr->lowerBound)
                val = attr->lowerBound;
        }
        if(!isnan(attr->upperBound)) {
            if(val > attr->upperBound)
                val = attr->upperBound;
        }
        float* f = (float*) attr->data;
        if(val != *f) {
            *f = val;
            changed = true;
        }
    }
    
    else if(attr->type & 0b00010000) {
        int val = atoi(str);
        if(!isnan(attr->lowerBound)) {
            if(val < attr->lowerBound)
                val = attr->lowerBound;
        }
        if(!isnan(attr->upperBound)) {
            if(val > attr->upperBound)
                val = attr->upperBound;
        }
        if(attr->type & 0b00001000) {
            if(attr->type == RM_ATTRIBUTE_INT8) {
                int8_t* i = (int8_t*) attr->data;
                if(val != *i) {
                    *i = val;
                    changed = true;
                }
            }
            else if(attr->type == RM_ATTRIBUTE_INT16) {
                int16_t* i = (int16_t*) attr->data;
                if(val != *i) {
                    *i = val;
                    changed = true;
                }
            }
            else if(attr->type == RM_ATTRIBUTE_INT32) {
                int32_t* i = (int32_t*) attr->data;
                if(val != *i) {
                    *i = val;
                    changed = true;
                }
            }
        }
        else {
            if(attr->type == RM_ATTRIBUTE_UINT8) {
                uint8_t* i = (uint8_t*) attr->data;
                if(val != *i) {
                    *i = val;
                    changed = true;
                }
            }
            else if(attr->type == RM_ATTRIBUTE_UINT16) {
                uint16_t* i = (uint16_t*) attr->data;
                if(val != *i) {
                    *i = val;
                    changed = true;
                }
            }
            else if(attr->type == RM_ATTRIBUTE_UINT32) {
                uint32_t* i = (uint32_t*) attr->data;
                if(val != *i) {
                    *i = val;
                    changed = true;
                }
            }
        }
    }
    
    if(changed && attr->onChange != NULL)
        attr->onChange();
}


static void callbackSet(int argc, char* argv[]) {
    if(argc != 2)
        return;
    
    rmInputAttribute* attr = _rmInputAttributeGet(argv[0]);
    if(attr != NULL)
        attributeSetValue(attr, argv[1]);
}


/**
 * @brief Sets the boundary to constrain the input data
 * 
 * @param key The attribute name
 * @param lower Lower bound value
 * @param upper Upper bound value
 */
void rmInputAttributeSetBoundaries(const char* key, float lower, float upper) {
    rmInputAttribute* attr = _rmInputAttributeGet(key);
    if(lower < upper) {
        attr->lowerBound = lower;
        attr->upperBound = upper;
    }
}


/**
 * @brief Assigns the callback function which triggers upon data input
 * 
 * @param key The attribute name
 * @param func The callback function
 */
void rmInputAttributeSetOnChange(const char* key, void (*func)()) {
    rmInputAttribute* attr = _rmInputAttributeGet(key);
    attr->onChange = func;
}
