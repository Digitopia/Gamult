#include "ofApp.h"
#include "Button.h"

ModuleConsole::ModuleConsole(ofVec2f origin, float width, int module) {
	
    this->width = width;
	this->origin.set(origin);
	this->module = module;
    
    int x = origin.x;
    int y = origin.y;

	remove = new Button(BUTTON_CLICK,  module, x+10, y+10, "Remove");
    loop = new Button(BUTTON_TOGGLE, module, x+10, y+30, "Loop");
	freeze = new Button(BUTTON_TOGGLE, module, x+10, y+50, "Freeze");

    x = x + width/2;
	gravity = new Fader(x, y+10+5, 10, 100, "Gravity");
	loopSpeed = new Fader(x, y+50+5, 10, 100, "Loop Speed");
}

void ModuleConsole::draw() {

	ofPushStyle();
	
	// TODO: should be a variable	
	ofSetColor(50, 128);

	ofFill();

	ofRect(origin.x, origin.y, width, ofApp::consoleHeight);

	ofPopStyle();

	remove->draw();
	loop->draw();
	freeze->draw();

	gravity->draw();
	loopSpeed->draw();

}

bool ModuleConsole::isFreezed() {
	return freeze->state;
}

bool ModuleConsole::isLooping() {
	return loop->state;
}