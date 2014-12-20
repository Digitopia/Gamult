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
}

float Fader::getValue() {
    
}

void Fader::keySupport(bool type) {
    
}

void Fader::display(){
    
}