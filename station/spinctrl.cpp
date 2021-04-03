/**
 * @file spinctrl.cpp
 * @brief A integer box widget for the control system
 * 
 * The input data for the attribute is integer type and is bounded. Spin control
 * looks like a text box with up and down arrow buttons next to it.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_WX_EXPORT


#include "rm/spinctrl.hpp"

#include <wx/string.h>


/**
 * @brief Constructs a spin ctrl widget
 * 
 * @param parent The parent window
 * @param cli The client
 * @param key Unique name of the attribute with maximum 11 characters
 * @param lower Lower bound value
 * @param upper Upper bound value
 */
rmSpinCtrl::rmSpinCtrl(wxWindow* parent, rmClient* cli, const char* key,
                       int32_t lower, int32_t upper)
           :rmWidget(cli),
            wxSpinCtrl(parent, wx_id, wxEmptyString)
{
    attribute = client->createAttribute(key, RM_ATTRIBUTE_INT, lower, upper);
    if(attribute != nullptr) {
        attribute->setNotifier(this);
        wxSpinCtrl::Connect(
            wx_id, wxEVT_SPINCTRL,
            (wxObjectEventFunction)&rmSpinCtrl::onUpdate
        );
    }
    else
        Disable();
}

/**
 * @brief Enables or disables the user input
 * 
 * @param en True for enable and false for otherwise
 */
void rmSpinCtrl::setEnabled(bool en) {
    if(attribute != nullptr)
        Enable(en);
    else
        Disable();
}

/**
 * @brief Triggers on attribute value change
 * 
 * Triggers when the value of attribute is changed by the client device's
 * reports.
 */
void rmSpinCtrl::onAttributeChange() {
    SetValue(attribute->getValue().i);
}

/**
 * @brief Updates the attribute value as the number is changed
 * 
 * @param evt The event object
 */
void rmSpinCtrl::onUpdate(wxSpinEvent& evt) {
    attribute->setValue(GetValue());
    client->sendAttribute(attribute);
}
