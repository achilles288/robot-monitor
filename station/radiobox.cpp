/**
 * @file radiobox.hpp
 * @brief Selection of an option from the radio box
 * 
 * Sends the selection of the option as an index number.
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_WX_EXPORT


#include "rm/radiobox.hpp"


 /**
  * @brief Gets an ID to use for constructing a wxWidget
  *
  * @return wxWidget ID
  */
long rmRadioBox::getWxID() {
    if (wx_id == 0)
        wx_id = wxNewId();
    return wx_id;
}

/**
 * @brief Constructs a box of radio button widgets
 * 
 * @param parent The parent window
 * @param cli The client
 * @param key Unique name of the attribute with maximum 11 characters
 * @param count Number of choices in the radio box
 * @param choices The group of choices as a string array
 */
rmRadioBox::rmRadioBox(wxWindow* parent, rmClient* cli, const char* key,
                       int count, const wxString choices[])
           :rmWidget(cli),
            wxRadioBox(parent, wx_id, wxEmptyString, wxDefaultPosition,
                       wxDefaultSize, count, choices, 0, wxRA_SPECIFY_ROWS)
{
    attribute = client->createAttribute(key, RM_ATTRIBUTE_INT);
    if(attribute != nullptr) {
        attribute->setNotifier(this);
        Connect(
            wx_id,
            wxEVT_RADIOBOX,
            wxCommandEventHandler(rmRadioBox::onRadio),
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
void rmRadioBox::setEnabled(bool en) {
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
void rmRadioBox::onAttributeChange() {
    int i = attribute->getValue().i;
    if(i > 0 && i < (int)GetCount())
        SetSelection(i);
}

/**
 * @brief Triggers when a radio button is selected
 * 
 * @param evt The event object
 */
void rmRadioBox::onRadio(wxCommandEvent& evt) {
    int i = GetSelection();
    if(i != wxNOT_FOUND) {
        attribute->setValue(i);
        client->updateAttribute(attribute);
    }
}
