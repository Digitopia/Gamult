#include "ofMain.h"
#include "ofApp.h"

int main() {
	  ofSetupOpenGL(1024, 768, OF_FULLSCREEN);
//    ofSetupOpenGL(1024/2, 768/2, OF_WINDOW);
	ofRunApp(new ofApp());
}
