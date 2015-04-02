#ifndef MODULE_H
#define MODULE_H

#include "ofMain.h"

#include "ModuleConsole.h"
#include "Particle.h"

class Module {

public:

	Module(int index, float x, float y, float width, float height, int population);

	void update();
	void addParticle(int life);
	void manageParticles();
	void eraseParticle();
	void draw();
	void boundingBox();

	bool isFreezed();
	bool isLooping();
	
	float getGravity();
	float getLoopSpeed();
	
	int index;
	int maximumPopulation;
	float sync;
	float modWidth;
	float modHeight;
	float maxWidth;
	float maxHeight;
	vector <Particle> population;
	ofVec2f modOrigin;
	ModuleConsole* console;

	// TODO: these should be multiple particles
	Particle* managedParticle;

private:

};

#endif
