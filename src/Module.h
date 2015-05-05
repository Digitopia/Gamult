#ifndef MODULE_H
#define MODULE_H

#include "ofMain.h"
#include "ModuleConsole.h"
#include "Particle.h"
#include "Panel.h"

class Module {

public:

	Module(int index, float x, float y, float width, float height, int maxPopulation, vector<string> soundPaths);

	void loadSounds();
	void update();
	void addParticle(int life, int x, int y);
	void eraseParticle();
	void draw();
	void drawGrid();
	void drawBorders();
	void drawParticles();
    void playSound(int index);
	
	float getGravity() { return console->getGravity(); }
	float getLoopSpeed() { return console->getLoopSpeed(); }
	bool isFreezed() { return console->isFreezed(); }
	bool isLooping() { return console->isLooping(); }
	int getIndex() { return index; }
	int getWidth() { return width; }
	int getHeight()	{ return height; }
	int getX0()	{ return x0; }
	int getX1()	{ return x1; }
    string getSoundLocation(int index) { return soundPaths[index]; }
	int getNumberOfInstrumentNotes() { return soundPaths.size(); }
	vector<Particle> getParticles() { return particles; }
	Particle* getParticle(int i) { return &particles[i]; }
	bool anyParticles() { return particles.size() > 0; }
	void removeParticle() { particles.erase(particles.end()); }
	int getNumberOfParticles() { return particles.size(); }
	
//	Panel* panel;

private:

	int x0, x1;
	int y;
	int index;
	int width;
	int height;
	int maxPopulation;
	ModuleConsole* console;
    vector<string> soundPaths;
    vector<ofSoundPlayer> sounds;
	vector<Particle> particles;
};

#endif