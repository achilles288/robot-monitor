#include "wxglade_out.h"


/*
class MyFrame: public wxFrame {
private:
    rmClient client;
    void construct_user();
    
public:
    void setPortList(rmSerialPortList& ports);
};


}
*/


static MyFrame* frame;

long ID_MENU_PORT;
long ID_MENU_BAUDRATE;
long ID_B9600;
long ID_B19200;
long ID_B38400;
long ID_B57600;


void MyFrame::construct_user() {
    frame = this;
    ID_MENU_PORT = wxNewId();
    ID_MENU_BAUDRATE = wxNewId();
    ID_B9600 = wxNewId();
    ID_B19200 = wxNewId();
    ID_B38400 = wxNewId();
    ID_B57600 = wxNewId();
    
    label_1 = new rmStaticText(this, &client, nullptr, "pot 1");
    label_2 = new rmStaticText(this, &client, nullptr, "LED 1");
    gauge_1 = new rmGauge(this, &client, "pot1", 1024);
    slider_1 = new rmSlider(this, &client, "led1", 0, 255);
    checkbox_1 = new rmCheckBox(this, &client, "btn1", "Button 1", false);
    checkbox_2 = new rmCheckBox(this, &client, "led2", "LED 2");
    button_1 = new rmButton(this, &client, "command1", "Command 1");
    text_ctrl_1 = new rmEchoBox(this, &client);
    
    rmSerialPort::setOnPortDetected([]() {
        auto ports = rmSerialPort::listPorts();
        frame->setPortList(ports);
    });
}


void MyFrame::setPortList(rmSerialPortList& ports) {
    if(ports.size() > 0)
        client.connectSerial(ports[0].port, 38400);
}


void MyFrame::onMenuPort(wxCommandEvent &event) {
    
}


void MyFrame::onMenuBaudrate(wxCommandEvent &event) {
    
}


void MyFrame::onMenuConnect(wxCommandEvent &event) {
    
}


void MyFrame::onMenuDisconnect(wxCommandEvent &event) {
    
}


#define ABOUT_MESSAGE \
"This is a wxWidgets application with Robot Monitor " RM_VERSION_STRING "."

void MyFrame::onMenuAbout(wxCommandEvent &event) {
    wxMessageBox(ABOUT_MESSAGE, "Robot Monitor demo", wxOK|wxICON_INFORMATION);
}


void MyFrame::onMenuExit(wxCommandEvent &event) {
    Close(true);
}
