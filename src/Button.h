#ifndef BUTTON_H
#define BUTTON_H

#include "ofMain.h"

#define BUTTON_SIZE 10
#define FONT_SIZE 10
#define FONT_FACE "verdana.ttf"
#define BUTTON_COLOR 255

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

	buttonType type;

};

#endif
