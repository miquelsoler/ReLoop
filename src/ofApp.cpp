#include "ofApp.h"

#include "MsAppSettings.hpp"

static const string STR_APPSETTINGS_FILENAME = "AppSettings.xml";
static const string STR_LASERSETTINGS_FILENAME = "LaserSettings.xml";

static const int LASER_GUI_WIDTH = 240;


//--------------------------------------------------------------
void ofApp::setup()
{
    ofBackground(ofColor::black);

    // Load app settings
    {
        MSAppSettings::getInstance().loadFile(STR_APPSETTINGS_FILENAME);

        resolumeHost = MSAppSettings::getInstance().getResolumeHost();
        resolumePort = MSAppSettings::getInstance().getResolumePort();
        myoPort = MSAppSettings::getInstance().getMyoPort();

        laserEnabled = MSAppSettings::getInstance().getLaserEnabled();
    }

    // Laser
    if (laserEnabled)
    {
        laserWidth = 400;
        laserHeight = 400;

        laser.setup(laserWidth, laserHeight);
        laser.connectToEtherdream();

        laserGui.setup();
        laserGui.add(laser.parameters);

        laserGui.add(laser.redParams);
        laserGui.add(laser.greenParams);
        laserGui.add(laser.blueParams);
        laserGui.setWidthElements(LASER_GUI_WIDTH);

        laserGui.loadFromFile(STR_LASERSETTINGS_FILENAME);
    }

    // OSC
    {
        resolumeOSCSender = new MSResolumeOSCSender(resolumeHost, resolumePort);
        myoOSCReceiver.setup(myoPort);
    }

    imgFacade = ofImage("Facade.png");

    state = StateSelectGenre;
}

//--------------------------------------------------------------
void ofApp::update()
{
    int posX, posY;

    if (laserEnabled) {
        showLaserEffect(0);
        laser.update();
    }

    // Myo events

    while (myoOSCReceiver.hasWaitingMessages())
    {
        ofxOscMessage m;
        myoOSCReceiver.getNextMessage(m);

        if (m.getAddress() == "/mouse/position") {
            posX = m.getArgAsInt32(0);
            posY = m.getArgAsInt32(1);
            cout << "Laser pos: " << posX << " " << posY << endl;
        }
        else if(m.getAddress() == "/mouse/start") {
            cout << "Mouse start" << endl;
//            posX = m.getArgAsInt32(0);
//            posX = m.getArgAsInt32(1);
//            startLaser(m.getArgAsInt32(0),m.getArgAsInt32(1));
        }
        else if (m.getAddress() == "/mouse/stop") {
            cout << "Mouse stop" << endl;
//            mouseX = m.getArgAsInt32(0);
//            mouseY = m.getArgAsInt32(1);
//            stopLaser(m.getArgAsInt32(0),m.getArgAsInt32(1));
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    drawFacade();
    drawInfo();

    if (laserEnabled) laser.draw();

    if (laserEnabled && showLaserGui){
        laserGui.setPosition(ofGetWidth() - laserGui.getWidth() - 10, 10);
        laserGui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::exit()
{
    if (laserEnabled)
        laserGui.saveToFile(STR_LASERSETTINGS_FILENAME);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    switch (key)
    {
        case '1':   setStateGenre(); break;
        case '2':   setStateWindows(); break;
        case 'f':   showFacade = !showFacade; break;
        case 'g':   showLaserGui = !showLaserGui; break;
        default:    break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    if (!drawingShape) return;

    ofPolyline &poly = laserPolylines.back();
    poly.addVertex(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    laserPolylines.push_back(ofPolyline());
    drawingShape = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    if (!drawingShape) return;

    ofPolyline &poly = laserPolylines.back();
    poly = poly.getSmoothed(2);
    drawingShape = false;

    // TODO add dot if the line is super short
}

#pragma mark - Custom drawing

//--------------------------------------------------------------
void ofApp::drawFacade()
{
    if (showFacade) imgFacade.draw(0, 0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::drawInfo()
{
    ofPushStyle();
    {
        int x = 10;

        ofSetColor(100, 50, 50, 200);
        ofDrawRectangle(0, ofGetHeight()-100, 240, ofGetHeight()-1);

        string stateMsg;
        switch(state)
        {
            case StateSelectGenre:      stateMsg = "Select Genre"; break;
            case StateSelectWindows:    stateMsg = "Select Windows"; break;
            default:                    break;
        }

        int windowHeight = ofGetHeight();

        ofSetColor(ofColor::gray);
        ofDrawBitmapString("Resolume Host: " + resolumeHost, x, windowHeight - 80);
        ofDrawBitmapString("Resolume Port: " + ofToString(resolumePort), x, windowHeight - 65);
        ofDrawBitmapString("MYO Port:      " + ofToString(myoPort), x, windowHeight - 50);

        ofSetColor(ofColor::orange);
        ofDrawBitmapString("State:         " + stateMsg, x, windowHeight - 25);

        ofSetColor(ofColor(0,255,0));
        ofDrawBitmapString(ofToString(roundf(ofGetFrameRate())) + "fps", x, windowHeight - 10);

    }
    ofPopStyle();
}

#pragma mark - State changes

//--------------------------------------------------------------
void ofApp::setStateGenre()
{
    state = StateSelectGenre;
}

//--------------------------------------------------------------
void ofApp::setStateWindows()
{
    state = StateSelectWindows;
}

#pragma mark - Laser

void ofApp::showLaserEffect(int effectnum)
{
    for (int i=0; i<laserPolylines.size(); ++i) {
        laser.addLaserPolyline(laserPolylines[i], ofColor::red);
    }
}
