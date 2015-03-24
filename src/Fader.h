#ifndef FADER
#define FADER

#include "ofMain.h"

class Fader {
    public:
    
    float getValue();
    void keySupport(bool type);
    void display();

//    void mousePressed(ofMouseEventArgs& event);

    Fader(string title, float initX, float initY, int extend);
    
    float zeroX, zeroY, leng;
    float low, high;
    int fadeRadius;
    float fadePos, faderVal;
    string myTitle;
    bool usage;
    
    ofTrueTypeFont verdana10;
    
private:
    
};

#endif