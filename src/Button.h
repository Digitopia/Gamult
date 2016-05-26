#ifndef BUTTON_H
#define BUTTON_H

#include "ofMain.h"

enum buttonType {
    BUTTON_TOGGLE,
    BUTTON_REMOVE,
    BUTTON_CLEAR
};

class Button {

public:

    Button(buttonType type, int module, int size, int x, int y, string title);
    void draw();
    bool getState() { return state; }
    void on() { state = true; }
    void off() { state = false; }
    void setDimensions(int size, int x, int y);

    void touchDown(ofTouchEventArgs& event);
    void touchMoved(ofTouchEventArgs& event);
    void touchUp(ofTouchEventArgs& event);

private:

    buttonType type;
    int module;
    int size;
    float x; // top left corner of the button
    float y; // top left corner of the button
    string title;
    ofRectangle rect;
    ofTrueTypeFont font;
    bool state;
    int id; // the id of the touch to which this fader is locked

};

#endif
