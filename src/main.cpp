#include "ofMain.h"
#include "ofApp.h"

int main() {
    
    #ifdef TARGET_OF_IOS
    ofiOSWindowSettings window;
    window.enableRetina = true;
    window.enableHardwareOrientation = true;
    window.enableAntiAliasing = true;
    window.numOfAntiAliasingSamples = 4;
    ofCreateWindow(window);
    #else
    ofSetupOpenGL(1024, 768, OF_WINDOW);
    #endif

    ofRunApp(new ofApp());
    return 0;
}