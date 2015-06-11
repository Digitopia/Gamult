#include "ofApp.h"

int ofApp::nModules = MODULES;
int ofApp::nParticlesPerModule = PARTICLES_PER_MODULE;
int ofApp::maxParticleY = 0;

ofxOscSender ofApp::oscSender;
ofxOscReceiver ofApp::oscReceiver;

Module** ofApp::modules = new Module* [ofApp::nModules];

void ofApp::setup() {

	ofSetFrameRate(FRAME_RATE);
	ofSetCircleResolution(CIRCLE_RESOLUTION);
	oscReceiver.setup(RECEIVE_PORT);
	oscSender.setup(HOST, SEND_PORT);

    // everything is controlled by multitouch so no need for cursor
	ofHideCursor();
	
	initModules();
	
	ofApp::maxParticleY = round(ofGetHeight() * (1-LIMIT_PARTICLE)); // TODO should be the height of the module instead
    
    about.loadImage("about.png");
    info.loadImage("info.png");
   // infoBox.set(ofGetWidth() - 65, ofGetHeight() - 65, 50, 50);
	
    infoDisplay = false;
}

void ofApp::update() {

    checkMultitouchData();

    for (int i = 0; i < touches.size(); i++) {
        touches[i].update();
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

    for (int i = 0; i < touches.size(); i++) {
        touches[i].draw();
    }
	
	drawLines();
	
    // ofPushStyle();
    // ofSetColor(255,255,255, 80);
    // about.draw(ofGetWidth() - 65, ofGetHeight() - 65, 50, 50);
    // ofPopStyle();
    
    // if(infoDisplay)
    // {
    // info.draw(0 + 50, CONSOLE_HEIGHT + 50, ofGetWidth() - 100, ofGetHeight() - CONSOLE_HEIGHT - 100);
    // }
	
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

//	vector<string> icons2;
//	icons2.push_back("1.png");
//	icons2.push_back("2.png");
//	
//	vector<string> icons3;
//	icons3.push_back("3.png");
//	icons3.push_back("4.png");
	
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

        int id = int(m.getArgAsFloat(0));

        // TODO: change numbers to 'add', 'update' and 'remove' preprends
        // Can't open and edit Max since already past trial

        // TODO: We should change to PureData rather than Max if commited to open source

        if (m.getAddress() == "/1/" || m.getAddress() == "/2/") {

            float x, y;
            x = m.getArgAsFloat(2) * ofGetWidth();
            y = m.getArgAsFloat(3) * ofGetHeight();

            if (m.getAddress() == "/1/") addTouch(id, x, y);
            else updateTouch(id, x, y);
        }

        else if (m.getAddress() == "/3/") {
            removeTouch(id);
        }
	}
}

void ofApp::addTouch(int id, int x, int y) {

    cout << "add (" << id << ", " << x << ", " << y << ")" << endl;

    if (infoDisplay == true) infoDisplay = false;

    if (infoBox.inside(x,y) ) {
        infoDisplay = true;
    }

    // for (int i = 0; i < ofApp::nModules; i++) {
        // if(ofApp::modules[i]->panel->select.inside(x, y)) {
            // ofApp::modules[i]->panel->showPanel = !ofApp::modules[i]->panel->showPanel;
        // }
    // }

    if (infoDisplay) {
        return;
    }

    if (y >= CONSOLE_HEIGHT && modules[getModuleId(x)]->isNotFull()) {
        touches.push_back(Touch(id, x, y));
    }

}

void ofApp::updateTouch(int id, int x, int y) {

    cout << "upd (" << id << ", " << x << ", " << y << ")" << endl;

    for (int i = 0; i < touches.size(); i++) {
        if (touches[i].getId() == id) {
            touches[i].setXY(x, y);
            return;
        }
    }

}

void ofApp::removeTouch(int id) {

    cout << "rem (" << id << ")" << endl;
    
	int x, y;
	float increment;
    
    cout << "touches size " << touches.size() << endl;

    bool found = false;
    for (int i = 0; i < touches.size(); i++) {
        if (touches[i].getId() == id) {
            found = true;
			x = touches[i].getX();
			y = touches[i].getY();
            increment = touches[i].getIncrement();
            cout << id << " " << increment << " " << x << " " << y << endl;
            touches.erase(touches.begin()+i);
        }
    }
    
    if (!found) {
        return;
    }

	if (y < ofApp::maxParticleY) {
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

void ofApp::mousePressed(int x, int y, int button) {
}

void ofApp::mouseReleased(int x, int y, int button) {
}

void ofApp::keyPressed(int key) {
}

void ofApp::keyReleased(int key) {
}

void ofApp::mouseMoved(int x, int y) {
}

void ofApp::mouseDragged(int x, int y, int button) {
}

void ofApp::windowResized(int w, int h) {
}
