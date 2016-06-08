#include "ofMain.h"
#include "ofApp.h"

#include "Defines.h"

int main( )
{
	ofSetupOpenGL(W_WIDTH * W_SCALEFACTOR, W_HEIGHT * W_SCALEFACTOR, OF_WINDOW);
	ofRunApp(new ofApp());
}
