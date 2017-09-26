#include "ofApp.h"

// Statics initialization

int ofApp::maxParticleY = 0;
int ofApp::mouseId = 0;
uint ofApp::inactivityCounter = 0;
bool ofApp::multitouch = true;
bool ofApp::inactive = false;
bool ofApp::runSetupAlready = false;
bool ofApp::drawedSplashScreen = false;
uint ofApp::moduleActive = 0;
uint ofApp::currentAlpha = DEFAULT_ALPHA;
int ofApp::androidOrientation = 1;
string ofApp::language = "en";
vector<Module*> ofApp::modules;
vector<ofSoundPlayer> ofApp::sounds;
map<string,string> ofApp::translations;

#if defined TARGET_SEMIBREVE
ofxOscSender ofApp::oscSender;
ofxOscReceiver ofApp::oscReceiver;
#endif

typedef map<int,Touch>::iterator touchesIterator;

void ofApp::setup() {

    initSplashScreen();

}

void ofApp::setupForApp() {

    ofSetLogLevel(OF_LOG_NOTICE);

    ofLogNotice() << "setupForApp()";

    if (ofApp::isOsx())     ofLogNotice() << "Plataform: OSX";
    if (ofApp::isIos())     ofLogNotice() << "Plataform: iOS";
    if (ofApp::isAndroid()) ofLogNotice() << "Plataform: Android";

    if (ofApp::isPhone())   ofLogNotice() << "Device: Phone";
    if (ofApp::isTablet())  ofLogNotice() << "Device: Tablet";
    if (!ofApp::isPhone() && !isTablet()) ofLogNotice() << "Device: Computer";

    #if defined TARGET_OSX
    ofSetDataPathRoot("../Resources/data/");
    #endif

    #if defined TARGET_SEMIBREVE
    ofLogNotice() << "Running **SEMIBREVE** version";
    oscReceiver.setup(RECEIVE_PORT);
    oscSender.setup(HOST, SEND_PORT);
    #endif

    #if defined TARGET_OF_IOS
     if (ofApp::isTablet()) {
        ofSetOrientation(OF_ORIENTATION_90_LEFT);
        swiper.setup();
        ofAddListener(swiper.swipeRecognized, this, &ofApp::onSwipe);
        swiping = false;
    } else {
        swiper.setup();
        ofAddListener(swiper.swipeRecognized, this, &ofApp::onSwipe);
        swiping = false;
    }
    #endif

    if (isAndroid() && isTablet()) {
        // ofSetOrientation(ofOrientation(OF_ORIENTATION_90_LEFT));
        // windowResized(20, 10);
    }

    if (isAndroid() || isIos()) {
        ofxAccelerometer.setup();
        accelCount = 0;
        crop = 0;
    }

    if (!ofApp::isIos()) {
        ofLogNotice() << "Registering for touch events if not ios";
        ofRegisterTouchEvents(this);
    }

    ofSetFrameRate(FRAME_RATE);
    ofSetCircleResolution(20);

    if (multitouch) ofHideCursor();

    ofApp::language = ofApp::getSystemLanguage();
    ofLogNotice() << "Language: " << ofApp::language;

    initTranslations();
    initModules();
    setupModules();
    setupModules();

    initImages(true);

    // appState = ABOUT;

    inactivityState = ACTIVE;

    // init global vars
    aboutY = 0;
    splashAlpha = 255;
    arrowDownY = ofGetHeight()/3*2;
    arrowDownYBase = arrowDownY;
    arrowDownDir = 1;
    showSwipeInfo = true;
    ofApp::maxParticleY = round(ofGetHeight() * (1-LIMIT_PARTICLE));

    swipeFont.load(UI_FONT_FACE, ofApp::getFontSize() * 0.8);

    // ofAddListener(ofEvents().windowResized, this, &ofApp::windowResized);

    runSetupAlready = true;

}

void ofApp::initSplashScreen() {

    if (isAndroid()) {
        appState = SPLASH_SCREEN;
        string splashScreenUrl = isPhone() ? "images/logo_portrait.png" : "images/logo_landscape.png";
        imgSplashScreen.load(splashScreenUrl);
    }

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

    if (isIos()) en["SWIPE_INFO_BOTTOM_LINE_TABLET"] = "Rotate device to control one instrument at a time";
    else en["SWIPE_INFO_BOTTOM_LINE_TABLET"] = "Swipe up at any time for the more detailed info";

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
    setupModules(ofGetOrientation());
}

void ofApp::setupModules(int newOrientation) {

    ofLogNotice() << "setupModules() start";

    if (isTablet() && (newOrientation == OF_ORIENTATION_DEFAULT || newOrientation == OF_ORIENTATION_180)) {
        ofLogNotice() << "Setting dimensions for module " << moduleActive << " width " << ofGetWidth() << " height " << ofGetHeight();
        modules[moduleActive]->setDimensions(-1, 0, ofGetWidth(), ofGetHeight());
    }
    else {
        ofLogNotice() << " here !!!";
        for (uint i = 0; i < modules.size(); i++) {
            ofLogNotice() << "Altering module " << i;
            int width = ofGetWidth()/modules.size();
            modules[i]->setDimensions(i*width, 0, width, ofGetHeight());
        }
    }

    // Update down arrow on orientation change
    int barRectLength = 0.1*ofGetWidth();
    int barRectHeight = barRectLength/4;
    barRect.set(ofGetWidth()/2 - barRectLength/2, ofGetHeight() - barRectHeight, barRectLength, barRectHeight);

    for (uint i = 0; i < modules.size(); i++) {
        modules[i]->loadSounds();
    }

    ofLogNotice() << "setupModules() end";

    drawModules();

}

void ofApp::initImages(bool first) {

    ofLogNotice() << "initImages() start";

    if (isPhone()) imgAbout.load("images/about_phone.png");
    else imgAbout.load("images/about.png");

    if (isPhone()) imgSwipeInfo.load("images/swipe_info.png");
    else imgSwipeInfo.load("images/touch_info.png");

    // NOTE: some loads (and resizes are only needed on first load, and not when changing language)
    if (first) {
        imgArrowDown.load("images/arrow_down.png");
        float imgArrowDownScaleFactor = isInPortrait() ? 0.2 : 0.1;
        imgArrowDown.resize(ofGetWidth() * imgArrowDownScaleFactor, ofGetWidth() * imgArrowDownScaleFactor);
        imgSwipeInfo.resize(ofGetWidth() * imgArrowDownScaleFactor, ofGetWidth() * imgArrowDownScaleFactor);
        imgArrow.load("images/arrow_up.png");
    }

    // TODO: try to avoid resize as slows downs starting of app
    // NOTE: 3080/1080 is the original image ratio
    if (ofApp::isPhone() && first) imgAbout.resize(ofGetWidth(), (int)((float)ofGetWidth()*3080/1080));

    if (ofApp::isAndroid())

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

    if (isIos() || isAndroid())
        shakeHandler();

    appStateHandler();

    for (touchesIterator it = touches.begin(); it != touches.end(); it++) {
        it->second.update();
    }

    for (unsigned int i = 0; i < modules.size(); i++) {
        modules[i]->update();
    }

    if (drawedSplashScreen && !runSetupAlready) {
        setupForApp();
        windowResized(ofGetWidth(), ofGetHeight());
    }
}

void ofApp::inactivityHandler() {

    ofLogNotice() << "inactivityState: " << inactivityState;

    // inactivity timer updatesposp
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

    int y;

    if(ofApp::isPhone()) {
        y = ofGetHeight() * 0.078;
    } else if (ofApp::isTablet()) {
        y = ofGetHeight() * 0.35;
    }

    if (crop < y) {

        y -= crop;

        float w, h, xPt, xEn;

        if (ofApp::isPhone()) {
            w = 0.1;
            h = 0.05;
            xPt = 0.715;
            xEn = 0.815;
        } else if (ofApp::isTablet()) {
            w = 0.05;
            h = 0.05;
            xPt = 0.03;
            xEn = 0.08;
        }

        // Vars were just percentages, so update them to actual pixel sizes
        w = ofGetWidth() * w;
        h = ofGetHeight() * h;
        xPt = ofGetWidth() * xPt;
        xEn = ofGetWidth() * xEn;

        ptLangRect.set(xPt, y, w, h);
        enLangRect.set(xEn, y, w, h);

        // NOTE: uncomment for debug
        // ofPushStyle();
        // ofNoFill();
        // ofDrawRectangle(ptLangRect);
        // ofDrawRectangle(enLangRect);
        // ofPopStyle();

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
        if (ofGetElapsedTimeMillis() > 2000) appState = SPLASH_FADE;
        drawedSplashScreen = true;
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
        }
        else {
            imgAbout.draw(0, 0, ofGetWidth(), ofGetHeight());
        }
        setLanguageBBoxes();
        drawBouncingArrow();
        return;
    }

    drawModules();
    drawLines();
    drawParticles();
    drawTouches();

    if (showSwipeInfo)  {
        ofPushStyle();

        if (isTablet()) ofSetColor(47); // NOTE: previous white font color wasn't readable on iPad;
        else ofSetHexColor(CONSOLE_COLOR);

        float swipeSizeFactor = isInPortrait() ? 0.2 : 0.1;
        int swipeSize = ofGetWidth() * swipeSizeFactor;
        int padding = (ofGetWidth()*80)/1080;

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
            aboutY -= 25;
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

        if (isOsx()) {
            currentAlpha = DEFAULT_ALPHA;
            drawArrow(true);
            ofPushStyle();
            ofSetColor(IMAGE_COLOR, DEFAULT_ALPHA);
            ofEnableAlphaBlending();
            ofDrawRectangle(barRect);
            ofDisableAlphaBlending();
            ofPopStyle();
        }
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

vector<string> ofApp::getSoundPaths(uint index) {

    vector<string> ret;

    switch (index) {

        case 0:
            ret.push_back("sounds/01_Kenong/A_KSL2.wav");
            ret.push_back("sounds/01_Kenong/A_KSL3.wav");
            ret.push_back("sounds/01_Kenong/A_KSL5.wav");
            break;

        case 1:
            ret.push_back("sounds/02_Gender/A_01_GBSL1.wav");
            ret.push_back("sounds/02_Gender/A_02_GBSL2.wav");
            ret.push_back("sounds/02_Gender/A_03_GBSL3.wav");
            ret.push_back("sounds/02_Gender/A_04_GBSL5.wav");
            ret.push_back("sounds/02_Gender/A_05_GBSL6.wav");
            ret.push_back("sounds/02_Gender/A_06_GBSL1h.wav");
            break;

        case 2:
            ret.push_back("sounds/03_Bonang/A_01_BBSL1.wav");
            ret.push_back("sounds/03_Bonang/A_02_BBSL2.wav");
            ret.push_back("sounds/03_Bonang/A_03_BBSL3.wav");
            ret.push_back("sounds/03_Bonang/A_04_BBSL5.wav");
            ret.push_back("sounds/03_Bonang/A_05_BBSL6.wav");
            ret.push_back("sounds/03_Bonang/A_06_BBSL1h.wav");
            ret.push_back("sounds/03_Bonang/A_07_BBSL2h.wav");
            break;

        case 3:
            ret.push_back("sounds/04_Saron/A_01_SBSL6l.ogg");
            ret.push_back("sounds/04_Saron/A_02_SBSL1.ogg");
            ret.push_back("sounds/04_Saron/A_03_SBSL2.ogg");
            ret.push_back("sounds/04_Saron/A_04_SBSL3.ogg");
            ret.push_back("sounds/04_Saron/A_05_SBSL5.ogg");
            ret.push_back("sounds/04_Saron/A_06_SBSL6.ogg");
            ret.push_back("sounds/04_Saron/A_07_SBSL1h.ogg");
            break;

        default:
            break;

    }

    return ret;

}

// NOTE: terrible hack: get the base offset in the linear vector of sounds for a specific module
size_t ofApp::getBaseModuleOffsetSound(uint imodule) {
    size_t ret = 0;
    for (size_t i = 0; i < imodule; i++) ret += getSoundPaths(i).size();
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
            ofLogNotice() << "New module active is now " << newModuleActive;
            modules[moduleActive]->unmakeMostRecent();
            moduleActive = newModuleActive;
            modules[moduleActive]->makeMostRecent();
        }
    }

}

void ofApp::touchDown(ofTouchEventArgs& touch) {

    // ofSetOrientation(OF_ORIENTATION_DEFAULT);

    lastTouchY = touch.y;

    if (appState == APP) updateNewModuleActive(touch.x);

    resetInactivityTime();

    if (appState == SPLASH_SCREEN) { appState = SPLASH_FADE; return; }
    if (appState == SPLASH_FADE)   { appState = ABOUT; return; }

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

        // ofLogNotice() << "down (" << id << ", " << x << ", " << y << ")" ;

        if (y > CONSOLE_HEIGHT*ofGetHeight() && (appState != BAR || y < aboutY) && modules[getModuleIdx(x)]->isNotFull()) {
            touches.insert(pair<int,Touch> (id, Touch(x, y)));
            // NOTE: dismiss swipe info, only if inside module area (not including console)
            if (showSwipeInfo) showSwipeInfo = false;
        }

    }

    #if defined TARGET_OF_IOS || defined TARGET_ANDROID
    swiping = false;
    #endif

}

void ofApp::changeLanguage(string language) {
    ofLogNotice() << "Changing language to " << language;
    if (language == ofApp::language) return;
    ofApp::language = language;
    initTranslations();
    initImages(false);
    setupModules();
}

void ofApp::touchMoved(ofTouchEventArgs& touch) {

    if (appState == APP) updateNewModuleActive(touch.x);

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
        // modules[getModuleIdx(x)]->addParticle(increment, x, y);
        ofLogNotice() << "Particle added" ;
        swiping = false;
    }
    #else
    if (y > CONSOLE_HEIGHT*ofGetHeight() && y < ofApp::maxParticleY && !swiping) {
        modules[getModuleIdx(x)]->addParticle(increment, x, y);
        ofLogNotice() << "Particle added" ;
        swiping = false;
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
    ofLogNotice() << "Current orientation is " << ofGetOrientation();

    if (isPhone()) return;

    if (isTablet() && appState != APP) {
        if (newOrientation == OF_ORIENTATION_DEFAULT || newOrientation == OF_ORIENTATION_180) {
            ofLogNotice() << "!!!Ignoring orientation change in tablet in app ";
            return;
        }
    }

    if (isTablet() && appState == APP) {

        // it transitioning to portrait, deactivate all modules, except the one with which last interacted
        if (newOrientation == OF_ORIENTATION_DEFAULT || newOrientation == OF_ORIENTATION_180) {
            for (uint i = 0; i < modules.size(); i++) {
                if (i == moduleActive)  modules[i]->activate();
                else modules[i]->deactivate();
            }
        }
        // it transitioning to landscape, activate back all modules
        else {
            for (uint i = 0; i < modules.size(); i++) modules[i]->activate();
        }


    }

    setupModules(newOrientation);

    // NOTE: in android ofGetOrientation() doesn't get update, so force it
    androidOrientation = newOrientation;

    // upside down is no good for anything
    // if (isTablet() && newOrientation == OF_ORIENTATION_180)
    //     ofSetOrientation(OF_ORIENTATION_DEFAULT);
    // else {
    //     ofLogNotice() << "Changing orientation change";
    //     ofSetOrientation(ofOrientation(newOrientation));
    // }

}

#if defined TARGET_OF_IOS
void ofApp::onSwipe(SwipeRecognitionArgs& args) {

    if (appState != APP) return;

    ofLogNotice() << "Detected swipe event";

    // multiplying swipeOriginY by 2 because of retina display
    if (args.swipeOriginY * 2 > CONSOLE_HEIGHT * ofGetHeight()) {
        int direction = args.direction;
        if (direction == 4 && !ofApp::isTabletInPortrait()) {
            appState = ABOUT_ASCENDING;
            swiping = true;
            ofLogNotice() << "Setting swiping true";
        }
        else {
            modules[0]->changeInstrument(direction);
            swiping = true;
            ofLogNotice() << "Prepare instrument change";
        }
    } else {
        ofLogNotice() << "Ignoring swipe event";
    }

}
#elif defined TARGET_ANDROID
void ofApp::swipe(ofxAndroidSwipeDir swipeDir, int id) {
    if (appState != APP) return;
    int dir;
    if (lastTouchY > CONSOLE_HEIGHT * ofGetHeight()) {
        if (swipeDir == OFX_ANDROID_SWIPE_UP) {
            appState = ABOUT_ASCENDING;
            swiping = true;
            ofLogNotice() << "Setting swiping true";
        }
        else if (swipeDir == OFX_ANDROID_SWIPE_LEFT) {
            dir = 2;
            swiping = true;
        }
        else if (swipeDir == OFX_ANDROID_SWIPE_RIGHT) {
            dir = 1;
            swiping = true;
        }

        if (dir == 1 || dir == 2) {
            modules[0]->changeInstrument(dir);
        }
    }
}

#endif

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
    // return true; // NOTE: this is for testing
    return min(ofGetWidth(), ofGetHeight()) <= 1100;
  #else
    return false;
  #endif
}

bool ofApp::isTablet() {
  #if defined TARGET_OF_IOS
    return ofxiOSGetDeviceType() == OFXIOS_DEVICE_IPAD;
  #else
    return !isPhone();
  #endif
}

bool ofApp::isInPortrait() {
    int orientation = isAndroid() ? androidOrientation : ofGetOrientation();
    return orientation == OF_ORIENTATION_DEFAULT || orientation == OF_ORIENTATION_180;
}

bool ofApp::isInLandscape() {
    int orientation = isAndroid() ? androidOrientation : ofGetOrientation();
    return orientation == OF_ORIENTATION_90_LEFT || orientation == OF_ORIENTATION_90_RIGHT;
}

bool ofApp::isTabletInPortrait() { return isTablet() && isInPortrait(); }
bool ofApp::isTabletInLandscape() { return isTablet() && isInLandscape(); }

int ofApp::getFontSize() {

    uint ret;

    uint width = ofGetWidth();

    if (isIos() || isAndroid()) {

        if (ofApp::isPhone()) {
            if (width <= 500) ret = width*24/640;
            else if (width <= 640)  ret = 24; // iPhone 5, 5s and SE
            else if (width <= 750)  ret = 28; // iPhone 6, 6s and 7
            else if (width <= 1242) ret = 34; // iPhone 6+, 6s+ and 7+
            else ret = 38;
        }

        else {

            if (isIos() && isTabletInPortrait()) {
                ofLogNotice() << "getFontSize in portrait mode!";
                if      (width <= 1024) ret = 14; // iPad 2
                else if (width <= 1536) ret = 24; // iPad Mini and iPad Air
                else ret = 26;
            }

            else if (isAndroid() && isTabletInPortrait()) {
                if      (width <= 1024) ret = 14; // iPad 2
                else if (width <= 1536) ret = 36; // iPad Mini and iPad Air
                else ret = 28;
            }

            else {
                ofLogNotice() << "getFontSize in landscape mode!";
                uint offset = 2;
                if      (width <= 1024) ret = 14-offset;   // iPad 2
                else if (width <= 1536) ret = 24-offset*2; // iPad Mini and iPad Air
                else ret = 26-offset*2;
            }
        }
    }

    else {
        ret = (width*24)/640;
    }

    ofLogNotice() << "Going to use font size " << ret;

    return ret;
}

string ofApp::getSystemLanguage() {

    string ret;

#if defined TARGET_OF_IOS

    // NOTE: Apparently one can mix Objective-C in C++ with no problem!

    NSString *lang = [[NSLocale preferredLanguages] objectAtIndex:0];

    // Convert NSString to C++ std::string
    ret = string([lang UTF8String]);

    ofLogNotice() << "language in getSystemLanguage is " << ret;

    if (ret == "pt-PT" || ret == "pt-BR") ret = "pt";

#else

    ret = ofApp::language;

#endif

    return ret;
}
