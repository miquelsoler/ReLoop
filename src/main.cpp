#include "ofMain.h"
#include "ofApp.h"

const unsigned int W_WIDTH    = 1024;
const unsigned int W_HEIGHT   = 512;

int main( )
{
	ofSetupOpenGL(W_WIDTH, W_HEIGHT, OF_WINDOW);
	ofRunApp(new ofApp());
}
