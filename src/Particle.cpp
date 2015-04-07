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

	this->gravity = ofApp::myModules[module]->getGravity()*0.1+0.1;
	this->loopGravity = ofApp::myModules[module]->getLoopSpeed()*10.;
	
	this->y0 = center.y;

}

void Particle::noGravity() {
	center.y += loopGravity;
}

void Particle::yesGravity() {

	gravity = ofApp::myModules[module]->getGravity();
	speed += gravity;
	
	if (center.y >= ofGetHeight()) {
		center.y = ofGetHeight();
		report(center.x);
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
        report(center.x);
    }
    loopGravity = ofApp::myModules[module]->getLoopSpeed()*10. * direction;
}

void Particle::report(int collision) {
//	ofxOscMessage m;
//	m.setAddress( "/mouse/button" );
//	m.addStringArg( "up" );
//	sender.sendMessage( m );
//	
//	ofxOscMessage m;
//	m.setAddress( "/mouse/position" );
//	m.addIntArg( touch.x );
//	m.addIntArg( touch.y );
//	sender.sendMessage( m );
//	
//	 OscMessage myOscMessage = new OscMessage("/GML");
//	 String messageFormat = myModule+" "+int(collision)+" "+int(myModules[myModule].modOrigin.x)+" "+int(myModules[myModule].maxWidth)+" "+int(life);
//	 myOscMessage.add(messageFormat);
//	 oscP5.send(myOscMessage, myRemoteLocation);
}
