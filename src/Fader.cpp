#include "Fader.h"

Fader::Fader(string title, float initX, float initY, int extend)
{
    low = 0.0f;
    high = 1.0f;
    fadeRadius = 10;
    faderVal = 0.1f;
    usage = false;
    
    zeroX = initX;
    zeroY = initY;
    leng = extend;
    fadePos = initX;
    myTitle = title;
    
    verdana10.loadFont("verdana.ttf", 10, true, true);
}

float Fader::getValue() {
    usage = false;
    if (ofDist(ofGetMouseX(), ofGetMouseY(), fadePos, zeroY)<fadeRadius*3) {
        if (ofGetMousePressed()) {
            usage = true;
            faderVal = ofMap(ofGetMouseX(), zeroX, zeroX+leng, low, high) ;
            if (faderVal<low) faderVal=low;
            if (faderVal>high) faderVal =high;
        }
    }
    return(faderVal);
    
}

void Fader::keySupport(bool type) {
    
    if (type) {
        faderVal+=0.05;
        if (faderVal<low) faderVal=low;
        if (faderVal>high) faderVal =high;
    }
    else {
        faderVal-=0.05;
        if (faderVal<low) faderVal=low;
        if (faderVal>high) faderVal =high;
    }
    
}

void Fader::display(){
    
    ofSetColor(255);
    ofLine(zeroX, zeroY, zeroX+leng, zeroY);
    fadePos = ofMap(getValue(), low, high, zeroX, zeroX+leng);
    ofRect(fadePos-5,zeroY-5,10,10);
    verdana10.drawString(myTitle, zeroX, zeroY+20);
    
}