#ifndef MODULECONSOLE_H
#define MODULECONSOLE_H

#include "ofMain.h"
#include "Button.h"
#include "Fader.h"

class ModuleConsole {

public:

	ModuleConsole(ofVec2f origin, float width, int module);

	int module;
	float width;
	ofVec2f origin;

	Button* remove;
	Button* loop;
	Button* freeze;

	Fader* gravity;
	Fader* loopSpeed;

	void draw();
	void drawButtons();
	void drawFaders();

private:

};

#endif