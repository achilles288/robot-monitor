/**
 * @file button.hpp
 * @brief A button widget
 * 
 * Sends a command to the client on clicking the button. To use this widget,
 * the client must have a call constructed and the widget must be constructed
 * with the same name parameter. Only then, the appropriate callback is
 * triggered on the client side on each click on the widget of the station.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_BUTTON_H__
#define __RM_BUTTON_H__ ///< Header guard

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

#include <wx/button.h>


/**
 * @brief A button widget
 * 
 * Sends a command to the client on clicking the button. To use this widget,
 * the client must have a call constructed and the widget must be constructed
 * with the same name parameter. Only then, the appropriate callback is
 * triggered on the client side on each click on the widget of the station.
 */
class RM_WX_API rmButton: public rmWidget, public wxButton {
  private:
    char command[128] = {0};
    
  protected:
    /**
     * @brief Gets an ID to use for constructing a wxWidget
     * 
     * @return wxWidget ID
     */
    long getWxID() override;
    
    /**
     * @brief Sends  a message to the client device
     * 
     * @param msg Message string
     * @param crypt True to encrypt the message if the connection supports it
     */
    void sendMessage(const char* msg, bool crypt=true);
    
  public:
    /**
     * @brief Default constructor
     */
    rmButton() = default;
    
    /**
     * @brief Constructs a button widget
     * 
     * @param parent The parent window
     * @param cli The client
     * @param cmd The command or call name declared on the client
     * @param label Text to display on the button
     */
    rmButton(wxWindow* parent, rmClient* cli, const char* cmd,
             const char* label);
    
    /**
     * @brief Enables or disables the user input
     * 
     * @param en True for enable and false for otherwise
     */
    void setEnabled(bool en) override;
    
    /**
     * @brief Sends a command to the client on clicking the button
     * 
     * @param evt The event object
     */
    virtual void onClick(wxCommandEvent& evt);
};

#endif
