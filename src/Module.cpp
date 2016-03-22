#include "ofApp.h"

Module::Module(int index, float x, float y, float width, float height, int maxPopulation, vector<string> soundPaths, vector<string> iconPaths) {

	this->index = index;
	this->x0 = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->maxPopulation = maxPopulation;

	this->console = new ModuleConsole(x0, width, index, iconPaths);

    // util vars
	this->x1 = x + width;
    this->consoleHeight = CONSOLE_HEIGHT*height;

    int w = round(BUTTON_CHANGE_INSTRUMENT_WIDTH * width);
    int h = round(BUTTON_CHANGE_INSTRUMENT_HEIGHT * height);
    int middleY = round((height - consoleHeight)/2 + consoleHeight);
    previousInstrumentRect.set(x0, middleY - h, w, h);
    nextInstrumentRect.set(x1, middleY - h, -w, h);

    ofLog() << "height " << height << endl;
    ofLog() << "getHeight() " << height << endl;

	loadSounds(soundPaths);

  ofAddListener(ofEvents().touchDown,  this, &Module::touchDown);

}

void Module::touchDown(ofTouchEventArgs& event) {
    
    if (previousInstrumentRect.inside(event.x, event.y)) {
        if (index == 0) changeInstrument(3);
        else changeInstrument(--index);
    }
    
    else if (nextInstrumentRect.inside(event.x, event.y)) {
        changeInstrument(++index % 4);
    }
    
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

void Module::changeInstrument(int index) {

    unloadSounds();
    switch (index) {
        case 0:
        {
            vector<string> bonangs;
            bonangs.push_back("sounds/BBPL1.wav");
            bonangs.push_back("sounds/BBPL2.wav");
            bonangs.push_back("sounds/BBPL3.wav");
            bonangs.push_back("sounds/BBPL4.wav");
            loadSounds(bonangs);
        }
            break;
        case 1:
        {
            vector<string> genders;
            genders.push_back("sounds/GBPL1.wav");
            genders.push_back("sounds/GBPL2.wav");
            genders.push_back("sounds/GBPL3.wav");
            genders.push_back("sounds/GBPL5.wav");
            loadSounds(genders);
        }
            break;
        case 2:
        {
            vector<string> gongs;
            gongs.push_back("sounds/GKPL1f.wav");
            gongs.push_back("sounds/GKPL2f.wav");
            gongs.push_back("sounds/GKPL3f.wav");
            gongs.push_back("sounds/GKPL5f.wav");
            loadSounds(gongs);
        }
            break;
        case 3:
        {
            vector<string> sarons;
            sarons.push_back("sounds/SBPL1.wav");
            sarons.push_back("sounds/SBPL2.wav");
            sarons.push_back("sounds/SBPL3.wav");
            sarons.push_back("sounds/SBPL4.wav");
            loadSounds(sarons);
        }
            break;
        default:
        {
            vector<string> bonangs;
            bonangs.push_back("sounds/BBPL1.wav");
            bonangs.push_back("sounds/BBPL2.wav");
            bonangs.push_back("sounds/BBPL3.wav");
            bonangs.push_back("sounds/BBPL4.wav");
            loadSounds(bonangs);
        }
            break;
    }

	cout << "changing instrument" << endl;
}

void Module::addParticle(int life, int x, int y) {
	if (particles.size() < maxPopulation) {
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
    ofSetColor(255 - (30 * index));
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
    ofPushStyle();
    ofSetColor(ofColor::fromHex(BUTTON_CHANGE_INSTRUMENT_COLOR), BUTTON_CHANGE_INSTRUMENT_COLOR_ALPHA);
    ofDrawRectangle(previousInstrumentRect);
    ofDrawRectangle(nextInstrumentRect);
    ofPopStyle();
}

void Module::drawParticles() {
    for (int i = 0; i < particles.size(); i++) {
        particles[i].draw();
    }
}

void Module::playSound(int index, float vol) {
    sounds[index].setVolume(vol);
    sounds[index].play();
}
