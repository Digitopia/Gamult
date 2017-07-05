#include "ofApp.h"

Module::Module(int index, int x, int y, int width, int height, int maxPopulation, vector<string> soundPaths) {

    this->index = index;
    this->maxPopulation = maxPopulation;
    this->console = NULL;
    this->iSoundPaths = index;
    this->soundPaths = soundPaths;

    setDimensions(x, y, width, height);
    ofAddListener(ofEvents().touchDown, this, &Module::touchDown);

    this->console = new ModuleConsole(this, x0, width);
    this->numberOfInstruments = soundPaths.size();

    this->active = true;

    // only needed for phone version
    this->iSoundPaths = 0;

    this->mostRecent = false;

    ofSoundPlayer s;
    this->sounds.push_back(s);

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

void Module::changeInstrument(int direction) {

    if (!ofApp::isPhone()) return;

    ofLogNotice() << "Direction received: " << direction;
    if (direction == 1)
    {
        iSoundPaths = iSoundPaths <= 0 ? 4 : iSoundPaths;
        --iSoundPaths;
    }
    else if (direction == 2) {
        iSoundPaths = iSoundPaths >= 3 ? -1 : iSoundPaths;
        ++iSoundPaths;
    }

    // soundPaths = ofApp::getSoundPaths(iSoundPaths);
    // numberOfInstruments = soundPaths.size();

    // NOTE: this is the hack, make it different for Android for now
    // if (!ofApp::isAndroid()) {
    //     unloadSounds();
    //     loadSounds();
    // }

    // Update module background color too
    backgroundColor = 255 - (30 * iSoundPaths);

}

void Module::loadSounds() {

    ofLogNotice() << "Loading sounds for module " << this->index;

    vector <string> paths = soundPaths;

    #if defined TARGET_ANDROID
    for (uint i = 0; i < 3; i++) {
        paths = ofApp::getSoundPaths(i);
        for (uint p = 0; p < paths.size(); p++) {
            ofSoundPlayer s;
            s.load(paths[p], false);
            s.setMultiPlay(true);
            ofApp::sounds.push_back(s);
        }
    }

    // #elif defined TARGET_OF_IOS
    // for (uint i = 0; i < paths.size(); i++) {
    //     sounds.load(paths[i]);
    // }
    #endif

}

void Module::unloadSounds() {
    #if !defined TARGET_ANDROID

    // #if !defined TARGET_OF_IOS && !defined TARGET_ANDROID
    for (uint = 0; i < sounds.size(); i++) {
        sounds[i].stop();
        ofLogNotice() << "Stopping sound " << i;
        sounds[i].unload();
        ofLogNotice() << "Unloading sound " << i;
        sounds.clear();
    }
    // #endif

    #endif
}

void Module::addParticle(uint life, uint x, uint y) {
    if (particles.size() < maxPopulation) {
        // NOTE: the following line is to make sure that when the particle is
        // created it always goes downwards first (was causing problems with Particle::gravity();
        if (y <= consoleHeight + life) y = consoleHeight + life + 1;
        particles.push_back(Particle(this, particles.size(), x, y, life));
    }
}

void Module::update() {

    if (!active) return;

    if (isFreezed())
        return;

    for (uint i = 0; i < particles.size(); i++) {
        Particle* p = &particles[i];
        p->update();
        if (p->getHealth() <= 0) particles.erase(particles.begin()+i);

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

    if (this->mostRecent) ofSetLineWidth(CONSOLE_BORDER_WIDTH_MOST_RECENT);
    else ofSetLineWidth(CONSOLE_BORDER_WIDTH);
    ofSetHexColor(CONSOLE_BORDER_COLOR);
    ofNoFill();
    ofDrawRectangle(x0, y, width, height);
    ofPopStyle();
}

void Module::drawGrid() {
	ofSetColor(GRID_COLOR);
    int gridNumberElements  = getNumberOfInstrumentNotes();
	int gridCellSize = round(float(width) / gridNumberElements);
	for (uint i = 1; i < gridNumberElements; i++) {
		int gridCellX = x0 + (i)*gridCellSize + 2;
    	ofDrawLine(gridCellX, height, gridCellX, consoleHeight); // top to bottom grids
	}
}

void Module::drawParticles() {
    if (!active) return;
    for (uint i = 0; i < particles.size(); i++) {
        particles[i].draw();
    }
}

void Module::playSound(int soundIndex, float vol) {

    if (!this->active) return;
    float soundPan;
    if(ofApp::isPhone()){
        soundPan = 50.0f*(float)soundIndex/(((float)numberOfInstruments) - 1.0f)-25.0f;
    }
    else {
        soundPan = ((50.0f*(float)index/((float)NMODULES)-1) + ((float)soundIndex/(float)numberOfInstruments)*((50.0f)/(float)NMODULES))-25.0f;
    }
    ofLogNotice() << "soundPan is " << soundPan;

    #if !defined TARGET_OF_IOS
    uint idx = ofApp::getBaseModuleOffsetSound(iSoundPaths) + soundIndex;
    ofLogNotice() << "!!!!!!!!!!!!!!!!! idx is " << idx;

    try {
        if (ofApp::sounds[idx].isLoaded()) {
            ofApp::sounds[idx].setPan(soundPan);
            ofApp::sounds[idx].setVolume(vol);
            ofApp::sounds[idx].play();
            ofLogNotice() << "playing sound!";
        }
        else
            ofLogNotice() << "sound isn't loaded yet so skipping...";
    }
    catch (...) {
        ofLogNotice() << "failed and trying to replay";
        // ofApp::sounds[idx].play();
    }

    #endif
}
