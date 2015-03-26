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
    ofPushStyle();
    ofFill();
    ofSetColor(0);
    ofCircle(center.x, center.y, diameter);
    ofPopStyle();
}

void Particle::displayParticleProgress(){
    ofPushStyle();
    ofFill();
    ofSetColor(0);
    ofCircle(center.x, center.y, life);
    ofPopStyle();
}

// TODO: finish editing these last few methods

void Particle::displayLifeCircle() {
    ofPolyline polyline;
    
    ofPushStyle();
    ofSetColor(0, 128);
    ofSetLineWidth(2);
    ofNoFill();
    ofPoint point1(center.x, center.y);
    polyline.arc(point1, life*2, life*2, 0, 360.-(counter*(360./life)), 50);
    polyline.draw();
    ofPopStyle();
}


void Particle::displaySync() {
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(1);
    ofSetColor(128);
    ofLine(ofApp::myModules[myModule]->modOrigin.x, floor-sync, ofApp::myModules[myModule]->maxWidth, floor-sync);
    ofPopStyle();
}

void Particle::report(float collision) {
  /*  OscMessage myOscMessage = new OscMessage("/GML");
    String messageFormat = myModule+" "+int(collision)+" "+int(myModules[myModule].modOrigin.x)+" "+int(myModules[myModule].maxWidth)+" "+int(life);
    myOscMessage.add(messageFormat);
    oscP5.send(myOscMessage, myRemoteLocation);
    
}*/
}