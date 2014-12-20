#include "ofMain.h"

class PolyClass {
    
public:
    
    PolyClass(int setIndex);
    
    int index;
    
    vector < ofVec2f > addedVertexes;
    
    void addVertex (float vX, float vY);
    void display(int i);
    
private:
    
};