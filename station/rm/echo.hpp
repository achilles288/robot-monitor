/**
 * @file echo.hpp
 * @brief To print regular messages for the client object
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
#ifndef __RM_ECHO_H__
#define __RM_ECHO_H__ ///< Header guard

#ifndef RM_API
#ifdef _WIN32
#ifdef RM_EXPORT
#define RM_API __declspec(dllexport) ///< API
#else
#define RM_API __declspec(dllimport) ///< API
#endif
#else
#define RM_API ///< API
#endif
#endif


/**
 * @brief To print regular messages for the client object
 * 
 * Intended to be used by the 'echo' command of the client device. This command
 * is usually to print normal output messages other than attribute changes and
 * functional callbacks.
 */
class RM_API rmEcho {
  public:
    /**
     * @brief Default constructor
     */
    rmEcho() = default;
    
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
    virtual void echo(const char* msg, int status=0);
    
    /**
     * @brief Enables or disables the user input
     * 
     * @param en True for enable and false for otherwise
     */
    virtual void setEnabled(bool en);
};

#endif
