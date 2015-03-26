#ifndef MODULECONSOLE_H
#define MODULECONSOLE_H

#include "ofMain.h"
#include "Button.h"
#include "Fader.h"

class ModuleConsole {

public:

	ModuleConsole(ofVec2f setOrigin, float setConsoleWidth, int setModule);

	int module;
	float consoleWidth;
	ofVec2f origin;

	Button* remove;
	Button* loop;
	Button* freeze;

	Fader* gravity;
	Fader* loopSpeed;

	void display();

	void displayButtons();

	void displayFaders();

private:

};

#endif