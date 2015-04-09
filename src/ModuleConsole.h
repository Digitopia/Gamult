#ifndef MODULECONSOLE_H
#define MODULECONSOLE_H

#include "ofMain.h"
#include "Button.h"
#include "Fader.h"

#include "ofxGui.h"

class ModuleConsole {

public:

	ModuleConsole(ofVec2f origin, float width, int module);

	void draw();
	
	bool isFreezed();
	bool isLooping();
	float getGravity() { return gravity->getValue(); }
    float getLoopSpeed() { return loopSpeed->getValue(); }
    
    void setInstrument1();
    void setInstrument2();
    bool showPanel;
    ofRectangle select;
	
private:

	int module;
	float width;

	ofVec2f origin;

	Button* remove;
	Button* loop;
	Button* freeze;

	Fader* gravity;
	Fader* loopSpeed;
    

    
    ofxPanel panel;
    
    ofxButton inst1;
    ofxButton inst2;

	
};

#endif
