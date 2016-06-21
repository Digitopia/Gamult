#include "ofApp.h"

Button::Button(Module* module, buttonType type, string title) {

    this->module = module;
    this->type = type;
    this->title = title;

    this->state = false;
    this->id = -1;
    this->font.load(UI_FONT_FACE, UI_FONT_SIZE, true);

    ofAddListener(ofEvents().touchDown,  this, &Button::touchDown);
    ofAddListener(ofEvents().touchMoved, this, &Button::touchMoved);
    ofAddListener(ofEvents().touchUp,    this, &Button::touchUp);

}

void Button::setDimensions(int x, int y, int size) {
    this->x = x;
    this->y = y;
    this->size = size;
    this->rect = ofRectangle(x, y, size, size);
}

void Button::touchDown(ofTouchEventArgs& event) {

    if (id == -1 && type == BUTTON_TOGGLE && rect.inside(event.x, event.y)) {
        state = !state;
    }

    if (id == -1 && (type == BUTTON_REMOVE || type == BUTTON_CLEAR) && rect.inside(event.x, event.y) && event.y < CONSOLE_HEIGHT*ofGetHeight()) {
        id = event.id;
        state = true;
    }

}

void Button::touchMoved(ofTouchEventArgs& event) {
    if ((type == BUTTON_REMOVE || type == BUTTON_CLEAR) && id == event.id && !rect.inside(event.x, event.y)) {
        state = false;
        id = -1;
    }
}

void Button::touchUp(ofTouchEventArgs& event) {
    if (event.id == id && state && (type == BUTTON_REMOVE || type == BUTTON_CLEAR)) {
        if (ofApp::modules[module]->anyParticles()) {
            if (type == BUTTON_REMOVE) {
                ofApp::modules[module]->removeParticle();
            } else {
                ofApp::modules[module]->removeAllParticles();
            }
        }
        state = false;
        id = -1;
    }
}

void Button::draw() {

    ofPushStyle();

    ofSetColor(UI_COLOR);
    ofSetLineWidth(BUTTON_WIDTH);
    ofNoFill();

    if (state)
        ofFill();

    if (type == BUTTON_TOGGLE) {
        ofDrawRectangle(rect);
        font.drawString(title, x+size*1.5, y+size-3);
    }

    if (type == BUTTON_REMOVE || type == BUTTON_CLEAR) {

        ofSetHexColor(BUTTON_REMOVE_COLOR);

        ofLogNotice() << rect;
        rect.setHeight(10);
        ofDrawRectangle(rect);

        ofDrawRectangle(x, y, ofApp::modules[module]->getWidth()/2, CONSOLE_HEIGHT*ofGetHeight()*(1-CONSOLE_SECTION_HEIGHT));

        int lowerPartHeight = (1-CONSOLE_SECTION_HEIGHT) * CONSOLE_HEIGHT * ofGetHeight();

        ofSetColor(UI_COLOR);
        font.drawString(title, x+ofApp::modules[module]->getWidth()/4-(font.stringWidth(title)/2), y+(lowerPartHeight/2)+font.stringHeight(title)/2);

    }

    ofPopStyle();
}
