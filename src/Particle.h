//
//  Particle.h
//  gamultOSX
//
//  Created by Oscar Rodrigues on 29/01/15.
//
//

#ifndef gamultOSX_Particle_h
#define gamultOSX_Particle_h

#include "ofMain.h"

class Particle {
    
public:
    
    Particle(int setModule, int setIndex, float setX, float setY, float setSync, int setLife);
    
    bool sendReport;
    int myModule;
    int index;
    float sync;
    float floor;
    float speed;
    float gravity;
    float loopGravity;
    float diameter;
    ofVec2f center;
    float initialPos;
    int counter;
    int life;
    
private:
    
};



#endif
