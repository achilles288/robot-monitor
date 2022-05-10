/**
 * @file orientation.h
 * @brief The orientation section of the application
 * 
 * Testing of device orientation. The orientation is displayed as both 3D
 * graphics and a set of attributes.
 * 
 * @copyright Copyright (c) 2022 Satan
 */


#pragma once
#ifndef __ORIENTATION_H__
#define __ORIENTATION_H__

#include "section.h"

#include <rm/client.hpp>
#include <rmg/object3d.hpp>
#include <rmg/wxcanvas.hpp>
#include <wx/wx.h>


class OrientationCanvas;


class OrientationSection: public Section {
  private:
    OrientationCanvas* canvas;
    
  public:
    OrientationSection(wxFrame* frame, wxBoxSizer* sizer1, wxBoxSizer* sizer2,
                       rmClient* cli);
};


class OrientationCanvas: public rmg::wxCanvas {
  private:
    rmAttribute* attrRoll;
    rmAttribute* attrPitch;
    rmAttribute* attrYaw;
    rmg::Object3D* model;
    rmg::Object3D *walls[4];
    float azimuth, elevation, distance;
    
    void setupCamera();
    
  public:
    OrientationCanvas(wxFrame* frame, rmClient* cli);
    
    void update() override;
    
    void onMouseMove(const rmg::MouseEvent &event) override;
    
    void onMouseWheel(const rmg::MouseEvent &event) override;
};

#endif
