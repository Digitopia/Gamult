#include "PolyClass.h"
#include "ofApp.h"

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
		if (ofApp::modules[j]->population.size() > i) {
			// TODO: check if following is correct
			Particle thisParticle = *new Particle(/*(Particle)*/ofApp::modules[j]->population[i]);
			ofCurveVertex(thisParticle.getX(), thisParticle.getY());
		}
	}

	ofCurveVertex(ofGetWidth(), ofGetHeight()/2);
	ofCurveVertex(ofGetWidth(), ofGetHeight()/2);
	ofEndShape();
	ofPopStyle();

}
