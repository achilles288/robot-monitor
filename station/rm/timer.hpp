/**
 * @file timer.hpp
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


#pragma once
#ifndef __RM_TIMER_H__
#define __RM_TIMER_H__ ///< Header guard

#ifndef RM_WX_API
#ifdef _WIN32
#ifdef RM_WX_EXPORT
#define RM_WX_API __declspec(dllexport) ///< API
#else
#define RM_WX_API __declspec(dllimport) ///< API
#endif
#else
#define RM_WX_API ///< API
#endif
#endif


#include "timerbase.hpp"

#include <wx/timer.h>


/**
 * @brief the wxTimer to handle the idle function of the client
 * 
 * This is used to replace the default extra thread that runs parallel with the
 * main thread. wxWidgets require its own type of timer to process the idle
 * function. If the default multithreading is used, there will be data races
 * and conflict with data and event systems of wxWidgets.
 */
class RM_API rmTimer: public rmTimerBase, public wxTimer {
  private:
    long wx_id = 0;
    
    long getWxID();
    
  public:
    /**
     * @brief Default constructor
     */
    rmTimer();
    
    /**
     * @brief Starts the timer
     * 
     * @param ms Time interval in milliseconds
     */
    void start(long ms) override;
    
    /**
     * @brief Stops the timer
     */
    void stop() override;
    
    /**
     * @brief Timer function to be executed in each interval
     * 
     * @param evt Timer event object
     */
    void onTimer(wxTimerEvent& evt);
};

#endif
