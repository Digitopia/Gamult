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

#ifdef TARGET_OF_IOS
#include "ofxiOS.h"
#include "ofxiOSExtras.h"
#endif


class Touch; // TODO: something about the includes not working so that this needs to be here

#ifdef TARGET_OF_IOS
class ofApp : public ofxiOSApp {
#else
class ofApp : public ofBaseApp {
#endif

public:

	void setup();
	void update();
	void draw();

    void touchDown(ofTouchEventArgs &touch);
    void touchMoved(ofTouchEventArgs &touch);
    void touchUp(ofTouchEventArgs &touch);
    void touchDoubleTap(ofTouchEventArgs &touch){}
    void touchCancelled(ofTouchEventArgs &touch){}
   
    void mouseMoved(ofMouseEventArgs &mouse){}
    void mouseDragged(ofMouseEventArgs &mouse);
    void mousePressed(ofMouseEventArgs &mouse);
    void mouseReleased(ofMouseEventArgs &mouse);
    
    void keyPressed(int key);
    
	void drawLines();
	void drawLine(int nth);
	void initModules();
	void checkMultitouchData();
    int getModuleId(int x);

	static int nModules;
	static int nParticlesPerModule;
    static int maxParticleY; // TODO does this really needs to be static and here
	static Module** modules; // TODO make this a vector or something
	static ofxOscSender oscSender;
	static ofxOscReceiver oscReceiver;
    static int mouseId;
    static bool multitouch;
	
private:
    
    map<int,Touch> touches;
    bool inAbout;
    ofImage imgAbout;
    ofImage imgAboutIcon;
    ofRectangle aboutRect; // TODO: collision check should be with a circle and not rectangle
	
};

#endif
