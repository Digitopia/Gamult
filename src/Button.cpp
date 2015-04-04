#include "ofApp.h"

int	Button::size = 10;

Button::Button(buttonType type, int module, float x, float y, string title) {

    this->type = type;
	this->module = module;
	this->x = x;
	this->y = y;
	this->title = title;
	this->state = false;

	this->rect = ofRectangle(x, y, size, size);

	font.loadFont("verdana.ttf", 10, true);

	ofAddListener(ofEvents().mousePressed, this, &Button::mousePressed);

}

void Button::mousePressed(ofMouseEventArgs& event) {

	if (!rect.inside(event.x, event.y))
		return;

	if (type == BUTTON_TOGGLE) {
	  	state = !state;
	}

	else if (type == BUTTON_CLICK) {
		if (ofApp::myModules[module]->population.size() > 0) {
			ofApp::myModules[module]->population.erase(ofApp::myModules[module]->population.end());
		}
	}

}

void Button::draw() {

	ofPushStyle();
	
	// TODO: color should be a variable
	ofSetColor(255);
	
	ofSetLineWidth(1);
	
	ofNoFill();
	
	if (state)
		ofFill();

	ofRect(rect);
	
	// TODO: é preciso este ofFill?
	ofFill();

	font.drawString(title, x+size*2, y+size);
	
	if (type == BUTTON_CLICK) {
		ofLine(x, y, x+size, y+size);
		ofLine(x+size, y, x, y+size);
	}

    ofPopStyle();
}
