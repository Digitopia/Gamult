//
//  Particle.cpp
//  gamultOSX
//
//  Created by Oscar Rodrigues on 29/01/15.
//
//

#include "Particle.h"

Particle::Particle(int setModule, int setIndex, float setX, float setY, float setSync, int setLife){
    
    counter = 0;
    sendReport = true;
    myModule = setModule;
    index = setIndex;
    sync = setSync;
    floor = ofGetHeight();//myModules[myModule].maxHeight;
    speed = 0;
// TODO: CHECK    gravity = ofApp::myModules*[myModule].console.gravity.getValue()*0.1+0.1;
// TODO: CHECK    loopGravity = myModules[myModule].console.loopSpeed.getValue()*5;
    diameter = 0;
    center.x = setX;
    center.y = setY;
    initialPos = center.y;
    life = setLife;
}
