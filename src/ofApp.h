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

    void drawFacade();
    void drawInfo();

    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);

private:

    void setStateGenre();
    void setStateWindows();

    RLState state;

    ofxOscReceiver myoOSCReceiver;
    MSResolumeOSCSender *resolumeOSCSender;

    string resolumeHost;
    int resolumePort;
    int myoPort;

    ofxLaser::Manager laser;

    bool shouldDrawFacade;
    ofImage imgFacade;
};
