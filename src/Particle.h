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
	
	void report(float collision);
	
	int getSpeed() { return speed; }
	int getGravity() { return gravity; }
	int getLife() { return life; }
	int getCounter() { return counter; }
	int getX() { return center.x; }
	int getY() { return center.y; }

private:

	int module;
	int index;
	int counter; // variavel que vai aumentando (ou diminuindo) em funcao da vida da particula
	int life;	// tamanho da particula, diametro, que é sempre igual
	float speed;
	float gravity;
	float loopGravity;
	float y0;
	ofVec2f center;

};

#endif
