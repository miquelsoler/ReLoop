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

void MSResolumeOSCSender::enableClip(unsigned int layer, unsigned int clip, bool enabled)
{
    ofxOscMessage m1, m2;
    stringstream addr1, addr2;

    if (enabled)
    {
        addr2 << "/layer" << layer << "/clear";
        m2.setAddress(addr2.str());
        m2.addIntArg(0);
        oscSender.sendMessage(m2);
        cout << "Sending OSC: " << addr2.str() << " 0" << endl;

        addr1 << "/layer" << layer << "/clip" << clip << "/connect";
        m1.setAddress(addr1.str());
        m1.addIntArg(1);
        oscSender.sendMessage(m1);
        cout << "Sending OSC: " << addr1.str() << " 1" << endl;
    }
    else
    {
        addr2 << "/layer" << layer << "/clear";
        m2.setAddress(addr2.str());
        m2.addIntArg(1);
        oscSender.sendMessage(m2);
        cout << "Sending OSC: " << addr2.str() << " 1" << endl;

        addr1 << "/layer" << layer << "/clip" << clip << "/connect";
        m1.setAddress(addr1.str());
        m1.addIntArg(0);
        oscSender.sendMessage(m1);
        cout << "Sending OSC: " << addr1.str() << " 0" << endl;
    }
}
