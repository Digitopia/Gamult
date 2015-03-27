#include "Fader.h"

Fader::Fader(float x0, float y, int size, int range, string title) {
    
    this->x0 = x0;
    this->y = y;
    this->size = size;
    this->range = range;
	this->title = title;
    
    this->rect = ofRectangle(x0+5, y-size/2, size, size);

	// TODO: this should be probably somewhere else
	font.loadFont("verdana.ttf", 10, true, true);

    this->dragging = false;
	
	ofAddListener(ofEvents().mousePressed, this, &Fader::mousePressed);
	ofAddListener(ofEvents().mouseDragged, this, &Fader::mouseDragged);
	ofAddListener(ofEvents().mouseReleased, this, &Fader::mouseReleased);
	
}

void Fader::mousePressed(ofMouseEventArgs& event) {
	if (rect.inside(event.x, event.y))
		dragging = true;
}

void Fader::mouseReleased(ofMouseEventArgs& event) {
	dragging = false;
}

void Fader::mouseDragged(ofMouseEventArgs& event) {

	// make sure that only updates fader position if first click was inside it
	if (dragging) {

		rect.setX(event.x);
	
		if (rect.x < x0 - size/2)
			rect.x = x0 - size/2;
		
		if (rect.x > x0 + range - size/2)
			rect.x = x0 + range - size/2;
		
		cout << getValue() << endl;
	}
}

float Fader::getValue() {
	return ofMap(rect.x, x0, x0+range-size, 0.0, 1.0);
}

void Fader::draw() {
    
	// TODO: should be a variable
	ofSetColor(255);

	// draw the line in which the fader moves
	ofLine(x0, y, x0+range, y);

	// draw the fader
	ofRect(rect);
    
	font.drawString(title, x0, y+20);

}