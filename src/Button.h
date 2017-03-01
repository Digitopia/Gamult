#ifndef BUTTON_H
#define BUTTON_H

#include "ofMain.h"
#include "Module.h"

class Module;

enum buttonType {
    BUTTON_TOGGLE,
    BUTTON_REMOVE,
    BUTTON_CLEAR
};

class Button {

public:

    Button(Module* module, buttonType type, string title);
    void draw();
    bool getState() { return state; }
    void on() { state = true; }
    void off() { state = false; }
    void setDimensions(int size, int x, int y);
    void setTitle(string title) { this->title = title; }

    void touchDown(ofTouchEventArgs& event);
    void touchMoved(ofTouchEventArgs& event);
    void touchUp(ofTouchEventArgs& event);

private:

    buttonType type;
    int size;
    int x, y; // top left corner of the button
    string title;
    int titleX, titleY;
    ofRectangle rect;
    ofRectangle rect2; // rect2 is used to increse the area when clicking on a mobile device
    ofTrueTypeFont font;
    bool state;
    int id; // the id of the touch to which this fader is locked
    Module* module;

};

#endif
