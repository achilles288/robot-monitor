/**
 * @file sync.hpp
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

#ifndef RM_API
#ifdef _WIN32
#ifdef RM_EXPORT
#define RM_API __declspec(dllexport) ///< API
#else
#define RM_API __declspec(dllimport) ///< API
#endif
#else
#define RM_API ///< API
#endif
#endif


/**
 * @brief Handles the data synchronization between client and station
 * 
 * The data to be updated on the station are the output attributes. This module
 * updates a good number of attributes at the same time in the most efficient
 * way.
 */
class RM_API rmSync {
  private:
    uint8_t id = 0;
    rmAttribute* attributes = nullptr;
    size_t count = 0;
    
  public:
    /**
     * @brief Default constructor
     */
    rmSync() = default;
    
    /**
     * @brief Constructs the sync table with an index
     * 
     * @param i ID or index
     */
    rmSync(uint8_t i);
    
    /**
     * @breif Retrive the list of attributes to work in a sync
     */
    void updateList();
    
    /**
     * @breif Updates the attribute values
     * 
     * @param str The string representing the values of the list
     */
    void onSync(const char* str);
};

#endif
