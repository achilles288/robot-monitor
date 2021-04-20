/**
 * @file timerbase.hpp
 * @brief the timer to handle the idle function of the client
 * 
 * This is used to replace the default extra thread that runs parallel with the
 * main thread. May also execute extra functions while processing the idle
 * function.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_TIMERBASE_H__
#define __RM_TIMERBASE_H__ ///< Header guard

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


class rmTimerBase;


#include "client.hpp"

#include <vector>


/**
 * @brief the timer to handle the idle function of the client
 * 
 * This is used to replace the default extra thread that runs parallel with the
 * main thread. May also execute extra functions while processing the idle
 * function.
 */
class RM_API rmTimerBase {
  protected:
    std::vector<rmClient*> clients; ///< Client devices that need processing
    long interval = 10; ///< Time interval in milliseconds
    
  public:
    /**
     * @brief Default constructor
     */
    rmTimerBase() = default;
    
    /**
     * @brief Constructor with a time interval
     * 
     * @param ms Time interval in milliseconds
     */
    rmTimerBase(long ms);
    
    /**
     * @brief Adds a client to the list to process
     * 
     * @param cli The client device
     */
    void appendClient(rmClient* cli);
    
    /**
     * @brief Removes a client from the list to process
     * 
     * @param cli The client device
     */
    void removeClient(rmClient* cli);
    
    /**
     * @brief Starts the timer
     * 
     * @param ms Time interval in milliseconds
     */
    virtual void start(long ms);
    
    /**
     * @brief Stops the timer
     */
    virtual void stop();
};

#endif
