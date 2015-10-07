#include "ofApp.h"

int ofApp::nModules = MODULES;
int ofApp::nParticlesPerModule = PARTICLES_PER_MODULE;
int ofApp::maxParticleY = 0;
int ofApp::mouseId = 0;
unsigned int ofApp::inactivityCounter = 0;
unsigned int ofApp::inactivityCounterWithinParticles = 0;
unsigned int ofApp::inactivityNumberOfParticles = 0;
bool ofApp::multitouch = false;
bool ofApp::inactive = false;

typedef map<int,Touch>::iterator touchesIterator;

ofxOscSender ofApp::oscSender;
ofxOscReceiver ofApp::oscReceiver;

Module** ofApp::modules = new Module* [ofApp::nModules];

void ofApp::setup() {

	ofSetFrameRate(FRAME_RATE);
	ofSetCircleResolution(CIRCLE_RESOLUTION);
	
    oscReceiver.setup(RECEIVE_PORT);
	oscSender.setup(HOST, SEND_PORT);
    
    #ifndef TARGET_OF_IOS
    ofSetDataPathRoot("../Resources/data/");
    #endif
   
    ofRegisterTouchEvents(this);
    
    if (multitouch) {
        ofHideCursor();
    }
	
	initModules();

	ofApp::maxParticleY = round(ofGetHeight() * (1-LIMIT_PARTICLE));
    
    int aboutRectLength = 100;
    int aboutRectHeight = 10;
    barRect.set(ofGetWidth()/2 - aboutRectLength/2, ofGetHeight() - aboutRectHeight, aboutRectLength, aboutRectHeight);
    
    aboutRect.set(100, ofGetHeight() - 100, 200, 50);
    
    imgSplashScreen.loadImage("images/splash-screen.png");
    imgAbout.loadImage("images/about.png");
    
    state = SPLASH_SCREEN;
    
    inactivityState = ACTIVE;
    
    aboutY = 0;
    splashAlpha = 255;
    
    inactivityThreshold = getNewInactivityThreshold();
    inactivityThresholdWithinParticles = getNewInactivityThresholdWithinParticles();
    
}

void ofApp::update() {
    
    handleInactivity();
    
    if (aboutY < 0) {
        state = ABOUT;
        aboutY = 0;
    }

    checkMultitouchData();
    
    if (state == BAR_ASCENDING && aboutY <= maxParticleY) {
        state = BAR;
    }
    
    if (state == BAR_DESCENDING && aboutY >= ofGetHeight()) {
        state = APP;
    }
    
    if (state == ABOUT_ASCENDING && aboutY <= 0) {
        state == ABOUT;
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
    
    cout << inactivityState << endl;
    
    // inactivity timer update
    if (inactivityState == ACTIVE || inactivityState == PRE_INACTIVE) {
        inactivityCounter += ofGetLastFrameTime() * 1000;
//        cout << inactivityCounter << endl;
    }
    
    // inactivity timer within particles update
    if (inactivityState == INACTIVE) {
        inactivityCounterWithinParticles += ofGetLastFrameTime() * 1000;
        cout << inactivityCounterWithinParticles << endl;
    }
    
    // ACTIVE -> PRE_INACTIVE
    if (inactivityState == ACTIVE && inactivityCounter >= INACTIVITY_STOP*1000) {
        inactivityState = PRE_INACTIVE;
        resetModules();
    }
    
    // PRE_INACTIVE -> INACTIVE
    if (inactivityState == PRE_INACTIVE && inactivityCounter >= inactivityThreshold) {
        inactivityState = INACTIVE;
        resetModules();
        inactivityNumberOfParticles = ofRandom(INACTIVITY_PARTICLES_MIN, INACTIVITY_PARTICLES_MAX);
    }
    
    // INACTIVE -> POST_INACTIVE
    if (inactivityState == INACTIVE && inactivityNumberOfParticles == 0) {
        inactivityState = POST_INACTIVE;
    }
    
    // POST_INACTIVE -> ACTIVE
    if (inactivityState == POST_INACTIVE) {
        inactivityCounter = 0;
        inactivityThreshold = getNewInactivityThreshold();
        inactivityThresholdWithinParticles = getNewInactivityThresholdWithinParticles();
        inactivityState = ACTIVE;
    }
        
    // INACTIVE
    if (inactivityState == INACTIVE && inactivityCounterWithinParticles >= inactivityThresholdWithinParticles) {
        
        unsigned int rModule = ofRandom(0, MODULES);
        unsigned int rX = ofRandom(modules[rModule]->getX0(), modules[rModule]->getX1());
        // unsigned int rY = ofRandom(100, modules[rModules].getHeight());
        unsigned int rY = ofRandom(100, 200);
        unsigned int rIncrement = ofRandom(INACTIVITY_TOUCH_MIN, INACTIVITY_TOUCH_MAX);
        
        modules[getModuleId(rX)]->addParticle(rIncrement, rX, rY);
        
        // cout << "inactivity particle " << rModule << " " << rX << " " << rY << " " << rIncrement << endl;
        
        inactivityThresholdWithinParticles = getNewInactivityThresholdWithinParticles();
        inactivityCounterWithinParticles = 0;
        inactivityNumberOfParticles--;
        
    }
    
}

void ofApp::resetModules() {
    for (unsigned int i = 0; i < nModules; i++) {
        modules[i]->removeAllParticles();
        modules[i]->unfreeze();
        modules[i]->enableGravity();
        modules[i]->resetFaderSpeed();
    }
}

int ofApp::getNewInactivityThreshold() {
    inactive = false;
    return ofRandom(INACTIVITY_TIME_MIN, INACTIVITY_TIME_MAX)*1000;
}

void ofApp::draw() {
    
    ofBackground(BACKGROUND_COLOR);
    
    if (state == SPLASH_SCREEN) {
        imgSplashScreen.draw(0, 0, ofGetWidth(), ofGetHeight());
        if(ofGetElapsedTimeMillis() > 1500) state = SPLASH_FADE;
        return;
    }
    
    else if (state == SPLASH_FADE) {
        ofPushStyle();
        ofEnableAlphaBlending();
        ofSetColor(255,255,255, 255);
        imgAbout.draw(0, 0, ofGetWidth(), ofGetHeight());
        splashAlpha -= 2;
        ofSetColor(255,255,255, splashAlpha);
        imgSplashScreen.draw(0, 0, ofGetWidth(), ofGetHeight());
        ofDisableAlphaBlending();
        ofPopStyle();
        if(splashAlpha <= 0) state = ABOUT;
        return;
    }
    
    else if (state == ABOUT) {
        imgAbout.draw(0, 0, ofGetWidth(), ofGetHeight());
        return;
    }
    
    for (int i = 0; i < ofApp::nModules; i++) {
        ofApp::modules[i]->draw();
    }
    
    for (int i = 0; i < ofApp::nModules; i++) {
        ofApp::modules[i]->drawParticles();
    }
    
    for (touchesIterator it = touches.begin(); it != touches.end(); it++) {
        it->second.draw();
    }
    
    drawLines();
    ofRect(barRect);

    if (state == BAR) {
        ofEnableAlphaBlending();
        ofSetColor(255,255,255,200);
        imgAbout.draw(0, aboutY, ofGetWidth(), ofGetHeight());
        ofDisableAlphaBlending();
        ofRect(barRect);
        ofRect(aboutRect);
    }
    
    else if (state == ABOUT_ASCENDING) {
        aboutY -= 15;
        imgAbout.draw(0, aboutY, ofGetWidth(), ofGetHeight());
    }
    
    else if (state == ABOUT_DESCENDING) {
        aboutY += 20;
        imgAbout.draw(0, aboutY, ofGetWidth(), ofGetHeight());
    }
    
    else if (state == BAR_ASCENDING) {
        aboutY -= 5;
        imgAbout.draw(0, aboutY, ofGetWidth(), ofGetHeight());
    }
    
    else if (state == BAR_DESCENDING) {
        aboutY += 5;
        imgAbout.draw(0, aboutY, ofGetWidth(), ofGetHeight());
    }
    
}

void ofApp::initModules() {
    
    // TODO: maybe this could just be a JSON config file?
    
    vector<string> bonangs;
	bonangs.push_back("sounds/BBPL1.wav");
	bonangs.push_back("sounds/BBPL2.wav");
	bonangs.push_back("sounds/BBPL3.wav");
//	bonangs.push_back("sounds/BBPL4.wav");
	
	vector<string> genders;
	genders.push_back("sounds/GBPL1.wav");
	genders.push_back("sounds/GBPL2.wav");
	genders.push_back("sounds/GBPL3.wav");
	genders.push_back("sounds/GBPL5.wav");
    genders.push_back("sounds/GBPL2.wav");
    genders.push_back("sounds/GBPL3.wav");
    genders.push_back("sounds/GBPL5.wav");
	
	vector<string> gongs;
	gongs.push_back("sounds/GKPL1f.wav");
	gongs.push_back("sounds/GKPL2f.wav");
	gongs.push_back("sounds/GKPL3f.wav");
	gongs.push_back("sounds/GKPL5f.wav");
    gongs.push_back("sounds/GKPL1f.wav");
    gongs.push_back("sounds/GKPL2f.wav");
    gongs.push_back("sounds/GKPL3f.wav");
	
	vector<string> sarons;
	sarons.push_back("sounds/SBPL1.wav");
	sarons.push_back("sounds/SBPL2.wav");
	sarons.push_back("sounds/SBPL3.wav");
	sarons.push_back("sounds/SBPL4.wav");
    sarons.push_back("sounds/SBPL3.wav");
    sarons.push_back("sounds/SBPL4.wav");
	
	vector< vector<string> > config;
	config.push_back(bonangs);
	config.push_back(genders);
	config.push_back(gongs);
	config.push_back(sarons);
	
	vector<string> icons;
	icons.push_back("images/1.png");
	icons.push_back("images/2.png");
	icons.push_back("images/3.png");
	icons.push_back("images/4.png");

	moduleWidth = ofGetWidth()/ofApp::nModules;
	moduleHeight = ofGetHeight();
	int moduleHabitants = ofApp::nParticlesPerModule;
	
	ofApp::modules[0] = new Module(0, 0*moduleWidth, 0, moduleWidth, moduleHeight, moduleHabitants, bonangs, icons);
	ofApp::modules[1] = new Module(1, 1*moduleWidth, 0, moduleWidth, moduleHeight, moduleHabitants, genders, icons);
	ofApp::modules[2] = new Module(2, 2*moduleWidth, 0, moduleWidth, moduleHeight, moduleHabitants, gongs, icons);
	ofApp::modules[3] = new Module(3, 3*moduleWidth, 0, moduleWidth, moduleHeight, moduleHabitants, sarons, icons);

}

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
    
    inactivityThreshold = getNewInactivityThreshold();
    
    if (state == SPLASH_SCREEN || state == SPLASH_FADE) {
        state = ABOUT;
        return;
    }
    
    if (state == ABOUT) {
        state = ABOUT_DESCENDING;
        return;
    }
    
    int x = touch.x;
    int y = touch.y;
    int id = touch.id;
    
    if (state == APP && barRect.inside(x, y)) {
        state = BAR_ASCENDING;
        return;
    }
    
    if (state == BAR && barRect.inside(x, y)) {
        state = BAR_DESCENDING;
        return;
    }
    
    if (state == BAR && aboutRect.inside(x, y)) {
        state = ABOUT_ASCENDING;
        return;
    }
    
    if (state == APP || state == BAR) {

        cout << "down (" << id << ", " << x << ", " << y << ")" << endl;

        if (y > CONSOLE_HEIGHT && modules[getModuleId(x)]->isNotFull()) {
            touches.insert(pair<int,Touch> (id, Touch(x, y)));
        }
        
    }
    
}

void ofApp::touchMoved(ofTouchEventArgs &touch) {
    
    inactivityThreshold = getNewInactivityThreshold();
    
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
    
    inactivityThreshold = getNewInactivityThreshold();
    
    if (state != APP && state != BAR) return;
    
    int id = touch.id;
    
    cout << "up (" << id << ")" << endl;
    
    touchesIterator it = touches.find(id);

    if (it == touches.end()) return;
    
    int x = it->second.getX();
    int y = it->second.getY();
    float increment = it->second.getIncrement();
  
    touches.erase(it);
    
    if (y > CONSOLE_HEIGHT && y < ofApp::maxParticleY) {
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
    for (int i = 0; i < ofApp::nModules; i++) {
        if (ofApp::modules[i]->getNumberOfParticles() > nth) {
            nthParticles.push_back(ofApp::modules[i]->getParticle(nth));
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
    ofCurveVertex(0, ofGetHeight()/2);
    ofCurveVertex(0, ofGetHeight()/2);
    
    for (int i = 0; i < nthParticles.size(); i++) {
        ofCurveVertex(nthParticles[i]->getX(), nthParticles[i]->getY());
    }
    
    ofCurveVertex(ofGetWidth(), ofGetHeight()/2);
    ofCurveVertex(ofGetWidth(), ofGetHeight()/2);
    
    ofEndShape();
    
    ofPopStyle();
}

void ofApp::drawLines() {
    for (int i = 0; i < PARTICLES_PER_MODULE; i++) {
        drawLine(i);
    }
}

int ofApp::getNewInactivityThresholdWithinParticles() {
    return ofRandom(INACTIVITY_PARTICLE_TIME_MIN, INACTIVITY_PARTICLE_TIME_MAX)*1000;
}

bool ofApp::hasParticles() {
    for (unsigned int i = 0; i < nModules; i++) {
        if (modules[i]->anyParticles()) {
            return true;
        }
    }
    return false;
}