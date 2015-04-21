#include "ofApp.h"

Particle::Particle(int module, int index, float x, float y, int life) {

	this->module = module;
	this->index = index;
	this->center.x = x;
	this->center.y = y;
	this->life = life;

	this->counter = 0;
	this->speed = 0;
    this->direction = 1.;

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
        playSound();
	}
		
	else if (center.y <= CONSOLE_HEIGHT + life) {
		center.y = CONSOLE_HEIGHT + life;
	}
	
	if (center.y >= ofGetHeight() || center.y <= CONSOLE_HEIGHT + life) {
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
	ofSetColor(PARTICLE_COLOR);
	ofCircle(center.x, center.y, life);
	
	ofPopStyle();
	
}

void Particle::drawLife() {

	ofPushStyle();

	ofSetColor(PARTICLE_LIFE_COLOR);
	ofSetLineWidth(PARTICLE_WIDTH);
	ofNoFill();
	
	ofPolyline polyline;
	ofPoint pt(center.x, center.y);
	float angleBegin = 0;
	float angleEnd = 360.-(counter*(360./life));
	int radius = life*2;
	polyline.arc(pt, radius, radius, angleBegin, angleEnd, ARC_RESOLUTION);
	polyline.draw();

	ofPopStyle();
}

void Particle::updateGravity() {
	
	if (center.y > ofGetHeight()) {
		direction = -1.;
		report();
        playSound();
	}

	else if (center.y < y0)
		direction = 1.;

	// TODO: magic number
	loopGravity = ofApp::modules[module]->getLoopSpeed()*50. * direction;
}

void Particle::report() {

	ofxOscMessage m;

	// set message address
	int idx = ofApp::modules[module]->getIndex();
	string addr = OSC_ADDRESS + ofToString(idx);
	m.setAddress(addr);

	// segment particle position x to 4 spaces
	float notef = ofMap(center.x, ofApp::modules[module]->getModOriginX(), ofApp::modules[module]->getMaxWidth(), 0, 3);
	int note = floor(notef + 0.5);
	m.addIntArg(note);

	// TODO: this should be the velocity of the MIDI note
	m.addIntArg(100);
	
	ofApp::oscSender.sendMessage(m);
	
}

void Particle::playSound() {
    
    // copied from above
    float notef = ofMap(center.x, ofApp::modules[module]->getModOriginX(), ofApp::modules[module]->getMaxWidth(), 0, 3);
    int note = floor(notef + 0.5);
    
    ofApp::modules[module]->playSound(note);
    
}
