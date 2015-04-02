#include "ofApp.h"

Particle::Particle(int module, int index, float x, float y, float sync, int life) {

	this->module = module;
	this->index = index;
	this->center.x = x;
	this->center.y = y;
	this->sync = sync;
	this->life = life;

	this->counter = 0;
	this->sendReport = true;
	this->floor = ofGetHeight();
	this->speed = 0;

	// TODO: too complicated lines...
	this->gravity = ofApp::myModules[module]->console->gravity->getValue()*0.1+0.1;
	this->loopGravity = ofApp::myModules[module]->console->loopSpeed->getValue()*5;
	
	this->initialPos = center.y;
	
	this->diameter = 0;

}

void Particle::noGravity() {
	center.y += loopGravity;
	if (center.y > floor)loopGravity = (ofApp::myModules[module]->console->loopSpeed->getValue()*5)*-1;
	if (center.y < initialPos) loopGravity = (ofApp::myModules[module]->console->loopSpeed->getValue()*5);
	if (center.y > floor-sync && sendReport == true) {
		report(center.x);
		sendReport = false;
	}
	if (center.y < floor-sync)sendReport = true;
}

void Particle::yesGravity() {

	gravity = ofApp::myModules[module]->console->gravity->getValue()*1;
	speed += gravity;
	center.y += speed;
	if (center.y > floor-sync && sendReport == true) {
		report(center.x);
		sendReport = false;
	}
	if (center.y < floor-sync)sendReport = true;
	if (center.y > floor) {
		speed = speed * -0.95;
		sync = sync * 0.95;
		counter++;
		center.y=floor;
	}
	
	if (center.y <= ofApp::consoleHeight + life)
		speed *= -0.95;
	
}

void Particle::draw() {
	drawSync();
	drawLifeCircle();
	drawParticleProgress();
}

void Particle::drawParticle() {
	ofPushStyle();
	ofFill();
	ofSetColor(0);
	ofPopStyle();
}


void Particle::drawParticleProgress() {
	
	// isto desenha o circulo de dentro
	
	ofPushStyle();
	ofFill();
	ofSetColor(0);
	ofCircle(center.x, center.y, life);
	ofPopStyle();
	
	cout << life << endl;
}

// TODO: finish editing these last few methods

void Particle::drawLifeCircle() {

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


void Particle::drawSync() {
//	ofPushStyle();
//	ofNoFill();
//	ofSetLineWidth(1);
//	ofSetColor(128);
//	ofLine(ofApp::myModules[module]->modOrigin.x, floor-sync, ofApp::myModules[module]->maxWidth, floor-sync);
//	ofPopStyle();
}

void Particle::report(float collision) {
	// OscMessage myOscMessage = new OscMessage("/GML");
	// String messageFormat = myModule+" "+int(collision)+" "+int(myModules[myModule].modOrigin.x)+" "+int(myModules[myModule].maxWidth)+" "+int(life);
	// myOscMessage.add(messageFormat);
	// oscP5.send(myOscMessage, myRemoteLocation);
}
