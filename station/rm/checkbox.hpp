/**
 * @file checkbox.hpp
 * @brief A check box widget for the control system
 * 
 * The binary input or output widget.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_CHECKBOX_H__
#define __RM_CHECKBOX_H__ ///< Header guard

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

#include <wx/checkbox.h>


/**
 * @brief A check box widget for the control system
 * 
 * The binary input or output widget.
 */
class RM_WX_API rmCheckBox: public rmWidget, public wxCheckBox {
  private:
    bool useInput = true;
    
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
    rmCheckBox() = default;
    
    /**
     * @brief Constructs a check box widget
     * 
     * @param parent The parent window
     * @param cli The client
     * @param key Unique name of the attribute with maximum 11 characters
     * @param label The text to display next to the check box
     * @param en This states if the widget is to be used for input
     */
    rmCheckBox(wxWindow* parent, rmClient* cli, const char* key,
               const char* label, bool en=true);
    
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
     * @brief Triggers when the check box is checked or unchecked
     * 
     * @param evt The event object
     */
    virtual void onCheck(wxCommandEvent& evt);
};

#endif
