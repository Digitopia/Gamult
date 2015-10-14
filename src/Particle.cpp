#include "ofApp.h"

Particle::Particle(int module, int index, float x, float y, int life) {
	this->module = module;
	this->index = index;
	this->center.x = x;
	this->center.y = y;
	this->life = life;
	this->health = life;
    this->radius = life;
    this->velocity = 0;
}

void Particle::gravity() {

	float gravity = ofApp::modules[module]->getSpeed();
	
	if (center.y >= ofGetHeight()) {
		center.y = ofGetHeight();
        trigger(true, true);
	}
    
	else if (center.y <= CONSOLE_HEIGHT + life) {
		center.y = CONSOLE_HEIGHT + life;
	}
	
	if (center.y >= ofGetHeight() || center.y <= CONSOLE_HEIGHT + life) {
		velocity = velocity * -0.95;
        health--;
    } else {
        velocity += gravity;
    }
    
    center.y += velocity;
    
}

void Particle::loop() {
    
    float loopCoef = ofApp::modules[module]->getSpeed();

	if (center.y >= ofGetHeight()) {
        trigger(true, true);
		velocity *= -1;
    } else if(center.y <= CONSOLE_HEIGHT + life) {
        center.y = CONSOLE_HEIGHT + life;
        velocity = 0;
        velocity += loopCoef;
    }
    else {
        velocity += loopCoef;
    }
	
    center.y += velocity;
}

void Particle::draw() {
	drawCircle();
	drawLife();
}

void Particle::drawCircle() {

	ofPushStyle();

	ofFill();
	ofSetColor(PARTICLE_COLOR_OUTER);
    ofCircle(center.x, center.y, 1.05*life);
    ofSetHexColor(PARTICLE_COLOR_INNER);
    ofCircle(center.x, center.y, health);
	
	ofPopStyle();
	
}

void Particle::drawLife() {
/*
	ofPushStyle();

	ofSetColor(PARTICLE_LIFE_COLOR);
	ofSetLineWidth(PARTICLE_WIDTH);
	ofNoFill();
	
	ofPolyline polyline;
	ofPoint pt(center.x, center.y);
	float angleBegin = 0;
//	float angleEnd = 360.-(counter*(360./life));
	float angleEnd = 360.-((life-health)*(360./life));
	int radius = life*2;
	polyline.arc(pt, radius, radius, angleBegin, angleEnd, ARC_RESOLUTION);
	polyline.draw();

	ofPopStyle();
    */
}

void Particle::report(int idx, int note, int vel) {
	ofxOscMessage m;
	string addr = OSC_ADDRESS + ofToString(idx);
	m.setAddress(addr);
	m.addIntArg(note);
	m.addIntArg(vel);
	ofApp::oscSender.sendMessage(m);
}

void Particle::trigger(bool play, bool send) {
	
	// segment particle position x to the amount of notes the instrument has
	float notef = ofMap(center.x, ofApp::modules[module]->getX0(), ofApp::modules[module]->getX1(), 0, ofApp::modules[module]->getNumberOfInstrumentNotes());
	int note = floor(notef);
    
    float vol = ofMap(velocity, 5, 60, 0, 1); // TODO: check magic numbers

    int idx = ofApp::modules[module]->getIndex();
    
    if (play) {
        ofApp::modules[module]->playSound(note, vol);
    }

	// TODO: still not sending the velocity properly
    if (send) {
		report(idx, note, (vol*100));
    }
}
