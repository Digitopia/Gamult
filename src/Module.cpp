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
            Particle thisParticle(index,population.size(), ofGetMouseX(), ofGetMouseY(), sync, life);
            population.push_back(thisParticle);
        }
    }
}

void Module::manageParticles() {
    for(int i=0; i<population.size(); i++){
        thisParticle = new Particle(population[i]);
//        Particle thisParticle = (Particle)population.get(i);
        if(console->freeze->state == true){
            if(console->loop->state == true){
                if(thisParticle->counter >= thisParticle->life) {
                    
                population.erase(population.begin() + i);
                thisParticle->yesGravity();
                }
            }
            else{
                thisParticle->noGravity();
            }
        }
        thisParticle->display();
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
   /* for(int i=0; i<=population.size()-1; i++){
        Particle thisParticle = (Particle)population.get(i);
        population.clear();
        
        
    }
*/
}