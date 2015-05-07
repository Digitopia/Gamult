#ifndef MODULECONSOLE_H
#define MODULECONSOLE_H

#include "ofMain.h"
#include "Button.h"
#include "Fader.h"

class ModuleConsole {

public:

	ModuleConsole(int x, float width, int module, vector<string> iconPaths);
	
	void loadIcons(vector<string> paths);

	void draw();
	void drawIcons();
	
	bool isFreezed();
	bool isLooping();
	float getGravity() { return gravity->getValue(); }
    float getLoopSpeed() { return loopSpeed->getValue(); }
	
	void mousePressed(ofMouseEventArgs& event);

private:

	int module;
	float width;

	int x, y;

	Button* remove;
	Button* loop;
	Button* freeze;

	Fader* gravity;
	Fader* loopSpeed;
	
	vector<ofImage> icons;
	vector<ofRectangle> rects;
	
	ofImage img;

};

#endif
