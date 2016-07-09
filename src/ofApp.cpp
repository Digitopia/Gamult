#include "ofApp.h"

int ofApp::maxParticleY = 0;
int ofApp::mouseId = 0;
unsigned int ofApp::inactivityCounter = 0;
bool ofApp::multitouch = true;
bool ofApp::inactive = false;
unsigned int ofApp::moduleActive = 0;
unsigned int ofApp::currentAlpha = DEFAULT_ALPHA;

typedef map<int,Touch>::iterator touchesIterator;

vector<Module*> ofApp::modules;
//UISwipeGestureRecognizer *swipe;

#if defined TARGET_SEMIBREVE
ofxOscSender ofApp::oscSender;
ofxOscReceiver ofApp::oscReceiver;
#endif

void ofApp::setup() {

    ofSetLogLevel(OF_LOG_NOTICE);

    ofLogNotice() << "setup";

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
    ofLogNotice() << "Running iOS version";
    if (ofApp::isTablet()) {
        ofLogNotice() << "Running iPad version";
        ofSetOrientation(OF_ORIENTATION_90_LEFT);
    } else {
        ofLogNotice() << "Running iPhone version";
        swiper.setup();
        ofAddListener(swiper.swipeRecognized, this, &ofApp::onSwipe);
        swiping = false;
    }
    ofxAccelerometer.setup();
    accelCount = 0;
    #endif

    #if !defined TARGET_OF_IOS
    ofLogNotice() << "Registering for touch events if not ios";
    ofRegisterTouchEvents(this);
    #endif

    ofSetFrameRate(FRAME_RATE);
    ofSetCircleResolution(CIRCLE_RESOLUTION);

    if (multitouch) ofHideCursor();

    initModules();
    setupModules();
    loadModuleSounds();
    initImages();

    state = APP; // TODO: don't forget to revert to SPLASH_SCREEN for release

    inactivityState = ACTIVE;

    aboutY = 0;
    splashAlpha = 255;
    arrowDownY = ofGetHeight()/3*2;
    arrowDownYBase = arrowDownY;
    arrowDownDir = 1;
    ofApp::maxParticleY = round(ofGetHeight() * (1-LIMIT_PARTICLE));

}


void ofApp::initModules() {
    
    ofLogNotice() << "start initModules";
    
    int nModules = isPhone() ? 1 : 4;
    ofLogNotice() << "Allocating " << nModules << " modules";
    modules.reserve(nModules);
    int width = ofGetWidth()/nModules;
    int height = ofGetHeight();
    int habitants = PARTICLES_PER_MODULE;
    for (unsigned int i = 0; i < nModules; i++) {
        int x = i*width;
        int y = 0;
        modules.push_back(new Module(i, x, y, width, height, habitants, getSoundPaths(i)));
    }
    
    ofLogNotice() << "end initModules";
    
}

void ofApp::setupModules() {
    
    ofLogNotice() << "start setupModules";
    
    if (isTabletInPortrait()) {
        modules[moduleActive]->setDimensions(0, 0, ofGetWidth(), ofGetHeight());
    }
    else {
        for (unsigned int i = 0; i < modules.size(); i++) {
            int width = ofGetWidth()/modules.size();
            modules[i]->setDimensions(i*width, 0, width, ofGetHeight());
        }
    }
    
    // update down arrow on orientation change
    int barRectLength = 0.1*ofGetWidth();
    int barRectHeight = barRectLength/4;
    barRect.set(ofGetWidth()/2 - barRectLength/2, ofGetHeight() - barRectHeight, barRectLength, barRectHeight);
    
    ofLogNotice() << "end setupModules";
}

void ofApp::loadModuleSounds() {

    for (int i = 0; i < modules.size(); i++) {
        modules[i]->loadSounds();
    }
}

void ofApp::initImages() {
    ofLogNotice() << "start initImages";
    imgSplashScreen.load("images/splash_screen.png");
    imgSplashScreen.resize(ofGetWidth(), ofGetHeight());
    imgAbout.load("images/about.png");
    imgAbout.resize(ofGetWidth(), ofGetHeight());
    imgArrow.load("images/arrow_up.png");
    imgArrowDown.load("images/arrow_down.png");
    ofLogNotice() << "end initImages";
}

void ofApp::update() {

    #if defined TARGET_SEMIBREVE
    handleInactivity();
    checkMultitouchData();
    #endif

    #if defined TARGET_OF_IOS
    detectShake();
    #endif

    if (state == BAR_ASCENDING && aboutY <= maxParticleY) {
        state = BAR;
    }

    if (state == BAR_DESCENDING && aboutY >= ofGetHeight()) {
        state = APP;
    }

    if (state == ABOUT_ASCENDING && aboutY <= 0) {
        state = ABOUT;
    }

    if (state == ABOUT_DESCENDING && aboutY >= ofGetHeight()) {
        state = APP;
    }

    if (state == ABOUT || state == SPLASH_SCREEN) {
        return;
    }

    for (touchesIterator it = touches.begin(); it != touches.end(); it++) {
        it->second.update();
    }

    #if defined TARGET_OF_IOS
    if (ofxiOSGetDeviceType() == OFXIOS_DEVICE_IPAD && ofGetOrientation() == OF_ORIENTATION_DEFAULT) {
        ofApp::modules[moduleActive]->update();
    } else {
        for (int i = 0; i < modules.size(); i++) {
            ofApp::modules[i]->update();
        }
    }
    #else
    for (int i = 0; i < NMODULES; i++) {
        ofApp::modules[i]->update();
    }
    #endif
    
}

void ofApp::handleInactivity() {

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

        if (particleChance == 27) {

            unsigned int rModule = ofRandom(0, NMODULES);
            unsigned int rX = ofRandom(modules[rModule]->getX0(), modules[rModule]->getX1());
            // unsigned int rY = ofRandom(100, modules[rModules].getHeight());
            unsigned int rY = ofRandom(CONSOLE_HEIGHT*ofGetHeight(), ofGetHeight()*(1-LIMIT_PARTICLE));
            unsigned int rIncrement = ofRandom(INACTIVITY_TOUCH_MIN, INACTIVITY_TOUCH_MAX);

            modules[getModuleId(rX)]->addParticle(rIncrement, rX, rY);
        }

    }

    // POST_INACTIVE -> ACTIVE
    if (inactivityState == POST_INACTIVE) {
        for (int i = 0; i < modules.size(); i++) {
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
    for (unsigned int i = 0; i < modules.size(); i++) {
        modules[i]->removeAllParticles();
        modules[i]->unfreeze();
        modules[i]->enableGravity();
        modules[i]->resetFaderSpeed();
    }
}

void ofApp::draw() {

    ofBackground(BACKGROUND_COLOR);

    if (state == SPLASH_SCREEN) {
        imgSplashScreen.draw(0, 0, ofGetWidth(), ofGetHeight());
        if (ofGetElapsedTimeMillis() > 1500) state = SPLASH_FADE;
        return;
    }

    else if (state == SPLASH_FADE) {
        ofPushStyle();
        ofEnableAlphaBlending();
        ofSetColor(255, 255, 255, 255);
        imgAbout.draw(0, 0, ofGetWidth(), ofGetHeight());
        splashAlpha -= 2;
        ofSetColor(255, 255, 255, splashAlpha);
        imgSplashScreen.draw(0, 0, ofGetWidth(), ofGetHeight());
        ofDisableAlphaBlending();
        ofPopStyle();
        if (splashAlpha <= 0) state = ABOUT;
        return;
    }

    else if (state == ABOUT) {
        imgAbout.draw(0, 0, ofGetWidth(), ofGetHeight());
        drawBouncingArrow();
        return;
    }

    drawModules();
    drawLines();
    drawParticles();
    drawTouches();

    if (ofGetOrientation() == OF_ORIENTATION_DEFAULT) {
        ofPushStyle();
        ofSetColor(ofColor::fromHex(BUTTON_CHANGE_INSTRUMENT_COLOR), BUTTON_CHANGE_INSTRUMENT_COLOR_ALPHA);
        ofDrawRectangle(previousInstrumentRect);
        ofDrawRectangle(nextInstrumentRect);
        ofPopStyle();
    }

    if (state == BAR) {
        ofPushStyle();
        ofEnableAlphaBlending();
        ofSetColor(255, 255, 255, DEFAULT_ALPHA);
        imgAbout.draw(0, aboutY, ofGetWidth(), ofGetHeight());
        ofDisableAlphaBlending();
        drawArrow(false);
        ofPopStyle();
    }

    else if (state == ABOUT_ASCENDING) {
        if (aboutY < 15) {
            aboutY = 0;
            state = ABOUT;
        } else {
            aboutY -= 15;
        }
        if (currentAlpha < 255) currentAlpha += 3;
        ofPushStyle();
        ofEnableAlphaBlending();
        ofSetColor(255, 255, 255, currentAlpha);
        imgAbout.draw(0, aboutY, ofGetWidth(), ofGetHeight());
        ofDisableAlphaBlending();
        ofPopStyle();
    }

    else if (state == ABOUT_DESCENDING) {
        aboutY += 20;
        imgAbout.draw(0, aboutY, ofGetWidth(), ofGetHeight());
    }

    else if (state == BAR_ASCENDING) {
        ofPushStyle();
        ofEnableAlphaBlending();
        ofSetColor(255, 255, 255, DEFAULT_ALPHA);
        aboutY -= 5;
        imgAbout.draw(0, aboutY, ofGetWidth(), ofGetHeight());
        drawArrow(false);
        ofDisableAlphaBlending();
        ofPopStyle();
    }

    else if (state == BAR_DESCENDING) {
        ofPushStyle();
        ofEnableAlphaBlending();
        ofSetColor(255, 255, 255, DEFAULT_ALPHA);
        aboutY += 5;
        imgAbout.draw(0, aboutY, ofGetWidth(), ofGetHeight());
        drawArrow(true);

        ofDisableAlphaBlending();
        ofPopStyle();
    }

    else if (state == APP) {
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

}

vector<string> ofApp::getSoundPaths(unsigned int index) {

    vector<string> ret;

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

    return ret;

}

#if defined TARGET_SEMIBREVE
void ofApp::checkMultitouchData() {

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
        // Can't open and edit Max since already past trial

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
    ofLogNotice() << "pressed" ;
    ofTouchEventArgs touchEvent;
    touchEvent.id = ++mouseId;
    touchEvent.x = mouse.x;
    touchEvent.y = mouse.y;
    touchEvent.type = ofTouchEventArgs::down;
    ofNotifyEvent(ofEvents().touchDown, touchEvent, this);
}

void ofApp::mouseDragged(ofMouseEventArgs& mouse) {
    if (multitouch) return;
    ofLogNotice() << "dragged" ;
    ofTouchEventArgs touchEvent;
    touchEvent.id = mouseId;
    touchEvent.x = mouse.x;
    touchEvent.y = mouse.y;
    touchEvent.type = ofTouchEventArgs::move;
    ofNotifyEvent(ofEvents().touchMoved, touchEvent, this);
}

void ofApp::mouseReleased(ofMouseEventArgs& mouse) {
    if (multitouch) return;
    ofLogNotice() << "released" ;
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
        int moduleWidth = ofGetWidth() / float(modules.size());
        int newModuleActive = floor(x/moduleWidth);
        if (newModuleActive != moduleActive) {
            ofLogNotice() << "new module active is now " << newModuleActive;
            moduleActive = newModuleActive;
        }
    }
    
}

void ofApp::touchDown(ofTouchEventArgs& touch) {
    
    updateNewModuleActive(touch.x);

    resetInactivityTime();
    
    if (state == SPLASH_SCREEN || state == SPLASH_FADE) {
        state = ABOUT;
        return;
    }

    int x = touch.x;
    int y = touch.y;
    int id = touch.id;

    if (state == ABOUT) {
        ofRectangle arrowDownRect(ofGetWidth()/2 - imgArrowDown.getWidth()/2, arrowDownY, imgArrowDown.getWidth(), imgArrowDown.getHeight());
        if (arrowDownRect.inside(x, y)) {
            state = ABOUT_DESCENDING;
        }
        return;
    }

    if (state == APP && barRect.inside(x, y)) {
        state = BAR_ASCENDING;
        return;
    }

    if (state == BAR && barRect.inside(x, y)) {
        state = BAR_DESCENDING;
        return;
    }

    if (state == BAR && y >= ofGetHeight()*(1-LIMIT_PARTICLE)) {
        state = ABOUT_ASCENDING;
        return;
    }

    if (state == APP || state == BAR) {

        ofLogNotice() << "down (" << id << ", " << x << ", " << y << ")" ;

        if (y > CONSOLE_HEIGHT*ofGetHeight() && (state != BAR || y < aboutY) && modules[getModuleId(x)]->isNotFull()) {
            touches.insert(pair<int,Touch> (id, Touch(x, y)));
        }

    }

    #if defined TARGET_OF_IOS
    swiping = false;
    #endif

}

void ofApp::touchMoved(ofTouchEventArgs& touch) {
    
    updateNewModuleActive(touch.x);

    resetInactivityTime();

    if (state != APP && state != BAR) return;

    int x = touch.x;
    int y = touch.y;
    int id = touch.id;

    ofLogNotice() << "moved (" << id << ", " << x << ", " << y << ")" ;

    touchesIterator it = touches.find(id);
    if (it == touches.end()) return;

    it->second.setXY(x, y);

}

void ofApp::touchUp(ofTouchEventArgs& touch) {

    resetInactivityTime();

    if (state != APP && state != BAR) return;

    int id = touch.id;

    ofLogNotice() << "up (" << id << ")" ;

    touchesIterator it = touches.find(id);

    if (it == touches.end()) return;

    int x = it->second.getX();
    int y = it->second.getY();
    float increment = it->second.getIncrement();

    touches.erase(it);

    #if defined TARGET_OF_IOS
    if (y > CONSOLE_HEIGHT*ofGetHeight() && y < ofApp::maxParticleY && !swiping) {
        modules[getModuleId(x)]->addParticle(increment, x, y);
        ofLogNotice() << "particle added" ;
        swiping = false;
    }
    #else
    if (y > CONSOLE_HEIGHT*ofGetHeight() && y < ofApp::maxParticleY) {
        modules[getModuleId(x)]->addParticle(increment, x, y);
        ofLogNotice() << "particle added" ;
    }
    #endif

}

int ofApp::getModuleId(int x) {
    
    if (isTabletInPortrait()) {
        return moduleActive;
    }
    
    for (int i = 0; i < modules.size(); i++) {
        if (x >= modules[i]->getX0() && x < modules[i]->getX1())
            return i;
    }
    return -1;
}

void ofApp::drawLine(int nth) {
    
    vector<Particle*> nthParticles;
    
    #if defined TARGET_OSX
    for (int i = 0; i < NMODULES; i++) {
        if (ofApp::modules[i]->getNumberOfParticles() > nth) {
            nthParticles.push_back(ofApp::modules[i]->getParticle(nth));
        }
    }
    #endif
    
    #if defined TARGET_OF_IOS
    if (ofGetOrientation() == OF_ORIENTATION_DEFAULT) {
        if (ofApp::modules[moduleActive]->getNumberOfParticles() > nth)
            nthParticles.push_back(ofApp::modules[moduleActive]->getParticle(nth));
    } else {
        for (int i = 0; i < modules.size(); i++) {
            if (ofApp::modules[i]->getNumberOfParticles() > nth) {
                nthParticles.push_back(ofApp::modules[i]->getParticle(nth));
            }
        }
    }
    #endif

    if (nthParticles.size() == 0)
        return;

    ofPushStyle();

    ofNoFill();
    ofSetLineWidth(POLY_WIDTH);
    ofSetColor(POLY_COLOR);

    ofBeginShape();

    // need four points at least otherwise the shape doesn't 'close' and therefore doesn't draw

    int middleY = round((ofGetHeight() - CONSOLE_HEIGHT*ofGetHeight())/2.0 + CONSOLE_HEIGHT*ofGetHeight());

    ofCurveVertex(0, middleY);
    ofCurveVertex(0, middleY);

    for (int i = 0; i < nthParticles.size(); i++) {
        ofCurveVertex(nthParticles[i]->getX(), nthParticles[i]->getY());
    }

    ofCurveVertex(ofGetWidth(), middleY);
    ofCurveVertex(ofGetWidth(), middleY);

    ofEndShape();

    ofPopStyle();
}

void ofApp::drawLines() {
    for (int i = 0; i < PARTICLES_PER_MODULE; i++) {
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
    
    ofLogNotice() << "detected orientation change";
    
    if (isPhone()) {
        ofLogNotice() << "ignoring orientation change since it's a phone";
        return;
    }
    
    // upside down is no good for anything
    if (newOrientation == OF_ORIENTATION_180)
        ofSetOrientation(OF_ORIENTATION_DEFAULT);
    else {
        ofSetOrientation(ofOrientation(newOrientation));
    }
    
    // it transitioning to landscape, activate back all modules
    if (newOrientation == OF_ORIENTATION_90_LEFT || newOrientation == OF_ORIENTATION_90_RIGHT) {
        for (unsigned int i = 0; i < modules.size(); i++) modules[i]->activate();
    }
    // it transitioning to portrait, deactivate all modules, except the one with which last interacted
    else {
        for (unsigned int i = 0; i < modules.size(); i++) {
            if (i == moduleActive) {
                ofLogNotice() << "activating module " << i;
                modules[i]->activate();
            }
            else {
                ofLogNotice() << "deactivating module " << i;
                modules[i]->deactivate();
            }
        }
    }
    setupModules();
    
}

#if defined TARGET_OF_IOS
void ofApp::onSwipe(swipeRecognitionArgs& args) {

    ofLogNotice() << "Swipe Event! Yes!";

    // multiplying swipeOriginY by 2 because of retina display
    if (args.swipeOriginY * 2 > CONSOLE_HEIGHT * ofGetHeight()) {
        int direction = args.direction  ;

        ofApp::modules[0]->prepareInstrumentChange(direction);

        swiping = true;
    } else {
        ofLogNotice() << "THOU SHALL NOT PASS!!";
    }

}

void ofApp::detectShake() {
    float offset = 1.7;
    float accelX = ofxAccelerometer.getForce().x;
    float accelY = ofxAccelerometer.getForce().y;
    if ((accelX < - offset || accelX > offset) || (accelY < - offset || accelY > offset)) {
        accelCount++;
    } else {
        accelCount=0;
    }

    if (accelCount>=4) {
        ofLogNotice() << "shake it baby, shake it";
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
    
    // check iPad in portrait
#if defined TARGET_OF_IOS
    if (ofxiOSGetDeviceType() == OFXIOS_DEVICE_IPAD) {
        if (ofGetOrientation() == OF_ORIENTATION_90_LEFT || ofGetOrientation() == OF_ORIENTATION_90_RIGHT) {
            return true;
        }
    }
#endif
    
    // check Android in portrait
    // TODO: check for android tablet in portrait too
    
    // Otherwise, is false
    return false;
    
}