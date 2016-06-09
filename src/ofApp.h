#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#include "MSResolumeOSCSender.hpp"
#include "ofxLaserManager.h"

typedef enum
{
    StateSelectGenre = 0,
    StateSelectWindows = 1
} RLState;


class ofApp : public ofBaseApp
{
public:

    void setup();
    void update();
    void draw();
    void exit();

    void keyReleased(int key);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);

private:

    void drawFacade();
    void drawInfo();

    void setStateGenre();
    void setStateWindows();

    void showLaserEffect(int effectNumber);

    // App state
    RLState state;

    // OSC
    ofxOscReceiver myoOSCReceiver;
    MSResolumeOSCSender *resolumeOSCSender;
    string resolumeHost;
    int resolumePort;
    int myoPort;

    // Laser
    bool laserEnabled;
    ofxLaser::Manager laser;
    int laserWidth;
    int laserHeight;
    ofxPanel laserGui;
    vector<ofPolyline> laserPolylines;
    bool drawingShape = false;

    // Look
    bool showLaserGui = true;
    bool showFacade = false;
    ofImage imgFacade;
};
