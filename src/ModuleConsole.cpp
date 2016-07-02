#include "ofApp.h"
#include "Button.h"

ModuleConsole::ModuleConsole(Module* module, int x, float width) {
    
    this->module = module;

    setDimensions(x, width, true);

    btnGravity->on();
    btnRemove->off();
    btnFreeze->off();
}

void ModuleConsole::setDimensions(int x, int width, bool first) {

    this->width = width;
    this->x = x;
    this->y = 0;
    this->height = CONSOLE_HEIGHT * ofGetHeight();

    if (first) {
        btnGravity = new Button(this->getModule(), BUTTON_TOGGLE, "Gravity");
        btnFreeze  = new Button(this->getModule(), BUTTON_TOGGLE, "Freeze");
        btnRemove  = new Button(this->getModule(), BUTTON_REMOVE, "Remove");
        btnClear   = new Button(this->getModule(), BUTTON_CLEAR, "Clear");
        fadSpeed   = new Fader("Speed");
    }

    int y = 0;
    int buttonSize = BUTTON_SIZE*height;
    int faderSize = FADER_SIZE*height;
    int buttonPositionOffset = (height*CONSOLE_SECTION_HEIGHT)/3;

    btnGravity->setDimensions(
        x + width*0.1,
        y + buttonPositionOffset -buttonSize/2,
        buttonSize
    );

    btnFreeze->setDimensions(
        x + width*0.6,
        y + buttonPositionOffset - buttonSize/2,
        buttonSize
    );

    btnRemove->setDimensions(
        x+1,
        CONSOLE_SECTION_HEIGHT*height,
        width/2
    );

    btnClear->setDimensions(
        x + width/2 + 1,
        CONSOLE_SECTION_HEIGHT*height,
        width/2
    );

    fadSpeed->setDimensions(
        x + width*0.1,
        y + buttonPositionOffset*2 + buttonSize/2,
        faderSize,
        first
    );

}

void ModuleConsole::draw() {

    // draw console
    ofPushStyle();
    ofSetHexColor(CONSOLE_COLOR);
    ofFill();
    ofDrawRectangle(x, y, width, height);
    ofSetHexColor(BUTTON_COLOR);
    ofDrawRectangle(x , height * 0.75, width, height * 0.25);
    ofPopStyle();

    // and then what belongs to console
    btnRemove->draw();
    btnClear->draw();
    btnGravity->draw();
    btnFreeze->draw();
    fadSpeed->draw();

}