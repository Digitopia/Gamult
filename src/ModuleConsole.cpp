#include "ofApp.h"

ModuleConsole::ModuleConsole(ofVec2f origin, float width, int module) {
	
    this->width = width;
	this->origin.set(origin);
	this->module = module;
    
    int x = origin.x;
    int y = origin.y;

	remove = new Button(module, x+10, y+10, "Remove");
    loop   = new Button(module, x+10, y+30, "Loop");
	freeze = new Button(module, x+10, y+50, "Freeze");

    x = x + width/2;
	gravity   = new Fader(x, y+10+5, 10, 100, "Gravity");
	loopSpeed = new Fader(x, y+50+5, 10, 100, "Loop Speed");
}

void ModuleConsole::draw() {

	ofPushStyle();
	ofSetColor(50, 128);
	ofFill();
	// changing original code to include a variable defined in setup to indicate console height
	ofRect(origin.x, origin.y, width, ofApp::consoleHeight);
	drawButtons();
	drawFaders();
	ofPopStyle();

}

void ModuleConsole::drawButtons() {
	remove->drawClick();
	loop->drawToggle();
	freeze->drawToggle();
}

void ModuleConsole::drawFaders() {
	gravity->draw();
	loopSpeed->draw();
}
