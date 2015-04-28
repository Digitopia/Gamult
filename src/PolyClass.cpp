#include "PolyClass.h"
#include "ofApp.h"

//TODO: this probably shouldn't be a class but simply part of the draw method of the Module..

PolyClass::PolyClass(int index) {
	this->index = index;
}

void PolyClass::display(int i) {

	ofPushStyle();

	ofNoFill();
	ofSetLineWidth(POLY_WIDTH);
	ofSetColor(POLY_COLOR);

	ofBeginShape();
	ofCurveVertex(0, ofGetHeight()/2);
	ofCurveVertex(0, ofGetHeight()/2);

	for (int j = 0; j < ofApp::nModules; j++) {
		if (ofApp::modules[j]->getParticles().size() > i) {
			Particle *p = new Particle(ofApp::modules[j]->getParticles()[i]);
			ofCurveVertex(p->getX(), p->getY());
		}
	}

	ofCurveVertex(ofGetWidth(), ofGetHeight()/2);
	ofCurveVertex(ofGetWidth(), ofGetHeight()/2);
	ofEndShape();
	ofPopStyle();

}
