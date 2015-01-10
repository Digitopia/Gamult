#include "PolyClass.h"
#include "ofApp.h"

PolyClass::PolyClass(int setIndex)
{
    index = setIndex;
    
}

void PolyClass::addVertex(float vX, float vY) {
    ofVec2f newVertex;
    newVertex.set(vX, vY);
    addedVertexes.push_back(newVertex);
}

void PolyClass::display(int i) {
    
    ofPushStyle();
    ofBeginShape();
    ofNoFill();
    ofSetLineWidth(1); //TODO: check if works
    ofSetColor(0,128);
    ofCurveVertex(0, ofGetHeight()/2);
    ofCurveVertex(0, ofGetHeight()/2);
 /*   for (int j=0; j<= ofApp::myModules.size() - 1; j++) {
        if (ofApp::myModules[j].population.size()-1>=i) {
            Particle thisParticle = (Particle)myModules[j].population.get(i);
            curveVertex(thisParticle.center.x, thisParticle.center.y);
        }
    }
  */
    ofCurveVertex(ofGetWidth(), ofGetHeight()/2);
    ofCurveVertex(ofGetWidth(), ofGetHeight()/2);
    ofEndShape();
    ofPopStyle();
  
}