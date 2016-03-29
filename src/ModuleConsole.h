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
    
    void unfreeze() { btnFreeze->off(); }
    void enableGravity() { btnGravity->on(); }
    
    void resetFaderSpeed() { fadSpeed->resetFader(); }
    
    void mousePressed(ofMouseEventArgs& event);
    
    void setDimensions(int x, int width, bool first);

private:

	int module;
	float width;
    float height;

	int x, y;

	vector<ofRectangle> rects;
	
	ofImage img;
	
	Button* btnGravity;
	Button* btnFreeze;
    Button* btnRemove;
    Button* btnClear;

	Fader* fadSpeed;
	
};

#endif
