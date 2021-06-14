#include "wxglade_out.h"

#include <iostream>


/*
class MyFrame: public wxFrame {
private:
    rmClient client;
    rmTimer timer;
    wxMenu* menuPort;
    void construct_user();
    
public:
    void onPortDetected(wxEvent& evt);
};
*/


long ID_MENU_PORT;
long ID_MENU_BAUDRATE;
long ID_MENU_CONNECT;
long ID_MENU_DISCONNECT;
long ID_B9600;
long ID_B19200;
long ID_B38400;
long ID_B57600;
long ID_B74880;
long ID_B115200;


void MyFrame::construct_user() {
    ID_MENU_PORT = wxNewId();
    ID_MENU_BAUDRATE = wxNewId();
    ID_MENU_CONNECT = wxNewId();
    ID_MENU_DISCONNECT = wxNewId();
    ID_B9600 = wxNewId();
    ID_B19200 = wxNewId();
    ID_B38400 = wxNewId();
    ID_B57600 = wxNewId();
    ID_B74880 = wxNewId();
    ID_B115200 = wxNewId();
    
    client.setTimer(&timer);
    lblAccX = new rmStaticText(this, &client, "accX", "AccX", RM_ATTRIBUTE_INT);
    lblAccY = new rmStaticText(this, &client, "accY", "AccY", RM_ATTRIBUTE_INT);
    lblAccZ = new rmStaticText(this, &client, "accZ", "AccZ", RM_ATTRIBUTE_INT);
    lblGyroX = new rmStaticText(this, &client, "gyroX", "GyroX", RM_ATTRIBUTE_INT);
    lblGyroY = new rmStaticText(this, &client, "gyroY", "GyroY", RM_ATTRIBUTE_INT);
    lblGyroZ = new rmStaticText(this, &client, "gyroZ", "GyroZ", RM_ATTRIBUTE_INT);
    lblMagX = new rmStaticText(this, &client, "magX", "MagX", RM_ATTRIBUTE_INT);
    lblMagY = new rmStaticText(this, &client, "magY", "MagY", RM_ATTRIBUTE_INT);
    lblMagZ = new rmStaticText(this, &client, "magZ", "MagZ", RM_ATTRIBUTE_INT);
    btnCaliGryo = new rmButton(this, &client, "calibrate gyro", "Calibrate 1");
    btnCaliMag = new rmButton(this, &client, "calibrate mag", "Calibrate 2");
    txtEcho = new rmEchoBox(this, &client);
    lblPortStatus = new wxStaticText(this, wxID_ANY, wxT("Not connected"));
    #ifdef _WIN32
    lblPortAddress = new wxStaticText(this, wxID_ANY, wxT("COMXX"));
    #else
    lblPortAddress = new wxStaticText(this, wxID_ANY, wxT("/dev/ttyACMX"));
    #endif
    menuPort = new wxMenu();
    
    rmSerialPort::setOnPortDetected(
        wxEventHandler(MyFrame::onPortDetected),
        this
    );
    
    /*wxglade_tmp_menu = menuPort->GetParent();
    wxglade_tmp_menu->Enable(ID_MENU_PORT, false);
    wxglade_tmp_menu->Enable(ID_MENU_CONNECT, false);
    wxglade_tmp_menu->Enable(ID_MENU_DISCONNECT, false);*/
}


static char selectedPort[16] = "";
static uint32_t selectedBaudrate = 9600;


void MyFrame::onPortDetected(wxEvent &evt) {
    rmSerialPortList* ports = (rmSerialPortList*) evt.GetEventUserData();
    auto items = menuPort->GetMenuItems();
    for(auto it=items.begin(); it!=items.end(); ++it) {
        menuPort->Destroy(*it);
    }
    for(auto it=ports->begin(); it!=ports->end(); ++it) {
        long id = wxNewId();
        wxMenuItem* item = menuPort->AppendRadioItem(id, wxString(it->port));
        if(strncmp(it->port, selectedPort, 15) == 0) {
            item->Check();
        }
        Bind(wxEVT_MENU, &MyFrame::onMenuPort, this, id);
    }
    if(strcmp(selectedPort, "") == 0 && ports->size()) {
        strncpy(selectedPort, (*ports)[0].port, 15);
        selectedPort[15] = '\0';
    }
    wxMenu* parent = menuPort->GetParent();
    if(ports->size() == 0) {
        parent->Enable(ID_MENU_PORT, false);
        parent->Enable(ID_MENU_CONNECT, false);
        parent->Enable(ID_MENU_DISCONNECT, false);
    }
    else {
        parent->Enable(ID_MENU_PORT, true);
        parent->Enable(ID_MENU_CONNECT, true);
        parent->Enable(ID_MENU_DISCONNECT, client.isConnected());
    }
}


void MyFrame::onMenuPort(wxCommandEvent &event) {
    wxMenuItem* item = menuPort->FindItem(event.GetId());
    strncpy(selectedPort, item->GetItemLabel().c_str(), 15);
    selectedPort[15] = '\0';
}


void MyFrame::onMenuBaudrate(wxCommandEvent &event) {
    if(event.GetId() == ID_B9600)
        selectedBaudrate = 9600;
    else if(event.GetId() == ID_B19200)
        selectedBaudrate = 19200;
    else if(event.GetId() == ID_B38400)
        selectedBaudrate = 38400;
    else if(event.GetId() == ID_B57600)
        selectedBaudrate = 57600;
    else if(event.GetId() == ID_B74880)
        selectedBaudrate = 74880;
    else if(event.GetId() == ID_B115200)
        selectedBaudrate = 115200;
}


void MyFrame::onMenuConnect(wxCommandEvent &event) {
    client.connectSerial(selectedPort, selectedBaudrate);
    if(client.isConnected()) {
        lblPortStatus->SetLabel(wxString("Connected"));
        lblPortAddress->SetLabel(wxString(selectedPort));
        lblPortStatus->Refresh();
        lblPortAddress->Refresh();
        menuPort->GetParent()->Enable(ID_MENU_DISCONNECT, true);
    }
}


void MyFrame::onMenuDisconnect(wxCommandEvent &event) {
    client.disconnect();
    lblPortStatus->SetLabel(wxString("Not connected"));
    lblPortStatus->Refresh();
}


#define ABOUT_MESSAGE \
"This is a wxWidgets application with Robot Monitor " RM_VERSION_STRING "."

void MyFrame::onMenuAbout(wxCommandEvent &event) {
    wxMessageBox(ABOUT_MESSAGE, "Robot Monitor demo", wxOK|wxICON_INFORMATION);
}


void MyFrame::onMenuExit(wxCommandEvent &event) {
    Close(true);
}
