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
    
    size_t size = sizeof(rmAttribute) * (attrCount + 1);
    rmAttribute* newArr = (rmAttribute*) malloc(size);
    for(uint8_t i=0; i<attrCount; i++)
        newArr[i] = attributes[i];
    newArr[attrCount++] = attr;
    
    if(attributes != NULL)
        free(attributes);
    attributes = newArr;
    
    return &attributes[attrCount - 1];
}

/**
 * @brief Looks for an attribute by name
 * 
 * @param key Unique name
 * 
 * @return Requested attribute. Null if the request is unavailable.
 */
rmAttribute* _rmGetAttribute(const char* key) {
    for(uint8_t i=0; i<attrCount; i++) {
        if(strcmp(attributes[i].name, key) == 0)
            return &attributes[i];
    }
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
    
    size_t size = sizeof(rmCall) * (callCount + 1);
    rmCall* newArr = (rmCall*) malloc(size);
    for(uint8_t i=0; i<callCount; i++)
        newArr[i] = calls[i];
    newArr[callCount++] = call;
    
    if(calls != NULL)
        free(calls);
    calls = newArr;
    
    return &calls[callCount - 1];
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
    
    for(uint8_t i=0; i<callCount; i++) {
        if(strcmp(calls[i].name, key) == 0)
            return &calls[i];
    }
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
        
        if(c == ' ') {
            if(!space) {
                cmd[i] = '\0';
                space = true;
            }
        }
        else if(c == '\n') {
            cmd[i] = '\0';
            call = _rmGetCall(cmd);
            if(call != NULL)
                call->callback(tokenCount, tokens);
            i = 0;
            tokenCount = 0;
            space = false;
            c = rmRead();
            continue;
        }
        else {
            cmd[i] = c;
            if(space) {
                tokens[tokenCount++] = cmd + i;
                space = false;
            }
        }
        c = rmRead();
        i++;
    }
}
