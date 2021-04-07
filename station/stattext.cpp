/**
 * @file stattext.cpp
 * @brief A label to display the real time status of the client device
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_WX_EXPORT


#include "rm/stattext.hpp"


/**
 * @brief Gets an ID to use for constructing a wxWidget
 * 
 * @return wxWidget ID
 */
long rmStaticText::getWxID() { 
    if(wx_id == 0)
        wx_id = wxNewId();
    return wx_id;
}

/**
 * @brief Constructs a label widget
 * 
 * @param parent The parent window
 * @param cli The client
 * @param key Unique name of the attribute with maximum 11 characters
 * @param label The text displayed along side the attribute value
 * @param t Data type of the value stored
 */
rmStaticText::rmStaticText(wxWindow* parent, rmClient* cli, const char* key,
                           const char* label, int8_t t)
             :rmWidget(cli),
              wxStaticText(parent, wx_id, wxEmptyString)
{
    attribute = client->createAttribute(key, t);
    strncpy(this->label, label, 23);
    this->label[23] = '\0';
    wxString wstr;
    if(attribute != nullptr) {
        attribute->setNotifier(this);
        std::string val_str = attribute->getValueString();
        wstr = wxString::Format(wxT("%s: %s"), label, val_str);
    }
    else {
        wstr = wxString::Format(wxT("%s:"), label);
    }
    SetLabel(wstr);
}

/**
 * @brief Triggers on attribute value change
 * 
 * Triggers when the value of attribute is changed by the client device's
 * reports.
 */
void rmStaticText::onAttributeChange() {
    std::string val_str = attribute->getValueString();
    SetLabel(wxString::Format(wxT("%s: %s"), label, val_str));
}
