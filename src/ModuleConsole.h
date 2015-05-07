#ifndef MODULECONSOLE_H
#define MODULECONSOLE_H

#include "ofMain.h"
#include "Button.h"
#include "Fader.h"

class ModuleConsole {

public:

	ModuleConsole(int x, float width, int module);

	void draw();
	
	bool isFreezed()   { return btnFreeze->getState(); }
	bool isGravityOn() { return btnGravity->getState(); }
	bool isLooping()   { return !isGravityOn(); }
    float getSpeed()   { return fadSpeed->getValue(); }
	
private:

	int module;
	float width;

	int x, y;

	Button* btnRemove;
	Button* btnGravity;
	Button* btnFreeze;

	Fader* fadSpeed;
	
};

#endif
