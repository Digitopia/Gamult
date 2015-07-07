#include "Touch.h"

Touch::Touch(int x, int y) {
    this->x = x;
    this->y = y;
    this->increment = 0.0;
}

void Touch::update() {

    // TODO: this should be different
    increment++; 
    
    if (increment > TOUCH_MAX) {
        increment = TOUCH_MAX;
    }

}

void Touch::draw() {
    
    ofPushStyle();
    
    ofSetColor(TOUCH_COLOR);
    
    if (y > ofApp::maxParticleY || y < CONSOLE_HEIGHT) {
        ofSetHexColor(PARTICLE_LIMIT_COLOR);
    }
    
    ofSetLineWidth(TOUCH_LINE_WIDTH);
    ofNoFill();
    
    ofPolyline polyline;
    ofPoint pt(x, y);
    float angleBegin = 180;
    float angleEnd = float(increment)*(360./float(TOUCH_MAX)) - 180;
    polyline.arc(pt, TOUCH_RADIUS, TOUCH_RADIUS, angleBegin, angleEnd, ARC_RESOLUTION);
    polyline.draw();
    
    // TODO: check these magic numbers when rebuilding code according to screen size
    ofFill();
    ofEllipse(x, y, increment*3, increment*3);
    
    ofPopStyle();

}
