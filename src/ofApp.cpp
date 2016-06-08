#include "ofApp.h"

#include "MsAppSettings.hpp"

static const string STR_APPSETTINGS_FILENAME = "AppSettings.xml";

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
    }

    state = StateSelectGenre;

    resolumeOSCSender = new MSResolumeOSCSender(resolumeHost, resolumePort);
    myoOSCReceiver.setup(myoPort);
}

//--------------------------------------------------------------
void ofApp::update()
{
    int posX, posY;
    
    while (myoOSCReceiver.hasWaitingMessages())
    {
        ofxOscMessage m;
        myoOSCReceiver.getNextMessage(m);

        if (m.getAddress() == "/mouse/position")
        {
            posX = m.getArgAsInt32(0);
            posY = m.getArgAsInt32(1);
            cout << "Laser pos: " << posX << " " << posY << endl;
        }
        else if(m.getAddress() == "/mouse/start")
        {
            cout << "Mouse start" << endl;
//            posX = m.getArgAsInt32(0);
//            posX = m.getArgAsInt32(1);
//            startLaser(m.getArgAsInt32(0),m.getArgAsInt32(1));
        }
        else if (m.getAddress() == "/mouse/stop")
        {
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
    ofPushStyle();
    {
        int x = 10;
        string stateMsg;
        switch(state)
        {
            case StateSelectGenre:      stateMsg = "Select Genre"; break;
            case StateSelectWindows:    stateMsg = "Select Windows"; break;
            default:                    break;
        }
        int windowHeight = ofGetHeight();
        ofSetColor(ofColor::darkorange);
        ofDrawBitmapString("State:         " + stateMsg, x, windowHeight - 75);
        ofSetColor(ofColor::darkGreen);
        ofDrawBitmapString("Resolume Host: " + resolumeHost, x, windowHeight - 40);
        ofDrawBitmapString("Resolume Port: " + ofToString(resolumePort), x, windowHeight - 25);
        ofDrawBitmapString("MYO Port:      " + ofToString(myoPort), x, windowHeight - 10);
    }
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    switch (key)
    {
        case '1':   setStateGenre(); break;
        case '2':   setStateWindows(); break;
        default:    break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{

}

void ofApp::setStateGenre()
{
    state = StateSelectGenre;
}

void ofApp::setStateWindows()
{
    state = StateSelectWindows;
}
