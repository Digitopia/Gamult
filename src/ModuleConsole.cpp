#include "ofApp.h"
#include "Button.h"

ModuleConsole::ModuleConsole(int x, float width, int module) {
	
	this->width = width;
	this->x = x;
	this->y = 0;
	this->module = module;
	
	int y = 0;
    
    int buttonSize = ofGetHeight()*0.018;
	
	remove = new Button(BUTTON_CLICK,  module, buttonSize, x+buttonSize, y+buttonSize,   "Remove");
	loop =   new Button(BUTTON_TOGGLE, module, buttonSize, x+buttonSize, y+buttonSize*3, "Loop");
	freeze = new Button(BUTTON_TOGGLE, module, buttonSize, x+buttonSize, y+buttonSize*5, "Freeze");

	x = x + width/2;
	gravity   = new Fader(x, y+10+5, FADER_SIZE, FADER_RANGE, "Gravity");
	loopSpeed = new Fader(x, y+50+5, FADER_SIZE, FADER_RANGE, "Loop Speed");
    
}

void ModuleConsole::draw() {

	ofPushStyle();
	
	// TODO: should be a variable	
	ofSetColor(50, 128);

	ofFill();

	ofRect(x, y, width, CONSOLE_HEIGHT);

	ofPopStyle();

	remove->draw();
	loop->draw();
	freeze->draw();

	gravity->draw();
	loopSpeed->draw();

}

bool ModuleConsole::isFreezed() {
	return freeze->getState();
}

bool ModuleConsole::isLooping() {
	return loop->getState();
}
