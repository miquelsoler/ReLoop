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

const unsigned int COLUMN_BASEVIDEO = 5;
const unsigned int COLUMN_MUTERS = 6;
const unsigned int NUM_WINDOWS = 8;

typedef enum
{
    GenreNone = 0,
    GenreElectronica = 1,
    GenreFunk = 2,
    GenreHiphop = 3,
    GenreJazz = 4
} RLGenreIndex;


class MSResolumeOSCSender
{
public:

    MSResolumeOSCSender(string host, int port);

    void startBaseVideo();
    void stopBaseVideo();

    void startGenre(RLGenreIndex genreIndex);
    void stopGenre();

    void onWindowTouched(unsigned int numWindow);

private:

    void startWindow(unsigned int numWindow);
    void stopWindow(unsigned int numWindow);

    ofxOscSender oscSender;

    RLGenreIndex currentGenre = GenreNone;
    vector<bool> layersStatus; // For a column, it contains boolean values tell if the "window" is playing or not
};

#endif /* MSResolumeOSCSender_hpp */
