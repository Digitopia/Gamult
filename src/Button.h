#ifndef BUTTON_H
#define BUTTON_H

#include "ofMain.h"

enum buttonType {
	BUTTON_TOGGLE,
	BUTTON_CLICK
};

class Button {

public:

	Button(buttonType type, int module, float x, float y, string title);

	void mousePressed(ofMouseEventArgs& event);
	void draw();
    
	// TODO: isto devia tudo ser privado e ser accessivel com metodos get
	int module;

	string title;

	float x;
	float y;

	bool state;

	ofRectangle rect;

	ofTrueTypeFont font;

private:

	static int size;
	buttonType type;

};

#endif
