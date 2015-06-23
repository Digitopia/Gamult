#ifndef OFAPP_H
#define OFAPP_H

#include "ofMain.h"
#include "ofxOsc.h"

#include "Button.h"
#include "Fader.h"
#include "Module.h"
#include "ModuleConsole.h"
#include "Particle.h"
#include "Panel.h"
#include "Touch.h"
//#include "Constants.h"

class Touch; // TODO: something about the includes not working so that this needs to be here

class ofApp : public ofBaseApp {

public:

	// OF stuff
	void setup();
	void update();
	void draw();

	// events
	void keyPressed(int key){}
	void keyReleased(int key){}
	void windowResized(int w, int h){}
	void mouseMoved(int x, int y){}
	void mouseDragged(int x, int y, int button){}
	void mousePressed(int x, int y, int button){}
	void mouseReleased(int x, int y, int button){}
    void touchDown(ofTouchEventArgs &touch);
    void touchMoved(ofTouchEventArgs &touch);
    void touchUp(ofTouchEventArgs &touch);
    void touchDoubleTap(ofTouchEventArgs &touch){}
    void touchCancelled(ofTouchEventArgs &touch){}

	// custom
	void drawLines();
	void drawLine(int nth);
	void initModules();
	void checkMultitouchData();
    int getModuleId(int x);

	static int nModules;
	static int nParticlesPerModule;
	static Module** modules; // TODO make this a vector or something
	static ofxOscSender oscSender;
	static ofxOscReceiver oscReceiver;
    
    static int maxParticleY; // TODO does this really needs to be static and here
	
private:
    
    map<int, Touch> touches;

    bool infoDisplay;
    ofImage about;
    ofImage info;
    ofRectangle infoBox;
	
};

#endif
