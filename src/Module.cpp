#include "Module.h"

Module::Module(int index, float x, float y, float width, float height, int population) {
	this->index = index;
	this->maximumPopulation = population;
	this->modWidth = width;
	this->modHeight = height;
	this->modOrigin.set(x, y);
	this->maxWidth = modOrigin.x + width;
	this->maxHeight = modOrigin.y + height;
	this->console = new ModuleConsole(modOrigin, width, index);
}

void Module::update() {
	// TODO: should be doing updates here
}

void Module::addParticle(int life) {
	if (population.size() < maximumPopulation) {
		// TODO: fixe modOrigin.y+100 (index it to moduleconsole)
		if (ofGetMouseX() >= modOrigin.x && ofGetMouseX() <=maxWidth && ofGetMouseY() >= modOrigin.y+100 && ofGetMouseY() <= maxHeight/2) {
			Particle newParticle(index, population.size(), ofGetMouseX(), ofGetMouseY(), life);
			population.push_back(newParticle);
		}
	}
}

void Module::manageParticles() {
	for (int i=0; i < population.size(); i++) {
		managedParticle = &population[i];
		if (isFreezed()) {
				if ((managedParticle->getCounter()) >= (managedParticle->getLife())) {
					population.erase(population.begin() + i);
					managedParticle->noGravity();
				}

				else {
					managedParticle->yesGravity();
				}
		}
		managedParticle->draw();
	}
}

void Module::draw() {
	console->draw();
}

void Module::boundingBox() {
	ofPushStyle();
	ofSetLineWidth(1);
	ofSetColor(50, 128);
	ofNoFill();
	ofRect(modOrigin.x, modOrigin.y, modWidth, modHeight);
	ofPopStyle();
}

void Module::eraseParticle() {
	for (int i=0; i< population.size(); i++) {
		//        thisParticle = new Particle(population[i]);
		population.clear();
	}
}

bool Module::isFreezed() {
	return console->isFreezed();
}

bool Module::isLooping() {
	return console->isLooping();
}

float Module::getGravity() {
	return console->getGravity();
}

float Module::getLoopSpeed() {
	return console->getLoopSpeed();
}

