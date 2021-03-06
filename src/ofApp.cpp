#include "ofApp.h"

#include "Defines.h"
#include "MsAppSettings.hpp"

static const string APPSETTINGS_FILENAME = "AppSettings.xml";

static const string LASERSETTINGS_TITLE = "Laser";
static const string LASERSETTINGS_FILENAME = "LaserSettings.xml";
static const int LASERSETTINGS_WIDTH = 240;

static const string FACADESETTINGS_TITLE = "Facade";
static const string FACADESETTINGS_FILENAME = "FacadeSettings.xml";

static const float AREA_GEN_TOP_WIDTH = 0.321667;
static const float AREA_GEN_TOP_HEIGHT = 0.117895;
static const float AREA_GEN_BOTTOM_WIDTH = 0.186667;
static const float AREA_GEN_BOTTOM_HEIGHT = 0.203158;

static const float AREA_WDW_WIDTH = 0.088333;
static const float AREA_WDW_HEIGHT = 0.097895;
static const float AREA_WDW_Y = 0.101053;

///--------------------------------------------------------------

void ofApp::setup()
{
    ofBackground(ofColor::black);

    // Load app settings
    {
        MSAppSettings::getInstance().loadFile(APPSETTINGS_FILENAME);

        resolumeHost = MSAppSettings::getInstance().getResolumeHost();
        resolumePort = MSAppSettings::getInstance().getResolumePort();
        myoPort = MSAppSettings::getInstance().getMyoPort();
        mouseEnabled = MSAppSettings::getInstance().getMouseEnabled();
        laserEnabled = MSAppSettings::getInstance().getLaserEnabled();
        laserLength = MSAppSettings::getInstance().getLaserLength();
//        windowsSceneMaxDuration = MSAppSettings::getInstance().getWindowsSceneMaxDuration();
//        endingDuration = MSAppSettings::getInstance().getEndingDuration();
        clickEffectTime = MSAppSettings::getInstance().getClickEffectTime();
        clickEffectRadius = MSAppSettings::getInstance().getClickEffectRadius();
    }

    // Laser
    if (laserEnabled)
    {
        laserWidth = int(float(W_WIDTH * W_SCALE));
        laserHeight = int(float(W_HEIGHT * W_SCALE));

        laser.setup(laserWidth, laserHeight);
        laser.connectToEtherdream();

        laserGui.setup(LASERSETTINGS_TITLE, LASERSETTINGS_FILENAME);
        laserGui.add(laser.parameters);
        laserGui.add(laser.redParams);
        laserGui.add(laser.greenParams);
        laserGui.add(laser.blueParams);
//        laserGui.setWidthElements(LASERSETTINGS_WIDTH);
        laserGui.loadFromFile(LASERSETTINGS_FILENAME);
        
        laser.warp.loadSettings();
    }

    // Facade
    {
        facadeParams.setName("Facade");
        facadeParams.add(facadeX.set("X", 0, 0, ofGetWidth()));
        facadeParams.add(facadeY.set("Y", 0, 0, ofGetHeight()));
        facadeParams.add(facadeW.set("Width", ofGetWidth()/2, 0, ofGetWidth()));
        facadeParams.add(facadeH.set("Height", ofGetHeight()/2, 0, ofGetWidth()));

        facadeGui.setup(FACADESETTINGS_TITLE, FACADESETTINGS_FILENAME);
        facadeGui.add(facadeParams);
        facadeGui.loadFromFile(FACADESETTINGS_FILENAME);
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
            genreAreas.push_back(MSActiveArea(0, 0.054167, 0.222105, AREA_GEN_TOP_WIDTH, AREA_GEN_TOP_HEIGHT, W_SCALE));
            // Up-right
            genreAreas.push_back(MSActiveArea(1, 0.620000, 0.222105, AREA_GEN_TOP_WIDTH, AREA_GEN_TOP_HEIGHT, W_SCALE));
            // Down-left
            genreAreas.push_back(MSActiveArea(2, 0.054167, 0.449474, AREA_GEN_BOTTOM_WIDTH, AREA_GEN_BOTTOM_HEIGHT, W_SCALE));
            // Down-right
            genreAreas.push_back(MSActiveArea(3, 0.755833, 0.449474, AREA_GEN_BOTTOM_WIDTH, AREA_GEN_BOTTOM_HEIGHT, W_SCALE));
        }

        // Window Areas
        {
            windowAreas.push_back(MSActiveArea(0, 0.008333, AREA_WDW_Y, AREA_WDW_WIDTH, AREA_WDW_HEIGHT, W_SCALE));
            windowAreas.push_back(MSActiveArea(1, 0.133333, AREA_WDW_Y, AREA_WDW_WIDTH, AREA_WDW_HEIGHT, W_SCALE));
            windowAreas.push_back(MSActiveArea(2, 0.260000, AREA_WDW_Y, AREA_WDW_WIDTH, AREA_WDW_HEIGHT, W_SCALE));
            windowAreas.push_back(MSActiveArea(3, 0.388333, AREA_WDW_Y, AREA_WDW_WIDTH, AREA_WDW_HEIGHT, W_SCALE));
            windowAreas.push_back(MSActiveArea(4, 0.515000, AREA_WDW_Y, AREA_WDW_WIDTH, AREA_WDW_HEIGHT, W_SCALE));
            windowAreas.push_back(MSActiveArea(5, 0.640000, AREA_WDW_Y, AREA_WDW_WIDTH, AREA_WDW_HEIGHT, W_SCALE));
            windowAreas.push_back(MSActiveArea(6, 0.763333, AREA_WDW_Y, AREA_WDW_WIDTH, AREA_WDW_HEIGHT, W_SCALE));
            windowAreas.push_back(MSActiveArea(7, 0.888333, AREA_WDW_Y, AREA_WDW_WIDTH, AREA_WDW_HEIGHT, W_SCALE));
        }
    }

    facadeImage = ofImage("Facade.png");

    isDoingClickEffect = false;

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
            cout << ">>> Received /laser/start" << endl;
//            laserX = m.getArgAsInt32(0);
//            laserY = m.getArgAsInt32(1);
            startLaser(laserX, laserY);
        }
        else if (m.getAddress() == "/laser/stop") {
            cout << ">>> Received /laser/stop" << endl;
            pickArea(laserX, laserY);
////            laserX = m.getArgAsInt32(0);
////            laserY = m.getArgAsInt32(1);
//            stopLaser(laserX, laserY);
        }
    }

    if (isDoingClickEffect) {
        float deltaTime = ofGetElapsedTimef() - clickStartTime;
        if (deltaTime < clickEffectTime) {
            float radius = ofMap(deltaTime, 0, clickEffectTime, clickEffectRadius, 0);
//            float radius = explosionRadius * (deltaTime / explosionMaxTime);
            laser.addLaserCircle(ofPoint(clickEffectX, clickEffectY), radius, ofColor::green);
        } else {
            isDoingClickEffect = false;
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

    if (showFacadeGui) facadeGui.draw();
}

///--------------------------------------------------------------

void ofApp::exit()
{
    if (laserEnabled) laser.warp.saveSettings();
//    if (laserEnabled) laserGui.saveToFile(LASERSETTINGS_FILENAME);
}

///--------------------------------------------------------------

void ofApp::keyReleased(int key)
{
    switch (key)
    {
        case '1':   setStateGenre(); break;
        case '2':   setStateWindows(); break;
//        case '3':   setStateEnd(); break;
        case 'g':
            showLaserGui = !showLaserGui;
            showFacadeGui = !showFacadeGui;
            break;
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

//    laserX = x;
//    laserY = y;
//    stopLaser(laserX, laserY);
}

#pragma mark - Custom drawing

///--------------------------------------------------------------

void ofApp::drawFacade()
{
    if (showFacade)
        facadeImage.draw(facadeX, facadeY, facadeW, facadeH);
//        imgFacade.draw(0, 300, ofGetWidth() - (W_RIGHTPANEL_WIDTH * W_SCALE), ofGetHeight());
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
                ofDrawRectangle(facadeX + (genreAreas[i].x * facadeW),
                                facadeY + (genreAreas[i].y * facadeH),
                                genreAreas[i].w * facadeW,
                                genreAreas[i].h * facadeH);
            break;
        }
        case ScenePickWindows:
        {
            for (int i=0; i<windowAreas.size(); ++i)
                ofDrawRectangle(facadeX + (windowAreas[i].x * facadeW),
                                facadeY + (windowAreas[i].y * facadeH),
                                windowAreas[i].w * facadeW,
                                windowAreas[i].h * facadeH);
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

    resolumeOSCSender->stopGenre();
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
        laser.addLaserPolyline(laserPolylines[i], ofColor::green);
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

    poly.addVertex(ofMap(x, 0, ofGetWidth(), ofGetWidth()-1, 0), y);
}

///--------------------------------------------------------------

void ofApp::pickArea(int x, int y)
{
    bool found = false;
    unsigned int areaIndex = 0;

    isDoingClickEffect = true;
    clickStartTime = ofGetElapsedTimef();
    clickEffectX = x;
    clickEffectY = y;
//            windowsSceneRemainingTime = windowsSceneMaxDuration - (ofGetElapsedTimef() - windowsSceneStartTime);

    switch (scene)
    {
        case ScenePickGenre:
        {
            for (unsigned int i=0; i<genreAreas.size() && !found; ++i) {
                found = genreAreas[i].isPointInside(float(x - facadeX) / float(facadeW), float(y - facadeY) / float(facadeH));
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

                cout << "Picked " << genreIndex << endl;
            }
            break;
        }
        case ScenePickWindows:
        {
            for (unsigned int i=0; i<windowAreas.size() && !found; ++i) {
                found = windowAreas[i].isPointInside(float(x - facadeX) / float(facadeW), float(y - facadeY) / float(facadeH));
                if (found) areaIndex = i;
            }

            if (found) {
                resolumeOSCSender->onWindowTouched(areaIndex + 1);
                cout << "Picked " << areaIndex << endl;
            }
            break;
        }
        default: break;
    }

    if (!found) return;
}
