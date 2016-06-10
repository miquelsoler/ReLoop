#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxLaserManager.h"

#include "MSResolumeOSCSender.hpp"
#include "MSActiveArea.hpp"

typedef enum
{
    SceneSelectGenre = 0,
    ScenePickWindows = 1
} RLScene;


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
    void drawPickAreas();
    void drawInfo();

    void setStateGenre();
    void setStateWindows();

    void showLaserEffect(int effectNumber);
    void startLaser(int x, int y);
    void stopLaser(int x, int y);
    void coordinatesToLaser(int x, int y);

    // App scene
    RLScene scene;

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

    // Active areas
    vector<MSActiveArea> genreAreas;
    vector<MSActiveArea> windowAreas;

    // Mouse
    bool mouseEnabled;

    // Look
    bool showLaserGui = true;
    bool showFacade = false;
    ofImage imgFacade;
};
