#include "ofApp.h"

Module::Module(int index, float x, float y, float width, float height, int population, vector<string> soundVector) {
	this->index = index; 
	this->maximumPopulation = population; 
	this->modWidth = width; 
	this->modHeight = height; 
	this->modOrigin.set(x, y); 
	this->maxWidth = modOrigin.x + width; 
	this->maxHeight = modOrigin.y + height; 
	this->console = new ModuleConsole(modOrigin, width, index);
    this->panel = new Panel(index, modOrigin.x, modOrigin.y, modWidth);
    this->soundVector = soundVector;

    for (int i=0; i<soundVector.size(); i++) {
        
        //TODO: this looks pretty bad...
        ofSoundPlayer placeholderSound;
        sounds.push_back(placeholderSound);
        sounds[i].loadSound(soundVector[i], true);
        
    }

} 

void Module::update() {
	// TODO: should be doing updates here
}

// TODO: this is way too complicated method..
void Module::addParticle(int life) {
	if (population.size() < maximumPopulation) {
        if (ofGetMouseX() >= modOrigin.x && ofGetMouseX() <= maxWidth && ofGetMouseY() >= CONSOLE_HEIGHT && ofGetMouseY() <= maxHeight) {
			Particle newParticle(index, population.size(), ofGetMouseX(), ofGetMouseY(), life);
			population.push_back(newParticle);
		}
	}
}

// TODO: this is way too complicated method..
void Module::manageParticles() {
	for (int i=0; i < population.size(); i++) {
		managedParticle = &population[i];
		if (!isFreezed() && !isLooping()) { // if the particle is not freezed, apply gravity
            
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
    panel->draw();
}

void Module::boundingBox() {

	ofPushStyle();

	ofSetLineWidth(CONSOLE_BORDER);
	ofSetColor(50, 128); // TODO: not working the hex value
//	ofSetColor(0x989898);
	ofNoFill();
	ofRect(modOrigin.x, modOrigin.y, modWidth, modHeight);

	ofPopStyle();
	
}

// TODO: erase particle shouldn't clear.. this looks weird...
void Module::eraseParticle() {
	for (int i = 0; i < population.size(); i++)
		population.clear();
}

void Module::playSound(int index) {
    sounds[index].play();
}