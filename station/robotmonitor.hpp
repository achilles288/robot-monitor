/**
 * @file robotmonitor.hpp
 * @brief Monitors and commands the microcontrollers for robots
 * 
 * In programming an MCU for robots, it is usually needed to what how the data
 * input and output are going on. In the example of a line following robot, IR
 * sensor input values and PID controller output value are displayed by means
 * of text or graph. The Robot Monitor software make it possible to use
 * CLI-like interface, custom GUI for machine control, graphing and 3D
 * visualization.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __ROBOT_MONITOR_H__
#define __ROBOT_MONITOR_H__

#include "rm/attribute.hpp"
#include "rm/call.hpp"
#include "rm/client.hpp"

#ifndef RM_NO_WX
#include "rm/button.hpp"
#include "rm/checkbox.hpp"
#include "rm/gauge.hpp"
#include "rm/slider.hpp"
#include "rm/spinctrl.hpp"
#include "rm/stattext.hpp"
#include "rm/textctrl.hpp"
#endif

#endif
