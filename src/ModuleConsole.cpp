#include "ofApp.h"
#include "Button.h"

ModuleConsole::ModuleConsole(int x, float width, int module) {
	
	this->width = width;
	this->x = x;
	this->y = 0;
	this->module = module;
	
	int y = 0;
	
	remove = new Button(BUTTON_CLICK,  module, BUTTON_SIZE, x+BUTTON_SIZE, y+BUTTON_SIZE,   "Remove");
	loop =   new Button(BUTTON_TOGGLE, module, BUTTON_SIZE, x+BUTTON_SIZE, y+BUTTON_SIZE*3, "Loop");
	freeze = new Button(BUTTON_TOGGLE, module, BUTTON_SIZE, x+BUTTON_SIZE, y+BUTTON_SIZE*5, "Freeze");

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
