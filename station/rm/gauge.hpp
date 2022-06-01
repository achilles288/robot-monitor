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
class RM_WX_API rmGauge: public rmWidget, public wxGauge {
  private:
    float stepSize = 0.0f;
    
  protected:
    /**
     * @brief Gets an ID to use for constructing a wxWidget
     * 
     * @return wxWidget ID
     */
    long getWxID() override;
    
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
     * @param range Upper bound value
     */
    rmGauge(wxWindow* parent, rmClient* cli, const char* key, int range);
    
    /**
     * @brief Constructs a gauge widget
     * 
     * @param parent The parent window
     * @param cli The client
     * @param key Unique name of the attribute with maximum 11 characters
     * @param range Upper bound value
     */
    rmGauge(wxWindow* parent, rmClient* cli, const char* key, float range);
    
    /**
     * @brief Triggers on attribute value change
     * 
     * Triggers when the value of attribute is changed by the client device's
     * reports.
     */
    virtual void onAttributeChange() override;
};

#endif
