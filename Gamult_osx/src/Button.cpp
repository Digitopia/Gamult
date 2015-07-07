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
    
    this->id = -1;

	ofAddListener(ofEvents().touchDown,  this, &Button::touchDown);
	ofAddListener(ofEvents().touchMoved, this, &Button::touchMoved);
	ofAddListener(ofEvents().touchUp,    this, &Button::touchUp);

}

void Button::touchDown(ofTouchEventArgs& event) {
    
    if (id == -1 && type == BUTTON_TOGGLE && rect.inside(event.x, event.y)) {
		state = !state;
    }

    if (id == -1 && type == BUTTON_CLICK && ofDist(event.x, event.y, x+size/2, y+size/2) <= size/2) {
        id = event.id;
		state = true;
    }

}

void Button::touchMoved(ofTouchEventArgs &event) {
    if (type == BUTTON_CLICK && id == event.id && !rect.inside(event.x, event.y)) {
        state = false;
        id = -1;
    }
}

void Button::touchUp(ofTouchEventArgs& event) {
    if (event.id == id && state && type == BUTTON_CLICK) {
        if (ofApp::modules[module]->anyParticles()) {
            ofApp::modules[module]->removeParticle();
        }
        state = false;
        id = -1;
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
