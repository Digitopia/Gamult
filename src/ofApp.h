#ifndef OFAPP_H
#define OFAPP_H

#include "ofMain.h"
#include "ofxOsc.h"

#include "Button.h"
#include "Fader.h"
#include "Module.h"
#include "ModuleConsole.h"
#include "PolyClass.h"
#include "Particle.h"
#include "Panel.h"

class ofApp : public ofBaseApp {

public:

	// OF stuff
	void setup();
	void update();
	void draw();

	// events
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);

	// custom
	void drawIncrement();

	static int nModules;
	static int nPolygons;
	static int consoleHeight;
	static Module** modules;
	static PolyClass** polygons;
	static ofxOscSender oscSender;

private:
	
	float increment; // current radius of the particle animation before releasing the mouse
	float maxIncrement; // maximum radius allowed for the particle animation before releasing the mouse
	bool drawingParticle;

};

#endif
