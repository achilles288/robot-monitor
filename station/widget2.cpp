/**
 * @file widget2.cpp
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

#include <wx/utils.h>


/**
 * @brief Constructs with a client object
 * 
 * @param cli The client
 */
rmWidget::rmWidget(rmClient* cli) {
    wx_id = wxNewId();
    client = cli;
    client->appendWidget(this);
}

/**
 * @brief Destructor
 */
rmWidget::~rmWidget() {
    client->removeWidget(this);
    if(attribute != nullptr)
        client->removeAttribute(attribute->getName());
}
