#ifndef MODULE
#define MODULE

#include "ofMain.h"
#include "ModuleConsole.h"

class Module {
    
public:
    
    int index;
    int maximumPopulation;
    float sync;
    float modWidth;
    float modHeight;
    float maxWidth;
    float maxHeight;
    

    Module(int setIndex, float setOriginX, float setOriginY, float setWidth, float setHeight, int setPopulation);
    
    ModuleConsole* console;

private:
    
};

#endif