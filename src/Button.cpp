#include "ofApp.h"

Button::Button(Module* module, buttonType type, string title) {

    this->module = module;
    this->type = type;
    this->title = title;
    this->state = false;
    this->id = -1;
    this->font.load(UI_FONT_FACE, ofApp::getFontSize(), true);

    ofAddListener(ofEvents().touchDown,  this, &Button::touchDown);
    ofAddListener(ofEvents().touchMoved, this, &Button::touchMoved);
    ofAddListener(ofEvents().touchUp,    this, &Button::touchUp);

}

void Button::setDimensions(int x, int y, int size) {
    
    this->x = x;
    this->y = y;
    this->rect = ofRectangle(x, y, size, size);
    
#ifdef TARGET_OF_IOS
    int offset = size*.6;
#else
    int offset = 0;
#endif
    this->rect2 = ofRectangle(x-offset/2, y-offset/2, size+offset, size+offset);

    this->size = size;

    if (type == BUTTON_TOGGLE) {
        rect = ofRectangle(x, y, size, size);
        titleX = x + size * 1.5;
        titleY = y + size - 3;
    }
    
    // TODO: this is most probably unnecessarily overly complicated
    else if (this->type == BUTTON_REMOVE || this->type == BUTTON_CLEAR) {
        int lowerPartHeight = (1-CONSOLE_SECTION_HEIGHT) * CONSOLE_HEIGHT * ofGetHeight();
        titleX = x + module->getWidth()/4 - (font.stringWidth(title)/2);
        titleY = y + (lowerPartHeight/2) + font.stringHeight(title)/2;
        rect = ofRectangle(x, y, module->getWidth()/2, CONSOLE_HEIGHT*ofGetHeight()*(1-CONSOLE_SECTION_HEIGHT));
    }
}

void Button::touchDown(ofTouchEventArgs& event) {

    if (id == -1 && type == BUTTON_TOGGLE && rect2.inside(event.x, event.y)) {
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
        if (module->anyParticles()) {
            if (type == BUTTON_REMOVE) {
                module->removeParticle();
            } else {
                module->removeAllParticles();
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
    
    if (type == BUTTON_REMOVE || type == BUTTON_CLEAR) {
        ofSetHexColor(BUTTON_REMOVE_COLOR);
    }

    if (state) ofFill();
    else ofNoFill();
    
    ofDrawRectangle(rect);
    
    ofSetColor(UI_COLOR);
    font.drawString(title, titleX, titleY);

    ofPopStyle();
    
}
