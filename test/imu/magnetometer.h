/**
 * @file magnetometer.h
 * @brief The magnetometer section of the application
 * 
 * Magnetometer data calibration and testing. The plot data is visualized on
 * the MathGL panel.
 * 
 * @copyright Copyright (c) 2022 Satan
 */


#pragma once
#ifndef __MAGNETOMETER_H__
#define __MAGNETOMETER_H__

#include "section.h"

#include <rm/client.hpp>
#include <rmg/line3d.hpp>
#include <rmg/particle.hpp>
#include <rmg/wxcanvas.hpp>
#include <wx/wx.h>


class MagnetometerCanvas;


class MagnetometerSection: public Section {
  private:
    MagnetometerCanvas* canvas;
    
  public:
    MagnetometerSection(wxFrame* frame, wxBoxSizer* sizer1, wxBoxSizer* sizer2,
                        rmClient* cli);
};


#define MAG_MAX_PLOT_COUNT 8192


class MagnetometerCanvas: public rmg::wxCanvas, rmCall {
    class Plane {
      protected:
        rmg::Line3D *gridLines1[11];
        rmg::Line3D *gridLines2[11];
        int8_t s = 0;
        
      public:
        Plane() = default;
        
        Plane(rmg::Context* ctx);
    };
    
    class PlaneYZ: public Plane {
      public:
        PlaneYZ() = default;
        
        PlaneYZ(rmg::Context* ctx);
        
        void movePlane(int8_t s);
    };
    
    class PlaneXZ: public Plane {
      public:
        PlaneXZ() = default;
        
        PlaneXZ(rmg::Context* ctx);
        
        void movePlane(int8_t s);
    };
    
    class PlaneXY: public Plane {
      public:
        PlaneXY() = default;
        
        PlaneXY(rmg::Context* ctx);
        
        void movePlane(int8_t s);
    };
    
    class Axis {
      protected:
        rmg::Context *context = nullptr;
        rmg::Line3D *axis = nullptr;
        rmg::Text2D *label = nullptr;
        rmg::Text2D *scale[3];
        char labelText[7];
        int8_t a = 0;
        int8_t b = 0;
        
      public:
        Axis() = default;
        
        Axis(rmg::Context* ctx);
        
        void setFont(rmg::Font* ft1, rmg::Font* ft2);
        
        void setScale(float s);
        
        void setHidden(bool b);
    };
    
    class AxisX: public Axis {
      public:
        AxisX() = default;
        
        AxisX(rmg::Context* ctx);
        
        void moveAxis(int8_t a, int8_t b);
    };
    
    class AxisY: public Axis {
      public:
        AxisY() = default;
        
        AxisY(rmg::Context* ctx);
        
        void moveAxis(int8_t a, int8_t b);
    };
    
    class AxisZ: public Axis {
      public:
        AxisZ() = default;
        
        AxisZ(rmg::Context* ctx);
        
        void moveAxis(int8_t a, int8_t b);
    };
    
  private:
    rmClient* client;
    rmAttribute* attrMode;
    rmAttribute* attrCount;
    rmg::Vec3 plot[MAG_MAX_PLOT_COUNT];
    uint16_t plotCount = 0;
    float xmax_ = -999999.999;
    float ymax_ = -999999.999;
    float zmax_ = -999999.999;
    float xmin_ = 999999.999;
    float ymin_ = 999999.999;
    float zmin_ = 999999.999;
    float radius = 0;
    rmg::Vec3 offset;
    
    PlaneYZ plane1;
    PlaneXZ plane2;
    PlaneXY plane3;
    AxisX axis1;
    AxisY axis2;
    AxisZ axis3;
    rmg::Particle3D *dots[MAG_MAX_PLOT_COUNT];
    float azimuth, elevation, fov;
    
    void setupCamera();
    
  public:
    MagnetometerCanvas(wxFrame* frame, rmClient* cli);
    
    void clear();
    
    void setScale(float s);
    
    rmg::Mat4 calculateMatrix();
    
    void onResize() override;
    
    void onMouseMove(const rmg::MouseEvent &event) override;
    
    void onMouseWheel(const rmg::MouseEvent &event) override;
    
    void invoke(int argc, char *argv[]) override;
};

#endif
