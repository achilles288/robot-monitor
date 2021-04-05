/**
 * @file widget1.cpp
 * @brief The base class of the widgets for robot monitor
 * 
 * The widgets are for wxWidgets library. The functions here are to make the
 * widgets sync with the client device's status.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_EXPORT


#include "rm/widget.hpp"


/**
 * @brief Enables or disables the user input
 * 
 * @param en True for enable and false for otherwise
 */
void rmWidget::setEnabled(bool en) {}
