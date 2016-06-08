//
//  MSResolumeOSCSender.hpp
//  RLResolumeTrigger
//
//  Created by Miquel Àngel Soler on 6/6/16.
//
//

#ifndef MSResolumeOSCSender_hpp
#define MSResolumeOSCSender_hpp

#include <stdio.h>
#include "ofxOsc.h"

class MSResolumeOSCSender
{
public:

    MSResolumeOSCSender(string host, int port);

    void enableClip(unsigned int layer, unsigned int clip, bool enabled);

private:

    ofxOscSender oscSender;
};

#endif /* MSResolumeOSCSender_hpp */
