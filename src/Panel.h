#ifndef __gamultOSX__Panel__
#define __gamultOSX__Panel__

#include "ofMain.h"
#include "ofxGui.h"

class Panel {
    
public:
    
    Panel(int index, int x, int y, int width);
    
    void draw();
    
    void setInstrument1();
    void setInstrument2();
    bool showPanel;
    
    ofRectangle select;
    
private:
    
    int x;
    int y;
    int width;
    
    int index;
    
    ofxPanel panel;
    
    ofxButton inst1;
    ofxButton inst2;
    
    string tag;
    
};

#endif
