/**
 * @file section.cpp
 * @brief Allows user to change the widgets on section changes
 * 
 * Puts a good number of widgets into different groups. Upon selecting a
 * section on the window, it shows the widgets of the group related to the
 * section and hides those of the others. In other words, this acts like the
 * pages of a website.
 * 
 * @copyright Copyright (c) 2022 Satan
 */


#include "section.h"


Section::Section(rmClient* cli) {
    client = cli;
}

void Section::show(bool b) {
    shown = b;
    if(sizerFocus != nullptr)
        sizerFocus->ShowItems(b);
    if(sizerWidgets != nullptr)
        sizerWidgets->ShowItems(b);
}
