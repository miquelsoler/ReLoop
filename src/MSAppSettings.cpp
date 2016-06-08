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

const static string DEFAULT_RESOLUMEHOST    = "localhost";
const static int    DEFAULT_RESOLUMEPORT    = 30301;
const static int    DEFAULT_MYOPORT         = 30302;

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
            settings.tagExists(STR_TAG_MAIN + ":" + STR_TAG_MYOPORT);

            if (!loaded)
            {
                createSettingsFile(filename);
                loadFile(filename);
            }
        }

        resolumeHost = ofToString(settings.getValue(STR_TAG_MAIN + ":" + STR_TAG_RESOLUMEHOST, DEFAULT_RESOLUMEHOST));
        resolumePort = int(settings.getValue(STR_TAG_MAIN + ":" + STR_TAG_RESOLUMEPORT, DEFAULT_RESOLUMEPORT));
        myoPort = float(settings.getValue(STR_TAG_MAIN + ":" + STR_TAG_MYOPORT, DEFAULT_MYOPORT));
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
    settings.saveFile(filename);
}
