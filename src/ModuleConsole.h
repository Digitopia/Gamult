#ifndef MODULECONSOLE_H
#define MODULECONSOLE_H

#include "ofMain.h"
#include "Button.h"
#include "Fader.h"

class ModuleConsole {

public:

	ModuleConsole(ofVec2f origin, float width, int module);

	void draw();
	
	bool isFreezed();
	bool isLooping();
	float getGravity() { return gravity->getValue(); }
	float getLoopSpeed() { return loopSpeed->getValue(); }
	
private:

	int module;
	float width;

	ofVec2f origin;

	Button* remove;
	Button* loop;
	Button* freeze;

	Fader* gravity;
	Fader* loopSpeed;
	
};

#endif
