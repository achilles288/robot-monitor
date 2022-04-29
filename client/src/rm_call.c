/**
 * @file call.h
 * @brief Calls that invoke on the station's request
 * 
 * The callback functions take the parameters only as an integer that
 * represents number of extra tokens and the array of strings which are those
 * tokens. This is similar to "int main(int argc, char* argv[])". The tokens
 * strings are to be converted into appropriate data types within the callback.
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#include "rm/call.h"
#include "call_private.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>


static rmCall* calls = NULL;
static uint8_t count = 0;


static int8_t cmp = 0;


static uint8_t binarySearch(int8_t low, int8_t high, const char* key) {
    while(1) {
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
    return 0;
}


rmCall* _rmCallGet(const char* key) {
    uint8_t pos = binarySearch(0, count - 1, key);
    if(cmp == 0)
        return &calls[pos];
    return NULL;
}


/**
 * @brief Creates a named callback and appends it to the list
 * 
 * @param key Unique name of the call (maximum length is 11)
 * @param func The callback function. The callback should have two parameters,
 *        an integer representing the number of extra tokens and the array of
 *        strings.
 */
void rmCreateCall(const char* key, void (*func)(int, char**)) {
    rmCall call;
    strncpy(call.name, key, 11);
    call.name[11] = '\0';
    call.callback = func;
    
    uint8_t pos = 0;
    if(count > 0)
        pos = binarySearch(0, count - 1, call.name);
    
    size_t size = sizeof(rmCall) * (count + 1);
    rmCall* newArr = (rmCall*) malloc(size);
    for(uint8_t i=0; i<pos; i++) {
        newArr[i] = calls[i];
    }
    for(uint8_t i=pos; i<count; i++) {
        newArr[i + 1] = calls[i];
    }
    newArr[pos] = call;
    count++;
    
    if(calls != NULL)
        free(calls);
    calls = newArr;
}
