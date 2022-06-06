// -*- C++ -*-
//
// generated by wxGlade 0.9.4 on Wed Apr 27 00:19:21 2022
//
// Example for compiling a single file project under Linux using g++:
//  g++ MyApp.cpp $(wx-config --libs) $(wx-config --cxxflags) -o MyApp
//
// Example for compiling a multi file project under Linux using g++:
//  g++ main.cpp $(wx-config --libs) $(wx-config --cxxflags) -o MyApp Dialog1.cpp Frame1.cpp
//

#include "wxglade_out.h"

// begin wxGlade: ::extracode
// end wxGlade



MyFrame::MyFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    wxFrame(parent, id, title, pos, size, wxDEFAULT_FRAME_STYLE)
{
    // begin wxGlade: MyFrame::MyFrame
    SetSize(wxSize(970, 555));
    pnlToolBar = new wxPanel(this, wxID_ANY);
    const wxString chMenu_choices[] = {
        wxT("Magnetometer Calibration"),
        wxT("Orientation Test")
    };
    chMenu = new wxChoice(pnlToolBar, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, chMenu_choices);
    const wxString chPort_choices[] = {
        wxT("/dev/ttyACM0"),
    };
    chPort = new wxChoice(pnlToolBar, wxID_ANY, wxDefaultPosition, wxDefaultSize, 1, chPort_choices);
    btnConnect = new wxButton(pnlToolBar, wxID_ANY, wxT("Connect"));
    wxFont font = btnConnect->GetFont();
    font.SetPointSize(13);
    btnConnect->SetFont(font);
    userConstruct();
    /*
    txtEcho = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_RICH);
    */
    txtEcho = new rmEchoBox(this, &client);

    set_properties();
    do_layout();
    // end wxGlade
}


void MyFrame::set_properties()
{
    // begin wxGlade: MyFrame::set_properties
    SetTitle(wxT("frame"));
    wxIcon _icon;
    _icon.CopyFromBitmap(rmGetIcon());
    SetIcon(_icon);
    chMenu->SetMinSize(wxSize(220, 20));
    chMenu->SetSelection(0);
    chPort->SetSelection(0);
    btnConnect->SetMinSize(wxSize(120, 34));
    btnConnect->SetBackgroundColour(wxColour(41, 204, 126));
    btnConnect->SetForegroundColour(wxColour(255, 255, 255));
    pnlToolBar->SetMinSize(wxSize(800, 42));
    pnlToolBar->SetBackgroundColour(wxColour(166, 223, 255));
    // end wxGlade
}


void MyFrame::do_layout()
{
    // begin wxGlade: MyFrame::do_layout
    wxBoxSizer* sizerMain = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizerSplit1 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* sizerControl = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Control")), wxVERTICAL);
    sizerWidgets = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizerSplit2 = new wxBoxSizer(wxVERTICAL);
    sizerEcho = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Message")), wxVERTICAL);
    sizerFocus = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizerToolBar = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizerConnect = new wxBoxSizer(wxHORIZONTAL);
    sizerToolBar->Add(chMenu, 0, wxALL|wxEXPAND, 4);
    sizerToolBar->Add(20, 20, 1, wxALL, 0);
    wxStaticText* lblPort = new wxStaticText(pnlToolBar, wxID_ANY, wxT("Port"));
    sizerConnect->Add(lblPort, 0, wxALIGN_CENTER_VERTICAL|wxTOP, 0);
    sizerConnect->Add(chPort, 0, 0, 0);
    sizerConnect->Add(btnConnect, 0, wxLEFT|wxRIGHT, 8);
    sizerToolBar->Add(sizerConnect, 0, wxALL|wxEXPAND, 4);
    pnlToolBar->SetSizer(sizerToolBar);
    sizerMain->Add(pnlToolBar, 0, wxEXPAND, 0);
    sizerFocus->Add(0, 0, 0, 0, 0);
    sizerSplit2->Add(sizerFocus, 2, wxALL|wxEXPAND, 8);
    sizerEcho->Add(txtEcho, 1, wxALL|wxEXPAND, 8);
    sizerSplit2->Add(sizerEcho, 1, wxEXPAND, 0);
    sizerSplit1->Add(sizerSplit2, 3, wxEXPAND, 0);
    sizerWidgets->Add(0, 0, 0, 0, 0);
    sizerControl->Add(sizerWidgets, 1, wxALL|wxEXPAND, 8);
    sizerControl->Add(270, 20, 0, 0, 0);
    sizerSplit1->Add(sizerControl, 1, wxEXPAND, 0);
    sizerMain->Add(sizerSplit1, 1, wxEXPAND, 0);
    SetSizer(sizerMain);
    Layout();
    // end wxGlade
}


BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    // begin wxGlade: MyFrame::event_table
    EVT_CHOICE(wxID_ANY, MyFrame::onMenuSelection)
    EVT_BUTTON(wxID_ANY, MyFrame::onBtnConnect)
    // end wxGlade
END_EVENT_TABLE();


void MyFrame::onMenuSelection(wxCommandEvent &event)  // wxGlade: MyFrame.<event_handler>
{
    int x = chMenu->GetSelection();
    for(int i=0; i<sectionCount; i++)
        sections[i]->show(i == x);
    Layout();
}

#include <iostream>

void MyFrame::onBtnConnect(wxCommandEvent &event)  // wxGlade: MyFrame.<event_handler>
{
    if(client.isConnected()) {
        client.disconnect();
        btnConnect->SetLabel("Connect");
    }
    else {
        int i = chPort->GetSelection();
        if(i == wxNOT_FOUND)
            return;
        client.connectSerial(portNames[i], 115200);
        if(client.isConnected()) {
            btnConnect->SetLabel("Disconnect");
            client.sendMessage("$set testing 1\n");
        }
    }
}


// wxGlade: add MyFrame event handlers


class MyApp: public wxApp {
public:
    bool OnInit();
};

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    wxInitAllImageHandlers();
    MyFrame* frame = new MyFrame(NULL, wxID_ANY, wxEmptyString);
    SetTopWindow(frame);
    frame->Show();
    return true;
}
