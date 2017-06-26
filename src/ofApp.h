#pragma once

#ifndef OFAPP_H
#define OFAPP_H

#include "ofMain.h"

#define ofLogNotice() ofLogNotice() << ofGetTimestampString("[%H:%M:%S.%i] ")

#include "ofxImage.h"

#if defined TARGET_OF_IOS
    #include "ofxiOS.h"
    #include "ofxiOSExtras.h"
    #include "SwipeRecognition.h"
    #include "Wrapper.h"
#elif defined TARGET_SEMIBREVE
    #include "ofxOsc.h"
#endif

#if defined TARGET_SEMIBREVE
  #include "ofxOsc.h"
#endif

#if defined TARGET_ANDROID
  #include "ofxAndroid.h"
  #include "ofxAccelerometer.h"
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
    void setLanguageBBoxes();
    void changeLanguage(string language);

    static size_t getModuleIdx(unsigned int x);
    void drawArrow(bool up);
    void drawBouncingArrow();

    bool hasParticles();
    void resetModules();
    void resetInactivityTime();

    void updateNewModuleActive(int x);

    void initTranslations();
    void initModules();
    void initImages();

    static int getFontSize();
    static string getSystemLanguage();

    #if defined TARGET_OF_IOS
    void onSwipe(SwipeRecognitionArgs& args);
    #endif

    void shakeHandler();

    #if defined TARGET_SEMIBREVE
    void oscMultitouchHandler();
    static ofxOscSender oscSender;
    static ofxOscReceiver oscReceiver;
    #endif

    #if defined TARGET_ANDROID
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
	    void swipe(ofxAndroidSwipeDir swipeDir, int id);
	  #endif

    static vector<string> getSoundPaths(unsigned int index);

    static unsigned int moduleActive; // NOTE: when in tablets, this indicates which is the active module when in portrait

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

    static size_t getBaseModuleOffsetSound(uint imodule);

    static int mouseId;

    static bool inactive;
    static bool multitouch;
    static unsigned int inactivityCounter;
    static unsigned int currentAlpha;
    static string language;
    static map<string,string> translations;

    static vector<Module*> modules;

    #if !defined TARGET_OF_IOS
    static vector<ofSoundPlayer> sounds;
    #endif

private:

    map<int,Touch> touches;

    appStateEnum appState;
    inactivityStateEnum inactivityState;

    ofRectangle barRect;
    ofRectangle ptLangRect, enLangRect;

#if defined TARGET_OF_IOS
    ofxImage imgAbout;
    ofxImage imgSplashScreen;
    ofxImage imgArrow;
    ofxImage imgArrowDown;
    ofxImage imgSwipeInfo;
#else
    ofImage imgAbout;
    ofImage imgSplashScreen;
    ofImage imgArrow;
    ofImage imgArrowDown;
    ofImage imgSwipeInfo;
#endif

    int barY;
    int aboutY;
    int arrowDownY;
    int arrowDownYBase;
    int arrowDownDir;
    int splashAlpha;
    int crop;
    int pY; //for toucMoved() to scroll aboutImage on iPhone;
    uint inactivityThreshold;
    uint inactivityThresholdWithinParticles;
    bool showSwipeInfo;
    ofTrueTypeFont swipeFont;
    int accelCount;
    int lastTouchY; // NOTE: this is a hack since the oF API for Android doesn't provide (x,y) coordinates for the swipe
    bool swiping;

    #if defined TARGET_OF_IOS
    SwipeRecognition swiper;
    #endif

};

#endif
