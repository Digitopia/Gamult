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
	int counter; // variavel que vai aumentando (ou diminuindo) em funcao da vida da particula
	int life; // tamamho da particulo, diametro, que é sempre igual
	float sync; // TODO!
	
	float diameter;
	
	float floor;

	float speed;
	float gravity;
	float loopGravity;
	float initialPos;
	bool sendReport;
	ofVec2f center;

private:

};

#endif
