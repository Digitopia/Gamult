#ifndef PARTICLE_H
#define PARTICLE_H

#include "ofMain.h"

class Particle {

public:

	Particle(int module, int index, float x, float y, float sync, int life);

	void noGravity();
	void yesGravity();
	void draw();
	void drawParticle();
	void drawParticleProgress();
	void drawLifeCircle();
	void drawSync();
	void report(float collision);

	// TODO: this should be private members
	int module;
	int index;
	int counter;
	int life;
	float sync;
	float floor;
	float speed;
	float gravity;
	float loopGravity;
	float diameter;
	float initialPos;
	bool sendReport;
	ofVec2f center;

private:

};

#endif
