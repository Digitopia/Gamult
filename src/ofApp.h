#ifndef OFAPP_H
#define OFAPP_H

#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#include "Button.h"
#include "Fader.h"
#include "Module.h"
#include "ModuleConsole.h"
#include "PolyClass.h"
#include "Particle.h"

#define HOST "localhost"
#define PORT 5000

class ofApp : public ofBaseApp {

public:

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	
	void displayIncrementation(int realIncrement);

	static float increment;
	static float maxIncrement;
	static int totalModules;
	static int totalPolygons;

	static int consoleHeight;

	static Module** myModules;
	static PolyClass** myPolygons;
	
	bool drawingParticle;
	
	static ofxOscSender oscSender;

private:
	
};

#endif