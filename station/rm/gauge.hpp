/**
 * @file gauge.hpp
 * @brief A gauge to display the real time value of an attribute
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_GAUGE_H__
#define __RM_GAUGE_H__ ///< Header guard

#ifndef RM_WX_API
#ifdef _WIN32
#ifdef RM_WX_EXPORT
#define RM_WX_API __declspec(dllexport) ///< API
#else
#define RM_WX_API __declspec(dllimport) ///< API
#endif
#else
#define RM_WX_API ///< API
#endif
#endif


#include "widget.hpp"

#include <wx/gauge.h>


/**
 * @brief A gague to display the real time value of an attribute
 */
class RM_API rmGauge: public rmWidget, public wxGauge {
  private:
    float stepSize = 0.0f;
    
  public:
    /**
     * @brief Default constructor
     */
    rmGauge() = default;
    
    /**
     * @brief Constructs a gauge widget
     * 
     * @param parent The parent window
     * @param cli The client
     * @param key Unique name of the attribute with maximum 11 characters
     * @param lower Lower bound value
     * @param upper Upper bound value
     */
    rmGauge(wxWindow* parent, rmClient* cli, const char* key, int32_t lower,
            int32_t upper);
    
    /**
     * @brief Constructs a gauge widget
     * 
     * @param parent The parent window
     * @param cli The client
     * @param key Unique name of the attribute with maximum 11 characters
     * @param lower Lower bound value
     * @param upper Upper bound value
     */
    rmGauge(wxWindow* parent, rmClient* cli, const char* key, float lower,
            float upper);
    
    /**
     * @brief Triggers on attribute value change
     * 
     * Triggers when the value of attribute is changed by the client device's
     * reports.
     */
    void onAttributeChange() override;
};

#endif
