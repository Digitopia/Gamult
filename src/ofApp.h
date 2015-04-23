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
#define OSC_ADDRESS "/GML/"

#define UI_FONT_FACE "verdana.ttf" // TODO: change this font!
#define UI_FONT_SIZE 10
#define UI_COLOR 255

//#define BUTTON_SIZE 16
#define FADER_SIZE 12
#define FADER_RANGE 150

#define FRAME_RATE 60
#define BACKGROUND_COLOR 255
#define CIRCLE_RESOLUTION 50
#define ARC_RESOLUTION 30
#define MODULES 4
#define PARTICLES_PER_MODULE 10

#define INCREMENT_LINE_WIDTH 3
#define INCREMENT_RADIUS 30
#define INCREMENT_COLOR 128

#define PARTICLE_COLOR 0
#define PARTICLE_LIFE_COLOR 128
#define PARTICLE_LIMIT_COLOR 0xFF3300
#define PARTICLE_WIDTH 2

#define CONSOLE_COLOR 0x989898
#define CONSOLE_BORDER_WIDTH 1
#define CONSOLE_BORDER_COLOR 0x717171
#define CONSOLE_HEIGHT 120

#define GRID_COLOR 240
#define GRID_HEIGHT 20

#define POLY_WIDTH 1

#define LIMIT_PARTICLE 0.2 // percentage of the console height below which it is not possible to add particles

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
	void initModules();

	static int nModules;
	static int nPolygons;
	static Module** modules;
	static PolyClass** polygons;
	static ofxOscSender oscSender;
    static int maxParticleY;
    
private:
	
	float increment;	// current radius of the particle animation before releasing the mouse
	float maxIncrement; // maximum radius allowed for the particle animation before releasing the mouse
	bool drawingParticle;
	

};

#endif
