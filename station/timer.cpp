/**
 * @file timer.cpp
 * @brief the wxTimer to handle the idle function of the client
 * 
 * This is used to replace the default extra thread that runs parallel with the
 * main thread. wxWidgets require its own type of timer to process the idle
 * function. If the default multithreading is used, there will be data races
 * and conflict with data and event systems of wxWidgets.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_EXPORT


#include "rm/timer.hpp"


long rmTimer::getWxID() { 
    if(wx_id == 0)
        wx_id = wxNewId();
    return wx_id;
}

/**
 * @brief Default constructor
 */
rmTimer::rmTimer()
        :wxTimer(this, getWxID())
{
    Connect(
        wxEVT_TIMER,
        wxTimerEventHandler(rmTimer::onTimer),
        NULL,
        this
    );
}

/**
 * @brief Starts the timer
 * 
 * @param ms Time interval in milliseconds
 */
void rmTimer::start(long ms) { Start(ms); }

/**
 * @brief Stops the timer
 */
void rmTimer::stop() { Stop(); }

/**
 * @brief Timer function to be executed in each interval
 * 
 * @param evt Timer event object
 */
void rmTimer::onTimer(wxTimerEvent& evt) {
    if(clients.size() == 0) {
        Stop();
        return;
    }
    auto vec = clients;
    
    for(auto it=vec.begin(); it!=vec.end(); it++) {
        if((*it)->isConnected() == false) {
            (*it)->echo("Port disconnected", 1);
            removeClient(*it);
            (*it)->onDisconnected();
        }
        (*it)->onIdle();
    }
}
