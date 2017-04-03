#include "ofApp.h"

bool ofxImage::load(string fn) {

    string fnCopy = fn;
    string needle = ".png"; // NOTE: change later to proper regex

    if (ofApp::language != "pt") {
        string fnLocalized = fnCopy.replace(fn.find(needle), needle.size(), "_en"+needle);
        if (ofFile::doesFileExist(fnLocalized)) fn = fnLocalized;
    }

    return ofImage::load(fn);
}
