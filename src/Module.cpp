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

void Module::addParticle(int life, int x, int y) {
	if (population.size() < maxPopulation) {
        if (x >= x0 && x <= x1 && y >= CONSOLE_HEIGHT) {
			population.push_back(Particle(index, population.size(), x, y, life));
		}
	}
}

void Module::manageParticles() {

	for (int i = 0; i < population.size(); i++) {
		
		managedParticle = &population[i];

		if (!isFreezed()) {
			if (!isLooping()) {
				if (managedParticle->getCounter() >= managedParticle->getLife()) {
					population.erase(population.begin() + i);
				}
				else {
					managedParticle->yesGravity();
				}
			}
			else { // not freezed and not looping
				managedParticle->updateGravity();
				managedParticle->noGravity();
			}
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
//    	ofLine(gridCellX, ofGetHeight(), gridCellX, ofGetHeight()-GRID_HEIGHT); // small grids at bottom
    	ofLine(gridCellX, ofGetHeight(), gridCellX, CONSOLE_HEIGHT); // top to bottom grids
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
