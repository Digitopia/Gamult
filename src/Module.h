#ifndef MODULE_H
#define MODULE_H

#include "ofMain.h"
#include "ModuleConsole.h"
#include "Particle.h"

#if defined TARGET_OF_IOS
#include "Wrapper.h"
#endif

//class ModuleConsole;

class Module {

public:

    Module(int index, int x, int y, int width, int height, int maxPopulation, vector<string> soundPaths);

    void loadSounds();
    void unloadSounds();
    void update();
    void addParticle(uint life, uint x, uint y);
    void eraseParticle();

    void draw();
    void drawBackground();
    void drawGrid();
    void drawBorders();
    void drawParticles();
    void drawChangeInstrumentButtons();

    void playSound(int index, float vol);
    void changeInstrument(int direction);
    void setDimensions(int x, int y, int width, int height);
    void updateParticlesOnOrientationChange();

    void touchDown(ofTouchEventArgs& event);

    void removeParticle()            { particles.erase(particles.end() - 1); }
    void removeAllParticles()        { particles.clear(); }
    void removeParticle(int i)       { particles.erase(particles.end() + i); }
    void setParticles(vector<Particle> particles) { this->particles = particles; }

    uint getX0()                      { return x0; }
    uint getX1()                      { return x1; }
    uint getWidth()                   { return width; }
    int getHeight()                   { return height; }
    float getSpeed()                  { return console->getSpeed(); }
    bool isFreezed()                  { return console->isFreezed(); }
    bool isLooping()                  { return console->isLooping(); }
    bool isGravityOn()                { return console->isGravityOn(); }
    uint getNumberOfInstrumentNotes() { return numberOfInstruments; }
    bool anyParticles()               { return particles.size() > 0; }
    uint getNumberOfParticles()       { return particles.size(); }
    vector<Particle> getParticles()   { return particles; }
    Particle* getParticle(int i)      { return &particles[i]; }
    Particle getParticleObj(int i)    { return particles[i]; }
    vector<string> getSoundPaths()    { return soundPaths; }
    bool isFull()                     { return particles.size() == maxPopulation; }
    bool isNotFull()                  { return !isFull();}
    void activate()                   { this->active = true; }
    void deactivate()                 { this->active = false; }
    bool isActive()                   { return this->active; }

    void resetFaderSpeed()            { console->resetFaderSpeed(); }
    void unfreeze()                   { console->unfreeze(); }
    void enableGravity()              { console->enableGravity(); }

    void addParticle(Particle p)      { this->particles.push_back(p); }

    void makeMostRecent()   { this->mostRecent = true;  }
    void unmakeMostRecent() { this->mostRecent = false; }

private:

    uint x0, x1;
    uint y;
    uint index;
    uint iSoundPaths;
    uint width;
    uint height;
    uint consoleHeight;
    uint maxPopulation;
    uint numberOfInstruments;
    ModuleConsole* console;
    vector<Particle> particles;

    #if defined TARGET_OF_IOS
    Wrapper sounds;
    #else
    vector<ofSoundPlayer> sounds; // NOTE: Leave it here, just so that it compiles...
    #endif

    bool active;     // is it being drawn and updated
    bool mostRecent; // is it the module with which the user interacted most recently

    vector<string> soundPaths;
    int backgroundColor;

};

#endif
