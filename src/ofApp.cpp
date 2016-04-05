#include "ofApp.h"

int ofApp::nModules = MODULES;
int ofApp::nParticlesPerModule = PARTICLES_PER_MODULE;
int ofApp::maxParticleY = 0;
int ofApp::mouseId = 0;
unsigned int ofApp::inactivityCounter = 0;
bool ofApp::multitouch = true;
bool ofApp::inactive = false;
unsigned int ofApp::moduleActive = 0;
unsigned int ofApp::currentAlpha = DEFAULT_ALPHA;

typedef map<int,Touch>::iterator touchesIterator;

Module** ofApp::modules = new Module* [ofApp::nModules];

#if defined TARGET_SEMIBREVE
ofxOscSender ofApp::oscSender;
ofxOscReceiver ofApp::oscReceiver;
#endif

void ofApp::setup() {
    
    #if defined TARGET_OF_OSX
    ofSetDataPathRoot("../Resources/data/");
    
    #elif defined TARGET_OF_IPAD
    ofSetOrientation(OF_ORIENTATION_90_LEFT);
    
    #elif defined TARGET_SEMIBREVE
    oscReceiver.setup(RECEIVE_PORT);
    oscSender.setup(HOST, SEND_PORT);
    #endif

    ofSetFrameRate(FRAME_RATE);
    ofSetCircleResolution(CIRCLE_RESOLUTION);

    ofRegisterTouchEvents(this);

    if (multitouch) ofHideCursor();

	initModules();
    setupModules();

	ofApp::maxParticleY = round(ofGetHeight() * (1-LIMIT_PARTICLE));

    int barRectLength = 0.1*ofGetWidth();
    int barRectHeight = barRectLength/4;

    barRect.set(ofGetWidth()/2 - barRectLength/2, ofGetHeight() - barRectHeight, barRectLength, barRectHeight);

    imgSplashScreen.load("images/splash-screen.png");
   // imgSplashScreen.resize(ofGetWidth(), ofGetHeight());
    imgAbout.load("images/about7.png");
    imgAbout.resize(ofGetWidth(), ofGetHeight());
    imgArrow.load("images/arrow.png");
    imgArrowDown.load("images/arrow_down.png");

    state = APP; // state = SPLASH_SCREEN;

    inactivityState = ACTIVE;

    aboutY = 0;
    splashAlpha = 255;
    arrowDownY = ofGetHeight()/3*2;
    arrowDownYBase = arrowDownY;
    arrowDownDir = 1;
    
    #if defined TARGET_OF_IPHONE
    int w = round(BUTTON_CHANGE_INSTRUMENT_WIDTH * ofGetWidth());
    int h = round(BUTTON_CHANGE_INSTRUMENT_HEIGHT * ofGetHeight());
    int consoleHeight = CONSOLE_HEIGHT*ofGetHeight();
    int middleY = round((ofGetHeight() - consoleHeight)/2 + consoleHeight);
    previousInstrumentRect.set(0, middleY - h/2, w, h);
    nextInstrumentRect.set(ofGetWidth(), middleY - h/2, -w, h);
    #endif
    
}

void ofApp::update() {

    #if defined TARGET_SEMIBREVE
    handleInactivity();
    checkMultitouchData();
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

    for (int i = 0; i < ofApp::nModules; i++) {
        ofApp::modules[i]->update();
    }

}

void ofApp::handleInactivity() {

   // cout << inactivityState << endl;

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

            unsigned int rModule = ofRandom(0, MODULES);
            unsigned int rX = ofRandom(modules[rModule]->getX0(), modules[rModule]->getX1());
            // unsigned int rY = ofRandom(100, modules[rModules].getHeight());
            unsigned int rY = ofRandom(CONSOLE_HEIGHT*ofGetHeight(), ofGetHeight()*(1-LIMIT_PARTICLE));
            unsigned int rIncrement = ofRandom(INACTIVITY_TOUCH_MIN, INACTIVITY_TOUCH_MAX);

            modules[getModuleId(rX)]->addParticle(rIncrement, rX, rY);
        }

    }

    // POST_INACTIVE -> ACTIVE
    if (inactivityState == POST_INACTIVE) {
        for (int i = 0; i < ofApp::nModules; i++) {
            modules[i]->removeAllParticles();
        }
        inactivityState = ACTIVE;
    }

}

void ofApp::resetInactivityTime() {

    if(inactivityState == INACTIVE) inactivityState = POST_INACTIVE;
    inactivityCounter = 0;
}

void ofApp::resetModules() {
    for (unsigned int i = 0; i < nModules; i++) {
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
        }
        else {
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
    
    if (ofGetOrientation() == OF_ORIENTATION_DEFAULT) {
        ofApp::modules[moduleActive]->drawParticles();
    }
    else {
        for (int i = 0; i < ofApp::nModules; i++) {
            ofApp::modules[i]->drawParticles();
        }
    }
}

void ofApp::drawTouches() {
    for (touchesIterator it = touches.begin(); it != touches.end(); it++) {
        it->second.draw();
    }
}


void ofApp::drawModules() {
    
    if (ofGetOrientation() == OF_ORIENTATION_DEFAULT) {
        ofApp::modules[moduleActive]->draw();
    }
    
    else {
        for (int i = 0; i < ofApp::nModules; i++) {
            ofApp::modules[i]->draw();
        }
    }
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
        imgArrow.draw(barRect.x + ARROW_OFFSET, barRect.y + ARROW_OFFSET, barRect.width - 2*ARROW_OFFSET, barRect.height - 2*ARROW_OFFSET);
    }

    else {
        imgArrow.draw(barRect.x + barRect.width - ARROW_OFFSET, barRect.y + barRect.height - ARROW_OFFSET, -barRect.width + 2*ARROW_OFFSET, -barRect.height +2*ARROW_OFFSET);
    }

    ofDisableAlphaBlending();

    ofPopStyle();

}

void ofApp::initModules() {
    int width = ofGetWidth()/ofApp::nModules;
    int height = ofGetHeight();
    int habitants = ofApp::nParticlesPerModule;
    for (unsigned int i = 0; i < ofApp::nModules; i++) {
        int x = i*width;
        int y = 0;
        ofApp::modules[i] = new Module(i, x, y, width, height, habitants, getSoundPaths(i));
    }
}

void ofApp::setupModules() {
    
    ofLog() << ofGetWidth() << " " << ofGetHeight() << endl;
    
    if (ofGetOrientation() == OF_ORIENTATION_DEFAULT) {
        ofLog() << "setting up in default orientation" << endl;
        ofApp::modules[moduleActive]->setDimensions(0, 0, ofGetWidth(), ofGetHeight());
        ofApp::modules[moduleActive]->updateParticlesOnOrientationChange(0, 0, ofGetWidth(), ofGetHeight());
    }
    else {
        int width = ofGetWidth()/ofApp::nModules;
        int height = ofGetHeight();
        for (unsigned int i = 0; i < ofApp::nModules; i++) {
            int x = i*width;
            int y = 0;
            ofApp::modules[i]->updateParticlesOnOrientationChange(x, y, width, height);
            ofApp::modules[i]->setDimensions(x, y, width, height);
        }
    }
    
    drawModules();
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
        // cout << m.getAddress() << endl;

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

            cout << "x is " << x << " and y is " << y << endl;

            touchEvent.x = x;
            touchEvent.y = y;

            if (m.getAddress() == "/1/") {
                touchEvent.type = ofTouchEventArgs::down;
                ofNotifyEvent(ofEvents().touchDown, touchEvent, this);
            }
            else {
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


void ofApp::mousePressed(ofMouseEventArgs &mouse) {
    if (multitouch) return;
    cout << "pressed" << endl;
    ofTouchEventArgs touchEvent;
    touchEvent.id = ++mouseId;
    touchEvent.x = mouse.x;
    touchEvent.y = mouse.y;
    touchEvent.type = ofTouchEventArgs::down;
    ofNotifyEvent(ofEvents().touchDown, touchEvent, this);
}

void ofApp::mouseDragged(ofMouseEventArgs &mouse) {
    if (multitouch) return;
    cout << "dragged" << endl;
    ofTouchEventArgs touchEvent;
    touchEvent.id = mouseId;
    touchEvent.x = mouse.x;
    touchEvent.y = mouse.y;
    touchEvent.type = ofTouchEventArgs::move;
    ofNotifyEvent(ofEvents().touchMoved, touchEvent, this);
}

void ofApp::mouseReleased(ofMouseEventArgs &mouse) {
    if (multitouch) return;
    cout << "released" << endl;
    ofTouchEventArgs touchEvent;
    touchEvent.id = ofApp::mouseId;
    touchEvent.type = ofTouchEventArgs::up;
    ofNotifyEvent(ofEvents().touchUp, touchEvent, this);
}

void ofApp::mouseMoved(ofMouseEventArgs &mouse) {
    resetInactivityTime();
}

void ofApp::keyPressed(int key) {

    if (key == 'm' || key == 'M' ) {

        multitouch = !multitouch;

        if (multitouch) {
            cout << "multitouch on" << endl;
            ofHideCursor();
        }
        else {
            cout << "multitouch off" << endl;
            ofShowCursor();
        }

    }

}

void ofApp::touchDown(ofTouchEventArgs &touch) {

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

        cout << "down (" << id << ", " << x << ", " << y << ")" << endl;

        if (y > CONSOLE_HEIGHT*ofGetHeight() && (state != BAR || y < aboutY) && modules[getModuleId(x)]->isNotFull()) {
            touches.insert(pair<int,Touch> (id, Touch(x, y)));
        }

    }
    
    #if defined TARGET_OF_IPHONE
    if (ofGetOrientation() == OF_ORIENTATION_DEFAULT) {
        int previousModuleActive = moduleActive;
        if (previousInstrumentRect.inside(touch.x, touch.y)) {
            if (moduleActive <= 0) return;
            moduleActive--;
        }
        else if (nextInstrumentRect.inside(touch.x, touch.y)) {
            if (moduleActive >= 3) return;
            moduleActive++;
        }
        
        if (previousModuleActive != moduleActive) {
            for (unsigned int i = 0; i < ofApp::modules[previousModuleActive]->getNumberOfParticles(); i++) {
                ofApp::modules[previousModuleActive]->getParticle(i)->setModule(moduleActive);
            }
            ofApp:modules[moduleActive]->setParticles(ofApp::modules[previousModuleActive]->getParticles());
            ofApp::modules[previousModuleActive]->removeAllParticles();
        }
    }
    #endif

}

void ofApp::touchMoved(ofTouchEventArgs &touch) {

    resetInactivityTime();

    if (state != APP && state != BAR) return;

    int x = touch.x;
    int y = touch.y;
    int id = touch.id;

    cout << "moved (" << id << ", " << x << ", " << y << ")" << endl;

    touchesIterator it = touches.find(id);
    if (it == touches.end()) return;

    it->second.setXY(x, y);

}

void ofApp::touchUp(ofTouchEventArgs &touch) {

    resetInactivityTime();

    if (state != APP && state != BAR) return;

    int id = touch.id;

    cout << "up (" << id << ")" << endl;

    touchesIterator it = touches.find(id);

    if (it == touches.end()) return;

    int x = it->second.getX();
    int y = it->second.getY();
    float increment = it->second.getIncrement();

    touches.erase(it);

    if (y > CONSOLE_HEIGHT*ofGetHeight() && y < ofApp::maxParticleY) {
        modules[getModuleId(x)]->addParticle(increment, x, y);
    }

}

int ofApp::getModuleId(int x) {
    for (int i = 0; i < nModules; i++) {
        if (x >= modules[i]->getX0() && x < modules[i]->getX1())
            return i;
    }
    return -1;
}

void ofApp::drawLine(int nth) {
    
    vector<Particle*> nthParticles;
    if (ofGetOrientation() == OF_ORIENTATION_DEFAULT) {
        if (ofApp::modules[moduleActive]->getNumberOfParticles() > nth)
            nthParticles.push_back(ofApp::modules[moduleActive]->getParticle(nth));
    }
    else {
        for (int i = 0; i < ofApp::nModules; i++) {
            if (ofApp::modules[i]->getNumberOfParticles() > nth) {
                nthParticles.push_back(ofApp::modules[i]->getParticle(nth));
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
    for (unsigned int i = 0; i < nModules; i++) {
        if (modules[i]->anyParticles()) {
            return true;
        }
    }
    return false;
}

void ofApp::deviceOrientationChanged(int newOrientation) {

    // upside down is no good for anything
    if (newOrientation == OF_ORIENTATION_180)
        ofSetOrientation(OF_ORIENTATION_DEFAULT);
    else {
        ofSetOrientation(ofOrientation(newOrientation));
    }

    if (newOrientation == OF_ORIENTATION_90_LEFT || newOrientation == OF_ORIENTATION_90_RIGHT) {
        setupModules();
        for (unsigned int i = 0; i < ofApp::nModules; i++) {
            ofApp::modules[i]->activate();
        }
    }

    else {
        for (unsigned int i = 0; i < ofApp::nModules; i++) {
            if (i == moduleActive) ofApp::modules[i]->activate();
            else ofApp::modules[i]->deactivate();
        }
        setupModules();
    }

    ofLog() << "width is " << ofGetWidth();
    ofLog() << "height is " << ofGetHeight();

}
