#include "ofApp.h"

int ofApp::nModules = MODULES;
int ofApp::nParticlesPerModule = PARTICLES_PER_MODULE;
int ofApp::maxParticleY = 0;

typedef map<int,Touch>::iterator touchesIterator;

ofxOscSender ofApp::oscSender;
ofxOscReceiver ofApp::oscReceiver;

Module** ofApp::modules = new Module* [ofApp::nModules];

void ofApp::setup() {

	ofSetFrameRate(FRAME_RATE);
	ofSetCircleResolution(CIRCLE_RESOLUTION);
	oscReceiver.setup(RECEIVE_PORT);
	oscSender.setup(HOST, SEND_PORT);
    
    ofRegisterTouchEvents(this);

    // with multitouch no need for mouse cursor
    ofHideCursor();
	
	initModules();

    // TODO should be the height of the module instead
	ofApp::maxParticleY = round(ofGetHeight() * (1-LIMIT_PARTICLE));
    
    imgAboutIcon.loadImage("about.png"); // TODO: refactor filenames
    imgAbout.loadImage("info.png"); // TODO: refactor filenames
    aboutRect.set(ofGetWidth() - 65, ofGetHeight() - 65, 50, 50); // TODO: magic numbers
    inAbout = false;
    
}

void ofApp::update() {
    
    checkMultitouchData();
    
    if (inAbout) return;
    
    for (touchesIterator it = touches.begin(); it != touches.end(); it++) {
        it->second.update();
    }
    
    for (int i = 0; i < ofApp::nModules; i++) {
        ofApp::modules[i]->update();
    }
    
}

void ofApp::draw() {
    
    ofBackground(BACKGROUND_COLOR);
    
    for (int i = 0; i < ofApp::nModules; i++) {
        ofApp::modules[i]->draw();
    }
    
    for (touchesIterator it = touches.begin(); it != touches.end(); it++) {
        it->second.draw();
    }
    
    imgAboutIcon.draw(ofGetWidth() - 65, ofGetHeight() - 65, 50, 50); // TODO: magic numbers
    
    if (inAbout) {
        // TODO: magic numbers
        imgAbout.draw(0 + 50, CONSOLE_HEIGHT + 50, ofGetWidth() - 100, ofGetHeight() - CONSOLE_HEIGHT - 100);
    }
    
    drawLines();
    
}

void ofApp::initModules() {
    
    // TODO: maybe this could just be a JSON config file?
    
    vector<string> bonangs;
	bonangs.push_back("sounds/BBPL1.wav");
	bonangs.push_back("sounds/BBPL2.wav");
	bonangs.push_back("sounds/BBPL3.wav");
	bonangs.push_back("sounds/BBPL4.wav");
	
	vector<string> genders;
	genders.push_back("sounds/GBPL1.wav");
	genders.push_back("sounds/GBPL2.wav");
	genders.push_back("sounds/GBPL3.wav");
	genders.push_back("sounds/GBPL5.wav");
	
	vector<string> gongs;
	gongs.push_back("sounds/GKPL1f.wav");
	gongs.push_back("sounds/GKPL2f.wav");
	gongs.push_back("sounds/GKPL3f.wav");
	gongs.push_back("sounds/GKPL5f.wav");
	
	vector<string> sarons;
	sarons.push_back("sounds/SBPL1.wav");
	sarons.push_back("sounds/SBPL2.wav");
	sarons.push_back("sounds/SBPL3.wav");
	sarons.push_back("sounds/SBPL4.wav");
	
	vector< vector<string> > config;
	config.push_back(bonangs);
	config.push_back(genders);
	config.push_back(gongs);
	config.push_back(sarons);
	
	vector<string> icons;
	icons.push_back("1.png");
	icons.push_back("2.png");
	icons.push_back("3.png");
	icons.push_back("4.png");

	int moduleWidth = ofGetWidth()/ofApp::nModules;
	int moduleHeight = ofGetHeight();
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

        ofTouchEventArgs touchEvent;
        
        int id = int(m.getArgAsFloat(0));
        touchEvent.id = id;
        
        // TODO: change numbers to 'add', 'update' and 'remove' preprends
        // Can't open and edit Max since already past trial
        
        // TODO: We should change to PureData rather than Max if commited to open source
        
        if (m.getAddress() == "/1/" || m.getAddress() == "/2/") {
            
            float x, y;
            x = m.getArgAsFloat(2) * ofGetWidth();
            y = m.getArgAsFloat(3) * ofGetHeight();
            
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

void ofApp::touchDown(ofTouchEventArgs &touch) {
    
    if (inAbout) {
        inAbout = false;
        return;
    }
    
    int x = touch.x;
    int y = touch.y;
    int id = touch.id;
    
    if (aboutRect.inside(x, y)) {
        inAbout = true;
        return;
    }
    
    // cout << "down (" << id << ", " << x << ", " << y << ")" << endl;
    
    if (modules[getModuleId(x)]->isNotFull()) {
        touches.insert(pair<int,Touch> (id, Touch(x, y)));
    }
    
}

void ofApp::touchMoved(ofTouchEventArgs &touch) {
    
    if (inAbout) return;
    
    int x = touch.x;
    int y = touch.y;
    int id = touch.id;
    
    // cout << "moved (" << id << ", " << x << ", " << y << ")" << endl;
    
    touchesIterator it = touches.find(id);
    it->second.setXY(x, y);
    
}

void ofApp::touchUp(ofTouchEventArgs &touch) {
    
    if (inAbout) return;
    
    int id = touch.id;
    
    // cout << "up (" << id << ")" << endl;
    
    touchesIterator it = touches.find(id);
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
