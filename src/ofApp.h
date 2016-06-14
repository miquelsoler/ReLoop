#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxLaserManager.h"

#include "MSResolumeOSCSender.hpp"
#include "MSActiveArea.hpp"

typedef enum
{
    ScenePickGenre = 0,
    ScenePickWindows = 1,
//    SceneEnd = 2
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
//    void setStateEnd();

    void showLaserEffect(int effectNumber);
    void startLaser(int x, int y);
    void stopLaser(int x, int y);
    void coordinatesToLaser(int x, int y);

    void pickArea(int x, int y);

    // App scenes
    RLScene scene;
//    float windowsSceneStartTime;
//    int windowsSceneMaxDuration;
//    float windowsSceneRemainingTime;
    RLGenreIndex genreIndex;
//    float endingStartTime;
//    int endingDuration;
//    float endingRemainingTime;

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
    int laserX, laserY;
    int laserLength;

    bool isDoingClickEffect;
    float clickStartTime;
    float clickEffectTime;
    int clickEffectRadius;
    int clickEffectX, clickEffectY;

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
