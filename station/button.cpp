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
 * @brief Constructs a button widget
 * 
 * @param parent The parent window
 * @param cli The client
 * @param cmd The command or call name declared on the client
 * @param label Text to display on the button
 */
rmButton::rmButton(wxWindow* parent, rmClient* cli, const char* cmd,
                   const char* label)
         :rmWidget(cli),
          wxButton(parent, wx_id, wxString(label))
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
