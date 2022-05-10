/**
 * @file magnetometer.cpp
 * @brief The magnetometer section of the application
 * 
 * Magnetometer data calibration and testing. The plot data is visualized on
 * the MathGL panel.
 * 
 * @copyright Copyright (c) 2022 Satan
 */


#include "magnetometer.h"

#include <rmg/config.h>
#include <robotmonitor.hpp>


class magClearButton: public rmButton {
  private:
    MagnetometerCanvas* analyzer;
    
  public:
    magClearButton(wxWindow* parent, rmClient* cli, const char* cmd,
                   const char* label) :rmButton(parent, cli, cmd, label)
    {}
    
    void setAnalyzer(MagnetometerCanvas* cv) {
        analyzer = cv;
    }
    
    void onClick(wxCommandEvent& evt) {
        analyzer->clear();
    }
};




class magCalculateButton: public rmButton {
  private:
    MagnetometerCanvas* analyzer;
    
  public:
    magCalculateButton(wxWindow* parent, rmClient* cli, const char* cmd,
                       const char* label) :rmButton(parent, cli, cmd, label)
    {}
    
    void setAnalyzer(MagnetometerCanvas* cv) {
        analyzer = cv;
    }
    
    void onClick(wxCommandEvent& evt) {
        analyzer->calculateMatrix();
    }
};




MagnetometerSection::MagnetometerSection(wxFrame* frame, wxBoxSizer* sizer1,
                                         wxBoxSizer* sizer2, rmClient* cli)
                    :Section(cli)
{
    // Construct
    const wxString radioMagMode_choices[] = {
        wxT("Calibration"),
        wxT("Testing"),
    };
    wxRadioBox* radioMagMode = new rmRadioBox(frame, cli, "mag-mode", 2,
                                              radioMagMode_choices);
    wxButton* btnStartScan = new rmButton(frame, cli, "set mag-scan 1",
                                          "Start Scanning");
    wxButton* btnStopScan = new rmButton(frame, cli, "set mag-scan 0",
                                         "Stop Scanning");
    wxButton* btnClearScan = new magClearButton(frame, cli, "",
                                                "Clear Scanning");
    wxButton* btnCalcMat = new magCalculateButton(frame, cli, "",
                                                  "Calculate Matrix");
    
    // Set properties
    radioMagMode->SetSelection(0);
    btnCalcMat->SetToolTip(wxT("Calculates the calibration matrix from the "
                               "scatter plot and uploads it to the client "
                               "device."));
    
    // Set layout
    sizerWidgets = new wxBoxSizer(wxVERTICAL);
    wxStaticText* lblVolt = new rmStaticText(frame, cli, "volt1", "voltage",
                                             RM_ATTRIBUTE_FLOAT);
    sizerWidgets->Add(lblVolt, 0, wxALL, 4);
    sizerWidgets->Add(radioMagMode, 0, wxALL|wxEXPAND, 4);
    sizerWidgets->Add(btnStartScan, 0, wxALL|wxEXPAND, 4);
    sizerWidgets->Add(btnStopScan, 0, wxALL|wxEXPAND, 4);
    sizerWidgets->Add(btnClearScan, 0, wxALL|wxEXPAND, 4);
    sizerWidgets->Add(btnCalcMat, 0, wxALL|wxEXPAND, 4);
    sizerWidgets->Add(20, 16, 0, 0, 0);
    wxStaticText* lblScanCount = new rmStaticText(frame, cli, "mag-dsize",
                                               "data count", RM_ATTRIBUTE_INT);
    sizerWidgets->Add(lblScanCount, 0, wxALL, 4);
    sizer2->Add(sizerWidgets, 1, 0, 0);
    
    // Focus panel
    canvas = new MagnetometerCanvas(frame, client);
    sizerFocus = new wxBoxSizer(wxHORIZONTAL);
    sizerFocus->Add(canvas, 1, wxEXPAND, 0);
    sizer1->Add(sizerFocus, 1, wxEXPAND, 0);
}




MagnetometerCanvas::Plane::Plane(rmg::Context* ctx) {
    for(int i=0; i<11; i++) {
        gridLines1[i] = new rmg::Line3D(ctx, 0.05f, rmg::Color(0.5f,0.5f,0.5f));
        gridLines2[i] = new rmg::Line3D(ctx, 0.05f, rmg::Color(0.5f,0.5f,0.5f));
        ctx->addObject(gridLines1[i]);
        ctx->addObject(gridLines2[i]);
    }
}

MagnetometerCanvas::PlaneYZ::PlaneYZ(rmg::Context* ctx)
                            :Plane(ctx)
{}

void MagnetometerCanvas::PlaneYZ::movePlane(int8_t s) {
    if(s == this->s)
        return;
    for(int i=0; i<11; i++) {
        rmg::Vec3 a = rmg::Vec3(5*s, i-5, -5 );
        rmg::Vec3 b = rmg::Vec3(5*s, i-5,  5 );
        rmg::Vec3 c = rmg::Vec3(5*s, -5 , i-5);
        rmg::Vec3 d = rmg::Vec3(5*s,  5 , i-5);
        gridLines1[i]->setPoints(a, b);
        gridLines2[i]->setPoints(c, d);
    }
    this->s = s;
}

MagnetometerCanvas::PlaneXZ::PlaneXZ(rmg::Context* ctx)
                            :Plane(ctx)
{}

void MagnetometerCanvas::PlaneXZ::movePlane(int8_t s) {
    if(s == this->s)
        return;
    for(int i=0; i<11; i++) {
        rmg::Vec3 a = rmg::Vec3(i-5, 5*s, -5 );
        rmg::Vec3 b = rmg::Vec3(i-5, 5*s,  5 );
        rmg::Vec3 c = rmg::Vec3(-5 , 5*s, i-5);
        rmg::Vec3 d = rmg::Vec3( 5 , 5*s, i-5);
        gridLines1[i]->setPoints(a, b);
        gridLines2[i]->setPoints(c, d);
    }
    this->s = s;
}

MagnetometerCanvas::PlaneXY::PlaneXY(rmg::Context* ctx)
                            :Plane(ctx)
{}

void MagnetometerCanvas::PlaneXY::movePlane(int8_t s) {
    if(s == this->s)
        return;
    for(int i=0; i<11; i++) {
        rmg::Vec3 a = rmg::Vec3(i-5, -5 , 5*s);
        rmg::Vec3 b = rmg::Vec3(i-5,  5 , 5*s);
        rmg::Vec3 c = rmg::Vec3(-5 , i-5, 5*s);
        rmg::Vec3 d = rmg::Vec3( 5 , i-5, 5*s);
        gridLines1[i]->setPoints(a, b);
        gridLines2[i]->setPoints(c, d);
    }
    this->s = s;
}

MagnetometerCanvas::Axis::Axis(rmg::Context* ctx) {
    axis = new rmg::Line3D(ctx, 0.1f, rmg::Color(0,0,0));
    ctx->addObject(axis);
}

MagnetometerCanvas::AxisX::AxisX(rmg::Context* ctx)
                          :Axis(ctx)
{
    axis->setColor(1, 0, 0);
}

void MagnetometerCanvas::AxisX::moveAxis(int8_t a, int8_t b) {
    if(a == this->a && b == this->b)
        return;
    for(int i=0; i<11; i++) {
        rmg::Vec3 p1 = rmg::Vec3(-5, 5*a, 5*b);
        rmg::Vec3 p2 = rmg::Vec3( 5, 5*a, 5*b);
        axis->setPoints(p1, p2);
    }
    this->a = a;
    this->b = b;
}

MagnetometerCanvas::AxisY::AxisY(rmg::Context* ctx)
                          :Axis(ctx)
{
    axis->setColor(0, 1, 0);
}

void MagnetometerCanvas::AxisY::moveAxis(int8_t a, int8_t b) {
    if(a == this->a && b == this->b)
        return;
    for(int i=0; i<11; i++) {
        rmg::Vec3 p1 = rmg::Vec3(5*a, -5, 5*b);
        rmg::Vec3 p2 = rmg::Vec3(5*a,  5, 5*b);
        axis->setPoints(p1, p2);
    }
    this->a = a;
    this->b = b;
}

MagnetometerCanvas::AxisZ::AxisZ(rmg::Context* ctx)
                          :Axis(ctx)
{
    axis->setColor(0, 0, 1);
}

void MagnetometerCanvas::AxisZ::moveAxis(int8_t a, int8_t b) {
    if(a == this->a && b == this->b)
        return;
    for(int i=0; i<11; i++) {
        rmg::Vec3 p1 = rmg::Vec3(5*a, 5*b, -5);
        rmg::Vec3 p2 = rmg::Vec3(5*a, 5*b,  5);
        axis->setPoints(p1, p2);
    }
    this->a = a;
    this->b = b;
}

MagnetometerCanvas::MagnetometerCanvas(wxFrame* frame, rmClient* cli)
                   :rmg::wxCanvas(frame),
                    rmCall("mag-data", nullptr)
{
    client = cli;
    attrMode = cli->getAttribute("mag-mode");
    attrCount = cli->getAttribute("mag-dsize");
    
    azimuth = 0;
    elevation = 0;
    fov = 17;
    setBackgroundColor(0.95f, 0.95f, 0.95f);
    plane1 = PlaneYZ(this);
    plane2 = PlaneXZ(this);
    plane3 = PlaneXY(this);
    plane3.movePlane(-1);
    axis1 = AxisX(this);
    axis2 = AxisY(this);
    axis3 = AxisZ(this);
    setupCamera();
    
    plot[0] = new rmg::Particle3D(this, RMG_RESOURCE_PATH "/dot.png");
    plot[0]->setColor(0.1f, 0.1f, 1);
    plot[0]->setSize(0.3f, 0.3f);
    plot[0]->setHidden(true);
    addObject(plot[0]);
    for(int i=1; i<8192; i++) {
        plot[i] = new rmg::Particle3D(*plot[0]);
        addObject(plot[i]);
    }
}

static float clip(float n, float lower, float upper) {
    n = (n > lower) * n + !(n > lower) * lower;
    return (n < upper) * n + !(n < upper) * upper;
}

void MagnetometerCanvas::onMouseMove(const rmg::MouseEvent &event) {
    if(event.isLeftPressed()) {
        azimuth = azimuth - 0.005f * event.getDiffX();
        if(azimuth < -M_PI)
            azimuth += 2*M_PI;
        else if(azimuth > M_PI)
            azimuth -= 2*M_PI;
        elevation = elevation + 0.005f * event.getDiffY();
        elevation = clip(elevation, 0, M_PI/2);
        setupCamera();
    }
}

void MagnetometerCanvas::clear() {
    for(int i=0; i<plotCount; i++)
        plot[i]->setHidden(true);
    plotCount = 0;
    xmax = -999999.999;
    ymax = -999999.999;
    zmax = -999999.999;
    xmin = 999999.999;
    ymin = 999999.999;
    zmin = 999999.999;
    radius = 0;
}

void MagnetometerCanvas::calculateMatrix() {
    
}

void MagnetometerCanvas::onMouseWheel(const rmg::MouseEvent &event) {
    fov = fov - 0.001f * event.getScroll();
    setupCamera();
}

void MagnetometerCanvas::setupCamera() {
    float r = 50 * cos(elevation);
    float c = 50 * sin(elevation);
    float a = - r * cos(azimuth);
    float b = - r * sin(azimuth);
    setCameraTranslation(a, b, c);
    setCameraRotation(0, elevation, azimuth);
    setOrthographicProjection(fov, 1, 100);
    plane1.movePlane((a > 0) ? -1 : 1);
    plane2.movePlane((b > 0) ? -1 : 1);
    axis1.moveAxis((b > 0) ? 1 : -1, -1);
    axis2.moveAxis((a > 0) ? 1 : -1, -1);
    if(b > 0)
        axis3.moveAxis((a > 0) ? 1 : -1, -1);
    else
        axis3.moveAxis((a > 0) ? 1 : -1, 1);
}

void MagnetometerCanvas::invoke(int argc, char *argv[]) {
    if(argc != 3 || plotCount == 8192)
        return;
    
    bool toCalibrate = false;
    if(attrMode->getValue().i == 0)
        toCalibrate = true;
    
    float x = atof(argv[0]);
    float y = atof(argv[1]);
    float z = atof(argv[2]);
    float r = (rmg::Vec3(x, y, z) - offset).magnitude();
    
    // Filters the points
    if(toCalibrate) {
        if(plotCount > 10 && plotCount < 50) {
            if(r > 2 * radius)
                return;
        }
        else if(plotCount < 100) {
            if(r > 1.5f * radius)
                return;
        }
        else if(plotCount < 1000) {
            if(r > 1.25f * radius)
                return;
        }
        else {
            if(r > 1.1f * radius)
                return;
        }
    }
    
    // Adds a new point
    plot[plotCount]->setTranslation(x, y, z);
    plot[plotCount]->setHidden(false);
    plotCount++;
    attrCount->setValue(plotCount);
    attrCount->getNotifier()->triggerCallback();
    
    // Calculates the offset point for the matrix
    if(!toCalibrate)
        return;
    
    if(x < xmin)
        xmin = x;
    else if(x > xmax)
        xmax = x;
    
    if(y < ymin)
        ymin = y;
    else if(y > ymax)
        ymax = y;
    
    if(z < zmin)
        zmin = z;
    else if(y > zmax)
        zmax = z;
    
    offset.x = (xmax + xmin) / 2.0f;
    offset.y = (ymax + ymin) / 2.0f;
    offset.z = (zmax + zmin) / 2.0f;
    float xdist = xmax - xmin;
    float ydist = ymax - ymin;
    float zdist = zmax - zmin;
    if(xdist > ydist)
        radius = (xdist > zdist) ? (xdist / 2.0f) : (zdist / 2.0f);
    else
        radius = (ydist > zdist) ? (ydist / 2.0f) : (zdist / 2.0f);
}
