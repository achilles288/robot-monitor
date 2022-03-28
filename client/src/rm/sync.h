/**
 * @file sync.h
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


#pragma once
#ifndef __RM_SYNC_H__
#define __RM_SYNC_H__ ///< Header guard


#include "attribute.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Creates a new sync table
 * 
 * @return ID of the newly created sync table
 */
uint8_t rmCreateSync();


/**
 * @brief Creates an output attribute and appends it to the sync table
 * 
 * @param key Unique name of the attribute (maximum length is 11)
 * @param ptr The pointer of the data which the key links with
 * @param t The data type
 * @param sync The sync table ID
 */
void rmCreateOutputAttribute(const char* key, void* ptr, int8_t t,
                             uint8_t sync_id);


/**
 * @brief Performs an update operation for all the attributes in the table
 * 
 * @param id The sync table ID
 */
void rmSyncUpdate(uint8_t id);


#ifdef __cplusplus
}
#endif

#endif
