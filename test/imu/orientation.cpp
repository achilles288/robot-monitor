/**
 * @file orientation.cpp
 * @brief The orientation section of the application
 * 
 * Testing of device orientation. The orientation is displayed as both 3D
 * graphics and a set of attributes.
 * 
 * @copyright Copyright (c) 2022 Satan
 */


#include "orientation.h"

#include <rmg/config.h>
#include <rmg/cube.hpp>
#include <rmg/line3d.hpp>
#include <robotmonitor.hpp>

#define PI 3.1415926535897932384626433832795f


OrientationSection::OrientationSection(wxFrame* frame, wxBoxSizer* sizer1,
                                       wxBoxSizer* sizer2, rmClient* cli)
                   :Section(cli)
{
    // Construct
    const wxString radioSenseMode_choices[] = {
        wxT("Accelerometer"),
        wxT("Gyrosensor"),
        wxT("Magnetometer"),
        wxT("System"),
    };
    wxRadioBox* radioSenseMode = new rmRadioBox(frame, cli, "imu-mode", 4,
                                                radioSenseMode_choices);
    wxCheckBox* chkLockYaw = new rmCheckBox(frame, cli, "lock-yaw",
                                            "Lock Yaw");
    wxButton* btnGyroCali = new rmButton(frame, cli, "cali-gyro",
                                         "Calibrate Gyro");
    
    // Set properties
    radioSenseMode->SetSelection(0);
    chkLockYaw->SetToolTip(wxT("In this system, the yaw value cannot be "
                               "measured when there is a roll. So, the "
                               "correct yaw value is to be locked before "
                               "making pitch and roll."));
    
    // Set layout
    sizerWidgets = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizerSense = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizerRate = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizerPos = new wxBoxSizer(wxVERTICAL);
    wxStaticText* lblVolt = new rmStaticText(frame, cli, "volt2", "voltage",
                                             RM_ATTRIBUTE_FLOAT);
    sizerWidgets->Add(lblVolt, 0, wxALL, 4);
    sizerWidgets->Add(radioSenseMode, 0, wxALL|wxEXPAND, 4);
    wxStaticText* lblRoll = new rmStaticText(frame, cli, "roll", "roll",
                                             RM_ATTRIBUTE_FLOAT);
    sizerPos->Add(lblRoll, 0, wxALL, 4);
    wxStaticText* lblPitch = new rmStaticText(frame, cli, "pitch", "pitch",
                                              RM_ATTRIBUTE_FLOAT);
    sizerPos->Add(lblPitch, 0, wxALL, 4);
    wxStaticText* lblYaw = new rmStaticText(frame, cli, "yaw", "yaw",
                                            RM_ATTRIBUTE_FLOAT);
    sizerPos->Add(lblYaw, 0, wxALL, 4);
    sizerSense->Add(sizerPos, 1, wxALL|wxEXPAND, 4);
    wxStaticText* lblGyroX = new rmStaticText(frame, cli, "gyro-x", "gyro-x",
                                              RM_ATTRIBUTE_FLOAT);
    sizerRate->Add(lblGyroX, 0, wxALL, 4);
    wxStaticText* lblGyroY = new rmStaticText(frame, cli, "gyro-y", "gyro-y",
                                              RM_ATTRIBUTE_FLOAT);
    sizerRate->Add(lblGyroY, 0, wxALL, 4);
    wxStaticText* lblGyroZ = new rmStaticText(frame, cli, "gyro-z", "gyro-z",
                                              RM_ATTRIBUTE_FLOAT);
    sizerRate->Add(lblGyroZ, 0, wxALL, 4);
    sizerSense->Add(sizerRate, 1, wxALL|wxEXPAND, 4);
    sizerWidgets->Add(sizerSense, 1, wxEXPAND, 0);
    sizerWidgets->Add(20, 16, 0, 0, 0);
    sizerWidgets->Add(chkLockYaw, 0, wxALL|wxEXPAND, 4);
    sizerWidgets->Add(btnGyroCali, 0, wxALL|wxEXPAND, 4);
    sizer2->Add(sizerWidgets, 1, 0, 0);
    
    // Focus panel
    canvas = new OrientationCanvas(frame, client);
    sizerFocus = new wxBoxSizer(wxHORIZONTAL);
    sizerFocus->Add(canvas, 1, wxEXPAND, 0);
    sizer1->Add(sizerFocus, 1, wxEXPAND, 0);
}




OrientationCanvas::OrientationCanvas(wxFrame* frame, rmClient* cli)
                  :rmg::wxCanvas(frame)
{
    attrRoll = cli->getAttribute("roll");
    attrPitch = cli->getAttribute("pitch");
    attrYaw = cli->getAttribute("yaw");
    
    azimuth = rmg::radian(118.5f);
    elevation = rmg::radian(36);
    distance = 20;
    setBackgroundColor(0.847f, 0.949f, 1.0f);
    setDirectionalLightAngles(rmg::radian(75), rmg::radian(30));
    setDirectionalLightColor(1, 1, 1, 2);
    setPerspectiveProjection(rmg::radian(30), 2.0f, 60.0f);
    setContextSize(684, 301);
    
    rmg::Line3D* line;
    line = new rmg::Line3D(this, 0.1f);
    line->setPoints(rmg::Vec3(-8, 0, 0), rmg::Vec3(8, 0, 0));
    addObject(line);
    line = new rmg::Line3D(this, 0.1f);
    line->setPoints(rmg::Vec3(7.5f, -0.5f, 0), rmg::Vec3(8, 0, 0));
    addObject(line);
    line = new rmg::Line3D(this, 0.1f);
    line->setPoints(rmg::Vec3(7.5f, 0.5f, 0), rmg::Vec3(8, 0, 0));
    addObject(line);
    line = new rmg::Line3D(this, 0.1f);
    line->setPoints(rmg::Vec3(0, -8, 0), rmg::Vec3(0, 8, 0));
    addObject(line);
    line = new rmg::Line3D(this, 0.1f);
    line->setPoints(rmg::Vec3(-0.5f, 7.5f, 0), rmg::Vec3(0, 8, 0));
    addObject(line);
    line = new rmg::Line3D(this, 0.1f);
    line->setPoints(rmg::Vec3(0.5f, 7.5f, 0), rmg::Vec3(0, 8, 0));
    addObject(line);
    line = new rmg::Line3D(this, 0.1f);
    line->setPoints(rmg::Vec3(0, 0, -5), rmg::Vec3(0, 0, 5));
    addObject(line);
    
    rmg::Font* ft1 = new rmg::Font(this, RMG_DEFAULT_FONT, 22);
    rmg::Font* ft2 = new rmg::Font(this, RMG_DEFAULT_FONT, 20);
    addFont(ft1);
    addFont(ft2);
    xAxis = new rmg::Text2D(this, ft1, "X-Axis");
    yAxis = new rmg::Text2D(this, ft1, "Y-Axis");
    zAxis = new rmg::Text2D(this, ft2, "Z-Axis");
    zAxis->setRotation(PI/2);
    addObject(xAxis);
    addObject(yAxis);
    addObject(zAxis);
    
    rmg::Object3D* floor = new rmg::Cube3D(this, 20.5f, 20.5f, 1);
    floor->setColor(0.5f, 0, 0);
    floor->setRoughness(0.5f);
    floor->setTranslation(0, 0, -5.5f);
    
    rmg::Cube3D wall = rmg::Cube3D(this, 0.5f, 20.5f, 10);
    for(int i=0; i<4; i++) {
        walls[i] = new rmg::Object3D(wall);
        walls[i]->setColor(1, 1, 1);
        walls[i]->setRoughness(0.4f);
        walls[i]->setRotation(0, 0, PI/2 * i);
        addObject(walls[i]);
    }
    walls[0]->setTranslation(10, 0, 0);
    walls[1]->setTranslation(0, 10, 0);
    walls[2]->setTranslation(-10, 0, 0);
    walls[3]->setTranslation(0, -10, 0);
    walls[3]->setHidden(true);
    
    model = new rmg::Object3D(this, RMG_RESOURCE_PATH "/models/teapot.obj");
    model->setColor(0, 1.0f, 0.2f);
    model->setRoughness(0.4f);
    model->setScale(3.5f);
    
    addObject(floor);
    addObject(model);
    
    setupCamera();
}

void OrientationCanvas::update() {
    float roll = attrRoll->getValue().f;
    float pitch = attrPitch->getValue().f;
    float yaw = attrYaw->getValue().f;
    model->setRotation(roll, pitch, yaw, rmg::AngleUnit::Degree);
}

void OrientationCanvas::onResize() {
    setupCamera();
}

static float clip(float n, float lower, float upper) {
    n = (n > lower) * n + !(n > lower) * lower;
    return (n < upper) * n + !(n < upper) * upper;
}

void OrientationCanvas::onMouseMove(const rmg::MouseEvent &event) {
    if(event.isLeftPressed()) {
        azimuth = azimuth - 0.005f * event.getDiffX();
        if(azimuth < -PI)
            azimuth += 2*PI;
        else if(azimuth > PI)
            azimuth -= 2*PI;
        elevation = elevation + 0.005f * event.getDiffY();
        elevation = clip(elevation, 0, PI/2);
        setupCamera();
        
        uint8_t flag = 0b0000;
        
        if(azimuth > -0.3927f && azimuth < 0.3927f)
            flag = 0b0100;
        else if(azimuth > 1.1781f && azimuth < 1.9635f)
            flag = 0b1000;
        else if(azimuth < -2.7489f || azimuth > 2.7489f)
            flag = 0b0001;
        else if(azimuth > -1.9635f && azimuth < -1.1781f)
            flag = 0b0010;
        
        else if(azimuth > 0 && azimuth < PI/2)
            flag = 0b1100;
        else if(azimuth > PI/2)
            flag = 0b1001;
        else if(azimuth < -PI/2)
            flag = 0b0011;
        else if(azimuth > -PI/2 && azimuth < 0)
            flag = 0b0110;
        
        walls[0]->setHidden(flag & 0b0001);
        walls[1]->setHidden(flag & 0b0010);
        walls[2]->setHidden(flag & 0b0100);
        walls[3]->setHidden(flag & 0b1000);
    }
}

void OrientationCanvas::onMouseWheel(const rmg::MouseEvent &event) {
    distance = distance - 0.001f * event.getScroll();
    setupCamera();
}

void OrientationCanvas::setupCamera() {
    float r = distance * cos(elevation);
    float c = distance * sin(elevation);
    float a = - r * cos(azimuth);
    float b = - r * sin(azimuth);
    setCameraTranslation(a, b, c);
    setCameraRotation(0, elevation, azimuth);
    
    xAxis->setScreenCoordinate(worldToScreen(6, 0, 0.4f));
    yAxis->setScreenCoordinate(worldToScreen(0, 6, 0.4f));
    rmg::Rect rect = worldToScreen(0, 0, 4);
    rect.x += 14;
    zAxis->setScreenCoordinate(rect);
}
