/**
 * @file spinctrl.hpp
 * @brief A integer box widget for the control system
 * 
 * The input data for the attribute is integer type and is bounded. Spin control
 * looks like a text box with up and down arrow buttons next to it.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_SPINCTRL_H__
#define __RM_SPINCTRL_H__ ///< Header guard

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

#include <wx/spinctrl.h>


/**
 * @brief A integer box widget for the control system
 * 
 * The input data for the attribute is integer type and is bounded. Spin control
 * looks like a text box with up and down arrow buttons next to it.
 */
class RM_WX_API rmSpinCtrl: public rmWidget, public wxSpinCtrl {
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
    rmSpinCtrl() = default;
    
    /**
     * @brief Constructs a spin control widget
     * 
     * @param parent The parent window
     * @param cli The client
     * @param key Unique name of the attribute with maximum 11 characters
     * @param lower Lower bound value
     * @param upper Upper bound value
     */
    rmSpinCtrl(wxWindow* parent, rmClient* cli, const char* key,
               int32_t lower, int32_t upper);
    
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
    void onAttributeChange() override;
    
    /**
     * @brief Updates the attribute value as the number is changed
     * 
     * @param evt The event object
     */
    void onUpdate(wxSpinEvent& event);
};

#endif
