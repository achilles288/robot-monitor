/**
 * @file echobox.hpp
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


#pragma once
#ifndef __RM_ECHOBOX_H__
#define __RM_ECHOBOX_H__ ///< Header guard

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


#include "client.hpp"

#include <wx/textctrl.h>


/**
 * @brief A text box widget that shows the console output messages
 * 
 * Intended to be used by the 'echo' command of the client device. This command
 * is usually to print normal output messages other than attribute changes and
 * functional callbacks.
 */
class RM_WX_API rmEchoBox: public rmEcho, public wxTextCtrl {
  private:
    int prevColor = 0;
    
  public:
    /**
     * @brief Default constructor
     */
    rmEchoBox() = default;
    
    /**
     * @brief Constructs a echo box widget
     * 
     * @param parent The parent window
     * @param cli The client
     */
    rmEchoBox(wxWindow* parent, rmClient* cli);
    
    /**
     * @brief Echos the messages
     * 
     * Intended to be called from 'echo' command by the client device. This
     * command is usually to print normal output messages other than attribute
     * changes and functional callbacks.
     * 
     * @param msg The message
     * @param status The status code. Status code other than 0 may print red
     *         messages.
     */
    void echo(const char* msg, int status=0) override;
    
    /**
     * @brief Enables or disables the user input
     * 
     * @param en True for enable and false for otherwise
     */
    void setEnabled(bool en) override;
};

#endif
