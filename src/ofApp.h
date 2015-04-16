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

// osc
#define HOST "localhost"
#define PORT 5000
#define OSC_ADDRESS "/GML/"

// user interface
#define UI_FONT_FACE "verdana.ttf" // TODO: change this font!
#define UI_FONT_SIZE 10
#define UI_COLOR 255

// buttons and faders
#define BUTTON_SIZE 10
#define FADER_SIZE 10
#define FADER_RANGE 100

// general conf stuff
#define FRAME_RATE 60
#define BACKGROUND_COLOR 255
#define CIRCLE_RESOLUTION 50
#define ARC_RESOLUTION 30

// increment
#define INCREMENT_LINE_WIDTH 3
#define INCREMENT_RADIUS 30
#define INCREMENT_COLOR 128

// particle
#define PARTICLE_COLOR 0
#define PARTICLE_LIFE_COLOR 128
#define PARTICLE_WIDTH 2

// console
#define CONSOLE_COLOR 0x989898
#define CONSOLE_BORDER 1

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
    void initSounds();

	static int nModules;
	static int nPolygons;
	static int consoleHeight;
	static Module** modules;
	static PolyClass** polygons;
	static ofxOscSender oscSender;
    
    vector <string> bonang;
    vector <string> gender;
    vector <string> gong;
    vector <string> saron;
    
    vector <vector<string> > instruments;
    

private:
	
	float increment;	// current radius of the particle animation before releasing the mouse
	float maxIncrement; // maximum radius allowed for the particle animation before releasing the mouse
	bool drawingParticle;

};

#endif
