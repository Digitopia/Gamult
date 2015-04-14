#ifndef FADER_H
#define FADER_H

#include "ofMain.h"

class Fader {

public:

	Fader(float x0, float y, int size, int range, string title);
	void draw();
    float getValue();
	void mousePressed(ofMouseEventArgs& event);
	void mouseDragged(ofMouseEventArgs& event);
	void mouseReleased(ofMouseEventArgs& event);

private:
	
	int x0; // the inital position of the fader in the full window
    int y;  // the y position of the fader (is constant all the time)
    ofRectangle rect; // the rectangle of the fader itself
	ofTrueTypeFont font;
	string title;
    int size; // size in pixels of the square of the fader
    int range; // range in pixels of the line in which the fader moves
    bool dragging; // true if the fader is being dragged and false otherwise
    
};

#endif