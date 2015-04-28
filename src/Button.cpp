#include "ofApp.h"

Button::Button(buttonType type, int module, int size, float x, float y, string title) {

    this->type = type;
	this->module = module;
	this->x = x;
	this->y = y;
	this->title = title;
	this->state = false;
	this->size = size;

	this->rect = ofRectangle(x, y, size, size);

	font.loadFont(UI_FONT_FACE, UI_FONT_SIZE, true);

	ofAddListener(ofEvents().mousePressed, this, &Button::mousePressed);

}

void Button::mousePressed(ofMouseEventArgs& event) {

	if (!rect.inside(event.x, event.y))
		return;

	if (type == BUTTON_TOGGLE) {
	  	state = !state;
	}

	else if (type == BUTTON_CLICK) {
		if (ofApp::modules[module]->anyParticles()) {
			ofApp::modules[module]->removeParticle();
		}
	}

}

void Button::draw() {

	ofPushStyle();
	
	ofSetColor(UI_COLOR);
	
	ofSetLineWidth(BUTTON_WIDTH);
	
	ofNoFill();
	
	if (state)
		ofFill();

	ofRect(rect);
	
	font.drawString(title, x+size*1.5, y+size-3);
	
	if (type == BUTTON_CLICK) {
		ofLine(x, y, x+size, y+size);
		ofLine(x+size, y, x, y+size);
	}

    ofPopStyle();
}
