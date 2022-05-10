/**
 * @file section.h
 * @brief Allows user to change the widgets on section changes
 * 
 * Puts a good number of widgets into different groups. Upon selecting a
 * section on the window, it shows the widgets of the group related to the
 * section and hides those of the others. In other words, this acts like the
 * pages of a website.
 * 
 * @copyright Copyright (c) 2022 Satan
 */


#pragma once
#ifndef __SECTION_H__
#define __SECTION_H__

#include <rm/client.hpp>
#include <wx/sizer.h>


class Section {
  protected:
    wxBoxSizer* sizerFocus = nullptr;
    wxBoxSizer* sizerWidgets = nullptr;
    rmClient* client;
    bool shown;
    
  public:
    Section(rmClient* cli);
    
    virtual void show(bool b=true);
};

#endif
