/**
 * @file gauge.cpp
 * @brief A gauge to display the real time value of an attribute
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_WX_EXPORT


#include "rm/gauge.hpp"


/**
 * @brief Gets an ID to use for constructing a wxWidget
 * 
 * @return wxWidget ID
 */
long rmGauge::getWxID() { 
    if(wx_id == 0)
        wx_id = wxNewId();
    return wx_id;
}

/**
 * @brief Constructs a gauge widget
 * 
 * @param parent The parent window
 * @param cli The client
 * @param key Unique name of the attribute with maximum 11 characters
 * @param range Upper bound value
 */
rmGauge::rmGauge(wxWindow* parent, rmClient* cli, const char* key, int range)
        :rmWidget(cli),
         wxGauge(parent, wx_id, range)
{
    attribute = client->createAttribute(key, RM_ATTRIBUTE_INT, 0, range);
    if(attribute != nullptr)
        attribute->setNotifier(this);
    Disable();
}

/**
 * @brief Constructs a gauge widget
 * 
 * @param parent The parent window
 * @param cli The client
 * @param key Unique name of the attribute with maximum 11 characters
 * @param range Upper bound value
 */
rmGauge::rmGauge(wxWindow* parent, rmClient* cli, const char* key,
                 float range)
        :rmWidget(cli),
         wxGauge(parent, wx_id, 100)
{
    attribute = client->createAttribute(key, RM_ATTRIBUTE_FLOAT, 0.0f, range);
    stepSize = range / 100.0f;
    if(attribute != nullptr)
        attribute->setNotifier(this);
    Disable();
}

/**
 * @brief Triggers on attribute value change
 * 
 * Triggers when the value of attribute is changed by the client device's
 * reports.
 */
void rmGauge::onAttributeChange() {
    if(attribute->getType() == RM_ATTRIBUTE_INT) {
        SetValue(attribute->getValue().i);
    }
    else if(attribute->getType() == RM_ATTRIBUTE_FLOAT) {
        float val = attribute->getValue().f;
        int step = (int) (val / stepSize);
        SetValue(step);
    }
}
