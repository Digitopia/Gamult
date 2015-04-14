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

//
// CONSTANTS
//

#define HOST "localhost"
#define PORT 5000

#define UI_FONT_FACE "verdana.ttf" // TODO: change this font!
#define UI_FONT_SIZE 10
#define UI_COLOR 255

#define BUTTON_SIZE 10
#define FADER_SIZE 10
#define FADER_RANGE 100

#define FRAME_RATE 60
#define BACKGROUND_COLOR 255
#define CIRCLE_RESOLUTION 50

#define INCREMENT_LINE_WIDTH 3
#define INCREMENT_RADIUS 30
#define INCREMENT_COLOR 128
#define INCREMENT_RESOLUTION 30

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
