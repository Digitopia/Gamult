#include "ofApp.h"

// Statics initialization
int ofApp::maxParticleY = 0;
int ofApp::mouseId = 0;
uint ofApp::inactivityCounter = 0;
bool ofApp::multitouch = true;
bool ofApp::inactive = false;
uint ofApp::moduleActive = 0;
uint ofApp::currentAlpha = DEFAULT_ALPHA;
vector<Module*> ofApp::modules;
string ofApp::language;
map<string,string> ofApp::translations;

#if defined TARGET_SEMIBREVE
ofxOscSender ofApp::oscSender;
ofxOscReceiver ofApp::oscReceiver;
#endif

typedef map<int,Touch>::iterator touchesIterator;

void ofApp::setup() {

    ofSetLogLevel(OF_LOG_NOTICE);

    ofLogNotice() << "setup()";

    if (ofApp::isSemibreve()) ofLogNotice() << "Going to run Semibreve version";

    if (ofApp::isOsx())     ofLogNotice() << "OSX detected";
    if (ofApp::isIos())     ofLogNotice() << "iOS detected";
    if (ofApp::isAndroid()) ofLogNotice() << "Android detected";

    if (ofApp::isPhone())   ofLogNotice() << "Phone detected";
    if (ofApp::isTablet())  ofLogNotice() << "Tablet detected";

    // if (ofApp::isIphone())  ofLogNotice() << "iPhone detected";
    // if (ofApp::isIpad())    ofLogNotice() << "iPad detected";

    // if (ofApp::isAndroidPhone())   ofLogNotice() << "Android phone detected";
    // if (ofApp::isAndroidTablet())  ofLogNotice() << "Android tablet detected";

    #if defined TARGET_OSX
    ofLogNotice() << "Running OSX version";
    ofSetDataPathRoot("../Resources/data/");
    #endif

    #if defined TARGET_SEMIBREVE
    ofLogNotice() << "Running SEMIBREVE version";
    oscReceiver.setup(RECEIVE_PORT);
    oscSender.setup(HOST, SEND_PORT);
    #endif

    #if defined TARGET_OF_IOS
     if (ofApp::isTablet()) {
        ofLogNotice() << "Running iPad version";
        ofSetOrientation(OF_ORIENTATION_90_LEFT);
         swiper.setup();
         ofAddListener(swiper.swipeRecognized, this, &ofApp::onSwipe);
         swiping = false;
    } else {
        ofLogNotice() << "Running iPhone version";
        swiper.setup();
        ofAddListener(swiper.swipeRecognized, this, &ofApp::onSwipe);
        swiping = false;
    }
    ofxAccelerometer.setup();
    accelCount = 0;
    crop = 0;
    #endif

    if (!ofApp::isIos()) {
        ofLogNotice() << "Registering for touch events if not ios";
        ofRegisterTouchEvents(this);
    }

    ofSetFrameRate(FRAME_RATE);
    ofSetCircleResolution(CIRCLE_RESOLUTION);

    if (multitouch) ofHideCursor();

    ofApp::language = ofApp::getSystemLanguage();
    // ofApp::language = "pt";
    ofLogNotice() << "Language is " << ofApp::language;

    initTranslations();
    initModules();
    setupModules();
    loadModuleSounds();

    initImages();

    appState = ABOUT;

    inactivityState = ACTIVE;

    // init global vars
    aboutY = 0;
    splashAlpha = 255;
    arrowDownY = ofGetHeight()/3*2;
    arrowDownYBase = arrowDownY;
    arrowDownDir = 1;
    showSwipeInfo = true;
    ofApp::maxParticleY = round(ofGetHeight() * (1-LIMIT_PARTICLE));

    uint swipeFontSize;
    if (isTablet()) swipeFontSize = 26;
    else swipeFontSize = 20;
    swipeFont.load(UI_FONT_FACE, swipeFontSize);

}

void ofApp::initTranslations() {

    map<string, string> pt;
    pt["BUTTON_FREEZE_TEXT"]  = "Congelar";
    pt["BUTTON_GRAVITY_TEXT"] = "Gravidade";
    pt["BUTTON_REMOVE_TEXT"]  = "Remover";
    pt["BUTTON_CLEAR_TEXT"]   = "Limpar";
    pt["FADER_SPEED_TEXT"]    = "Velocidade";
    pt["SWIPE_INFO_TOP_LINE"] = "Carregar e manter para criar partículas";
    pt["SWIPE_INFO_BOTTOM_LINE_PHONE"]  = "Deslize para mudar de instrumento";
    pt["SWIPE_INFO_BOTTOM_LINE_TABLET"] = "Rodar dispositivo para controlar um só instrumento de cada vez";

    map<string, string> en;
    en["BUTTON_FREEZE_TEXT"]  = "Freeze";
    en["BUTTON_GRAVITY_TEXT"] = "Gravity";
    en["BUTTON_REMOVE_TEXT"]  = "Remove";
    en["BUTTON_CLEAR_TEXT"]   = "Clear";
    en["FADER_SPEED_TEXT"]    = "Speed";
    en["SWIPE_INFO_TOP_LINE"] = "Click and hold to create particles";
    en["SWIPE_INFO_BOTTOM_LINE_PHONE"]  = "Swipe left and right to change instrument";
    en["SWIPE_INFO_BOTTOM_LINE_TABLET"] = "Rotate device to control one instrument at a time";

    ofApp::translations = ofApp::language == "pt" ? pt : en;
}

void ofApp::initModules() {

    ofLogNotice() << "initModules() start";

    uint nModules = isPhone() ? 1 : 4;
    ofLogNotice() << "Allocating " << nModules << " modules";
    modules.reserve(nModules);
    uint width = ofGetWidth()/nModules;
    uint height = ofGetHeight();
    uint habitants = PARTICLES_PER_MODULE;
    for (uint i = 0; i < nModules; i++) {
        int x = i*width;
        int y = 0;
        modules.push_back(new Module(i, x, y, width, height, habitants, getSoundPaths(i)));
    }

    ofLogNotice() << "initModules() end";

}

void ofApp::setupModules() {

    ofLogNotice() << "setupModules() start";

    if (isTabletInPortrait()) {
        modules[moduleActive]->setDimensions(0, 0, ofGetWidth(), ofGetHeight());
    }
    else {
        for (unsigned int i = 0; i < modules.size(); i++) {
            int width = ofGetWidth()/modules.size();
            modules[i]->setDimensions(i*width, 0, width, ofGetHeight());
        }
    }

    // Update down arrow on orientation change
    int barRectLength = 0.1*ofGetWidth();
    int barRectHeight = barRectLength/4;
    barRect.set(ofGetWidth()/2 - barRectLength/2, ofGetHeight() - barRectHeight, barRectLength, barRectHeight);

    ofLogNotice() << "setupModules() end";
}

void ofApp::loadModuleSounds() {

    for (unsigned int i = 0; i < modules.size(); i++) {
        modules[i]->loadSounds();
    }
}

void ofApp::initImages() {

    ofLogNotice() << "initImages() start";
    if (ofApp::isPhone()) {
        imgAbout.load("images/about_phone.png");
    } else {
    imgAbout.load("images/about.png");
    }
    imgArrow.load("images/arrow_up.png");
    imgArrowDown.load("images/arrow_down.png");

    if (isPhone()) imgSwipeInfo.load("images/swipe_info.png");
    else {
        imgSwipeInfo.load("images/touch_info.png");
    }

    // TODO: try to avoid resize as slows downs starting of app
    // NOTE: 3080/1080 is the original image ratio
    if (ofApp::isPhone()) imgAbout.resize(ofGetWidth(), (int)((float)ofGetWidth()*3080/1080));

    ofLogNotice() << "initImages() end";

}

void ofApp::appStateHandler() {

    if (appState == BAR_ASCENDING && aboutY <= maxParticleY) {
        appState = BAR;
    }

    if (appState == BAR_DESCENDING && aboutY >= ofGetHeight()) {
        appState = APP;
        crop = 0;
    }

    if (appState == ABOUT_ASCENDING && aboutY <= 0) {
        appState = ABOUT;
    }

    if (appState == ABOUT_DESCENDING && aboutY >= ofGetHeight()) {
        appState = APP;
        crop = 0;
    }

    if (appState == ABOUT || appState == SPLASH_SCREEN) {
        return;
    }

}

void ofApp::update() {

    #if defined TARGET_SEMIBREVE
    inactivityHandler();
    oscMultitouchHandler();
    #endif

    #if defined TARGET_OF_IOS
    shakeHandler();
    #endif

    appStateHandler();

    for (touchesIterator it = touches.begin(); it != touches.end(); it++) {
        it->second.update();
    }

    for (unsigned int i = 0; i < modules.size(); i++) {
        modules[i]->update();
    }
}

void ofApp::inactivityHandler() {

    ofLogNotice() << "inactivityState: " << inactivityState;

    // inactivity timer update
    if (inactivityState == ACTIVE || inactivityState == PRE_INACTIVE) {
        inactivityCounter += ofGetLastFrameTime() * 1000;
    }

    // ACTIVE -> PRE_INACTIVE
    if (inactivityState == ACTIVE && inactivityCounter >= INACTIVITY_THRESHOLD*1000) {
        inactivityState = PRE_INACTIVE;
        return;
    }

    // PRE_INACTIVE -> INACTIVE
    if (inactivityState == PRE_INACTIVE) {
        resetModules();
        inactivityState = INACTIVE;
        return;
    }

    // INACTIVE
    if (inactivityState == INACTIVE) {

        int particleChance = ofRandom(0, 1499);

        // NOTE: magic number!
        if (particleChance == 27) {
            unsigned int rModule = ofRandom(0, NMODULES);
            unsigned int rX = ofRandom(modules[rModule]->getX0(), modules[rModule]->getX1());
            unsigned int rY = ofRandom(CONSOLE_HEIGHT*ofGetHeight(), ofGetHeight()*(1-LIMIT_PARTICLE));
            unsigned int rIncrement = ofRandom(INACTIVITY_TOUCH_MIN, INACTIVITY_TOUCH_MAX);
            modules[getModuleIdx(rX)]->addParticle(rIncrement, rX, rY);
        }

    }

    // POST_INACTIVE -> ACTIVE
    if (inactivityState == POST_INACTIVE) {
        for (unsigned int i = 0; i < modules.size(); i++) {
            modules[i]->removeAllParticles();
        }
        inactivityState = ACTIVE;
    }

}

void ofApp::resetInactivityTime() {
    if (inactivityState == INACTIVE) inactivityState = POST_INACTIVE;
    inactivityCounter = 0;
}

void ofApp::resetModules() {
    ofLogNotice() << "Resetting modules";
    for (unsigned int i = 0; i < modules.size(); i++) {
        modules[i]->removeAllParticles();
        modules[i]->unfreeze();
        modules[i]->enableGravity();
        modules[i]->resetFaderSpeed();
    }
}

void ofApp::setLanguageBBoxes() {

    // NOTE: extremely hacky solution but it works and gets the job done, so keep calm and don't worry
    
    int y = ofGetHeight() * 0.078;

    if (crop < y) {

        y -= crop;
        int w = ofGetWidth() * 0.1;
        int h = ofGetHeight() * 0.05;

        ptLangRect.set(ofGetWidth()*0.715, y, w, h);
        enLangRect.set(ofGetWidth()*0.815, y, w, h);

        // NOTE: uncomment for debug
        // ofNoFill();
        // ofDrawRectangle(ptLangRect);
        // ofDrawRectangle(enLangRect);

    }
    else {
        // NOTE: this way .inside() never gets triggered
        ptLangRect.set(0, 0, 0, 0);
        enLangRect.set(0, 0, 0, 0);
    }
    
    // ofLogNotice() << "crop is " << crop;

}

void ofApp::draw() {

    ofBackground(BACKGROUND_COLOR);

    if (appState == SPLASH_SCREEN) {
        imgSplashScreen.draw(0, 0, ofGetWidth(), ofGetHeight());
        if (ofGetElapsedTimeMillis() > 1500) appState = SPLASH_FADE;
        return;
    }

    else if (appState == SPLASH_FADE) {
        ofPushStyle();
        ofEnableAlphaBlending();
        ofSetColor(255, 255, 255, 255);
        if(ofApp::isPhone()){
            imgAbout.drawSubsection(0.0f, 0.0f, (float)ofGetWidth(), (float)ofGetHeight(), 0.0f, (float)crop);
        }
        else {
            imgAbout.draw(0, 0, ofGetWidth(), ofGetHeight());
        }
        splashAlpha -= 2;
        ofSetColor(255, 255, 255, splashAlpha);
        imgSplashScreen.draw(0, 0, ofGetWidth(), ofGetHeight());
        ofDisableAlphaBlending();
        ofPopStyle();
        if (splashAlpha <= 0) appState = ABOUT;
        return;
    }

    else if (appState == ABOUT) {
        if(ofApp::isPhone()){
            imgAbout.drawSubsection(0.0f, 0.0f, (float)ofGetWidth(), (float)ofGetHeight(), 0.0f, (float)crop);
            setLanguageBBoxes();
        }
        else {
            imgAbout.draw(0, 0, ofGetWidth(), ofGetHeight());
        }
        drawBouncingArrow();
        return;
    }

    drawModules();
    drawLines();
    drawParticles();
    drawTouches();

    #ifdef OF_TARGET_IPHONE
    #endif


    if (showSwipeInfo) {
        ofPushStyle();

        if (isTablet()) ofSetHexColor(0xffffff);
        else ofSetHexColor(CONSOLE_COLOR);

        int swipeSize = 150;
        int padding = 80;

        imgSwipeInfo.draw(ofGetWidth()/2-swipeSize/2, ofGetHeight()/2, swipeSize, swipeSize);

        string s1 = ofApp::translations["SWIPE_INFO_TOP_LINE"];
        int s1w = swipeFont.getStringBoundingBox(s1, 0, 0).width;
        int s1x = (ofGetWidth() - s1w) / 2;
        int s1y = ofGetHeight()/2 - padding;
        swipeFont.drawString(s1, s1x, s1y);

        string s2;
        if (ofApp::isPhone()) s2 = ofApp::translations["SWIPE_INFO_BOTTOM_LINE_PHONE"];
        else s2 = ofApp::translations["SWIPE_INFO_BOTTOM_LINE_TABLET"];

        int s2w = swipeFont.getStringBoundingBox(s2, 0, 0).width;
        int s2x = (ofGetWidth() - s2w)/2;
        int s2y = ofGetHeight()/2 + swipeSize + padding;
        swipeFont.drawString(s2, s2x, s2y);

        ofPopStyle();
    }

    if (appState == BAR) {
        ofPushStyle();
        ofEnableAlphaBlending();
        ofSetColor(255, 255, 255, DEFAULT_ALPHA);
        if(ofApp::isPhone()){
            imgAbout.drawSubsection(0.0f, aboutY, (float)ofGetWidth(), (float)ofGetHeight(), 0.0f, (float)crop);
        }
        else {
            imgAbout.draw(0, aboutY, ofGetWidth(), ofGetHeight());
        }

        ofDisableAlphaBlending();
        drawArrow(false);
        ofPopStyle();
    }

    else if (appState == ABOUT_ASCENDING) {
        if (aboutY < 15) {
            aboutY = 0;
            appState = ABOUT;
        } else {
            aboutY -= 15;
        }
        if (currentAlpha < 255) currentAlpha += 3;
        ofPushStyle();
        ofEnableAlphaBlending();
        ofSetColor(255, 255, 255, currentAlpha);
        if(ofApp::isPhone()){
            imgAbout.drawSubsection(0.0f, aboutY, (float)ofGetWidth(), (float)ofGetHeight(), 0.0f, (float)crop);
        }
        else {
            imgAbout.draw(0, aboutY, ofGetWidth(), ofGetHeight());
        }
        ofDisableAlphaBlending();
        ofPopStyle();
    }

    else if (appState == ABOUT_DESCENDING) {
        aboutY += 20;
        if(ofApp::isPhone()){
            imgAbout.drawSubsection(0.0f, aboutY, (float)ofGetWidth(), (float)ofGetHeight(), 0.0f, (float)crop);
        }
        else {
            imgAbout.draw(0, aboutY, ofGetWidth(), ofGetHeight());
        }
    }

    else if (appState == BAR_ASCENDING) {
        ofPushStyle();
        ofEnableAlphaBlending();
        ofSetColor(255, 255, 255, DEFAULT_ALPHA);
        aboutY -= 5;
        if(ofApp::isPhone()){
            imgAbout.drawSubsection(0.0f, aboutY, (float)ofGetWidth(), (float)ofGetHeight(), 0.0f, (float)crop);
        }
        else {
            imgAbout.draw(0, aboutY, ofGetWidth(), ofGetHeight());
        }
        drawArrow(false);
        ofDisableAlphaBlending();
        ofPopStyle();
    }

    else if (appState == BAR_DESCENDING) {
        ofPushStyle();
        ofEnableAlphaBlending();
        ofSetColor(255, 255, 255, DEFAULT_ALPHA);
        aboutY += 5;
        if(ofApp::isPhone()){
            imgAbout.drawSubsection(0.0f, aboutY, (float)ofGetWidth(), (float)ofGetHeight(), 0.0f, (float)crop);
        }
        else {
            imgAbout.draw(0, aboutY, ofGetWidth(), ofGetHeight());
        }
        drawArrow(true);

        ofDisableAlphaBlending();
        ofPopStyle();
    }

    else if (appState == APP) {
        currentAlpha = DEFAULT_ALPHA;
        drawArrow(true);
        ofPushStyle();
        ofSetColor(IMAGE_COLOR, DEFAULT_ALPHA);
        ofEnableAlphaBlending();
    #ifndef TARGET_OF_IOS
        ofDrawRectangle(barRect);
    #endif
        ofDisableAlphaBlending();
        ofPopStyle();
    }

}

void ofApp::drawParticles() {
    for (unsigned int i = 0; i < modules.size(); i++)
      modules[i]->drawParticles();
}

void ofApp::drawTouches() {
    for (touchesIterator it = touches.begin(); it != touches.end(); it++) {
        it->second.draw();
    }
}

void ofApp::drawModules() {
    for (unsigned int i = 0; i < modules.size(); i++)
        modules[i]->draw();
}

void ofApp::drawBouncingArrow() {
    if (arrowDownY > arrowDownYBase + 5) arrowDownDir = -1;
    else if (arrowDownY < arrowDownYBase - 15) arrowDownDir = 1;
    arrowDownY += arrowDownDir;
    imgArrowDown.draw(ofGetWidth()/2-imgArrowDown.getWidth()/2, arrowDownY);
}

void ofApp::drawArrow(bool up) {
#ifndef TARGET_OF_IOS
    ofPushStyle();
    ofEnableAlphaBlending();

    if (up) {
        if (ofGetOrientation() == OF_ORIENTATION_DEFAULT || ofGetOrientation() == OF_ORIENTATION_180) {
            imgArrow.draw(barRect.x + ARROW_OFFSET, barRect.y + ARROW_OFFSET, barRect.width - 2*ARROW_OFFSET, barRect.height - 2*ARROW_OFFSET);
        } else {
            imgArrow.draw(barRect.x + ARROW_OFFSET, barRect.y + ARROW_OFFSET, barRect.width - 2*ARROW_OFFSET, barRect.height - 2*ARROW_OFFSET);
        }
    }

    else {
        imgArrow.draw(barRect.x + barRect.width - ARROW_OFFSET, barRect.y + barRect.height - ARROW_OFFSET, -barRect.width + 2*ARROW_OFFSET, -barRect.height +2*ARROW_OFFSET);
    }

    ofDisableAlphaBlending();

    ofPopStyle();
#endif
}

vector<string> ofApp::getSoundPaths(unsigned int index) {

    vector<string> ret;

    if (!isIos()) {

      // bonangs
      if (index == 0) {
          ret.push_back("sounds/BBPL1.wav");
          ret.push_back("sounds/BBPL2.wav");
          ret.push_back("sounds/BBPL3.wav");
      }

      // genders
      else if (index == 1) {
          ret.push_back("sounds/GBPL1.wav");
          ret.push_back("sounds/GBPL2.wav");
          ret.push_back("sounds/GBPL3.wav");
          ret.push_back("sounds/GBPL5.wav");
          ret.push_back("sounds/GBPL2.wav");
          ret.push_back("sounds/GBPL3.wav");
      }

      // gongs
      else if (index == 2) {
          ret.push_back("sounds/GKPL1f.wav");
          ret.push_back("sounds/GKPL2f.wav");
          ret.push_back("sounds/GKPL3f.wav");
          ret.push_back("sounds/GKPL5f.wav");
          ret.push_back("sounds/GKPL1f.wav");
          ret.push_back("sounds/GKPL2f.wav");
          ret.push_back("sounds/GKPL3f.wav");
      }

      // sarons
      else if (index == 3) {
          ret.push_back("sounds/SBPL1.wav");
          ret.push_back("sounds/SBPL2.wav");
          ret.push_back("sounds/SBPL3.wav");
          ret.push_back("sounds/SBPL4.wav");
          ret.push_back("sounds/SBPL3.wav");
          ret.push_back("sounds/SBPL4.wav");
          ret.push_back("sounds/SBPL4.wav");
      }
    }

    else {


      // bonangs
      if (index == 0) {
          ret.push_back("sounds/01_Kenong/A_KSL2.wav");
          ret.push_back("sounds/01_Kenong/A_KSL3.wav");
          ret.push_back("sounds/01_Kenong/A_KSL5.wav");
      }

      // genders
      else if (index == 1) {
          ret.push_back("sounds/02_Gender/A_01_GBSL1.wav");
          ret.push_back("sounds/02_Gender/A_02_GBSL2.wav");
          ret.push_back("sounds/02_Gender/A_03_GBSL3.wav");
          ret.push_back("sounds/02_Gender/A_04_GBSL5.wav");
          ret.push_back("sounds/02_Gender/A_05_GBSL6.wav");
          ret.push_back("sounds/02_Gender/A_06_GBSL1h.wav");
      }

      // gongs
      else if (index == 2) {
          ret.push_back("sounds/03_Bonang/A_01_BBSL1.wav");
          ret.push_back("sounds/03_Bonang/A_02_BBSL2.wav");
          ret.push_back("sounds/03_Bonang/A_03_BBSL3.wav");
          ret.push_back("sounds/03_Bonang/A_04_BBSL5.wav");
          ret.push_back("sounds/03_Bonang/A_05_BBSL6.wav");
          ret.push_back("sounds/03_Bonang/A_06_BBSL1h.wav");
          ret.push_back("sounds/03_Bonang/A_07_BBSL2h.wav");
      }

      // sarons
      else if (index == 3) {
          ret.push_back("sounds/04_Saron/A_01_SBSL6l.wav");
          ret.push_back("sounds/04_Saron/A_02_SBSL1.wav");
          ret.push_back("sounds/04_Saron/A_03_SBSL2.wav");
          ret.push_back("sounds/04_Saron/A_04_SBSL3.wav");
          ret.push_back("sounds/04_Saron/A_05_SBSL5.wav");
          ret.push_back("sounds/04_Saron/A_06_SBSL6.wav");
          ret.push_back("sounds/04_Saron/A_07_SBSL1h.wav");
      }
    }

    return ret;

}

#if defined TARGET_SEMIBREVE
void ofApp::oscMultitouchHandler() {

    while (oscReceiver.hasWaitingMessages()) {

        ofxOscMessage m;
        oscReceiver.getNextMessage(&m);
        ofLogNotice() << m.getAddress() ;

        if (!multitouch)
            return;

        ofTouchEventArgs touchEvent;

        int id = int(m.getArgAsFloat(0));
        touchEvent.id = id;

        // TODO: change numbers to 'add', 'update' and 'remove' preprends

        if (m.getAddress() == "/1/" || m.getAddress() == "/2/") {

            float x, y;
            x = m.getArgAsFloat(2) * ofGetWidth();
            y = m.getArgAsFloat(3) * ofGetHeight();

            ofLogNotice() << "x is " << x << " and y is " << y ;

            touchEvent.x = x;
            touchEvent.y = y;

            if (m.getAddress() == "/1/") {
                touchEvent.type = ofTouchEventArgs::down;
                ofNotifyEvent(ofEvents().touchDown, touchEvent, this);
            } else {
                touchEvent.type = ofTouchEventArgs::move;
                ofNotifyEvent(ofEvents().touchMoved, touchEvent, this);
            }
        }

        else if (m.getAddress() == "/3/") {
            touchEvent.type = ofTouchEventArgs::up;
            ofNotifyEvent(ofEvents().touchUp, touchEvent, this);
        }
    }

}
#endif

void ofApp::mousePressed(ofMouseEventArgs& mouse) {
    if (multitouch) return;
    ofLogNotice() << "mousePressed" ;
    ofTouchEventArgs touchEvent;
    touchEvent.id = ++mouseId;
    touchEvent.x = mouse.x;
    touchEvent.y = mouse.y;
    touchEvent.type = ofTouchEventArgs::down;
    ofNotifyEvent(ofEvents().touchDown, touchEvent, this);
}

void ofApp::mouseDragged(ofMouseEventArgs& mouse) {
    if (multitouch) return;
    ofLogNotice() << "mouseDragged" ;
    ofTouchEventArgs touchEvent;
    touchEvent.id = mouseId;
    touchEvent.x = mouse.x;
    touchEvent.y = mouse.y;
    touchEvent.type = ofTouchEventArgs::move;
    ofNotifyEvent(ofEvents().touchMoved, touchEvent, this);
}

void ofApp::mouseReleased(ofMouseEventArgs& mouse) {
    if (multitouch) return;
    ofLogNotice() << "mouseReleased" ;
    ofTouchEventArgs touchEvent;
    touchEvent.id = ofApp::mouseId;
    touchEvent.type = ofTouchEventArgs::up;
    ofNotifyEvent(ofEvents().touchUp, touchEvent, this);
}

void ofApp::mouseMoved(ofMouseEventArgs& mouse) {
    resetInactivityTime();
}

void ofApp::keyPressed(int key) {
    if (key == 'm' || key == 'M' ) {
        multitouch = !multitouch;
        if (multitouch) {
            ofLogNotice() << "multitouch on" ;
            ofHideCursor();
        } else {
            ofLogNotice() << "multitouch off" ;
            ofShowCursor();
        }
    }
}

void ofApp::updateNewModuleActive(int x) {

    if (isTabletInLandscape()) {
        uint moduleWidth = ofGetWidth() / float(modules.size());
        uint newModuleActive = floor(x/moduleWidth);
        if (newModuleActive != moduleActive) {
            ofLogNotice() << "new module active is now " << newModuleActive;
            modules[moduleActive]->unmakeMostRecent();
            moduleActive = newModuleActive;
            modules[moduleActive]->makeMostRecent();
        }
    }

}

void ofApp::touchDown(ofTouchEventArgs& touch) {

    updateNewModuleActive(touch.x);

    resetInactivityTime();

    if (appState == SPLASH_SCREEN || appState == SPLASH_FADE) {
        appState = ABOUT;
        return;
    }

    int x = touch.x;
    int y = touch.y;
    int id = touch.id;

    if (appState == ABOUT) {
        ofRectangle arrowDownRect(ofGetWidth()/2 - imgArrowDown.getWidth()/2, arrowDownY, imgArrowDown.getWidth(), imgArrowDown.getHeight());
        // register starting Y to scroll from
        pY = touch.y;
        if (arrowDownRect.inside(x, y)) {
            appState = ABOUT_DESCENDING;
        }

        if (ptLangRect.inside(x,y)) changeLanguage("pt");
        else if (enLangRect.inside(x,y)) changeLanguage("en");
        
        return;
    }

    if (appState == APP && barRect.inside(x, y)) {
        appState = BAR_ASCENDING;
        return;
    }

    if (appState == BAR && barRect.inside(x, y)) {
        appState = BAR_DESCENDING;
        return;
    }

    if (appState == BAR && y >= ofGetHeight()*(1-LIMIT_PARTICLE)) {
        appState = ABOUT_ASCENDING;
        return;
    }

    if (appState == APP || appState == BAR) {

        // NOTE: dismiss swipe info after first touch
        if (showSwipeInfo) showSwipeInfo = false;
        
        // ofLogNotice() << "down (" << id << ", " << x << ", " << y << ")" ;

        if (y > CONSOLE_HEIGHT*ofGetHeight() && (appState != BAR || y < aboutY) && modules[getModuleIdx(x)]->isNotFull()) {
            touches.insert(pair<int,Touch> (id, Touch(x, y)));
        }

    }

    #if defined TARGET_OF_IOS
    swiping = false;
    #endif

}

void ofApp::changeLanguage(string language) {
    ofLogNotice() << "Changing language to " << language;
    if (language == ofApp::language) return;
    ofApp::language = language;
    initTranslations();
    initImages();
    setupModules();
}

void ofApp::touchMoved(ofTouchEventArgs& touch) {

    updateNewModuleActive(touch.x);

    resetInactivityTime();

    if(appState == ABOUT) {
        crop -= touch.y - pY;

        if(crop > (imgAbout.getHeight() - ofGetHeight())) {
            crop = (imgAbout.getHeight() - ofGetHeight());
        } else if(crop < 0) crop = 0;

        pY = touch.y;

    }

    if (appState != APP && appState != BAR) return;

    int x = touch.x;
    int y = touch.y;
    int id = touch.id;

    // ofLogNotice() << "touchMoved (" << id << ", " << x << ", " << y << ")" ;

    touchesIterator it = touches.find(id);
    if (it == touches.end()) return;

    it->second.setXY(x, y);

}

void ofApp::touchUp(ofTouchEventArgs& touch) {

    resetInactivityTime();

    if (appState != APP && appState != BAR && appState != ABOUT_ASCENDING) return; //adding ABOUT_ASCENDING because of swipe on iOS

    int id = touch.id;

    // ofLogNotice() << "touchUp (" << id << ")" ;

    touchesIterator it = touches.find(id);

    if (it == touches.end()) return;

    int x = it->second.getX();
    int y = it->second.getY();
    float increment = it->second.getIncrement();

    touches.erase(it);

    #if defined TARGET_OF_IOS
    if (y > CONSOLE_HEIGHT*ofGetHeight() && y < ofApp::maxParticleY && !swiping) {
        modules[getModuleIdx(x)]->addParticle(increment, x, y);
        ofLogNotice() << "Particle added" ;
        swiping = false;
    }
    #else
    if (y > CONSOLE_HEIGHT*ofGetHeight() && y < ofApp::maxParticleY) {
        modules[getModuleIdx(x)]->addParticle(increment, x, y);
        ofLogNotice() << "Particle added" ;
    }
    #endif

}

size_t ofApp::getModuleIdx(unsigned int x) {

    if (isPhone() || isTabletInPortrait()) {
        return moduleActive;
    }

    for (unsigned int i = 0; i < modules.size(); i++) {
        if (x >= modules[i]->getX0() && x < modules[i]->getX1())
            return i;
    }

    return -1;

}

void ofApp::drawLine(uint nth) {

    vector<Particle*> nthParticles;

    if (isTabletInPortrait()) {
        if (modules[moduleActive]->getNumberOfParticles() > nth)
            nthParticles.push_back(modules[moduleActive]->getParticle(nth));
    } else {
        for (unsigned int i = 0; i < modules.size(); i++) {
            if (modules[i]->getNumberOfParticles() > nth) {
                nthParticles.push_back(modules[i]->getParticle(nth));
            }
        }
    }

    if (nthParticles.size() == 0)
        return;

    ofPushStyle();

    ofNoFill();
    ofSetLineWidth(POLY_WIDTH);
    ofSetColor(POLY_COLOR);

    ofBeginShape();

    // Need four points at least otherwise the shape doesn't 'close' and therefore doesn't draw

    int middleY = round((ofGetHeight() - CONSOLE_HEIGHT*ofGetHeight())/2.0 + CONSOLE_HEIGHT*ofGetHeight());

    ofCurveVertex(0, middleY);
    ofCurveVertex(0, middleY);

    for (unsigned int i = 0; i < nthParticles.size(); i++) {
        ofCurveVertex(nthParticles[i]->getX(), nthParticles[i]->getY());
    }

    ofCurveVertex(ofGetWidth(), middleY);
    ofCurveVertex(ofGetWidth(), middleY);

    ofEndShape();

    ofPopStyle();
}

void ofApp::drawLines() {
    for (unsigned int i = 0; i < PARTICLES_PER_MODULE; i++) {
        drawLine(i);
    }
}

bool ofApp::hasParticles() {
    for (unsigned int i = 0; i < modules.size(); i++) {
        if (modules[i]->anyParticles()) {
            return true;
        }
    }
    return false;
}

void ofApp::deviceOrientationChanged(int newOrientation) {

    ofLogNotice() << "Detected orientation change " << newOrientation;

    if (isPhone()) {
      ofLogNotice() << "Ignoring orientation change since it's a phone";
      return;
    }

    if (newOrientation == OF_ORIENTATION_UNKNOWN) {
        return;
    }

    if (isTablet() && appState != APP) {
        if (newOrientation == OF_ORIENTATION_DEFAULT || newOrientation == OF_ORIENTATION_180) {
            ofLogNotice() << "!!!Ignoring orientation change in tablet in app ";
            return;
        }
    }

    // upside down is no good for anything
    if (isPhone() && newOrientation == OF_ORIENTATION_180)
        ofSetOrientation(OF_ORIENTATION_DEFAULT);
    else {
        ofLogNotice() << "Changing orientation change";
        ofSetOrientation(ofOrientation(newOrientation));
    }

    // it transitioning to landscape, activate back all modules
    if (newOrientation == OF_ORIENTATION_90_LEFT || newOrientation == OF_ORIENTATION_90_RIGHT) {
        //imgAbout.rotate90(3);
        for (unsigned int i = 0; i < modules.size(); i++) modules[i]->activate();
    }
    // it transitioning to portrait, deactivate all modules, except the one with which last interacted
    else {
        //imgAbout.rotate90(1);
        for (unsigned int i = 0; i < modules.size(); i++) {
            if (i == moduleActive) {

                ofLogNotice() << "Activating module " << i;
                modules[i]->activate();
            }
            else {
                ofLogNotice() << "Deactivating module " << i;
                modules[i]->deactivate();
            }
        }
    }

    setupModules();
}

#if defined TARGET_OF_IOS
void ofApp::onSwipe(SwipeRecognitionArgs& args) {

    ofLogNotice() << "Detected swipe event";

    // multiplying swipeOriginY by 2 because of retina display
    if (args.swipeOriginY * 2 > CONSOLE_HEIGHT * ofGetHeight()) {
        int direction = args.direction;
        if (direction == 4)
        {
            if (appState == APP) {
                appState = ABOUT_ASCENDING;
                swiping = true;
                ofLogNotice() << "setting swiping true";
            }
        }
        else {
            modules[0]->prepareInstrumentChange(direction);
            swiping = true;
            ofLogNotice() << "prepare instrument change";
        }
    } else {
        ofLogNotice() << "Ignoring swipe event";
    }

}
#endif

#if defined TARGET_OF_IOS
void ofApp::shakeHandler() {

    float offset = 1.7;
    float accelX = ofxAccelerometer.getForce().x;
    float accelY = ofxAccelerometer.getForce().y;
    if ((accelX < - offset || accelX > offset) || (accelY < - offset || accelY > offset)) {

        accelCount++;
    } else {
        accelCount = 0;
    }

    if (accelCount >= 4) {
        ofLogNotice() << "Shake detected";
        resetModules();
    }

}
#endif

bool ofApp::isOsx() {
  #if defined TARGET_OSX
    return true;
  #else
    return false;
  #endif
}

bool ofApp::isSemibreve() {
  #if defined TARGET_SEMIBREVE
    return true;
  #else
    return false;
  #endif
}

bool ofApp::isIos() {
  #if defined TARGET_OF_IOS
    return true;
  #else
    return false;
  #endif
}

bool ofApp::isAndroid() {
  #if defined TARGET_ANDROID
    return true;
  #else
    return false;
  #endif
}

bool ofApp::isPhone() {
  #if defined TARGET_OF_IOS
    if (ofxiOSGetDeviceType() == OFXIOS_DEVICE_IPHONE) return true;
    else return false;
  #elif defined TARGET_ANDROID
    return true;
  #else
    return false;
  #endif
}

bool ofApp::isTablet() {
  #if defined TARGET_OF_IOS
    if (ofxiOSGetDeviceType() == OFXIOS_DEVICE_IPAD) return true;
    return false;
  #else
    return false;
  #endif
}

bool ofApp::isTabletInPortrait() {

  // check iPad in portrait
  #if defined TARGET_OF_IOS
    if (ofxiOSGetDeviceType() == OFXIOS_DEVICE_IPAD) {
        if (ofGetOrientation() == OF_ORIENTATION_DEFAULT || ofGetOrientation() == OF_ORIENTATION_180) {
            return true;
        }
    }
  #endif

  // check Android in portrait
  // TODO

  // Otherwise, is false
  return false;

}

bool ofApp::isTabletInLandscape() {

  // iPad in portrait
  #if defined TARGET_OF_IOS
  if (ofxiOSGetDeviceType() == OFXIOS_DEVICE_IPAD) {
      if (ofGetOrientation() == OF_ORIENTATION_90_LEFT || ofGetOrientation() == OF_ORIENTATION_90_RIGHT) {
          return true;
      }
  }
  #endif

  // Android in portrait
  // TODO: check for android tablet in portrait too

  // Otherwise, is false
  return false;

}

int ofApp::getFontSize() {
    uint width = ofGetWidth();
    if (ofApp::isPhone()) {
        if     (width <= 640)  return 24; // iPhone 5, 5s and SE
        else if(width <= 750)  return 28; // iPhone 6, 6s and 7
        else if(width <= 1242) return 34; // iPhone 6+, 6s+ and 7+
        else return 38;
    }
    else {
        if     (width <= 1024) return 12; // iPad 2
        else if(width <= 1536) return 22; // iPad Mini and iPad Air
        else return 24;
    }
}

string ofApp::getSystemLanguage() {

    // NOTE: Apparently one can mix Objective-C in C++ with no problem

    NSString *lang = [[NSLocale preferredLanguages] objectAtIndex:0];

    // Convert NSString to C++ std:string
    string ret = string([lang UTF8String]);

    return ret;
}
