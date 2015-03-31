#ifndef POLYCLASS_H
#define POLYCLASS_H

#include "ofMain.h"

class PolyClass {

public:

	PolyClass(int index);

	void addVertex(float vX, float vY);
	void display(int i);

	// TODO: this should be private
	int index;
	vector <ofVec2f> addedVertexes;

private:

};

#endif
