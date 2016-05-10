#include "Touch.h"
#include "Constants.h"

Touch::Touch(int x, int y) {
    this->x = x;
    this->y = y;
    this->increment = TOUCH_MIN;
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

    ofSetColor(ofColor::fromHex(TOUCH_COLOR), TOUCH_ALPHA);

    if (y > ofApp::maxParticleY || y < CONSOLE_HEIGHT*ofGetHeight()) {
        ofSetColor(ofColor::fromHex(PARTICLE_LIMIT_COLOR), PARTICLE_LIMIT_ALPHA);
    }
    
    /*
       ofSetLineWidth(TOUCH_LINE_WIDTH);
       ofNoFill();

       ofPolyline polyline;
       ofPoint pt(x, y);
       float angleBegin = 180;
       float angleEnd = float(increment)*(360./float(TOUCH_MAX)) - 180;
       polyline.arc(pt, TOUCH_RADIUS, TOUCH_RADIUS, angleBegin, angleEnd, ARC_RESOLUTION);
       polyline.draw();
    */
    // TODO: check these magic numbers when rebuilding code according to screen size
    
    ofFill();
    ofDrawCircle(x, y, increment*PARTICLE_OFFSET);

    ofPopStyle();

}
