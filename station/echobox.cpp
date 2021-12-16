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
 * @brief Constructs a echo box widget
 * 
 * @param parent The parent window
 * @param cli The client
 */
rmEchoBox::rmEchoBox(wxWindow* parent, rmClient* cli)
          :wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition,
                      wxDefaultSize, wxTE_MULTILINE | wxTE_RICH)
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
void rmEchoBox::echo(const char* msg, int status) {
    int c;
    wxColor color;
    if(status == 0) {
        color = wxNullColour;
        c = 0;
    }
    else if(status == 2) {
        color = wxColour(230, 173, 0);
        c = 2;
    }
    else {
        color = wxColour(230, 0, 0);
        c = 1;
    }
    
    if(c != prevColor) {
        SetDefaultStyle(wxTextAttr(color));
        prevColor = c;
    }
    
    AppendText(wxString(msg));
    AppendText(wxT("\n"));
    int ln = GetNumberOfLines();
    if(ln > 32)
        Remove(0, GetLineLength(0) + 1);
}

/**
 * @brief Enables or disables the user input
 * 
 * @param en True for enable and false for otherwise
 */
void rmEchoBox::setEnabled(bool en) { Enable(en); }
