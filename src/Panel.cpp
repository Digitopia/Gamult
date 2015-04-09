#include "ofApp.h"

Panel::Panel(int index, int x, int y, int width) {
    
    this->index = index;
    this->x = x;
    this->y = y;
    this->width = width;
    
    select.set(x + width*0.9, y + ofApp::consoleHeight * 3/4, width*0.1 - 3, ofApp::consoleHeight*1/4 - 3);
    
    
    inst1.addListener(this, &Panel::setInstrument1);
    inst2.addListener(this, &Panel::setInstrument2);
    
    //warning: now writing everything to the same file
    panel.setup("panel", "settings.xml", x + width/2, ofGetHeight()/2);
    panel.add(inst1.setup("inst1"));
    panel.add(inst2.setup("inst2"));
    
    showPanel = false;
    
    tag = "inst1";
    
}

void Panel::draw() {
    
    ofPushStyle();
    
    ofSetColor(190);
    ofFill();
    ofRect(select);
    ofSetColor(70);
    ofNoFill();
    ofRect(select);
    
    ofPopStyle();
    
    if (showPanel) panel.draw();
    
    ofDrawBitmapString(tag, x + width*0.9, y + ofApp::consoleHeight * 3/4+ (ofApp::consoleHeight*1/4)/2);
    
}

void Panel::setInstrument1() {
    tag = "inst1";
    cout << "setting up instrument 1" << endl;
}

void Panel::setInstrument2() {
    
    tag = "inst2";
    cout << "setting up instrument 2" << endl;
}