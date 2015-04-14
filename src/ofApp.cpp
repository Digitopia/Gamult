#include "ofApp.h"

#define HOST "localhost"
#define PORT 5000
#define FRAME_RATE 60
#define CIRCLE_RESOLUTION 50
#define BACKGROUND_COLOR 255

int ofApp::nModules = 4;
int ofApp::nPolygons = 10;
int ofApp::consoleHeight = 90;

ofxOscSender ofApp::oscSender;

Module** ofApp::modules = new Module* [ofApp::nModules];

// TODO: see if there's need to make this static
PolyClass** ofApp::polygons = new PolyClass* [ofApp::nPolygons];

void ofApp::setup() {
	
	ofSetFrameRate(FRAME_RATE);
	ofSetCircleResolution(CIRCLE_RESOLUTION);
	
	oscSender.setup(HOST, PORT);

	int moduleWidth = ofGetWidth()/ofApp::nModules;
	int moduleHeight = ofGetHeight();
	int moduleHabitants = ofApp::nPolygons;
	
	for (int i = 0; i < ofApp::nModules; i++)
		ofApp::modules[i] = new Module(i, i*moduleWidth, 0, moduleWidth, moduleHeight, moduleHabitants);

	for (int i = 0; i < ofApp::nPolygons; i++)
		ofApp::polygons[i] = new PolyClass(i);

	drawingParticle = false;
	increment = 0.0;
	maxIncrement = 20.0;

}

void ofApp::update() {
	
}

void ofApp::draw() {
	
	ofBackground(BACKGROUND_COLOR);

	for (int i = 0; i < ofApp::nModules; i++) {
		ofApp::modules[i]->draw();
		ofApp::modules[i]->boundingBox();
		ofApp::modules[i]->manageParticles();
	}

	// TODO: this block can be really better written
	// commenting everything but the display(i) line makes the code all most work entirely
	for (int i = 0; i < ofApp::nPolygons; i++) {
		int checkVertex = 0;
		for (int j = 0; j < ofApp::nModules; j++) {
			if (ofApp::modules[j]->population.size() > i) {
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
		
		if (ofGetMouseY() > ofApp::consoleHeight)
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

	if (y >= ofApp::consoleHeight) {
		drawingParticle = true;
	}
    
    for (int i = 0; i < ofApp::nModules; i++) {
        if(ofApp::modules[i]->panel->select.inside(x, y)) {
            ofApp::modules[i]->panel->showPanel = !ofApp::modules[i]->panel->showPanel;
        }
    }
}

void ofApp::mouseReleased(int x, int y, int button) {
	
	drawingParticle = false;
	
	for (int i = 0; i < ofApp::nModules; i++)
		ofApp::modules[i]->addParticle(increment);
	
	increment = 0;

}

void ofApp::windowResized(int w, int h) {
}

void ofApp::dragEvent(ofDragInfo dragInfo) {
}

// draws the increase of the particle after mouse press and before mouse release
// TODO: deveria estar em Particle.cpp
// através de registar um mouse** event
void ofApp::drawIncrement() {

	ofPushStyle();

	ofSetColor(0, 128);
	ofSetLineWidth(4);
	ofNoFill();
	
	ofPolyline polyline;
	ofPoint pt(mouseX, mouseY);
	polyline.arc(pt, 30, 30, 0, float(increment)*(360./float(maxIncrement)), 50);
	polyline.draw();

	ofPopStyle();

}