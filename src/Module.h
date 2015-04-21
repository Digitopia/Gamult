#ifndef MODULE_H
#define MODULE_H

#include "ofMain.h"
#include "ModuleConsole.h"
#include "Particle.h"
#include "Panel.h"

class Module {

public:

	Module(int index, float x, float y, float width, float height, int population, vector<string> soundVector);

	void update();
	void addParticle(int life);
	void manageParticles();
	void eraseParticle();
	void draw();
	void boundingBox();

	bool isFreezed() { return console->isFreezed(); }
	bool isLooping() { return console->isLooping(); }

	float getGravity() { return console->getGravity(); }
	float getLoopSpeed() { return console->getLoopSpeed(); }
	
	int getIndex()		{ return index; }
	int getMaxWidth()	{ return maxWidth; }
	int getMaxHeight()	{ return maxHeight; }
	int getModWidth()	{ return modWidth; }
	int getModHeight()	{ return modHeight; }
	int getModOriginX()	{ return modOrigin.x; }
	int getModOriginY()	{ return modOrigin.y; }
    void playSound(int index);
    
    vector<ofSoundPlayer> sounds;
    
    string getSoundLocation(int index) {return soundVector[index]; }

	// TODO: these should be private too
	vector<Particle> population;
	Particle* managedParticle;
	Panel* panel;
    vector<string> soundVector;

private:

	int index;
	int modWidth;
	int modHeight;
	int maxWidth;
	int maxHeight;
	int maximumPopulation;
	ofVec2f modOrigin;
	ModuleConsole* console;

};

#endif