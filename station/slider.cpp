/**
 * @file slider.hpp
 * @brief A slider widget for the control system
 * 
 * The input data for the attribute is number type and is bounded.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_WX_EXPORT


#include "rm/slider.hpp"

#include <wx/slider.h>
#include <wx/time.h>


/**
 * @brief Constructs a slider widget
 * 
 * @param parent The parent window
 * @param cli The client
 * @param key Unique name of the attribute with maximum 11 characters
 * @param lower Lower bound value
 * @param upper Upper bound value
 */
rmSlider::rmSlider(wxWindow* parent, rmClient* cli, const char* key, 
                   int32_t lower, int32_t upper)
         :rmWidget(cli),
          wxSlider(parent, wx_id, lower, lower, upper, wxDefaultPosition,
                   wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator,
                   wxString(key))
{
    attribute = client->createAttribute(key, RM_ATTRIBUTE_INT, lower, upper);
    if(attribute != nullptr) {
        SetClientData((void*) this);
        wxSlider::Connect(
            wx_id, wxEVT_SLIDER,
            (wxObjectEventFunction)&rmSlider::onSlide
        );
    }
    else
        Disable();
}

/**
 * @brief Constructs a slider widget
 * 
 * @param parent The parent window
 * @param cli The client
 * @param key Unique name of the attribute with maximum 11 characters
 * @param lower Lower bound value
 * @param upper Upper bound value
 */
rmSlider::rmSlider(wxWindow* parent, rmClient* cli, const char* key,
                   float lower, float upper)
         :rmWidget(cli),
          wxSlider(parent, wx_id, 0, 0, 100)
{
    attribute = client->createAttribute(key, RM_ATTRIBUTE_FLOAT, lower, upper);
    stepSize = (upper - lower) / 100.0f;
    if(attribute != nullptr) {
        attribute->setNotifier(this);
        Connect(
            wx_id,
            wxEVT_SLIDER,
            wxCommandEventHandler(rmSlider::onSlide),
            NULL,
            this
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
void rmSlider::setEnabled(bool en) {
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
void rmSlider::onAttributeChange() {
    if(attribute->getType() == RM_ATTRIBUTE_INT) {
        SetValue(attribute->getValue().i);
    }
    else if(attribute->getType() == RM_ATTRIBUTE_FLOAT) {
        float val = attribute->getValue().f;
        float a = attribute->getLowerBound().f;
        int step = (int) ((val - a) / stepSize);
        SetValue(step);
    }
}

/**
 * @brief Updates the attribute value along with the slider position
 * 
 * @param evt The event object
 */
void rmSlider::onSlide(wxCommandEvent& evt) {
    static wxLongLong t1 = 0;
    wxLongLong t2 = wxGetLocalTimeMillis();
    if(t2 - t1 < 100)
        return;
    t1 = t2;
    
    int8_t type = attribute->getType();
    if(type == RM_ATTRIBUTE_INT) {
        attribute->setValue(GetValue());
    }
    else if(type == RM_ATTRIBUTE_FLOAT) {
        float a = attribute->getLowerBound().f;
        float val = a + GetValue() * stepSize;
        attribute->setValue(val);
    }
    else {
        return;
    }
    client->sendAttribute(attribute);
}
