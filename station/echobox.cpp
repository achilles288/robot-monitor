/**
 * @file echobox.cpp
 * @brief A text box widget that shows the console output messages
 * 
 * Intended to be used by the 'echo' command of the client device. This command
 * is usually to print normal output messages other than attribute changes and
 * functional callbacks.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_WX_EXPORT


#include "rm/echobox.hpp"


/**
 * @brief Gets an ID to use for constructing a wxWidget
 * 
 * @return wxWidget ID
 */
long rmEchoBox::getWxID() { 
    if(wx_id == 0)
        wx_id = wxNewId();
    return wx_id;
}

/**
 * @brief Constructs a echo box widget
 * 
 * @param parent The parent window
 * @param cli The client
 */
rmEchoBox::rmEchoBox(wxWindow* parent, rmClient* cli)
          :wxTextCtrl(parent, getWxID(), wxEmptyString, wxDefaultPosition,
                      wxDefaultSize, wxTE_MULTILINE)
{
    SetCanFocus(false);
    SetEditable(false);
    cli->setEcho(this);
    Disable();
}

/**
 * @brief Echos the messages
 * 
 * Intended to be called from 'echo' command by the client device. This command
 * is usually to print normal output messages other than attribute changes and
 * functional callbacks.
 * 
 * @param msg The message
 * @param status The status code. Status code other than 0 may print red
 *         messages.
 */
void rmEchoBox::echo(const char* msg, int status) {/*
    AppendText(wxString(msg));
    AppendText(wxT("\n"));
    long pos = GetLastPosition();
    if(pos > 1024)*/
        ;//Remove(0, pos - 1024);
}

/**
 * @brief Enables or disables the user input
 * 
 * @param en True for enable and false for otherwise
 */
void rmEchoBox::setEnabled(bool en) { Enable(en); }
