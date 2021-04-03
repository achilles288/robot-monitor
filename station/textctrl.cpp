/**
 * @file textctrl.cpp
 * @brief A text box widget for the control system
 * 
 * The input data for the attribute is not bounded.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_WX_EXPORT


#include "rm/textctrl.hpp"


/**
 * @brief Constructs a text box widget
 * 
 * @param parent The parent window
 * @param cli The client
 * @param key Unique name of the attribute with maximum 11 characters
 * @param t Data type of the value stored
 */
rmTextCtrl::rmTextCtrl(wxWindow* parent, rmClient* cli, const char* key,
                       int8_t t)
           :rmWidget(cli),
            wxTextCtrl(parent, wx_id, wxEmptyString)
{
    attribute = client->createAttribute(key, t);
    if(attribute != nullptr) {
        attribute->setNotifier(this);
        wxTextCtrl::Connect(
            wx_id, wxEVT_COMMAND_TEXT_UPDATED,
            (wxObjectEventFunction)&rmTextCtrl::onUpdate
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
void rmTextCtrl::setEnabled(bool en) {
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
void rmTextCtrl::onAttributeChange() {
    SetValue(wxString(attribute->getValueString()));
}

/**
 * @brief Updates the attribute value along with what is in the text box
 * 
 * @param evt The event object
 */
void rmTextCtrl::onUpdate(wxCommandEvent& evt) {
    attribute->setValue(GetValue().mb_str());
    client->sendAttribute(attribute);
}
