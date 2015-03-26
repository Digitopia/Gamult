#include "ofApp.h"

Button::Button(int setModule, float setX, float setY, string setTag) {

	inUse = false;
	state = true;

	module = setModule;
	x = setX;
	y = setY;
	tag = setTag;

	verdana10.loadFont("verdana.ttf", 10, true, true);

}

void Button::toggle() {
	// TODO: mudar para box
	if (ofGetMouseX() >= x && ofGetMouseX() <=x+10 && ofGetMouseY() >= y && ofGetMouseY() <=y+10) {
		state = !state;
	}
}

void Button::click() {
	if (ofApp::myModules[module]->population.size() > 0) {
		if (ofGetMouseX() >= x && ofGetMouseX() <=x+10 && ofGetMouseY() >= y && ofGetMouseY() <=y+10) {

			ofApp::myModules[module]->population.erase(ofApp::myModules[module]->population.end());

		}
	}

}

void Button::keySupport() {

	state = !state;

}

void Button::displayToggle() {

	ofPushStyle();
	ofSetLineWidth(1);
	ofSetColor(255);
	ofNoFill();
	if (state == false) ofFill();
	ofRect(x,y,10,10);
	ofSetColor(255);
	ofFill();
	verdana10.drawString(tag, x+20, y+10);
	ofPopStyle();

}

void Button::displayClick() {

	ofPushStyle();
	ofSetLineWidth(1);
	ofSetColor(255);
	ofNoFill();
	if (state == false) ofFill();
	ofRect(x,y,10,10);
	ofLine(x, y, x+10, y+10);
	ofLine(x+10, y, x, y+10);
	ofSetColor(255);
	ofFill();
	verdana10.drawString(tag, x+20, y+10);
	ofPopStyle();

}
