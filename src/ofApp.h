#ifndef OFAPP_H
#define OFAPP_H

#include "ofMain.h"
#include "ofxOsc.h"

#include "Button.h"
#include "Fader.h"
#include "Module.h"
#include "ModuleConsole.h"
#include "Particle.h"
#include "Touch.h"
//#include "Constants.h"

#ifdef TARGET_OF_IOS
#include "ofxiOS.h"
#include "ofxiOSExtras.h"
#endif


enum appState {
    SPLASH_SCREEN,
    SPLASH_FADE,
    FADING_ABOUT,
    ABOUT,
    ABOUT_DESCENDING,
    ABOUT_ASCENDING,
    APP,
    BAR_ASCENDING,
    BAR_DESCENDING,
    BAR
};

enum inactivityStateEnum {
    ACTIVE, // default state
    PRE_INACTIVE, // after a min of no interaction, resets controls and clears particles
    INACTIVE, // when the auto generated animations are happening
    POST_INACTIVE // when the auto generated animations have finished already (it's simply transitory, goes directly to ACTIVE after)
};


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
   
    void mouseMoved(ofMouseEventArgs &mouse);
    void mouseDragged(ofMouseEventArgs &mouse);
    void mousePressed(ofMouseEventArgs &mouse);
    void mouseReleased(ofMouseEventArgs &mouse);
    
    void keyPressed(int key);
    
	void drawLines();
	void drawLine(int nth);
	void initModules();
	void checkMultitouchData();
    int getModuleId(int x);
    void drawArrow(bool up);
    void drawBouncingArrow();
    
    void handleInactivity();
    bool hasParticles();
    void resetModules();
    void resetInactivityTime();

	static int nModules;
	static int nParticlesPerModule;
    static int maxParticleY; // TODO does this really needs to be static and here
	static Module** modules; // TODO make this a vector or something
	static ofxOscSender oscSender;
	static ofxOscReceiver oscReceiver;
    static int mouseId;
    static bool inactive;
    static bool multitouch;
    static unsigned int inactivityCounter;
    static unsigned int currentAlpha;
    
private:
    
    map<int,Touch> touches;
    
    appState state;
    inactivityStateEnum inactivityState;
    
    ofRectangle barRect;
    ofRectangle aboutRect;
    
    ofImage imgAbout;
    ofImage imgSplashScreen;
    ofImage imgArrow;
    ofImage imgArrowDown;
    
    int barY;
    int aboutY;
    int arrowDownY;
    int arrowDownYBase;
    int arrowDownDir;
    
    int splashAlpha;
    
    int moduleWidth;
    int moduleHeight;
    
    unsigned int inactivityThreshold;
    unsigned int inactivityThresholdWithinParticles;
    
};
    
#endif
