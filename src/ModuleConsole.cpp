#include "ofApp.h"
#include "Button.h"

ModuleConsole::ModuleConsole(int x, float width, int module) {

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

    int y = 0;
    int buttonSize = BUTTON_SIZE*height;
    int faderSize = FADER_SIZE*height;
    int buttonPositionOffset = (height*CONSOLE_SECTION_HEIGHT)/3;

    // TODO: needs refactor...
    if (first) {
        btnGravity = new Button(BUTTON_TOGGLE, module, buttonSize, x+(width*0.1), y+buttonPositionOffset-buttonSize/2, "Gravity");
        btnFreeze  = new Button(BUTTON_TOGGLE, module, buttonSize, x+width*0.6, y+buttonPositionOffset-buttonSize/2, "Freeze");
        btnRemove  = new Button(BUTTON_REMOVE, module, width/2, x+1, CONSOLE_SECTION_HEIGHT*height, "Remove");
        btnClear   = new Button(BUTTON_CLEAR, module, width/2, x + width/2 + 1, CONSOLE_SECTION_HEIGHT*height, "Clear");
        fadSpeed = new Fader(x+(width*0.1), y+buttonPositionOffset*2+buttonSize/2, faderSize, "Speed");
    } else {
        btnGravity->setDimensions(buttonSize, x+(width*0.1), y+buttonPositionOffset-buttonSize/2);
        btnFreeze->setDimensions(buttonSize, x+width*0.6, y+buttonPositionOffset-buttonSize/2);
        btnRemove->setDimensions(width/2, x+1, CONSOLE_SECTION_HEIGHT*height);
        btnClear->setDimensions(width/2, x + width/2 + 1, CONSOLE_SECTION_HEIGHT*height);
        fadSpeed->setDimensions(x+(width*0.1), y+buttonPositionOffset*2+buttonSize/2, faderSize, false);
    }

    // TODO: dafuq
//    x = x + width/2;

}

void ModuleConsole::draw() {

    ofPushStyle();

    ofSetHexColor(CONSOLE_COLOR);

    ofFill();

    ofDrawRectangle(x, y, width, height);

    ofSetHexColor(BUTTON_COLOR);

    ofDrawRectangle(x , height * 0.75, width, height * 0.25);

    ofPopStyle();

    btnRemove->draw();
    btnClear->draw();
    btnGravity->draw();
    btnFreeze->draw();
    fadSpeed->draw();

}
