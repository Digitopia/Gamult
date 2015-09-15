#ifndef PARTICLE_H
#define PARTICLE_H

#include "ofMain.h"

class Particle {

public:

	Particle(int module, int index, float x, float y, int life);

	void draw();
	void drawCircle();
	void drawLife();
	void gravity();
    void loop();
	void update();
	
	void report(int idx, int note, int vel);
    void trigger(bool play, bool send);
    
	int getVelocity() { return velocity; }
	int getLife() { return life; }
	int getHealth() { return health; }
	int getX() { return center.x; }
	int getY() { return center.y; }

private:

	int module;
	int index;
	int health;
	int life;
	float velocity;
	ofVec2f center;
    int id;
    int radius;

};

#endif
