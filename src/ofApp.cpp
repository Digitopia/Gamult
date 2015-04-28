#include "ofApp.h"

int ofApp::nModules = MODULES;
int ofApp::nPolygons = PARTICLES_PER_MODULE;
int ofApp::maxParticleY = 0;

ofxOscSender ofApp::oscSender;
Module** ofApp::modules = new Module* [ofApp::nModules];
PolyClass** ofApp::polygons = new PolyClass* [ofApp::nPolygons];

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
	
	int moduleWidth = ofGetWidth()/ofApp::nModules;
	int moduleHeight = ofGetHeight();
	int moduleHabitants = ofApp::nPolygons;
	
	ofApp::modules[0] = new Module(0, 0*moduleWidth, 0, moduleWidth, moduleHeight, moduleHabitants, bonangs);
	ofApp::modules[1] = new Module(1, 1*moduleWidth, 0, moduleWidth, moduleHeight, moduleHabitants, genders);
	ofApp::modules[2] = new Module(2, 2*moduleWidth, 0, moduleWidth, moduleHeight, moduleHabitants, gongs);
	ofApp::modules[3] = new Module(3, 3*moduleWidth, 0, moduleWidth, moduleHeight, moduleHabitants, sarons);

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

	// TODO: this block can be really better written
	// commenting everything but the display(i) line makes the code all most work entirely
	for (int i = 0; i < ofApp::nPolygons; i++) {
		int checkVertex = 0;
		for (int j = 0; j < ofApp::nModules; j++) {
			if (ofApp::modules[j]->getNumberOfParticles() > i) {
				checkVertex++;
			}
		}

		if (checkVertex != 0)
			ofApp::polygons[i]->display(i);
	}
	
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

void ofApp::dragEvent(ofDragInfo dragInfo) {
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

