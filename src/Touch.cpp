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

    ofFill();
    ofDrawCircle(x, y, increment*PARTICLE_OFFSET);

    ofPopStyle();

}
