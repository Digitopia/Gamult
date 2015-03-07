//
//  Particle.cpp
//  gamultOSX
//
//  Created by Oscar Rodrigues on 29/01/15.
//
//

#include "ofApp.h"

Particle::Particle(int setModule, int setIndex, float setX, float setY, float setSync, int setLife){
    
    counter = 0;
    sendReport = true;
    myModule = setModule;
    index = setIndex;
    sync = setSync;
    floor = ofGetHeight();
    speed = 0;
    gravity = ofApp::myModules[myModule]->console->gravity->getValue()*0.1+0.1;
    loopGravity = ofApp::myModules[myModule]->console->loopSpeed->getValue()*5;
    diameter = 0;
    center.x = setX;
    center.y = setY;
    initialPos = center.y;
    life = setLife;
}

void Particle::noGravity() {
    center.y += loopGravity;
   if (center.y > floor)loopGravity = (ofApp::myModules[myModule]->console->loopSpeed->getValue()*5)*-1;
    if(center.y < initialPos) loopGravity = (ofApp::myModules[myModule]->console->loopSpeed->getValue()*5);
    if (center.y > floor-sync && sendReport == true) {
        report(center.x);
        sendReport = false;
    }
    if (center.y < floor-sync)sendReport = true;
}

void Particle::yesGravity() {
    gravity = ofApp::myModules[myModule]->console->gravity->getValue()*1;
    speed+=gravity;
    center.y+=speed;
    if (center.y > floor-sync && sendReport == true) {
        report(center.x);
        sendReport = false;
    }
    if (center.y < floor-sync)sendReport = true;
    if (center.y > floor) {
        speed = speed * -0.95;
        sync = sync * 0.95;
        counter++;
        center.y=floor;
    }
}

void Particle::display() {
    displaySync();
    displayLifeCircle();
    displayParticleProgress();
}

void Particle::displayParticle() {
    ofFill();
    ofSetColor(0);
    ofCircle(center.x, center.y, diameter);
}

void Particle::displayParticleProgress(){
    ofFill();
    ofSetColor(0);
    ofCircle(center.x, center.y, life);
}

// TODO: finish editing these last few methods

void Particle::displayLifeCircle() {
  /*  pushStyle();
    stroke(0, 128);
    strokeWeight(2);
    noFill();
    arc(center.x, center.y, life*2, life*2, 0, radians(360.-(counter*(360./life))));
    popStyle();*/
}


void Particle::displaySync() {
/*    pushStyle();
    noFill();
    strokeWeight(1);
    stroke(128);
    line(myModules[myModule].modOrigin.x, floor-sync, myModules[myModule].maxWidth, floor-sync);
    popStyle();*/
}

void Particle::report(float collision) {
  /*  OscMessage myOscMessage = new OscMessage("/GML");
    String messageFormat = myModule+" "+int(collision)+" "+int(myModules[myModule].modOrigin.x)+" "+int(myModules[myModule].maxWidth)+" "+int(life);
    myOscMessage.add(messageFormat);
    oscP5.send(myOscMessage, myRemoteLocation);
    
}*/
}