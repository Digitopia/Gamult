#ifndef PARTICLE_H
#define PARTICLE_H

#include "ofMain.h"
#include "Module.h"

class Particle {

public:

    Particle(Module* module, int index, float x, float y, int life);

    void draw();
    void drawCircle();
    void gravity();
    void loop();
    void update();

    #if defined TARGET_SEMIBREVE
    void report(int idx, int note, int vel);
    #endif

    void trigger(bool play, bool send);

    int getVelocity() { return velocity; }
    int getLife() { return life; }
    int getHealth() { return health; }
    int getX() { return center.x; }
    int getY() { return center.y; }
    void setXY(int x, int y) { center.x = x; center.y = y; }
    void setModule(int module) { this->module = module; }

private:

    Module* module;
    int index;
    int health;
    int life;
    float velocity;
    ofVec2f center;
    int id;
    int radius;

};

#endif
