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


#pragma once
#ifndef __RM_SLIDER_H__
#define __RM_SLIDER_H__ ///< Header guard

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

#include <wx/slider.h>


/**
 * @brief A slider widget for the control system
 * 
 * The input data for the attribute is number type and is bounded.
 */
class RM_WX_API rmSlider: public rmWidget, public wxSlider {
  private:
    float stepSize = 1.0f;
    
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
    rmSlider() = default;
    
    /**
     * @brief Constructs a slider widget
     * 
     * @param parent The parent window
     * @param cli The client
     * @param key Unique name of the attribute with maximum 11 characters
     * @param lower Lower bound value
     * @param upper Upper bound value
     */
    rmSlider(wxWindow* parent, rmClient* cli, const char* key, int lower,
             int upper);
    
    /**
     * @brief Constructs a slider widget
     * 
     * @param parent The parent window
     * @param cli The client
     * @param key Unique name of the attribute with maximum 11 characters
     * @param lower Lower bound value
     * @param upper Upper bound value
     */
    rmSlider(wxWindow* parent, rmClient* cli, const char* key, float lower,
             float upper);
    
    /**
     * @brief Enables or disables the user input
     * 
     * @param en True for enable and false for otherwise
     */
    void setEnabled(bool en) override;
    
    /**
     * @brief Triggers on attribute value change
     * 
     * Triggers when the value of attribute is changed by the client device's
     * reports.
     */
    virtual void onAttributeChange() override;
    
    /**
     * @brief Updates the attribute value along with the slider position
     * 
     * @param evt The event object
     */
    virtual void onSlide(wxCommandEvent& evt);
};

#endif
