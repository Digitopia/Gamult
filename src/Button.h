#ifndef BUTTON_H
#define BUTTON_H

#include "ofMain.h"

class Button {

public:

	Button(int module, float x, float y, string tag);
    
	void toggle();
	void click();

	void drawToggle();
	void drawClick();

	int module;
	float x;
	float y;
	bool inUse;
	string tag;

	bool state;

	ofTrueTypeFont font;

private:

};


#endif