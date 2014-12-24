#include "PolyClass.h"

PolyClass::PolyClass(int setIndex)
{
    index = setIndex;
    
}

void PolyClass::addVertex(float vX, float vY) {
    ofVec2f newVertex;
    newVertex.set(vX, vY);
    addedVertexes.push_back(newVertex);
}