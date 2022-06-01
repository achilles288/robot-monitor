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
    
    void onClick(wxCommandEvent& evt) override {
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
    
    void onClick(wxCommandEvent& evt) override {
        rmg::Mat4 M = analyzer->calculateMatrix();
        client->sendCommand("cali-mag %d %f %f %f %f", 0,
                            M[0][0], M[0][1], M[0][2], M[0][3]);
        client->sendCommand("cali-mag %d %f %f %f %f", 1,
                            M[1][0], M[1][1], M[1][2], M[1][3]);
        client->sendCommand("cali-mag %d %f %f %f %f", 2,
                            M[2][0], M[2][1], M[2][2], M[2][3]);
    }
};




class magModeRadio: public rmRadioBox {
  private:
    MagnetometerCanvas* analyzer;
    
  public:
    magModeRadio(wxWindow* parent, rmClient* cli, const char* key,
                 int count, const wxString choices[])
        :rmRadioBox(parent, cli, key, count, choices)
    {}
    
    void setAnalyzer(MagnetometerCanvas* cv) {
        analyzer = cv;
    }
    
    void onRadio(wxCommandEvent& evt)  {
        rmRadioBox::onRadio(evt);
        int i = attribute->getValue().i;
        if(i == 0)
            analyzer->setScale(500);
        else if(i == 1)
            analyzer->setScale(1);
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
    magModeRadio* radioMagMode = new magModeRadio(frame, cli, "mag-mode", 2,
                                                  radioMagMode_choices);
    wxButton* btnStartScan = new rmButton(frame, cli, "set mag-scan 1",
                                          "Start Scanning");
    wxButton* btnStopScan = new rmButton(frame, cli, "set mag-scan 0",
                                         "Stop Scanning");
    magClearButton* btnClearScan = new magClearButton(frame, cli, "",
                                                      "Clear Scanning");
    magCalculateButton* btnCalcMat = new magCalculateButton(frame, cli, "",
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
    
    btnClearScan->setAnalyzer(canvas);
    btnCalcMat->setAnalyzer(canvas);
    radioMagMode->setAnalyzer(canvas);
}




MagnetometerCanvas::Plane::Plane(rmg::Context* ctx) {
    for(int i=0; i<11; i++) {
        gridLines1[i] = new rmg::Line3D(ctx, 0.05f, rmg::Color(0.6f,0.6f,0.6f));
        gridLines2[i] = new rmg::Line3D(ctx, 0.05f, rmg::Color(0.6f,0.6f,0.6f));
        ctx->addObject(gridLines1[i]);
        ctx->addObject(gridLines2[i]);
    }
    for(int i=0; i<11; i+=5) {
        gridLines1[i]->setColor(0.4f, 0.4f, 0.4f);
        gridLines2[i]->setColor(0.4f, 0.4f, 0.4f);
        gridLines1[i]->setThickness(0.06f);
        gridLines2[i]->setThickness(0.06f);
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
    context = ctx;
    axis = new rmg::Line3D(ctx, 0.1f, rmg::Color(0,0,0));
    ctx->addObject(axis);
}

void MagnetometerCanvas::Axis::setFont(rmg::Font* ft1, rmg::Font* ft2) {
    if(label != nullptr)
        return;
    label = new rmg::Text2D(context, ft1, labelText);
    label->setColor(0, 0, 0);
    context->addObject(label);
    for(int i=0; i<3; i++) {
        scale[i] = new rmg::Text2D(context, ft2);
        scale[i]->setColor(0, 0, 0);
        context->addObject(scale[i]);
    }
    scale[1]->setText("0");
    setScale(500);
}

void MagnetometerCanvas::Axis::setScale(float s) {
    char msg[6];
    if(s > 1.0f)
        snprintf(msg, 5, "-%d", (int)(s+0.5f));
    else
        snprintf(msg, 5, "-%.1f", s);
    msg[5] = '\0';
    
    scale[0]->setText(&msg[0]);
    scale[2]->setText(&msg[1]);
}

void MagnetometerCanvas::Axis::setHidden(bool b) {
    label->setHidden(b);
    for(int i=0; i<3; i++)
        scale[i]->setHidden(b);
}

MagnetometerCanvas::AxisX::AxisX(rmg::Context* ctx)
                          :Axis(ctx)
{
    strcpy(labelText, "X-Axis");
}

void MagnetometerCanvas::AxisX::moveAxis(int8_t a, int8_t b) {
    rmg::Rect rect = context->getContextSize();
    uint16_t l = (rect.x < rect.y) ? rect.x : rect.y;
    if(l < 200)
        l = 200;
    float d1 = 6.25f + 250.0f/l;
    float d2 = 5.55f + 75.0f/l;
    float d3 = 5.0f - 175.0f/l;
    
    for(int i=0; i<3; i++) {
        int x = i-1;
        rect = context->worldToScreen(d3*x, d2*a, d2*b);
        scale[i]->setScreenCoordinate(rect);
    }
    rect = context->worldToScreen(1.25f, d1*a, d1*b);
    label->setScreenCoordinate(rect);
    
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
    strcpy(labelText, "Y-Axis");
}

void MagnetometerCanvas::AxisY::moveAxis(int8_t a, int8_t b) {
    rmg::Rect rect = context->getContextSize();
    uint16_t l = (rect.x < rect.y) ? rect.x : rect.y;
    if(l < 200)
        l = 200;
    float d1 = 6.25f + 250.0f/l;
    float d2 = 5.55f + 75.0f/l;
    float d3 = 5.0f - 175.0f/l;
    
    for(int i=0; i<3; i++) {
        int y = i-1;
        rect = context->worldToScreen(d2*a, d3*y, d2*b);
        scale[i]->setScreenCoordinate(rect);
    }
    rect = context->worldToScreen(d1*a, 1.25f, d1*b);
    label->setScreenCoordinate(rect);
    
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
    strcpy(labelText, "Z-Axis");
}

void MagnetometerCanvas::AxisZ::moveAxis(int8_t a, int8_t b) {
    rmg::Rect rect = context->getContextSize();
    uint16_t l = (rect.x < rect.y) ? rect.x : rect.y;
    if(l < 200)
        l = 200;
    float d1 = 6.25f + 250.0f/l;
    float d2 = 5.55f + 75.0f/l;
    float d3 = 5.0f - 175.0f/l;
    
    for(int i=0; i<3; i++) {
        int z = i-1;
        rect = context->worldToScreen(d2*a, d2*b, d3*z);
        scale[i]->setScreenCoordinate(rect);
    }
    rect = context->worldToScreen(d1*a, d1*b, 1.25f);
    label->setScreenCoordinate(rect);
    
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
    cli->appendCall(this);
    
    azimuth = rmg::radian(120);
    elevation = rmg::radian(30);
    fov = 17;
    setBackgroundColor(0.95f, 0.95f, 0.95f);
    plane1 = PlaneYZ(this);
    plane2 = PlaneXZ(this);
    plane3 = PlaneXY(this);
    plane3.movePlane(-1);
    rmg::Font* ft1 = new rmg::Font(this, RMG_DEFAULT_FONT, 18);
    rmg::Font* ft2 = new rmg::Font(this, RMG_DEFAULT_FONT, 15);
    addFont(ft1);
    addFont(ft2);
    axis1 = AxisX(this);
    axis2 = AxisY(this);
    axis3 = AxisZ(this);
    axis1.setFont(ft1, ft2);
    axis2.setFont(ft1, ft2);
    axis3.setFont(ft1, ft2);
    setContextSize(684, 301);
    setupCamera();
    
    dots[0] = new rmg::Particle3D(this, RMG_RESOURCE_PATH "/dot.png");
    dots[0]->setColor(0.1f, 0.1f, 1);
    dots[0]->setSize(0.3f, 0.3f);
    dots[0]->setHidden(true);
    addObject(dots[0]);
    for(uint16_t i=1; i<MAG_MAX_PLOT_COUNT; i++) {
        dots[i] = new rmg::Particle3D(*dots[0]);
        addObject(dots[i]);
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
    for(uint16_t i=0; i<plotCount; i++)
        dots[i]->setHidden(true);
    plotCount = 0;
    attrCount->setValue(0);
    attrCount->getNotifier()->onAttributeChange();
    xmax_ = -999999.999;
    ymax_ = -999999.999;
    zmax_ = -999999.999;
    xmin_ = 999999.999;
    ymin_ = 999999.999;
    zmin_ = 999999.999;
    radius = 0;
}

void MagnetometerCanvas::setScale(float s) {
    axis1.setScale(s);
    axis2.setScale(s);
    axis3.setScale(s);
}

rmg::Mat4 MagnetometerCanvas::calculateMatrix() {
    // Rotation matrix R1
    uint16_t ip = 0;
    uint16_t iq = 0;
    float dmax = 0;
    float dmin = 999999.999;
    
    for(uint16_t i=0; i<plotCount; i++) {
        float d = (plot[i] - offset).magnitude();
        if(d > dmax) {
            ip = i;
            dmax = d;
        }
        if(d < dmin) {
            iq = i;
            dmin = d;
        }
    }
    rmg::Vec3 p = plot[ip].normalize();
    rmg::Vec3 q = plot[iq].normalize();
    rmg::Vec3 r = (p * q).normalize();
    q = (r * p).normalize();
    rmg::Mat4 T = {
        {1, 0, 0, -offset.x},
        {0, 1, 0, -offset.y},
        {0, 0, 1, -offset.z},
        {0, 0, 0,      1   },
    };
    rmg::Mat4 R1 = {
        {p.x, p.y, p.z, 0},
        {q.x, q.y, q.z, 0},
        {r.x, r.y, r.z, 0},
        { 0 ,  0 ,  0 , 1},
    };
    
    // Scale matrix S
    rmg::Mat4 M = R1 * T;
    float xmin = 999999999.999;
    float ymin = 999999999.999;
    float zmin = 999999999.999;
    float xmax = -999999999.999;
    float ymax = -999999999.999;
    float zmax = -999999999.999;
    
    for(uint16_t i=0; i<plotCount; i++) {
        rmg::Vec3 v = rmg::Vec3(M * rmg::Vec4(plot[i]));
        
        if(v.x < xmin)
            xmin = v.x;
        if(v.x > xmax)
            xmax = v.x;
        
        if(v.y < ymin)
            ymin = v.y;
        if(v.y > ymax)
            ymax = v.y;
        
        if(v.z < zmin)
            zmin = v.z;
        if(v.y > zmax)
            zmax = v.z;
    }
    rmg::Mat4 S;
    S[0][0] = 2.0f / (xmax - xmin);
    S[1][1] = 2.0f / (ymax - ymin);
    S[2][2] = 2.0f / (zmax - zmin);
    
    // Rotation matrix R2
    rmg::Mat4 R2 = R1.inverse();
    
    // M = R2 * S * R1 * T
    M = R2 * S * M;
    return M;
}

void MagnetometerCanvas::onResize() {
    setupCamera();
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
    
    if(elevation > M_PI/2 - 0.1f) {
        axis3.setHidden(true);
    }
    else if(elevation < 0.1f) {
        if((azimuth > -0.1f && azimuth < 0.1f) ||
           (azimuth > M_PI-0.1f || azimuth < 0.1f-M_PI))
        {
            axis1.setHidden(true);
        }
        else {
            axis1.setHidden(false);
        }
        
        if((azimuth > -M_PI/2-0.1f && azimuth < -M_PI/2+0.1f) ||
           (azimuth > M_PI/2-0.1f && azimuth < M_PI/2+0.1f))
        {
            axis2.setHidden(true);
        }
        else {
            axis2.setHidden(false);
        }
        axis3.setHidden(false);
    }
    else {
        axis1.setHidden(false);
        axis2.setHidden(false);
        axis3.setHidden(false);
    }
}

void MagnetometerCanvas::invoke(int argc, char *argv[]) {
    if(argc != 3 || plotCount == MAG_MAX_PLOT_COUNT)
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
        if(plotCount > 1000) {
            if(r > 1.1f * radius)
                return;
        }
        else if(plotCount > 100) {
            if(r > 1.25 * radius)
                return;
        }
        else if(plotCount > 50) {
            if(r > 1.5 * radius)
                return;
        }
        else if(plotCount > 10) {
            if(r > 2 * radius)
                return;
        }
    }
    
    // Adds a new point
    plot[plotCount].x = x;
    plot[plotCount].y = y;
    plot[plotCount].z = z;
    float scale = 5;
    if(toCalibrate)
        scale = 0.01f;
    dots[plotCount]->setTranslation(x*scale, y*scale, z*scale);
    dots[plotCount]->setHidden(false);
    plotCount++;
    attrCount->setValue(plotCount);
    attrCount->getNotifier()->onAttributeChange();
    
    // Calculates the offset point for the matrix
    if(!toCalibrate)
        return;
    
    if(x < xmin_)
        xmin_ = x;
    if(x > xmax_)
        xmax_ = x;
    
    if(y < ymin_)
        ymin_ = y;
    if(y > ymax_)
        ymax_ = y;
    
    if(z < zmin_)
        zmin_ = z;
    if(y > zmax_)
        zmax_ = z;
    
    offset.x = (xmax_ + xmin_) / 2.0f;
    offset.y = (ymax_ + ymin_) / 2.0f;
    offset.z = (zmax_ + zmin_) / 2.0f;
    float xdist = xmax_ - xmin_;
    float ydist = ymax_ - ymin_;
    float zdist = zmax_ - zmin_;
    if(xdist > ydist)
        radius = (xdist > zdist) ? (xdist / 2.0f) : (zdist / 2.0f);
    else
        radius = (ydist > zdist) ? (ydist / 2.0f) : (zdist / 2.0f);
}
