/**
 * @file button.cpp
 * @brief A button widget
 * 
 * Sends a command to the client on clicking the button
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_WX_EXPORT


#include "rm/button.hpp"


/**
 * @brief Gets an ID to use for constructing a wxWidget
 * 
 * @return wxWidget ID
 */
long rmButton::getWxID() { 
    if(wx_id == 0)
        wx_id = wxNewId();
    return wx_id;
}

/**
 * @brief Constructs a button widget
 * 
 * @param parent The parent window
 * @param cli The client
 * @param cmd The command or call name declared on the client
 * @param label Text to display on the button
 */
rmButton::rmButton(wxWindow* parent, rmClient* cli, const char* cmd,
                   const char* label)
         :wxButton(parent, wx_id, wxString(label))
{
    strncpy(command, cmd, 11);
    command[11] = '\0';
    Connect(
        wx_id,
        wxEVT_BUTTON,
        wxCommandEventHandler(rmButton::onClick),
        NULL,
        this
    );
}

/**
 * @brief Enables or disables the user input
 * 
 * @param en True for enable and false for otherwise
 */
void rmButton::setEnabled(bool en) { Enable(en); }

/**
 * @brief Sends a command to the client on clicking the button
 * 
 * @param evt The event object
 */
void rmButton::onClick(wxCommandEvent& evt) {
    client->sendMessage(command);
}
