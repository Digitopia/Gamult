#include "ofApp.h"
#include "Button.h"

ModuleConsole::ModuleConsole(ofVec2f origin, float width, int module) {
	
	this->width = width;
	this->origin.set(origin);
	this->module = module;
	
	int x = origin.x;
	int y = origin.y;
    
    select.set(x + width*0.9, y + ofApp::consoleHeight * 3/4, width*0.1 - 3, ofApp::consoleHeight*1/4 - 3);

	remove = new Button(BUTTON_CLICK,  module, x+BUTTON_SIZE, y+10, "Remove");
	loop = new Button(BUTTON_TOGGLE, module, x+BUTTON_SIZE, y+30, "Loop");
	freeze = new Button(BUTTON_TOGGLE, module, x+BUTTON_SIZE, y+50, "Freeze");

	x = x + width/2;
	gravity = new Fader(x, y+10+5, 10, 100, "Gravity");
	loopSpeed = new Fader(x, y+50+5, 10, 100, "Loop Speed");
    
    inst1.addListener(this, &ModuleConsole::setInstrument1);
    
    panel.setup("panel", " ", x, ofGetHeight()/2);
    panel.add(inst1.setup("inst1"));
    panel.add(inst2.setup("inst2"));
    
    showPanel = false;
    
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
    
    ofPushStyle();
    
    ofSetColor(190);
    ofFill();
    ofRect(select);
    ofSetColor(70);
    ofNoFill();
    ofRect(select);
    
    ofPopStyle();
    
    if (showPanel) panel.draw();

}

bool ModuleConsole::isFreezed() {
	return freeze->state;
}

bool ModuleConsole::isLooping() {
	return loop->state;
}

void ModuleConsole::setInstrument1() {
    cout << "setting up instrument 1" << endl;
}

void ModuleConsole::setInstrument2() {
    cout << "setting up instrument 2" << endl;
}
