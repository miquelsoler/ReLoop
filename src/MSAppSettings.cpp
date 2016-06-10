//
//  MSAppSettings.cpp
//  ReLoop
//
//  Created by Miquel Àngel Soler on 8/6/16.
//
//

#include "MSAppSettings.hpp"
#include "ofxXmlSettings.h"

const static string STR_TAG_MAIN            = "AppSettings";

const static string STR_TAG_RESOLUMEHOST    = "ResolumeHost";
const static string STR_TAG_RESOLUMEPORT    = "ResolumePort";
const static string STR_TAG_MYOPORT         = "MyoPort";
const static string STR_TAG_MOUSEENABLED    = "MouseEnabled";
const static string STR_TAG_LASERENABLED    = "LaserEnabled";
const static string STR_TAG_LASERLENGTH     = "LaserLength";

const static string DEFAULT_RESOLUMEHOST    = "localhost";
const static int    DEFAULT_RESOLUMEPORT    = 30301;
const static int    DEFAULT_MYOPORT         = 30302;
const static string DEFAULT_MOUSEENABLED    = "false";
const static string DEFAULT_LASERENABLED    = "true";
const static int    DEFAULT_LASERLENGTH     = 10;

bool MSAppSettings::loadFile(string filename)
{
    ofxXmlSettings settings;

    bool loaded = settings.loadFile(filename);
    if (loaded)
    {
        // Check XML validity
        {
            loaded = settings.tagExists(STR_TAG_MAIN);
            loaded = loaded &&
            settings.tagExists(STR_TAG_MAIN + ":" + STR_TAG_RESOLUMEHOST) &&
            settings.tagExists(STR_TAG_MAIN + ":" + STR_TAG_RESOLUMEPORT) &&
            settings.tagExists(STR_TAG_MAIN + ":" + STR_TAG_MYOPORT) &&
            settings.tagExists(STR_TAG_MAIN + ":" + STR_TAG_MOUSEENABLED) &&
            settings.tagExists(STR_TAG_MAIN + ":" + STR_TAG_LASERENABLED) &&
            settings.tagExists(STR_TAG_MAIN + ":" + STR_TAG_LASERLENGTH);

            if (!loaded)
            {
                createSettingsFile(filename);
                loadFile(filename);
            }
        }

        resolumeHost = ofToString(settings.getValue(STR_TAG_MAIN + ":" + STR_TAG_RESOLUMEHOST, DEFAULT_RESOLUMEHOST));
        resolumePort = int(settings.getValue(STR_TAG_MAIN + ":" + STR_TAG_RESOLUMEPORT, DEFAULT_RESOLUMEPORT));
        myoPort = float(settings.getValue(STR_TAG_MAIN + ":" + STR_TAG_MYOPORT, DEFAULT_MYOPORT));
        mouseEnabled = (ofToLower(ofToString(settings.getValue(STR_TAG_MAIN + ":" + STR_TAG_MOUSEENABLED, DEFAULT_MOUSEENABLED))) == "true") ? true : false;
        laserEnabled = (ofToLower(ofToString(settings.getValue(STR_TAG_MAIN + ":" + STR_TAG_LASERENABLED, DEFAULT_LASERENABLED))) == "true") ? true : false;
        laserLength = int(settings.getValue(STR_TAG_MAIN + ":" + STR_TAG_LASERLENGTH, DEFAULT_LASERLENGTH));
    }
    else
    {
        createSettingsFile(filename);
        loadFile(filename);
    }

    return false;
}

void MSAppSettings::createSettingsFile(string filename)
{
    ofxXmlSettings settings;
    settings.setValue(STR_TAG_MAIN + ":" + STR_TAG_RESOLUMEHOST, DEFAULT_RESOLUMEHOST);
    settings.setValue(STR_TAG_MAIN + ":" + STR_TAG_RESOLUMEPORT, DEFAULT_RESOLUMEPORT);
    settings.setValue(STR_TAG_MAIN + ":" + STR_TAG_MYOPORT, DEFAULT_MYOPORT);
    settings.setValue(STR_TAG_MAIN + ":" + STR_TAG_MOUSEENABLED, DEFAULT_MOUSEENABLED);
    settings.setValue(STR_TAG_MAIN + ":" + STR_TAG_LASERENABLED, DEFAULT_LASERENABLED);
    settings.addValue(STR_TAG_MAIN + ":" + STR_TAG_LASERLENGTH, DEFAULT_LASERLENGTH);
    settings.saveFile(filename);
}
