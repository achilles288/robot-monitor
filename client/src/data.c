/**
 * @file data.c
 * @brief Process the command lines and stores the data attributes
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_EXPORT


#include "rm/attribute.h"
#include "rm/call.h"
#include "rm/connection.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>


static rmAttribute* attributes = NULL;
static uint8_t attrCount = 0;
static rmCall* calls = NULL;
static uint8_t callCount = 0;


static int8_t cmp = 0;

static uint8_t binarySearch1(int8_t low, int8_t high, const char* key) {
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

static uint8_t binarySearch2(int8_t low, int8_t high, const char* key) {
    while(true) {
        if(low >= high) {
            cmp = strcmp(key, calls[low].name);
            if(cmp > 0)
                return low + 1;
            else
                return low;
        }
        int8_t mid = low + (high - low) / 2;
        cmp = strcmp(key, calls[mid].name);
        if(cmp == 0)
            return mid;
        else if(cmp < 0)
            high = mid - 1;
        else
            low = mid + 1;
    }
}


/**
 * @brief Creates an attribute and stored in the list
 * 
 * @param key Unique name of the attribute with maximum 11 characters
 * @param t Data type of the value stored
 * 
 * @return The newly created attribute. Null if the attribute with the same
 *         name already exists or the creation is invalid.
 */
rmAttribute* _rmCreateAttribute(const char* key, int8_t t)
{
    rmAttribute attr;
    strncpy(attr.name, key, 11);
    attr.name[11] = '\0';
    attr.data.s = NULL;
    attr.type = t;
    attr.lowerBound.f = NAN;
    attr.upperBound.f = NAN;
    attr.onChange = NULL;
    
    uint8_t pos = 0;
    if(attrCount > 0)
        pos = binarySearch1(0, attrCount - 1, attr.name);
    
    size_t size = sizeof(rmAttribute) * (attrCount + 1);
    rmAttribute* newArr = (rmAttribute*) malloc(size);
    for(uint8_t i=0; i<pos; i++) {
        newArr[i] = attributes[i];
    }
    for(uint8_t i=pos; i<attrCount; i++) {
        newArr[i + 1] = attributes[i];
    }
    newArr[pos] = attr;
    attrCount++;
    
    if(attributes != NULL)
        free(attributes);
    attributes = newArr;
    
    return &attributes[pos];
}

/**
 * @brief Looks for an attribute by name
 * 
 * @param key Unique name
 * 
 * @return Requested attribute. Null if the request is unavailable.
 */
rmAttribute* _rmGetAttribute(const char* key) {
    uint8_t pos = binarySearch1(0, attrCount - 1, key);
    if(cmp == 0)
        return &attributes[pos];
    return NULL;
}

/**
 * @brief Creates a call in the map structure
 * 
 * @param key Unique name of the attribute with maximum 11 characters
 * @param func The callback function. The callback should have two parameters,
 *        an integer representing the number of extra tokens and the array of
 *        strings.
 * 
 * @return The newly created call. Null if the call with the same name
 *         already exists or the creation is invalid.
 */
rmCall* _rmCreateCall(const char* key, void (*func)(int, char**)) {
    rmCall call;
    strncpy(call.name, key, 11);
    call.name[11] = '\0';
    call.callback = func;
    
    uint8_t pos = 0;
    if(callCount > 0)
        pos = binarySearch2(0, callCount - 1, call.name);
    
    size_t size = sizeof(rmCall) * (callCount + 1);
    rmCall* newArr = (rmCall*) malloc(size);
    for(uint8_t i=0; i<pos; i++) {
        newArr[i] = calls[i];
    }
    for(uint8_t i=pos; i<callCount; i++) {
        newArr[i + 1] = calls[i];
    }
    newArr[pos] = call;
    callCount++;
    
    if(calls != NULL)
        free(calls);
    calls = newArr;
    
    return &calls[pos];
}

static void builtinCallSet(int argc, char* argv[]) {
    if(argc != 2)
        return;
    
    rmAttribute* attr = _rmGetAttribute(argv[0]);
    if(attr != NULL) {
        rmAttributeData prev = attr->data;
        rmAttributeSetValue(attr, argv[1]);
        if(attr->data.i != prev.i) {
            if(attr->onChange != NULL)
                attr->onChange();
        }
    }
}

/**
 * @brief Looks for a call by name
 * 
 * @param key Unique name
 * 
 * @return Requested call. Null if the request is unavailable.
 */
rmCall* _rmGetCall(const char* key) {
    static bool addedBuiltins = false;
    if(!addedBuiltins) {
        _rmCreateCall("set", &builtinCallSet);
        addedBuiltins = true;
    }
    
    uint8_t pos = binarySearch2(0, callCount - 1, key);
    if(cmp == 0)
        return &calls[pos];
    return NULL;
}

/**
 * @brief Reads a message and process the command line
 */
void rmReadMessage() {
    static char cmd[256];
    static char* tokens[4];
    static uint8_t i = 0;
    static uint8_t tokenCount = 0;
    static bool space = false;
    static rmCall* call = NULL;
    
    char c = rmRead();
    while(c != '\0') {
        if(i == 255)
            c = '\n';
        
        switch(c) {
          case ' ':
            if(!space) {
                cmd[i++] = '\0';
                space = true;
            }
            break;
            
          case '\n':
            cmd[i] = '\0';
            call = _rmGetCall(cmd);
            if(call != NULL)
                call->callback(tokenCount, tokens);
            i = 0;
            tokenCount = 0;
            space = false;
            break;
            
          default:
            if(space) {
                tokens[tokenCount++] = cmd + i;
                space = false;
            }
            cmd[i++] = c;
        }
        c = rmRead();
    }
}
