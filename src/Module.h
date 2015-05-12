#ifndef MODULE_H
#define MODULE_H

#include "ofMain.h"
#include "ModuleConsole.h"
#include "Particle.h"
#include "Panel.h"

class Module {

public:

	Module(int index, float x, float y, float width, float height, int maxPopulation, vector<string> soundPaths, vector<string> iconPaths);

	void loadSounds(vector<string> paths);
	void loadIcons(vector<string> paths);
	void update();
	void addParticle(int life, int x, int y);
	void eraseParticle();
	void draw();
	void drawGrid();
	void drawBorders();
	void drawParticles();
    void playSound(int index);
	void changeInstrument(int index);
	
	int getX0()	       { return x0; }
	int getX1()	       { return x1; }
	int getIndex()     { return index; }
	int getWidth()     { return width; }
	int getHeight()	   { return height; }
	float getSpeed()   { return console->getSpeed(); }
	bool isFreezed()   { return console->isFreezed(); }
	bool isLooping()   { return console->isLooping(); }
	bool isGravityOn() { return console->isGravityOn(); }
	int getNumberOfInstrumentNotes()   { return sounds.size(); }
	bool anyParticles()                { return particles.size() > 0; }
	void removeParticle()              { particles.erase(particles.end()); }
	int getNumberOfParticles()         { return particles.size(); }
	vector<Particle> getParticles()    { return particles; }
	Particle* getParticle(int i)       { return &particles[i]; }
	
private:

	int x0, x1;
	int y;
	int index;
	int width;
	int height;
	int maxPopulation;
	ModuleConsole* console;
    vector<ofSoundPlayer> sounds;
	vector<Particle> particles;
};

#endif