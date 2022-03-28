/**
 * @file sync.c
 * @brief Handles the data synchronization between client and station
 * 
 * The data to be updated on the station are the output attributes. This module
 * updates a good number of attributes at the same time in the most efficient
 * way.
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#include "rm/sync.h"

#include "connection_private.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct _rmSync {
    rmOutputAttribute* attributes;
    uint8_t count;
} rmSync;


static rmSync* syncTables = NULL;
static uint8_t count = 0;


/**
 * @brief Creates a new sync table
 * 
 * @return ID of the newly created sync table
 */
uint8_t rmCreateSync() {
    size_t size = sizeof(rmSync) * (count + 1);
    syncTables = (rmSync*) realloc(syncTables, size);
    syncTables[count].attributes = NULL;
    syncTables[count].count = 0;
    return count++;
}


/**
 * @brief Creates an output attribute and appends it to the sync table
 * 
 * @param key Unique name of the attribute (maximum length is 11)
 * @param ptr The pointer of the data which the key links with
 * @param t The data type
 * @param sync_id The sync table ID
 */
void rmCreateOutputAttribute(const char* key, void* ptr, int8_t t,
                             uint8_t sync_id)
{
    rmOutputAttribute attr;
    strncpy(attr.name, key, 11);
    attr.name[11] = '\0';
    attr.data = ptr;
    attr.type = t;
    rmSync* sync = &syncTables[sync_id];
    size_t size = sizeof(rmOutputAttribute) * (sync->count + 1);
    sync->attributes = (rmOutputAttribute*) realloc(sync->attributes, size);
    sync->attributes[sync->count] = attr;
    sync->count++;
}


/**
 * @brief Performs an update operation for all the attributes in the table
 * 
 * @param id The sync table ID
 */
void rmSyncUpdate(uint8_t id) {
    if(id >= count)
        return;
    
    char msg[256];
    uint8_t len;
    len = sprintf(msg, "$sync %d ", id);
    rmSync* sync = &syncTables[id];
    
    for(uint8_t i=0; i<sync->count; i++) {
        char* str = rmOutputAttributeGetStringData(&sync->attributes[i]);
        uint8_t n = strlen(str);
        if(n > 254 - len)
            n = 254 - len;
        memcpy(msg + len, str, n);
        len += n;
        if(i < sync->count - 1 && len < 254)
            msg[len++] = ','; 
    }
    msg[len++] = '\n';
    msg[len++] = '\0';
    rmSendMessage(msg);
}
