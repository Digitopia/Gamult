#ifndef BUTTON_H
#define BUTTON_H

#include "ofMain.h"

#define BUTTON_WIDTH 1

enum buttonType {
	BUTTON_TOGGLE,
	BUTTON_CLICK
};

class Button {

public:

	Button(buttonType type, int module, int size, float x, float y, string title);
	void mousePressed(ofMouseEventArgs& event);
	void draw();
	bool getState() { return state; }
    
private:

	buttonType type;
	int module;
	int size;
	float x;
	float y;
	string title;
	ofRectangle rect;
	ofTrueTypeFont font;
	bool state;

};

#endif
