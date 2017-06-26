#ifndef WRAPPER_H
#define WRAPPER_H

#include "ofMain.h"
#include "SimpleAudioPlayer.h"

class Wrapper {

    public:

        Wrapper();

        void load(string audioPath);

        void play(string name, float volume, float pan);
};


#endif
