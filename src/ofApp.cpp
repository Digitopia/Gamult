#include "ofApp.h"

float ofApp::increment = 0.0f;
float ofApp::maxIncrement = 20.0f;
int ofApp::totalModules = 4;
int ofApp::totalPolygons = 10;
int ofApp::consoleHeight = 90;



Module** ofApp::myModules = new Module* [ofApp::totalModules];

// TODO: see if there's need to make this static
PolyClass** ofApp::myPolygons = new PolyClass* [ofApp::totalPolygons];



//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);

	ofSetCircleResolution(50);

	int moduleWidth = ofGetWidth()/ofApp::totalModules;
	int moduleHeight = ofGetHeight();
	int moduleHabitants = ofApp::totalPolygons;

	for (int i = 0; i < ofApp::totalModules; i++) {
		ofApp::myModules[i] = new Module(i, i*moduleWidth, 0, moduleWidth, moduleHeight, moduleHabitants);
	}

	for (int i = 0; i < ofApp::totalPolygons; i++) {
		ofApp::myPolygons[i] = new PolyClass(i);
	}

}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	
	ofBackground(255);

	for (int i = 0; i < ofApp::totalModules; i++) {

		ofApp::myModules[i]->drawConsole();
		ofApp::myModules[i]->boundingBox();
		ofApp::myModules[i]->manageParticles();
	}

	for (int i = 0; i < ofApp::totalPolygons; i++) {

		int checkVertex = 0;
		for (int j = 0; j<ofApp::totalModules; j++) {
			if (myModules[j]->population.size() > i) {
				checkVertex++;
			}
		}

		if (checkVertex!=0) ofApp::myPolygons[i]->display(i);
	}
	
	if (drawingParticle) {
		increment++;
		if (increment>maxIncrement)increment=maxIncrement;
		if (ofGetMouseY() > ofApp::consoleHeight) displayIncrementation(increment);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	if (y >= ofApp::consoleHeight) {
		drawingParticle = true;
		cout << "true" << endl;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	
	drawingParticle = false;
	
	if (ofGetMousePressed()) {
		increment++;
		if (increment>maxIncrement)increment=maxIncrement;
		if (ofGetMouseY() > ofApp::consoleHeight) displayIncrementation(increment);
	}
	
	

	for (int i=0; i < ofApp::totalModules; i++) {

		ofApp::myModules[i]->addParticle(increment);
		ofApp::myModules[i]->console->remove->click();
		ofApp::myModules[i]->console->loop->toggle();
		ofApp::myModules[i]->console->freeze->toggle();
	}
	increment = 0;

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

void ofApp::displayIncrementation(int realIncrement) {

	ofPolyline polyline1;

	ofPushStyle();
	ofSetColor (0, 128);
	//stroke(0, 128);
	ofSetLineWidth(4);
	//strokeWeight(4);
	ofNoFill();
	ofPoint point1(mouseX, mouseY);
	polyline1.arc(point1, 30, 30, 0, float(increment)*(360./float(maxIncrement)), 50);
	polyline1.draw();
	ofPopStyle();

}