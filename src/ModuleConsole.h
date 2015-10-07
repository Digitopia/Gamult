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
	
	bool isFreezed()   { return btnFreeze->getState(); }
	bool isGravityOn() { return btnGravity->getState(); }
	bool isLooping()   { return !isGravityOn(); }
    float getSpeed()   { return fadSpeed->getValue(); }
    
    void unfreeze() { btnFreeze->off(); }
    void enableGravity() { btnGravity->on(); }
    
    void resetFaderSpeed() { fadSpeed->resetFader(); }
    
    void mousePressed(ofMouseEventArgs& event);

private:

	int module;
	float width;

	int x, y;

	vector<ofImage> icons;
	vector<ofRectangle> rects;
	
	ofImage img;
	
	Button* btnGravity;
	Button* btnFreeze;
    Button* btnRemove;
    Button* btnClear;

	Fader* fadSpeed;
	
};

#endif
