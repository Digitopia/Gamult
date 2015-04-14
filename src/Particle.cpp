#include "ofApp.h"

#include <stdio.h>
#include <stdlib.h>

Particle::Particle(int module, int index, float x, float y, int life) {

	this->module = module;
	this->index = index;
	this->center.x = x;
	this->center.y = y;
	this->life = life;

	this->counter = 0;
	this->speed = 0;
    this->direction = 1.;

	this->gravity = ofApp::modules[module]->getGravity()*0.1+0.1;
	this->loopGravity = ofApp::modules[module]->getLoopSpeed()*10.;
	
	this->y0 = center.y;

}

void Particle::noGravity() {
	center.y += loopGravity;
}

void Particle::yesGravity() {

	gravity = ofApp::modules[module]->getGravity();
	speed += gravity;
	
	if (center.y >= ofGetHeight()) {
		center.y = ofGetHeight();
		report();
	}
		
	else if (center.y <= ofApp::consoleHeight + life) {
		center.y = ofApp::consoleHeight + life;
	}
	
	if (center.y >= ofGetHeight() || center.y <= ofApp::consoleHeight + life) {
		speed = speed * -0.95;
		counter++;
	}
	
	center.y += speed;
}

void Particle::draw() {
	drawCircle();
	drawLife();
}

void Particle::drawCircle() {
	ofPushStyle();
	ofFill();
	ofSetColor(0);
	ofCircle(center.x, center.y, life);
	ofPopStyle();
}

void Particle::drawLife() {

	ofPolyline polyline;

	ofPushStyle();

	// TODO: should be a variable
	ofSetColor(0, 128);
	ofSetLineWidth(2);
	ofNoFill();
	
	ofPoint point1(center.x, center.y);
	polyline.arc(point1, life*2, life*2, 0, 360.-(counter*(360./life)), 50);
	polyline.draw();

	ofPopStyle();
}

void Particle::updateGravity() {
    if (center.y > ofGetHeight()) direction = -1.;
    if (center.y < y0) direction = 1.;
    if (center.y > ofGetHeight()) {
        report();
    }
    loopGravity = ofApp::modules[module]->getLoopSpeed()*50. * direction;
}

void Particle::report() {

	ofxOscMessage m;
	
	int idx = ofApp::modules[module]->index;
	string addr = "/GML/" + ofToString(idx);
	
	m.setAddress(addr);
	
	float notef = ofMap(center.x, ofApp::modules[module]->modOrigin.x, ofApp::modules[module]->maxWidth, 0, 3);
	int note = floor(notef + 0.5);
	m.addIntArg(note);
	
	m.addIntArg(100);
	
	ofApp::oscSender.sendMessage(m);
	
}
