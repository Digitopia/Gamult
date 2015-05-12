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
	ofAddListener(ofEvents().mouseReleased, this, &Button::mouseReleased);

}

void Button::mousePressed(ofMouseEventArgs& event) {
	
	if (type == BUTTON_TOGGLE && rect.inside(event.x, event.y))
		state = !state;

	if (type == BUTTON_CLICK && ofDist(event.x, event.y, x+size/2, y+size/2) <= size/2)
		state = true;

}

void Button::mouseReleased(ofMouseEventArgs& event) {
	if (state && type == BUTTON_CLICK) {
		if (ofDist(event.x, event.y, x+size/2, y+size/2) <= size/2) {
    		if (ofApp::modules[module]->anyParticles()) {
    			ofApp::modules[module]->removeParticle();
			}
		}
		state = false;
	}
}

void Button::draw() {
	
	ofPushStyle();
	
	ofSetColor(UI_COLOR);
	
	ofSetLineWidth(BUTTON_WIDTH);
	
	ofNoFill();
	
	if (state)
		ofFill();

	font.drawString(title, x+size*1.5, y+size-3);
	
	if (type == BUTTON_TOGGLE)
		ofRect(rect);
	
	if (type == BUTTON_CLICK) {
		ofCircle(x+size/2, y+size/2, size/2+1);
//		ofLine(x, y, x+size, y+size);
//		ofLine(x+size, y, x, y+size);
	}

    ofPopStyle();
}
