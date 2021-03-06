#include "ofApp.h"

Particle::Particle(Module* module, int index, float x, float y, int life) {
    this->module = module;
    this->index = index;
    this->center.x = x;
    this->center.y = y;
    this->life = life;
    this->health = life;
    this->radius = life;
    this->velocity = 0;
}

void Particle::update() {

    float speed = module->getSpeed();

    bool topCollision    = false;
    bool bottomCollision = false;

    center.y += velocity;

    if (center.y + life >= ofGetHeight()) {
        bottomCollision = true;
        center.y = ofGetHeight() - life;
        trigger(true, true);
    }

    else if (center.y - life <= CONSOLE_HEIGHT*ofGetHeight()) {
        topCollision = true;
        center.y = CONSOLE_HEIGHT*ofGetHeight() + life;
    }

    if (bottomCollision && module->isGravityOn()) health--;

    // HACK: keep so that previous behaviour is kept, even though it doesn't make sense
    if (topCollision && !module->isGravityOn()) velocity = 0;

    if (bottomCollision || topCollision)
        if (module->isGravityOn()) velocity *= -0.95;
        else velocity *= -1;
    else
        velocity += speed;

}

void Particle::draw() {
    drawCircle();
}

void Particle::drawCircle() {

    ofPushStyle();

    ofFill();
    ofSetColor(PARTICLE_COLOR_OUTER);
    ofDrawCircle(center.x, center.y, (life*PARTICLE_OFFSET));
    ofSetHexColor(PARTICLE_COLOR_INNER);
    ofDrawCircle(center.x, center.y, health);

    ofPopStyle();

}

#if defined TARGET_SEMIBREVE
void Particle::report(int idx, int note, int vel) {
    ofxOscMessage m;
    string addr = OSC_ADDRESS + ofToString(idx);
    m.setAddress(addr);
    m.addIntArg(note);
    m.addIntArg(vel);
    ofApp::oscSender.sendMessage(m);
}
#endif

void Particle::trigger(bool play, bool send) {

    // segment particle position x to the amount of notes the instrument has
    float notef = ofMap(center.x, module->getX0(), module->getX1(), 0, module->getNumberOfInstrumentNotes());
    int note = floor(notef);

    float vol = ofMap(velocity, 5, 60, 0.2, 0.8); // TODO: check magic numbers

    if (play) {
        module->playSound(note, vol);
    }

    // TODO: still not sending the velocity properly
    #if defined TARGET_SEMIBREVE
    if (send) {
        int idx = ofApp::modules[module]->getIndex();
        report(idx, note, (vol*100));
    }
    #endif
}
