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
 * @brief Gets an ID to use for constructing a wxWidget
 * 
 * @return wxWidget ID
 */
long rmTextCtrl::getWxID() { 
    if(wx_id == 0)
        wx_id = wxNewId();
    return wx_id;
}

/**
 * @brief Constructs a text box widget
 * 
 * @param parent The parent window
 * @param cli The client
 * @param key Unique name of the attribute with maximum 11 characters
 * @param t Data type of the value stored
 */
rmTextCtrl::rmTextCtrl(wxWindow* parent, rmClient* cli, const char* key,
                       rmAttributeDataType t)
           :rmWidget(cli),
            wxTextCtrl(parent, wx_id, wxEmptyString, wxDefaultPosition,
                       wxDefaultSize, wxTE_PROCESS_ENTER)
{
    attribute = client->createAttribute(key, t);
    if(attribute != nullptr) {
        attribute->setNotifier(this);
        Connect(
            wx_id,
            wxEVT_TEXT_ENTER,
            wxCommandEventHandler(rmTextCtrl::onEnter),
            NULL,
            this
        );
        Connect(
            wx_id,
            wxEVT_KILL_FOCUS,
            wxFocusEventHandler(rmTextCtrl::onFocusLoss),
            NULL,
            this
        );
    }
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
    ChangeValue(wxString(attribute->getValueString()));
}

/**
 * @brief Updates the attribute value along with what is in the text box
 * 
 * @param evt The event object
 */
void rmTextCtrl::onEnter(wxCommandEvent& evt) {
    std::string val = GetValue().ToStdString();
    if(strncmp(val.c_str(), lastVal, 127) != 0) {
        attribute->setValue(val.c_str());
        client->updateAttribute(attribute);
        strncpy(lastVal, val.c_str(), 127);
        lastVal[127] = '\0';
    }
}

/**
 * @brief Updates the attribute value along with what is in the text box
 * 
 * @param evt The event object
 */
void rmTextCtrl::onFocusLoss(wxFocusEvent& evt) {
    std::string val = GetValue().ToStdString();
    if(strncmp(val.c_str(), lastVal, 127) != 0) {
        attribute->setValue(val.c_str());
        client->updateAttribute(attribute);
        strncpy(lastVal, val.c_str(), 127);
        lastVal[127] = '\0';
    }
}
