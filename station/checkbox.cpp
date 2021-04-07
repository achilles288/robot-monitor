/**
 * @file checkbox.cpp
 * @brief A check box widget for the control system
 * 
 * The binary input or output widget.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_WX_EXPORT


#include "rm/checkbox.hpp"


 /**
  * @brief Gets an ID to use for constructing a wxWidget
  *
  * @return wxWidget ID
  */
long rmCheckBox::getWxID() {
    if (wx_id == 0)
        wx_id = wxNewId();
    return wx_id;
}

/**
 * @brief Constructs a check box widget
 * 
 * @param parent The parent window
 * @param cli The client
 * @param key Unique name of the attribute with maximum 11 characters
 * @param label The text to display next to the check box
 * @param en This states if the widget is to be used for input
 */
rmCheckBox::rmCheckBox(wxWindow* parent, rmClient* cli, const char* key,
                       const char* label, bool en)
           :rmWidget(cli),
            wxCheckBox(parent, wx_id, wxString(label))
{
    attribute = client->createAttribute(key, RM_ATTRIBUTE_BOOL);
    useInput = en;
    if(attribute != nullptr) {
        attribute->setNotifier(this);
        if(useInput) {
            Connect(
                wx_id,
                wxEVT_CHECKBOX,
                wxCommandEventHandler(rmCheckBox::onCheck),
                NULL,
                this
            );
        }
        else
            Disable();
    }
    else
        Disable();
}

/**
 * @brief Enables or disables the user input
 * 
 * @param en True for enable and false for otherwise
 */
void rmCheckBox::setEnabled(bool en) {
    if(attribute != nullptr && useInput)
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
void rmCheckBox::onAttributeChange() {
    SetValue(attribute->getValue().b);
}

/**
 * @brief Triggers when the check box is checked or unchecked
 * 
 * @param evt The event object
 */
void rmCheckBox::onCheck(wxCommandEvent& evt) {
    attribute->setValue(IsChecked());
    client->sendAttribute(attribute);
}
