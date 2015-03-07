#ifndef MODULE_H
#define MODULE_H

#include "ofMain.h"

#include "ModuleConsole.h"
#include "Particle.h"

class Module {
    
public:
    
    int index;
    int maximumPopulation;
    float sync;
    float modWidth;
    float modHeight;
    float maxWidth;
    float maxHeight;
    

    Module(int setIndex, float setOriginX, float setOriginY, float setWidth, float setHeight, int setPopulation);
    
    ModuleConsole* console;
    Particle* thisParticle;
    
    void update();
    void addParticle(int life);
    void manageParticles();
    void displayConsole();
    void boundingBox();
    void eraseParticle();
    
    vector <Particle> population;
    
    ofVec2f modOrigin;

private:
    
};

#endif