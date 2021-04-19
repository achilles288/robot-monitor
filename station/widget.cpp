/**
 * @file widget.cpp
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
 * @brief Gets an ID to use for constructing a wxWidget
 * 
 * @return wxWidget ID
 */
long rmWidget::getWxID() { return wx_id; }

/**
 * @brief Constructs with a client object
 * 
 * @param cli The client
 */
rmWidget::rmWidget(rmClient* cli) {
    wx_id = getWxID();
    client = cli;
    client->appendWidget(this);
}

/**
 * @brief Enables or disables the user input
 * 
 * @param en True for enable and false for otherwise
 */
void rmWidget::setEnabled(bool en) {}
