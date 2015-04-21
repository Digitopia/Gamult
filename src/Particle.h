#ifndef PARTICLE_H
#define PARTICLE_H

#include "ofMain.h"

class Particle {

public:

	Particle(int module, int index, float x, float y, int life);

	void draw();
	void drawCircle();
	void drawLife();
	void noGravity();
	void yesGravity();
    void updateGravity();
	
	void report(int  idx, int note, int vel);
    void playSound(bool send);
    
	int getSpeed() { return speed; }
	int getGravity() { return gravity; }
	int getLife() { return life; }
	int getCounter() { return counter; }
	int getX() { return center.x; }
	int getY() { return center.y; }

private:

	int module;
	int index;
	int counter; //	increases (or decreases) in function of the life of the particle
	int life;	 // radius of the particle, that is always the same
	float speed;
	float gravity;
    float direction;
	float loopGravity;
	float y0;
	ofVec2f center;

};

#endif
