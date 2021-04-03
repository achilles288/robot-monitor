/**
 * @file stattext.hpp
 * @brief A label to display the real time status of the client device
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_STATTEXT_H__
#define __RM_STATTEXT_H_ ///< Header guard

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

#include <wx/stattext.h>


/**
 * @brief A label to display the real time status of the client device
 */
class RM_API rmStaticText: public rmWidget, public wxStaticText {
  private:
    char label[24];
    
  public:
    /**
     * @brief Default constructor
     */
    rmStaticText() = default;
    
    /**
     * @brief Constructs a label widget
     * 
     * @param parent The parent window
     * @param cli The client
     * @param key Unique name of the attribute with maximum 11 characters
     * @param label The text displayed along side the attribute value
     * @param t Data type of the value stored
     */
    rmStaticText(wxWindow* parent, rmClient* cli, const char* key,
                 const char* label, int8_t t);
    
    /**
     * @brief Triggers on attribute value change
     * 
     * Triggers when the value of attribute is changed by the client device's
     * reports.
     */
    void onAttributeChange() override;
};

#endif
