#include "Module.h"

Module::Module(int setIndex, float setOriginX, float setOriginY, float setWidth, float setHeight, int setPopulation)
{
    index = setIndex;
    maximumPopulation = setPopulation;
    modWidth = setWidth;
    modHeight = setHeight;
    ofVec2f modOrigin;
    modOrigin.set(setOriginX, setOriginY);
    vector <ofVec2f> population;
    maxWidth = modOrigin.x + modWidth;
    maxHeight = modOrigin.y + modHeight;
    console = new ModuleConsole(modOrigin, modWidth, index);
    
}