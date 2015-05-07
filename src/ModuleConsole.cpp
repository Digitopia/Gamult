#include "ofApp.h"
#include "Button.h"

ModuleConsole::ModuleConsole(int x, float width, int module) {
	
	this->width = width;
	this->x = x;
	this->y = 0;
	this->module = module;
	
	int y = 0;
    
    int buttonSize = ofGetHeight()*0.018;
	
	btnRemove  = new Button(BUTTON_CLICK,  module, buttonSize, x+buttonSize, y+buttonSize,   "Remove");
	btnGravity = new Button(BUTTON_TOGGLE, module, buttonSize, x+buttonSize, y+buttonSize*3, "Gravity");
	btnFreeze  = new Button(BUTTON_TOGGLE, module, buttonSize, x+buttonSize, y+buttonSize*5, "Freeze");

	// by default gravity is on
	btnGravity->on();

	x = x + width/2;
	fadSpeed = new Fader(x, y+50+5, FADER_SIZE, FADER_RANGE, "Speed");
    
}

void ModuleConsole::draw() {

	ofPushStyle();
	
	ofSetHexColor(CONSOLE_COLOR);

	ofFill();

	ofRect(x, y, width, CONSOLE_HEIGHT);

	ofPopStyle();

	btnRemove->draw();
	btnGravity->draw();
	btnFreeze->draw();

	fadSpeed->draw();

}


