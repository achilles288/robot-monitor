/**
 * @file timerbase.cpp
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


#define RM_EXPORT
#define RM_NO_WX


#include "rm/timerbase.hpp"

#include <algorithm>


/**
 * @brief Constructor with a time interval
 * 
 * @param ms Time interval in milliseconds
 */
rmTimerBase::rmTimerBase(long ms) {
    interval = ms;
}

/**
 * @brief Adds a client to the list to process
 * 
 * @param cli The client device
 */
void rmTimerBase::appendClient(rmClient* cli) {
    auto it = std::find(clients.begin(), clients.end(), cli);
    if(it != clients.end()) {
        return;
    }
    bool toStart = (clients.size() == 0);
    clients.push_back(cli);
    if(toStart)
        start(interval);
}

/**
 * @brief Removes a client from the list to process
 * 
 * @param cli The client device
 */
void rmTimerBase::removeClient(rmClient* cli) {
    auto it = std::find(clients.begin(), clients.end(), cli);
    if(it != clients.end())
        clients.erase(it);
    bool toStop = (clients.size() == 0);
    if(toStop)
        stop();
}

/**
 * @brief Starts the timer
 * 
 * @param ms Time interval in milliseconds
 */
void rmTimerBase::start(long ms) {}

/**
 * @brief Stops the timer
 */
void rmTimerBase::stop() {}
