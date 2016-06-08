//
//  MSAppSettings.hpp
//  ReLoop
//
//  Created by Miquel Àngel Soler on 8/6/16.
//
//

#ifndef MSAppSettings_hpp
#define MSAppSettings_hpp

#include <stdio.h>
#include "ofMain.h"

class MSAppSettings
{
public:

    static MSAppSettings &getInstance() {
        static MSAppSettings instance;
        return instance;
    }

    bool loadFile(string filename);

    string getResolumeHost()        { return resolumeHost; };
    int getResolumePort()           { return resolumePort; };
    int getMyoPort()                { return myoPort; };

private:

    void createSettingsFile(string filename);

    string  resolumeHost;
    int     resolumePort;
    int     myoPort;
};

#endif /* MSAppSettings_hpp */
