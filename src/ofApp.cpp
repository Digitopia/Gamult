#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    
    increment = 0.0f;
    maxIncrement = 20.0f;
    totalModules = 4;
    totalPolygons = 10;
    
    int moduleWidth = ofGetWidth()/totalModules;
    int moduleHeight = ofGetHeight();
    int moduleHabitants = totalPolygons;
    
    myModules = new Module*[totalModules];
    
    for (int i = 0; i < totalModules; i++) {
        
        myModules[i] = new Module(i, i*moduleWidth, 0, moduleWidth, moduleHeight, moduleHabitants);
    }


}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

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