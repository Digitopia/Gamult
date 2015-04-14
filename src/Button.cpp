#include "ofApp.h"

Button::Button(buttonType type, int module, float x, float y, string title) {

    this->type = type;
	this->module = module;
	this->x = x;
	this->y = y;
	this->title = title;
	this->state = false;

	this->rect = ofRectangle(x, y, BUTTON_SIZE, BUTTON_SIZE);

	font.loadFont(FONT_FACE, FONT_SIZE, true);

	ofAddListener(ofEvents().mousePressed, this, &Button::mousePressed);

}

void Button::mousePressed(ofMouseEventArgs& event) {

	if (!rect.inside(event.x, event.y))
		return;

	if (type == BUTTON_TOGGLE) {
	  	state = !state;
	}

	else if (type == BUTTON_CLICK) {
		if (ofApp::modules[module]->population.size() > 0) {
			ofApp::modules[module]->population.erase(ofApp::modules[module]->population.end());
		}
	}

}

void Button::draw() {

	ofPushStyle();
	
	// TODO: color should be a variable
	ofSetColor(BUTTON_COLOR);
	
	ofSetLineWidth(1);
	
	ofNoFill();
	
	if (state)
		ofFill();

	ofRect(rect);
	
	// TODO: é preciso este ofFill?
	ofFill();

	font.drawString(title, x+BUTTON_SIZE*2, y+BUTTON_SIZE);
	
	if (type == BUTTON_CLICK) {
		ofLine(x, y, x+BUTTON_SIZE, y+BUTTON_SIZE);
		ofLine(x+BUTTON_SIZE, y, x, y+BUTTON_SIZE);
	}

    ofPopStyle();
}
