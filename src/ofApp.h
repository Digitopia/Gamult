#ifndef OFAPP_H
#define OFAPP_H

#pragma once

#include "ofMain.h"

#include "Button.h"
#include "Fader.h"
#include "Module.h"
#include "ModuleConsole.h"
#include "PolyClass.h"


class ofApp : public ofBaseApp{
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
		void gotMessage(ofMessage msg);
    
        float increment;
        float maxIncrement;
        int totalModules;
        int totalPolygons;
    
        Module** myModules;
        PolyClass** myPoligons;
    
    
    
    private:
    


};

#endif