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
 * @brief Constructs a gauge widget
 * 
 * @param parent The parent window
 * @param cli The client
 * @param key Unique name of the attribute with maximum 11 characters
 * @param lower Lower bound value
 * @param upper Upper bound value
 */
rmGauge::rmGauge(wxWindow* parent, rmClient* cli, const char* key,
                 int32_t lower, int32_t upper)
        :rmWidget(cli),
         wxGauge(parent, wx_id, upper - lower)
{
    attribute = client->createAttribute(key, RM_ATTRIBUTE_INT, lower, upper);
    if(attribute != nullptr)
        attribute->setNotifier(this);
}

/**
 * @brief Constructs a gauge widget
 * 
 * @param parent The parent window
 * @param cli The client
 * @param key Unique name of the attribute with maximum 11 characters
 * @param lower Lower bound value
 * @param upper Upper bound value
 */
rmGauge::rmGauge(wxWindow* parent, rmClient* cli, const char* key,
                 float lower, float upper)
        :rmWidget(cli),
         wxGauge(parent, wx_id, 100)
{
    attribute = client->createAttribute(key, RM_ATTRIBUTE_FLOAT, lower, upper);
    stepSize = (upper - lower) / 100.0f;
    if(attribute != nullptr)
        attribute->setNotifier(this);
}

/**
 * @brief Triggers on attribute value change
 * 
 * Triggers when the value of attribute is changed by the client device's
 * reports.
 */
void rmGauge::onAttributeChange() {
    if(attribute->getType() == RM_ATTRIBUTE_INT) {
        int32_t val = attribute->getValue().i;
        int32_t a = attribute->getLowerBound().i;
        SetValue(val - a);
    }
    else if(attribute->getType() == RM_ATTRIBUTE_FLOAT) {
        float val = attribute->getValue().f;
        float a = attribute->getLowerBound().f;
        int step = (int) ((val - a) / stepSize);
        SetValue(step);
    }
}
