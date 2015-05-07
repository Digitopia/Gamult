#include "ofApp.h"

int ofApp::nModules = MODULES;
int ofApp::nPolygons = PARTICLES_PER_MODULE;
int ofApp::maxParticleY = 0;

ofxOscSender ofApp::oscSender;
Module** ofApp::modules = new Module* [ofApp::nModules];

void ofApp::setup() {
	
	ofSetFrameRate(FRAME_RATE);
	ofSetCircleResolution(CIRCLE_RESOLUTION);
	oscSender.setup(HOST, PORT);

	initModules();
	
	drawingParticle = false;
	increment = 0.0;
	maxIncrement = 20.0;
    
	ofApp::maxParticleY = round(ofGetHeight() * (1-LIMIT_PARTICLE)); // TODO should be the height of the module instead

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

	vector<string> icons2;
	icons2.push_back("1.png");
	icons2.push_back("2.png");
	
	vector<string> icons3;
	icons3.push_back("3.png");
	icons3.push_back("4.png");
	
	int moduleWidth = ofGetWidth()/ofApp::nModules;
	int moduleHeight = ofGetHeight();
	int moduleHabitants = ofApp::nPolygons;
	
	ofApp::modules[0] = new Module(0, 0*moduleWidth, 0, moduleWidth, moduleHeight, moduleHabitants, bonangs, icons);
	ofApp::modules[1] = new Module(1, 1*moduleWidth, 0, moduleWidth, moduleHeight, moduleHabitants, genders, icons2);
	ofApp::modules[2] = new Module(2, 2*moduleWidth, 0, moduleWidth, moduleHeight, moduleHabitants, gongs, icons3);
	ofApp::modules[3] = new Module(3, 3*moduleWidth, 0, moduleWidth, moduleHeight, moduleHabitants, sarons, icons);

}

void ofApp::update() {
	for (int i = 0; i < ofApp::nModules; i++) {
		ofApp::modules[i]->update();
	}
}

void ofApp::draw() {
	
	ofBackground(BACKGROUND_COLOR);

	for (int i = 0; i < ofApp::nModules; i++) {
		ofApp::modules[i]->draw();
	}
	
	drawLines();
	
	if (drawingParticle) {
	
		increment++;
	
		if (increment > maxIncrement)
			increment = maxIncrement;
		
		if (ofGetMouseY() > CONSOLE_HEIGHT)
			drawIncrement();
	}
	
}

void ofApp::keyPressed(int key) {
}

void ofApp::keyReleased(int key) {
}

void ofApp::mouseMoved(int x, int y) {
}

void ofApp::mouseDragged(int x, int y, int button) {
}

void ofApp::mousePressed(int x, int y, int button) {

	if (y >= CONSOLE_HEIGHT) {
		drawingParticle = true;
	}
    
//    for (int i = 0; i < ofApp::nModules; i++) {
//        if(ofApp::modules[i]->panel->select.inside(x, y)) {
//            ofApp::modules[i]->panel->showPanel = !ofApp::modules[i]->panel->showPanel;
//        }
//    }
}

void ofApp::mouseReleased(int x, int y, int button) {
	
	drawingParticle = false;
	
	if (y > ofApp::maxParticleY) {
		increment = 0;
		return;
	}
	
	for (int i = 0; i < ofApp::nModules; i++)
		ofApp::modules[i]->addParticle(increment, x, y);
	
	increment = 0;
	
}

void ofApp::windowResized(int w, int h) {
}

// draws the increase of the particle after mouse press and before mouse release
void ofApp::drawIncrement() {

	ofPushStyle();
	
	ofSetColor(INCREMENT_COLOR);
	if (ofGetMouseY() > ofApp::maxParticleY) ofSetHexColor(PARTICLE_LIMIT_COLOR);
	ofSetLineWidth(INCREMENT_LINE_WIDTH);
	ofNoFill();
	
	ofPolyline polyline;
	ofPoint pt(mouseX, mouseY);
	float angleBegin = 180;
	float angleEnd = float(increment)*(360./float(maxIncrement)) - 180;
	polyline.arc(pt, INCREMENT_RADIUS, INCREMENT_RADIUS, angleBegin, angleEnd, ARC_RESOLUTION);
	polyline.draw();

	ofPopStyle();

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
	
	for (int i = 0; i < nthParticles.size(); i++)
        ofCurveVertex(nthParticles[i]->getX(), nthParticles[i]->getY());
	
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
