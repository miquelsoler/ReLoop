//
//  MSResolumeOSCSender.cpp
//  RLResolumeTrigger
//
//  Created by Miquel Àngel Soler on 6/6/16.
//
//

#include "MSResolumeOSCSender.hpp"

MSResolumeOSCSender::MSResolumeOSCSender(string host, int port)
{
    oscSender.setup(host, port);
}

void MSResolumeOSCSender::startBaseVideo()
{
    ofxOscMessage msg;
    stringstream address;

    address << "/layer" << NUM_WINDOWS << "/clip" << COLUMN_BASEVIDEO << "/connect";
    msg.setAddress(address.str());
    msg.addIntArg(1);
    oscSender.sendMessage(msg);

    cout << "Sending START BASE VIDEO: " << msg.getAddress() << " 1" << endl;
}

void MSResolumeOSCSender::stopBaseVideo()
{
    ofxOscMessage msg;
    stringstream address;

    address << "/layer" << NUM_WINDOWS << "/clip" << COLUMN_MUTERS << "/connect";
    msg.setAddress(address.str());
    msg.addIntArg(1);
    oscSender.sendMessage(msg);

    cout << "Sending STOP BASE VIDEO: " << msg.getAddress() << " 1" << endl;
}

void MSResolumeOSCSender::startGenre(RLGenreIndex genreIndex)
{
    layersStatus.clear();
    for (unsigned int i=0; i<NUM_WINDOWS; ++i)
        layersStatus.push_back(false);

    currentGenre = genreIndex;
}

void MSResolumeOSCSender::stopGenre()
{
    layersStatus.clear();
    for (unsigned int i=0; i<NUM_WINDOWS; ++i)
        layersStatus.push_back(false);

    ofxOscMessage msg;
    msg.setAddress("/composition/disconnectall");
    msg.addIntArg(1);
    oscSender.sendMessage(msg);

    cout << "Sending STOP GENRE: " << msg.getAddress() << " 1" << endl;

    currentGenre = GenreNone;
}

void MSResolumeOSCSender::onWindowTouched(unsigned int numWindow)
{
    if (layersStatus[numWindow - 1] == false) {
        startWindow(numWindow);
        layersStatus[numWindow - 1] = true;
    } else {
        stopWindow(numWindow);
        layersStatus[numWindow - 1] = false;
    }
}

void MSResolumeOSCSender::startWindow(unsigned int numWindow)
{
    ofxOscMessage msg;
    stringstream address;

    int layer = NUM_WINDOWS - numWindow + 1;

    address << "/layer" << layer << "/clip" << currentGenre << "/connect";
    msg.setAddress(address.str());
    msg.addIntArg(1);
    oscSender.sendMessage(msg);

    cout << "Sending START WINDOW: " << address.str() << " 1" << endl;
}

void MSResolumeOSCSender::stopWindow(unsigned int numWindow)
{
    ofxOscMessage msg;
    stringstream address;

    int layer = NUM_WINDOWS - numWindow + 1;

    address << "/layer" << layer << "/clip" << COLUMN_MUTERS << "/connect";
    msg.setAddress(address.str());
    msg.addIntArg(1);
    oscSender.sendMessage(msg);

    cout << "Sending STOP WINDOW: " << address.str() << " 1" << endl;
}

