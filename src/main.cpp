#include "ofMain.h"
#include "ofApp.h"

#include "Defines.h"

int main( )
{
    ofGLFWWindowSettings windowSettings;
    windowSettings.width = int(W_WIDTH * W_SCALE);
    windowSettings.height = int(W_HEIGHT * W_SCALE);
    windowSettings.windowMode = OF_WINDOW;
    windowSettings.resizable = false;

    ofCreateWindow(windowSettings);

    return ofRunApp(new ofApp);
}
