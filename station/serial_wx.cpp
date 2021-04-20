/**
 * @file serial_wx.cpp
 * @brief Serial ports to communicate with client devices
 * 
 * Port detection thread made specifically for wxWidgets.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_WX_EXPORT


#include "rm/serial.hpp"

#include <wx/timer.h>


class rmPortDetectionTimer;

static rmPortDetectionTimer* timer;
static wxEvtHandler* handler;
static void (wxEvtHandler::*callback)(wxEvent&) = nullptr;


class rmPortDetectionTimer: public wxTimer {
  private:
    size_t prevCount = 0;
    rmSerialPortList ports;
    
  public:
    rmPortDetectionTimer() {
        Connect(
            wxEVT_TIMER,
            wxTimerEventHandler(rmPortDetectionTimer::onTimer),
            (wxObject*) &ports,
            this
        );
    }
    
    void onTimer(wxTimerEvent& evt) {
        ports = rmSerialPort::listPorts();
        if(ports.size() != prevCount) {
            (handler->*callback)(evt);
            prevCount = ports.size();
        }
    }
};


void rmSetOnPortDetectedWx(void (wxEvtHandler::*func)(wxEvent&),
                           wxEvtHandler* h)
{
    bool toStart = (callback == nullptr);
    callback = func;
    handler = h;
    if(toStart) {
        timer = new rmPortDetectionTimer();
        timer->Start(200);
    }
}
