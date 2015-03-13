#include "ofApp.h"

float ofApp::increment = 0.0f;
float ofApp::maxIncrement = 20.0f;
int ofApp::totalModules = 4;
int ofApp::totalPolygons = 10;



Module** ofApp::myModules = new Module*[ofApp::totalModules];



//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    
    int moduleWidth = ofGetWidth()/ofApp::totalModules;
    int moduleHeight = ofGetHeight();
    int moduleHabitants = ofApp::totalPolygons;
    
    for (int i = 0; i < ofApp::totalModules; i++) {
        myModules[i] = new Module(i, i*moduleWidth, 0, moduleWidth, moduleHeight, moduleHabitants);
    }

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(128);
    
    for (int i=0; i< ofApp::totalModules; i++) {
        
        myModules[i]->displayConsole();
        myModules[i]->boundingBox();
        myModules[i]->manageParticles();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}