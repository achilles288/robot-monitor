/**
 * @file widget.hpp
 * @brief The base class of the widgets for robot monitor
 * 
 * The widgets are for wxWidgets library. The functions here are to make the
 * widgets sync with the client device's status.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_WIDGET_H__
#define __RM_WIDGET_H__ ///< Header guard

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


class rmWidget;


#include "client.hpp"


/**
 * @brief The base class of the widgets for robot monitor
 * 
 * The widgets are for wxWidgets library. The functions here are to make the
 * widgets sync with the client device's status.
 */
class RM_API rmWidget: public rmAttributeNotifier {
  protected:
    int wx_id = 0; ///< The ID for a wxWidget
    rmClient* client = nullptr; ///< The client associated with the widget
    
  public:
    /**
     * @brief Default constructor
     */
    rmWidget() = default;
    
    /**
     * @brief Constructs with a client object
     * 
     * @param cli The client
     */
    rmWidget(rmClient* cli);
    
    /**
     * @brief Destructor
     */
    virtual ~rmWidget();
    
    /**
     * @brief Enables or disables the user input
     * 
     * @param en True for enable and false for otherwise
     */
    virtual void setEnabled(bool en);
};

#endif
