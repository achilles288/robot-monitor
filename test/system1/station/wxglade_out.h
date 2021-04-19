// -*- C++ -*-
//
// generated by wxGlade 0.9.4 on Sun Apr 18 17:24:58 2021
//
// Example for compiling a single file project under Linux using g++:
//  g++ MyApp.cpp $(wx-config --libs) $(wx-config --cxxflags) -o MyApp
//
// Example for compiling a multi file project under Linux using g++:
//  g++ main.cpp $(wx-config --libs) $(wx-config --cxxflags) -o MyApp Dialog1.cpp Frame1.cpp
//

#ifndef WXGLADE_OUT_H
#define WXGLADE_OUT_H

#include <wx/wx.h>
#include <wx/image.h>

// begin wxGlade: ::dependencies
// end wxGlade

// begin wxGlade: ::extracode
#include <robotmonitor.hpp>

extern long ID_MENU_PORT;
extern long ID_MENU_BAUDRATE;
extern long ID_B9600;
extern long ID_B19200;
extern long ID_B38400;
extern long ID_B57600;
// end wxGlade


class MyFrame: public wxFrame {
public:
    // begin wxGlade: MyFrame::ids
    // end wxGlade

    MyFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE);

private:
    // begin wxGlade: MyFrame::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: MyFrame::attributes
    wxMenuBar* frame_menubar;
    wxStaticText* label_1;
    wxGauge* gauge_1;
    wxCheckBox* checkbox_1;
    wxStaticText* label_2;
    wxSlider* slider_1;
    wxCheckBox* checkbox_2;
    wxButton* button_1;
    wxTextCtrl* text_ctrl_1;
    wxStaticText* lblPortStatus;
    wxStaticText* lblPortAddress;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    void onMenuExit(wxCommandEvent &event); // wxGlade: <event_handler>
    void onMenuPort(wxCommandEvent &event); // wxGlade: <event_handler>
    void onMenuBaudrate(wxCommandEvent &event); // wxGlade: <event_handler>
    void onMenuConnect(wxCommandEvent &event); // wxGlade: <event_handler>
    void onMenuDisconnect(wxCommandEvent &event); // wxGlade: <event_handler>
    void onMenuAbout(wxCommandEvent &event); // wxGlade: <event_handler>
    
    
private:
    rmClient client;
    void construct_user();
    
public:
    void setPortList(rmSerialPortList& ports);
}; // wxGlade: end class


#endif // WXGLADE_OUT_H
