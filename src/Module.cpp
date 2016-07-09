#include "ofApp.h"

Module::Module(int x, int y, int width, int height, int maxPopulation, vector<string> soundPaths) {

    this->maxPopulation = maxPopulation;
    this->console = NULL;
    setDimensions(x, y, width, height);
    loadSounds(soundPaths);
    ofAddListener(ofEvents().touchDown, this, &Module::touchDown);

    this->console = new ModuleConsole(this, x0, width);

    this->active = true;
    
    // only needed for phone version
    this->iSoundPaths = 0;

}

void Module::setDimensions(int x, int y, int width, int height) {

    // updates particles first
    for (unsigned int i = 0; i < particles.size(); i++) {
        int newX = ofMap(particles[i].getX(), x0, x1, x, x+width);
        int newY = ofMap(particles[i].getY(), this->y, this->y+this->height, y, y+height);
        particles[i].setXY(newX, newY);
    }

    // and then can safely update the dimensions
    this->x0 = x;
    this->y = y;
    this->width = width;
    this->height = height;

    if (this->console != NULL)
        this->console->setDimensions(x, width, false);

    this->x1 = x + width;
    this->consoleHeight = CONSOLE_HEIGHT*height;
    
    this->backgroundColor = 255 - (30 * ofApp::getModuleIdx(x0));

}

void Module::touchDown(ofTouchEventArgs& event) {

}

void Module::prepareInstrumentChange(int direction) {

    if (ofApp::isPhone()) {
        ofLogNotice() << "Direction received: " << direction;
        if (direction == 1) {
            if (iSoundPaths <= 0) changeInstrument(iSoundPaths = 3);
            changeInstrument(--iSoundPaths);
        }

        else if (direction == 2) {
            if (iSoundPaths >= 3) changeInstrument(iSoundPaths = 0);
            changeInstrument(++iSoundPaths);
        }
    }

}

void Module::loadSounds(vector<string> paths) {
#ifndef TARGET_OF_IOS
    for (int i = 0; i < paths.size(); i++) {
        ofSoundPlayer s;
        // s.setMultiPlay(true);
        sounds.push_back(s);
        // sounds[i].setMultiPlay(true);
        sounds[i].load(paths[i], false);
    }
#else
    // cout << "setting up" << ofToString(paths) << endl;
    ofxCocosDenshion s;
    s.setup();
    sounds.push_back(s);
    for (int i = 0; i< paths.size(); i++) {
        sounds[0].addSoundEffect(paths[i], 0.7);
    }
    sounds[0].loadAllAudio();
#endif
}

void Module::unloadSounds() {
    for (int i = 0; i < sounds.size(); i++) {
#ifndef TARGET_OF_IOS
        sounds[i].stop();
        ofLogNotice() << "Stopping sound " << i;
        sounds[i].unload();
        ofLogNotice() << "Unloading sound " << i;
    }
#else
        sounds[0].stopAllSounds();
        cout << "stopping sound " << i << endl;
        sounds[0].destroy();
    }
#endif
    sounds.clear();
    
}

void Module::changeInstrument(int iSoundPaths) {
    unloadSounds();
    loadSounds(ofApp::getSoundPaths(iSoundPaths));
    numberOfInstruments = ofApp::getSoundPaths(iSoundPaths).size();
    ofLogNotice() << "Changing instrument";
    backgroundColor = 255 - (30 * iSoundPaths);
}

void Module::addParticle(int life, int x, int y) {
    if (particles.size() < maxPopulation) {
        // NOTE: the following line is to make sure that when the particle is \
        // created it always goes downwards first (was causing problems with Particle::gravity();
        if (y <= consoleHeight + life) y = consoleHeight + life + 1;
        particles.push_back(Particle(this, particles.size(), x, y, life));
    }
}

void Module::update() {
    
    if (!active) return;

    if (isFreezed())
        return;

    for (int i = 0; i < particles.size(); i++) {

        Particle* p = &particles[i];

        if (isLooping())
            p->loop();

        if (isGravityOn())
            p->gravity();

        if (p->getHealth() <= 0)
            particles.erase(particles.begin()+i);

    }

}

void Module::draw() {
    if (!active) return;
    console->draw();
    drawBackground();
    drawBorders();
    drawGrid();
}

void Module::drawBackground() {
    ofPushStyle();
    ofSetColor(backgroundColor);
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
    int gridNumberElements = getNumberOfInstrumentNotes();
	int gridCellSize = round(float(width) / gridNumberElements);
	for (int i = 1; i < gridNumberElements; i++) {
		int gridCellX = x0 + (i)*gridCellSize + 2;
    	ofDrawLine(gridCellX, height, gridCellX, consoleHeight); // top to bottom grids
	}
}

void Module::drawParticles() {
    if (!active) return;
    for (int i = 0; i < particles.size(); i++) {
        particles[i].draw();
    }
}

void Module::playSound(int soundIndex, float vol) {
    if (!this->active) return;
    #if !defined TARGET_OF_IOS
    sounds[soundIndex].setMultiPlay(true);
    #endif

#ifndef TARGET_OF_IOS
    sounds[soundIndex].setVolume(vol);
    sounds[soundIndex].play();
#else
    sounds[0].setSoundVolume(vol, 0.8f);
    sounds[0].playSound(soundIndex);
    cout << "index is " << soundIndex << " and module is " << this->index << endl;
#endif
}
