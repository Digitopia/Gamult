#include "ofApp.h"

Particle::Particle(int module, int index, float x, float y, int life) {

	this->module = module;
	this->index = index;
	this->center.x = x;
	this->center.y = y;
	this->life = life;

	this->counter = 0;
	this->speed = 0;

	this->gravity = ofApp::myModules[module]->getGravity()*0.1+0.1;
	this->loopGravity = ofApp::myModules[module]->getLoopSpeed()*5;
	
	this->y0 = center.y;

}

void Particle::noGravity() {
	center.y += loopGravity;
	if (center.y > ofGetHeight())loopGravity = (ofApp::myModules[module]->getLoopSpeed()*5)*-1;
	if (center.y < y0) loopGravity = (ofApp::myModules[module]->getLoopSpeed()*5);
	if (center.y > ofGetHeight()) {
		report(center.x);
	}
}

void Particle::yesGravity() {

	gravity = ofApp::myModules[module]->getGravity();
	speed += gravity;
	center.y += speed;
	if (center.y > ofGetHeight()) {
		report(center.x);
	}
	if (center.y > ofGetHeight()) {
		speed = speed * -0.95;
		counter++;
		center.y = ofGetHeight();
	}
	
	if (center.y <= ofApp::consoleHeight + life)
		speed *= -0.95;
	
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

void Particle::report(float collision) {
	// OscMessage myOscMessage = new OscMessage("/GML");
	// String messageFormat = myModule+" "+int(collision)+" "+int(myModules[myModule].modOrigin.x)+" "+int(myModules[myModule].maxWidth)+" "+int(life);
	// myOscMessage.add(messageFormat);
	// oscP5.send(myOscMessage, myRemoteLocation);
}
