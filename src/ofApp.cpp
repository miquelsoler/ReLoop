#include "ofApp.h"

#include "Defines.h"
#include "MsAppSettings.hpp"

static const string STR_APPSETTINGS_FILENAME = "AppSettings.xml";
static const string STR_LASERSETTINGS_FILENAME = "LaserSettings.xml";

static const int LASER_GUI_WIDTH = 240;

static const int GEN_TOP_WIDTH = 386;
static const int GEN_TOP_HEIGHT = 224;
static const int GEN_BOTTOM_WIDTH = GEN_TOP_HEIGHT;
static const int GEN_BOTTOM_HEIGHT = GEN_TOP_WIDTH;

static const int WDW_WIDTH = 106;
static const int WDW_HEIGHT = 186;
static const int WDW_Y = 192;

///--------------------------------------------------------------

void ofApp::setup()
{
    ofBackground(ofColor::black);

    // Load app settings
    {
        MSAppSettings::getInstance().loadFile(STR_APPSETTINGS_FILENAME);

        resolumeHost = MSAppSettings::getInstance().getResolumeHost();
        resolumePort = MSAppSettings::getInstance().getResolumePort();
        myoPort = MSAppSettings::getInstance().getMyoPort();
        mouseEnabled = MSAppSettings::getInstance().getMouseEnabled();
        laserEnabled = MSAppSettings::getInstance().getLaserEnabled();
        laserLength = MSAppSettings::getInstance().getLaserLength();
//        windowsSceneMaxDuration = MSAppSettings::getInstance().getWindowsSceneMaxDuration();
//        endingDuration = MSAppSettings::getInstance().getEndingDuration();
    }

    // Laser
    if (laserEnabled)
    {
        laserWidth = W_WIDTH * W_SCALE;
        laserHeight = W_HEIGHT * W_SCALE;

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

    // Pick Areas
    {
        // Genre Areas
        {
            // Up-left
            genreAreas.push_back(MSActiveArea(0, 65, 422, GEN_TOP_WIDTH, GEN_TOP_HEIGHT, W_SCALE));
            // Up-right
            genreAreas.push_back(MSActiveArea(1, 744, 422, GEN_TOP_WIDTH, GEN_TOP_HEIGHT, W_SCALE));
            // Down-left
            genreAreas.push_back(MSActiveArea(2, 65, 854, GEN_BOTTOM_WIDTH, GEN_BOTTOM_HEIGHT, W_SCALE));
            // Down-right
            genreAreas.push_back(MSActiveArea(3, 907, 854, GEN_BOTTOM_WIDTH, GEN_BOTTOM_HEIGHT, W_SCALE));
        }

        // Window Areas
        {
            windowAreas.push_back(MSActiveArea(0, 10, WDW_Y, WDW_WIDTH, WDW_HEIGHT, W_SCALE));
            windowAreas.push_back(MSActiveArea(1, 160, WDW_Y, WDW_WIDTH, WDW_HEIGHT, W_SCALE));
            windowAreas.push_back(MSActiveArea(2, 312, WDW_Y, WDW_WIDTH, WDW_HEIGHT, W_SCALE));
            windowAreas.push_back(MSActiveArea(3, 466, WDW_Y, WDW_WIDTH, WDW_HEIGHT, W_SCALE));
            windowAreas.push_back(MSActiveArea(4, 618, WDW_Y, WDW_WIDTH, WDW_HEIGHT, W_SCALE));
            windowAreas.push_back(MSActiveArea(5, 768, WDW_Y, WDW_WIDTH, WDW_HEIGHT, W_SCALE));
            windowAreas.push_back(MSActiveArea(6, 916, WDW_Y, WDW_WIDTH, WDW_HEIGHT, W_SCALE));
            windowAreas.push_back(MSActiveArea(7, 1066, WDW_Y, WDW_WIDTH, WDW_HEIGHT, W_SCALE));
        }
    }

    imgFacade = ofImage("Facade.png");

    setStateGenre();
}

///--------------------------------------------------------------

void ofApp::update()
{
    if (laserEnabled) {
        showLaserEffect(0);
        laser.update();
    }

    // Manage Myo events

    while (myoOSCReceiver.hasWaitingMessages())
    {
        ofxOscMessage m;
        myoOSCReceiver.getNextMessage(m);

        if (m.getAddress() == "/laser/position") {
            laserX += (m.getArgAsInt32(0) - laserX) * 0.2;
            laserY += (m.getArgAsInt32(1) - laserY) * 0.2;
            coordinatesToLaser(laserX, laserY);
        }
        else if(m.getAddress() == "/laser/start") {
//            laserX = m.getArgAsInt32(0);
//            laserY = m.getArgAsInt32(1);
            startLaser(laserX, laserY);
        }
        else if (m.getAddress() == "/laser/stop") {
//            laserX = m.getArgAsInt32(0);
//            laserY = m.getArgAsInt32(1);
            stopLaser(laserX, laserY);
        }
    }

//    // Manage scene changes
//
//    switch (scene) {
//        case ScenePickWindows: {
//            windowsSceneRemainingTime = windowsSceneMaxDuration - (ofGetElapsedTimef() - windowsSceneStartTime);
//            if (ofGetElapsedTimef() - windowsSceneStartTime > float(windowsSceneMaxDuration))
//                setStateEnd();
//            break;
//        }
//        case SceneEnd: {
//            endingRemainingTime = endingDuration - (ofGetElapsedTimef() - endingStartTime);
//            if (ofGetElapsedTimef() - endingStartTime > float(endingDuration))
//                setStateGenre();
//            break;
//        }
//        default: break;
//    }
}

///--------------------------------------------------------------

void ofApp::draw()
{
    drawFacade();
    drawInfo();
    drawPickAreas();

    if (laserEnabled) laser.draw();

    if (laserEnabled && showLaserGui) {
        laserGui.setPosition(ofGetWidth() - laserGui.getWidth() - 10, 10);
        laserGui.draw();
    }
}

///--------------------------------------------------------------

void ofApp::exit()
{
    if (laserEnabled) laserGui.saveToFile(STR_LASERSETTINGS_FILENAME);
}

///--------------------------------------------------------------

void ofApp::keyReleased(int key)
{
    switch (key)
    {
        case '1':   setStateGenre(); break;
        case '2':   setStateWindows(); break;
//        case '3':   setStateEnd(); break;
        case 'g':   showLaserGui = !showLaserGui; break;
        case 'f':   showFacade = !showFacade; break;
        default:    break;
    }
}

///--------------------------------------------------------------

void ofApp::mouseDragged(int x, int y, int button)
{
    if (!mouseEnabled) return;
    if (!drawingShape) return;

    ofPolyline &poly = laserPolylines.back();
    poly.addVertex(x, y);

//    laserX += (x - laserX) * 0.2;
//    laserY += (y - laserY) * 0.2;
//    coordinatesToLaser(laserX, laserY);
}

///--------------------------------------------------------------

void ofApp::mousePressed(int x, int y, int button)
{
    if (!mouseEnabled) return;

    laserPolylines.push_back(ofPolyline());
    drawingShape = true;

    pickArea(x, y);

//    laserX = x;
//    laserY = y;
//    startLaser(laserX, laserY);
}

///--------------------------------------------------------------

void ofApp::mouseReleased(int x, int y, int button)
{
    if (!mouseEnabled) return;
    if (!drawingShape) return;

    ofPolyline &poly = laserPolylines.back();
    poly = poly.getSmoothed(2);
    drawingShape = false;

    // TODO add dot if the line is super short

//    laserX = x;
//    laserY = y;
//    stopLaser(laserX, laserY);
}

#pragma mark - Custom drawing

///--------------------------------------------------------------

void ofApp::drawFacade()
{
    if (showFacade) imgFacade.draw(0, 0, ofGetWidth() - (W_RIGHTPANEL_WIDTH * W_SCALE), ofGetHeight());
}

///--------------------------------------------------------------

void ofApp::drawPickAreas()
{
    if (!showFacade) return;

    ofPushStyle();
    ofSetColor(255, 0, 0, 100);

    switch(scene)
    {
        case ScenePickGenre:
        {
            for (int i=0; i<genreAreas.size(); ++i)
                ofDrawRectangle(genreAreas[i].x, genreAreas[i].y, genreAreas[i].w, genreAreas[i].h);
            break;
        }
        case ScenePickWindows:
        {
            for (int i=0; i<windowAreas.size(); ++i)
                ofDrawRectangle(windowAreas[i].x, windowAreas[i].y, windowAreas[i].w, windowAreas[i].h);
            break;
        }
        default: break;
    }

    ofPopStyle();
}

///--------------------------------------------------------------

void ofApp::drawInfo()
{
    ofPushStyle();
    {
        int x = 10;

        ofSetColor(100, 50, 50, 200);
        ofDrawRectangle(0, ofGetHeight()-100, 380, ofGetHeight()-1);

        stringstream stateMsg;
        switch(scene)
        {
            case ScenePickGenre:    stateMsg << "Select Genre"; break;
//            case ScenePickWindows:  stateMsg << "Select Windows (time left: " << int(windowsSceneRemainingTime) << ")"; break;
            case ScenePickWindows:  stateMsg << "Select Windows"; break;
//            case SceneEnd:          stateMsg << "Ending (time left: " << int(endingRemainingTime) << ")"; break;
            default:                break;
        }

        int windowHeight = ofGetHeight();

        ofSetColor(ofColor::gray);
        ofDrawBitmapString("Resolume Host: " + resolumeHost, x, windowHeight - 80);
        ofDrawBitmapString("Resolume Port: " + ofToString(resolumePort), x, windowHeight - 65);
        ofDrawBitmapString("MYO Port:      " + ofToString(myoPort), x, windowHeight - 50);

        ofSetColor(ofColor::orange);
        ofDrawBitmapString("State:         " + stateMsg.str(), x, windowHeight - 25);

        ofSetColor(ofColor(0,255,0));
        ofDrawBitmapString(ofToString(roundf(ofGetFrameRate())) + "fps", x, windowHeight - 10);

    }
    ofPopStyle();
}

#pragma mark - State changes

///--------------------------------------------------------------

void ofApp::setStateGenre()
{
    scene = ScenePickGenre;

    resolumeOSCSender->startBaseVideo();
}

///--------------------------------------------------------------

void ofApp::setStateWindows()
{
    resolumeOSCSender->stopBaseVideo();

    scene = ScenePickWindows;
//    windowsSceneStartTime = ofGetElapsedTimef();
//    windowsSceneRemainingTime = windowsSceneMaxDuration;
}

//void ofApp::setStateEnd()
//{
//    resolumeOSCSender->stopGenre();
//
//    scene = SceneEnd;
//    endingStartTime = ofGetElapsedTimef();
//    endingRemainingTime = endingDuration;
//}

#pragma mark - Laser

///--------------------------------------------------------------

void ofApp::showLaserEffect(int effectnum)
{
    for (int i=0; i<laserPolylines.size(); ++i) {
        laser.addLaserPolyline(laserPolylines[i], ofColor::red);
    }
}

///--------------------------------------------------------------

void ofApp::startLaser(int x, int y)
{
    cout << "Start laser" << endl;
    laserPolylines.push_back(ofPolyline());
    drawingShape = true;
}

///--------------------------------------------------------------

void ofApp::stopLaser(int x, int y)
{
    cout << "Stop laser" << endl;
    if (laserPolylines.size() < 1) return;

    laserPolylines.clear();

//    ofPolyline &poly = laserPolylines.back();
//    poly = poly.getSmoothed(2);
    drawingShape = false;
}

///--------------------------------------------------------------

void ofApp::coordinatesToLaser(int x, int y)
{
    if (laserPolylines.size() < 1) return;

    ofPolyline &poly = laserPolylines.back();

    if (poly.size() > laserLength)
        poly.getVertices().erase(poly.getVertices().begin());

    poly.addVertex(x, y);
}

///--------------------------------------------------------------

void ofApp::pickArea(int x, int y)
{
    bool found = false;
    unsigned int areaIndex = 0;

    switch (scene)
    {
        case ScenePickGenre:
        {
            for (unsigned int i=0; i<genreAreas.size() && !found; ++i) {
                found = genreAreas[i].isPointInside(x, y);
                if (found) areaIndex = i;
            }

            if (found) {
                switch(areaIndex) {
                    case 0: genreIndex = GenreElectronica; break;
                    case 1: genreIndex = GenreFunk; break;
                    case 2: genreIndex = GenreHiphop; break;
                    case 3: genreIndex = GenreJazz; break;
                    default: break;
                }
                setStateWindows();
                resolumeOSCSender->startGenre(genreIndex);
            }
            break;
        }
        case ScenePickWindows:
        {
            for (unsigned int i=0; i<windowAreas.size() && !found; ++i) {
                found = windowAreas[i].isPointInside(x, y);
                if (found) areaIndex = i;
            }

            if (found) {
                resolumeOSCSender->onWindowTouched(areaIndex + 1);
            }
            break;
        }
        default: break;
    }

    if (!found) return;
}
