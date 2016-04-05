#include "ofApp.h"

Module::Module(int index, int x, int y, int width, int height, int maxPopulation, vector<string> soundPaths) {
	
    this->index = index;
    this->maxPopulation = maxPopulation;
    this->console = NULL;
    this->iSoundPaths = index;
    
    setDimensions(x, y, width, height);
    loadSounds(soundPaths);
    ofAddListener(ofEvents().touchDown, this, &Module::touchDown);
    
    this->console = new ModuleConsole(x0, width, index);
    
    this->active = true;

}

void Module::setDimensions(int x, int y, int width, int height) {
    
    this->x0 = x;
    this->y = y;
    this->width = width;
    this->height = height;
    
    if (this->console != NULL)
        this->console->setDimensions(x, width, false);
    
    this->x1 = x + width;
    this->consoleHeight = CONSOLE_HEIGHT*height;
    
//    int w = round(BUTTON_CHANGE_INSTRUMENT_WIDTH * width);
//    int h = round(BUTTON_CHANGE_INSTRUMENT_HEIGHT * height);
//    int middleY = round((height - consoleHeight)/2 + consoleHeight);
//    
//    #if defined TARGET_OF_IPHONE
//    previousInstrumentRect.set(x0, middleY - h/2, w, h);
//    nextInstrumentRect.set(x1, middleY - h/2, -w, h);
//    #endif

}

void Module::updateParticlesOnOrientationChange(int x, int y, int width, int height) {
    for (unsigned int i = 0; i < particles.size(); i++) {
        int newX = ofMap(particles[i].getX(), x0, x1, x, x+width);
        int newY = ofMap(particles[i].getY(), this->y, this->y+this->height, y, y+height);
        particles[i].setXY(newX, newY);
    }
}


void Module::touchDown(ofTouchEventArgs& event) {
    
//    #if defined TARGET_OF_IPHONE
//    if (previousInstrumentRect.inside(event.x, event.y)) {
//        if (iSoundPaths <= 0) return;
////        changeInstrument(--iSoundPaths);
//        changeInstrument(iSoundPaths-1);
//    }
//    
//    else if (nextInstrumentRect.inside(event.x, event.y)) {
//        if (iSoundPaths >= 3) return;
////        changeInstrument(++iSoundPaths);
//        changeInstrument(iSoundPaths+1);
//    }
//    #endif
    
}

void Module::loadSounds(vector<string> paths) {
    for (int i = 0; i < paths.size(); i++) {
        ofSoundPlayer s;
        sounds.push_back(s);
//        sounds[i].setMultiPlay(true);
        sounds[i].load(paths[i], true);
    }
}

void Module::unloadSounds() {
    for (int i = 0; i < sounds.size(); i++) {
        sounds[i].stop();
        cout << "stopping sound " << i << endl;
        sounds[i].unload();
        cout << "unloading sound " << i << endl;
    }
    sounds.clear();
}

void Module::changeInstrument(int iSoundPaths) {
//    unloadSounds();
//    loadSounds(ofApp::getSoundPaths(iSoundPaths));
//	cout << "changing instrument" << endl;
//    ofApp::moduleActive = iSoundPaths; // TODO: decide if inside or outside module, not in both
}

void Module::addParticle(int life, int x, int y) {
	if (particles.size() < maxPopulation
        && !previousInstrumentRect.inside(x, y)
        && !nextInstrumentRect.inside(x, y)) {
        // the following line is to make sure that when the particle is created it always goes downwards first (was causing problems with Particle::gravity();
        if (y <= consoleHeight + life) y = consoleHeight + life + 1;
        particles.push_back(Particle(index, particles.size(), x, y, life));
	}
}

void Module::update() {

    if (isFreezed())
        return;

	for (int i = 0; i < particles.size(); i++) {

        Particle *p = &particles[i];

        if (isLooping())
            p->loop();

		if (isGravityOn())
			p->gravity();

        if (p->getHealth() <= 0)
            particles.erase(particles.begin()+i);

	}

}

void Module::draw() {
	console->draw();
    drawBackground();
	drawBorders();
	drawGrid();
    drawChangeInstrumentButtons();
}

void Module::drawBackground() {
    ofPushStyle();
    
    // TODO: this is terrible design, but doing the trick for prototyping
    #if !defined TARGET_OF_IPHONE
    ofSetColor(255 - (30 * index));
    #else
    ofSetColor(255 - (30 * iSoundPaths));
    #endif
    
    ofDrawRectangle(x0, y + consoleHeight, width, height);
    ofPopStyle();
}

void Module::drawBorders() {
    ofPushStyle();
    ofSetLineWidth(CONSOLE_BORDER_WIDTH);
    ofSetHexColor(CONSOLE_BORDER_COLOR);
    ofNoFill();
    ofDrawRectangle(x0, y, width, height);
    ofPopStyle();
}

void Module::drawGrid() {
	ofSetColor(GRID_COLOR);
	int gridNumberElements = sounds.size();
	int gridCellSize = round(float(width) / gridNumberElements);
	for (int i = 1; i < gridNumberElements; i++) {
		int gridCellX = x0 + (i)*gridCellSize + 2;
//    	ofLine(gridCellX, height, gridCellX, height-GRID_HEIGHT); // small grids at bottom
    	ofDrawLine(gridCellX, height, gridCellX, consoleHeight); // top to bottom grids
	}
}

void Module::drawChangeInstrumentButtons() {
    
    if (ofGetOrientation() != OF_ORIENTATION_DEFAULT) return;
    
    #if defined TARGET_OF_IPHONE
    ofPushStyle();
    ofSetColor(ofColor::fromHex(BUTTON_CHANGE_INSTRUMENT_COLOR), BUTTON_CHANGE_INSTRUMENT_COLOR_ALPHA);
    ofDrawRectangle(previousInstrumentRect);
    ofDrawRectangle(nextInstrumentRect);
    ofPopStyle();
    #endif
}

void Module::drawParticles() {
    for (int i = 0; i < particles.size(); i++) {
        particles[i].draw();
    }
}

void Module::playSound(int index, float vol) {
    if (!this->active) return;
    sounds[index].setVolume(vol);
    sounds[index].play();
}