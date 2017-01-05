#pragma once

#ifndef OFAPP_H
#define OFAPP_H

#include "ofMain.h"

#define ofLogNotice() ofLogNotice() << ofGetTimestampString("[%H:%M:%S.%i] ")

#if defined TARGET_OF_IOS
    #include "ofxiOS.h"
    #include "ofxiOSExtras.h"
    #include "swipeRecognition.h"
#elif defined TARGET_SEMIBREVE
    #include "ofxOsc.h"
#endif

#if defined TARGET_SEMIBREVE
  #include "ofxOsc.h"
#endif

#if defined TARGET_ANDROID
  #include "ofxAndroid.h"
#endif

// NOTE: need to forward declare Touch, because of Touch requiring ofApp and ofApp requiring Touch.
class Button;
class Fader;
class Module;
class ModuleConsole;
class Particle;
class Touch;

#include "Module.h"
#include "ModuleConsole.h"
#include "Button.h"
#include "Fader.h"
#include "Particle.h"
#include "Touch.h"
// #include "Constants.h"

enum appStateEnum {
  SPLASH_SCREEN,
  SPLASH_FADE,
  ABOUT,
  ABOUT_DESCENDING,
  ABOUT_ASCENDING,
  APP,
  BAR_ASCENDING,
  BAR_DESCENDING,
  BAR
};

enum inactivityStateEnum {
  ACTIVE,         // default state
  PRE_INACTIVE,   // after a min of no interaction, resets controls and clears particles
  INACTIVE,       // when the auto generated animations are happening
  POST_INACTIVE   // when the auto generated animations have finished already (it's simply transitory, goes directly to ACTIVE after)
};

#if defined TARGET_ANDROID
class ofApp : public ofxAndroidApp {
#elif defined TARGET_OF_IOS
    class ofApp : public ofxiOSApp {
#else
    class ofApp : public ofBaseApp {
#endif

public:

    void setup();
    void update();
    void draw();

    void mouseMoved(ofMouseEventArgs& mouse);
    void mouseDragged(ofMouseEventArgs& mouse);
    void mousePressed(ofMouseEventArgs& mouse);
    void mouseReleased(ofMouseEventArgs& mouse);

    void touchDown(ofTouchEventArgs& touch);
    void touchMoved(ofTouchEventArgs& touch);
    void touchUp(ofTouchEventArgs& touch);
    void touchDoubleTap(ofTouchEventArgs& touch) {}
    void touchCancelled(ofTouchEventArgs& touch) {}

    void deviceOrientationChanged(int newOrientation);

    void keyPressed(int key);

    void drawParticles();
    void drawModules();
    void drawTouches();
    void drawLines();
    void drawLine(uint nth);
    void setupModules();
    void loadModuleSounds();

    void appStateHandler();
    void inactivityHandler();

    static size_t getModuleIdx(unsigned int x);
    void drawArrow(bool up);
    void drawBouncingArrow();

    bool hasParticles();
    void resetModules();
    void resetInactivityTime();

    void updateNewModuleActive(int x);

    void initModules();
    void initImages();
        
    static int getFontSize();

    #if defined TARGET_OF_IOS
      void onSwipe(swipeRecognitionArgs& args);
      void shakeHandler();
    #endif

    #if defined TARGET_SEMIBREVE
      void oscMultitouchHandler();
      static ofxOscSender oscSender;
      static ofxOscReceiver oscReceiver;
    #endif

    #ifdef TARGET_ANDROID
	    void pause(){}
	    void stop(){}
	    void resume(){}
	    void reloadTextures(){}

	    // bool backPressed(){}
	    void okPressed(){}
	    void cancelPressed(){}

	    // void touchDown(int x, int y, int id);
	    // void touchMoved(int x, int y, int id);
	    // void touchUp(int x, int y, int id);
	    // void touchDoubleTap(int x, int y, int id);
	    // void touchCancelled(int x, int y, int id);
	    void swipe(ofxAndroidSwipeDir swipeDir, int id){}
	  #endif

    static vector<string> getSoundPaths(unsigned int index);

    static unsigned int moduleActive;

    static int maxParticleY; // TODO does this really needs to be static and here?

    // helper methods
    static bool isOsx();
    static bool isSemibreve();
    static bool isIos();
    static bool isAndroid();
    static bool isPhone();
    static bool isTablet();
    static bool isTabletInPortrait();
    static bool isTabletInLandscape();
        

    static int mouseId;

    static bool inactive;
    static bool multitouch;
    static unsigned int inactivityCounter;
    static unsigned int currentAlpha;

    static vector<Module*> modules;

private:

    map<int,Touch> touches;

    appStateEnum appState;
    inactivityStateEnum inactivityState;

    ofRectangle barRect;
    ofRectangle aboutRect;

    ofImage imgAbout;
    ofImage imgSplashScreen;
    ofImage imgArrow;
    ofImage imgArrowDown;
    ofImage imgSwipeInfo;

    int barY;
    int aboutY;
    int arrowDownY;
    int arrowDownYBase;
    int arrowDownDir;

    ofTrueTypeFont swipeFont;

    int splashAlpha;

    int crop;

    int pY; //for toucMoved() to scroll aboutImage on iPhone;

    bool showSwipeInfo;

    unsigned int inactivityThreshold;
    unsigned int inactivityThresholdWithinParticles;

    #if defined TARGET_OF_IOS
      swipeRecognition swiper;
      bool swiping;
      int accelCount;
    #endif

};

#endif
