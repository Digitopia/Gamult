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

	font.load(UI_FONT_FACE, UI_FONT_SIZE, true);
    
    this->id = -1;

	ofAddListener(ofEvents().touchDown,  this, &Button::touchDown);
	ofAddListener(ofEvents().touchMoved, this, &Button::touchMoved);
	ofAddListener(ofEvents().touchUp,    this, &Button::touchUp);

}

void Button::touchDown(ofTouchEventArgs& event) {
    
    if (id == -1 && type == BUTTON_TOGGLE && rect.inside(event.x, event.y)) {
		state = !state;
    }

    if (id == -1 && (type == BUTTON_REMOVE || type == BUTTON_CLEAR) && rect.inside(event.x, event.y) && event.y < CONSOLE_HEIGHT*ofGetHeight()) {
        id = event.id;
		state = true;
    }

}

void Button::touchMoved(ofTouchEventArgs &event) {
    if ((type == BUTTON_REMOVE || type == BUTTON_CLEAR) && id == event.id && !rect.inside(event.x, event.y)) {
        state = false;
        id = -1;
    }
}

void Button::touchUp(ofTouchEventArgs& event) {
    if (event.id == id && state && (type == BUTTON_REMOVE || type == BUTTON_CLEAR)) {
        if (ofApp::modules[module]->anyParticles()) {
            if (type == BUTTON_REMOVE) {
                ofApp::modules[module]->removeParticle();
            } else {
                ofApp::modules[module]->removeAllParticles();
            }
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
	
    if (type == BUTTON_TOGGLE) {
        font.drawString(title, x+size*1.5, y+size-3);
		ofDrawRectangle(rect);
    }
	
	if (type == BUTTON_REMOVE || type == BUTTON_CLEAR) {

        ofSetHexColor(BUTTON_REMOVE_COLOR);
        ofDrawRectangle(x, y, ofApp::modules[module]->getWidth()/2, CONSOLE_HEIGHT*ofGetHeight()*0.25);

        ofSetColor(UI_COLOR);
        font.drawString(title, x+ofApp::modules[module]->getWidth()/6, y+((CONSOLE_HEIGHT*ofGetHeight())/6));
        
	}

    ofPopStyle();
}
