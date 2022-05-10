/**
 * @file monitor.c
 * @brief Construction of the UI widgets
 * 
 * The code here modifies the code generated from wxGlade.
 * 
 * @copyright Copyright (c) 2022 Satan
 */


#include "wxglade_out.h"


/*
private:
    rmClient client;
    rmTimer timer;
    Section *sections[4];
    int sectionCount = 4;
    bool initDone = false;
    
    void userConstruct();
    
public:
    void onPortDetected(wxEvent& evt);
    void onResize(wxSizeEvent& evt);
    void onBtnConnect_enter(wxMouseEvent& evt);
    void onBtnConnect_leave(wxMouseEvent& evt);
    void onBtnConnect_press(wxMouseEvent& evt);
    void onBtnConnect_release(wxMouseEvent& evt);
    
*/


void MyFrame::userConstruct() {
    client.setTimer(&timer);
    
    chPort->Clear();
    rmSerialPort::setOnPortDetected(
        wxEventHandler(MyFrame::onPortDetected),
        this
    );
    Connect(
        wxEVT_SIZE,
        wxSizeEventHandler(MyFrame::onResize),
        NULL,
        this
    );
    
    btnConnect->Connect(
        wxEVT_ENTER_WINDOW,
        wxMouseEventHandler(MyFrame::onBtnConnect_enter),
        NULL,
        this
    );
    btnConnect->Connect(
        wxEVT_LEAVE_WINDOW,
        wxMouseEventHandler(MyFrame::onBtnConnect_leave),
        NULL,
        this
    );
    btnConnect->Connect(
        wxEVT_LEFT_DOWN,
        wxMouseEventHandler(MyFrame::onBtnConnect_press),
        NULL,
        this
    );
    btnConnect->Connect(
        wxEVT_LEFT_UP,
        wxMouseEventHandler(MyFrame::onBtnConnect_release),
        NULL,
        this
    );
}


void MyFrame::onPortDetected(wxEvent &evt) {
    if(!client.isConnected())
        btnConnect->SetLabel("Connect");
    
    char selectedPort[16];
    int x = chPort->GetSelection();
    if(x != wxNOT_FOUND) {
        wxString wxstr = chPort->GetString(x);
        strncpy(selectedPort, wxstr.c_str(), 15);
        selectedPort[15] = '\0';
    }
    chPort->Clear();
    int i = 0;
    rmSerialPortList* ports = (rmSerialPortList*) evt.GetEventUserData();
    for(auto it=ports->begin(); it!=ports->end(); ++it) {
        chPort->Append(wxString(it->port));
        if(x != wxNOT_FOUND) {
            if(strcmp(it->port, selectedPort) == 0)
                chPort->SetSelection(i);
        }
        i++;
    }
    x = chPort->GetSelection();
    if(x == wxNOT_FOUND && chPort->GetCount() > 0)
        chPort->SetSelection(0);
}


void MyFrame::onResize(wxSizeEvent& evt) {
    if(!initDone) {
        sections[0] = new MagnetometerSection(this, sizerFocus, sizerWidgets,
                                              &client);
        sections[1] = new OrientationSection(this, sizerFocus, sizerWidgets,
                                             &client);
        sections[0]->show();
        sections[1]->show(false);
        Layout();
        sectionCount = 2;
        initDone = true;
    }
    evt.Skip();
}


#define BTN_CONNECT_BG         wxColour(41, 204, 126)
#define BTN_CONNECT_BG_HOVER   wxColour(73, 218, 141)
#define BTN_CONNECT_BG_PRESS   wxColour(32, 157, 90)


void MyFrame::onBtnConnect_enter(wxMouseEvent& evt) {
    btnConnect->SetBackgroundColour(BTN_CONNECT_BG_HOVER);
    evt.Skip();
}

void MyFrame::onBtnConnect_leave(wxMouseEvent& evt) {
    btnConnect->SetBackgroundColour(BTN_CONNECT_BG);
    evt.Skip();
}

void MyFrame::onBtnConnect_press(wxMouseEvent& evt) {
    btnConnect->SetBackgroundColour(BTN_CONNECT_BG_PRESS);
    evt.Skip();
}

void MyFrame::onBtnConnect_release(wxMouseEvent& evt) {
    btnConnect->SetBackgroundColour(BTN_CONNECT_BG);
    evt.Skip();
}
