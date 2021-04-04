// -*- C++ -*-
//
// generated by wxGlade 0.9.4 on Sun Apr  4 19:28:41 2021
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
    /*
    gauge_1 = new rmGauge(this, &client, "gauge1", 80);
    
    checkbox_2 = new rmCheckBox(this, &client, "chk1", "check 1");
    checkbox_3 = new rmCheckBox(this, &client, "chk2", "check 2", false);
    checkbox_1 = new rmCheckBox(this, &client, "chk3", "check 3", false);
    text_ctrl_1 = new rmTextCtrl(this, &client, "txt1", RM_ATTRIBUTE_FLOAT);
    spin_ctrl_1 = new rmSpinCtrl(this, &client, "spin1", 0, 120);
    slider_1 = new rmSlider(this, &client, "slider1", 0.0f, 1.0f);
    
    button_2 = new rmButton(this, &client, "btn1", "click me");
    */
    
    // begin wxGlade: MyFrame::MyFrame
    SetSize(wxSize(400, 385));
    gauge_1 = new rmGauge(this, &client, "gauge1", 80);
    const wxString choice_1_choices[] = {
        wxT("choice 1"),
        wxT("choice 2"),
        wxT("choice 3"),
    };
    choice_1 = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, choice_1_choices);
    checkbox_2 = new rmCheckBox(this, &client, "chk1", "check 1");
    checkbox_3 = new rmCheckBox(this, &client, "chk2", "check 2", false);
    checkbox_1 = new rmCheckBox(this, &client, "chk3", "check 3", false);
    text_ctrl_1 = new rmTextCtrl(this, &client, "txt1", RM_ATTRIBUTE_FLOAT);
    spin_ctrl_1 = new rmSpinCtrl(this, &client, "spin1", 0, 120);
    slider_1 = new rmSlider(this, &client, "slider1", 0.0f, 1.0f);
    button_1 = new wxToggleButton(this, wxID_ANY, wxT("button_1"));
    button_2 = new rmButton(this, &client, "btn1", "click me");
    const wxString radio_box_1_choices[] = {
        wxT("choice 1"),
        wxT("choice 2"),
        wxT("choice 3"),
    };
    radio_box_1 = new wxRadioBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 3, radio_box_1_choices, 1, wxRA_SPECIFY_ROWS);

    set_properties();
    do_layout();
    // end wxGlade
}


void MyFrame::set_properties()
{
    // begin wxGlade: MyFrame::set_properties
    SetTitle(wxT("frame"));
    choice_1->SetSelection(0);
    checkbox_3->Enable(0);
    checkbox_1->Enable(0);
    checkbox_1->SetValue(1);
    radio_box_1->SetSelection(0);
    // end wxGlade
}


void MyFrame::do_layout()
{
    /*
    rmStaticText* label_1 = new rmStaticText(this, &client, "stat1", "stat 1",
                                             RM_ATTRIBUTE_INT);
    */
    
    // begin wxGlade: MyFrame::do_layout
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_2 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_3 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_4 = new wxBoxSizer(wxVERTICAL);
    rmStaticText* label_1 = new rmStaticText(this, &client, "stat1", "stat 1",
                                             RM_ATTRIBUTE_INT);
    sizer_4->Add(label_1, 0, 0, 0);
    sizer_4->Add(gauge_1, 0, wxEXPAND, 0);
    sizer_4->Add(choice_1, 0, 0, 0);
    sizer_4->Add(checkbox_2, 0, 0, 0);
    sizer_4->Add(checkbox_3, 0, 0, 0);
    sizer_4->Add(checkbox_1, 0, 0, 0);
    sizer_2->Add(sizer_4, 1, wxEXPAND, 0);
    sizer_3->Add(text_ctrl_1, 0, 0, 0);
    sizer_3->Add(spin_ctrl_1, 0, 0, 0);
    sizer_3->Add(slider_1, 0, wxEXPAND, 0);
    sizer_3->Add(button_1, 0, 0, 0);
    sizer_3->Add(button_2, 0, 0, 0);
    sizer_2->Add(sizer_3, 1, wxEXPAND, 0);
    sizer_1->Add(sizer_2, 1, wxEXPAND, 0);
    sizer_1->Add(radio_box_1, 1, 0, 0);
    SetSizer(sizer_1);
    Layout();
    // end wxGlade
}


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