/**
 * @file radiobox.hpp
 * @brief Selection of an option from the radio box
 * 
 * Sends the selection of the option as an index number.
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_RADIOBOX_H__
#define __RM_RADIOBOX_H__ ///< Header guard

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

#include <wx/radiobox.h>


/**
 * @brief Selection of an option from the radio box
 * 
 * Sends the selection of the option as an index number.
 */
class RM_WX_API rmRadioBox: public rmWidget, public wxRadioBox {
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
    rmRadioBox() = default;
    
    /**
     * @brief Constructs a box of radio button widgets
     * 
     * @param parent The parent window
     * @param cli The client
     * @param key Unique name of the attribute with maximum 11 characters
     * @param count Number of choices in the radio box
     * @param choices The group of choices as a string array
     */
    rmRadioBox(wxWindow* parent, rmClient* cli, const char* key,
               int count, const wxString choices[]);
    
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
     * @brief Triggers when a radio button is selected
     * 
     * @param evt The event object
     */
    virtual void onRadio(wxCommandEvent& evt);
};

#endif
