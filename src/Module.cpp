#include "Module.h"

Module::Module(int setIndex, float setOriginX, float setOriginY, float setWidth, float setHeight, int setPopulation)
{
    index = setIndex;
    maximumPopulation = setPopulation;
    modWidth = setWidth;
    modHeight = setHeight;
    modOrigin.set(setOriginX, setOriginY);
    maxWidth = modOrigin.x + modWidth;
    maxHeight = modOrigin.y + modHeight;
    console = new ModuleConsole(modOrigin, modWidth, index);
    
}

void Module::update() {
    
}

void Module::addParticle(int life) {
    if (population.size() < maximumPopulation) {
        if (ofGetMouseX() >= modOrigin.x && ofGetMouseY() <=maxWidth && ofGetMouseY() >= modOrigin.y+100 && ofGetMouseY() <= maxHeight/2){
            Particle newParticle(index, population.size(), ofGetMouseX(), ofGetMouseY(), sync, life);
            population.push_back(newParticle);
        }
    }
}

void Module::manageParticles() {
    for(int i=0; i<population.size(); i++){
//        thisParticle = new Particle(population[i]);
         managedParticle = &population[i];
//        Particle thisParticle = (Particle)population.get(i);
        if(console->freeze->state == true){
            if(console->loop->state == true){
                if((managedParticle->counter) >= (managedParticle->life)) {
                    
                    cout << "entrou" << endl;
                    
                population.erase(population.begin() + i);
                managedParticle->yesGravity();
                }
            }
            else{
                managedParticle->noGravity();
            }
        }
        managedParticle->display();
    }
}

void Module::displayConsole() {
     console->display();
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
    for(int i=0; i< population.size(); i++){
//        thisParticle = new Particle(population[i]);
        population.clear();
    }
}