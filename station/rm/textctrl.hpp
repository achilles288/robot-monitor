/**
 * @file textctrl.hpp
 * @brief A text box widget for the control system
 * 
 * The input data for the attribute is not bounded.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_TEXTCTRL_H__
#define __RM_TEXTCTRL_H__ ///< Header guard

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

#include <wx/textctrl.h>


/**
 * @brief A text box widget for the control system
 * 
 * The input data for the attribute is not bounded.
 */
class RM_API rmTextCtrl: public rmWidget, public wxTextCtrl {
  public:
    /**
     * @brief Default constructor
     */
    rmTextCtrl() = default;
    
    /**
     * @brief Constructs a text box widget
     * 
     * @param parent The parent window
     * @param cli The client
     * @param key Unique name of the attribute with maximum 11 characters
     * @param t Data type of the value stored
     */
    rmTextCtrl(wxWindow* parent, rmClient* cli, const char* key, int8_t t);
    
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
     * @brief Updates the attribute value along with what is in the text box
     * 
     * @param evt The event object
     */
    void onUpdate(wxCommandEvent& evt);
};

#endif
