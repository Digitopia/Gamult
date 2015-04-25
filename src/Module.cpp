#include "ofApp.h"

Module::Module(int index, float x, float y, float width, float height, int maxPopulation, vector<string> soundPaths) {

	this->index = index;
	this->x0 = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->maxPopulation = maxPopulation;
    this->soundPaths = soundPaths;

	this->console = new ModuleConsole(x0, width, index);
//    this->panel = new Panel(index, x0, y, width);
	
	this->x1 = x + width;
	
	loadSounds();

}

void Module::loadSounds() {
    for (int i = 0; i < soundPaths.size(); i++) {
        ofSoundPlayer s;
        sounds.push_back(s);
        sounds[i].setMultiPlay(true);
        sounds[i].loadSound(soundPaths[i], true);
    }
}

void Module::update() {
	// TODO: should be doing updates here
}

// TODO: this should be working with callbacks
void Module::addParticle(int life) {
	if (population.size() < maxPopulation) {
        if (ofGetMouseX() >= x0 && ofGetMouseX() <= x1 && ofGetMouseY() >= CONSOLE_HEIGHT) {
			Particle p(index, population.size(), ofGetMouseX(), ofGetMouseY(), life);
			population.push_back(p);
		}
	}
}

// TODO: refactor!
void Module::manageParticles() {
	for (int i = 0; i < population.size(); i++) {
		managedParticle = &population[i];
		if (!isFreezed() && !isLooping()) {
            
				if ((managedParticle->getCounter()) >= (managedParticle->getLife())) {
					population.erase(population.begin() + i);
                    // TODO: check if this is really needed, maybe for collision detection?
					//managedParticle->noGravity();
				}
				else {
					managedParticle->yesGravity();
				}
            
        } else if (isLooping() && !isFreezed()) { // if the particle is looping, but not freezed
            managedParticle->updateGravity();
            managedParticle->noGravity();
        }
		managedParticle->draw();
	}
}

void Module::draw() {
	console->draw();
//    panel->draw();
	drawBorders();
	drawGrid();
}

void Module::drawGrid() {
	ofSetColor(GRID_COLOR);
	int gridNumberElements = soundPaths.size();
	int gridCellSize = round(float(width) / gridNumberElements);
	for (int i = 1; i < gridNumberElements; i++) {
		int gridCellX = x0 + (i)*gridCellSize;
//    	ofLine(gridCellX, ofGetHeight(), gridCellX, ofGetHeight()-GRID_HEIGHT);
    	ofLine(gridCellX, ofGetHeight(), gridCellX, CONSOLE_HEIGHT);
	}
}

void Module::drawBorders() {
	ofPushStyle();
	ofSetLineWidth(CONSOLE_BORDER_WIDTH);
	ofSetHexColor(CONSOLE_BORDER_COLOR);
	ofNoFill();
	ofRect(x0, y, width, ofGetHeight());
	ofPopStyle();
}

void Module::playSound(int index) {
    sounds[index].play();
}