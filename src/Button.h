#ifndef BUTTON
#define BUTTON

#include "ofMain.h"

class Button {
public:
    
    void toggle();
    void click();
    void keySupport();
    
    void displayToggle();
    void displayClick();
    
    int module;
    float x;
    float y;
    bool inUse;
    string tag;
    
    Button(int setModule, float setX, float setY, string setTag);
    
    bool state;
    
    ofTrueTypeFont verdana10;
    
private:
    
};


#endif