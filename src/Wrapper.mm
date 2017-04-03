#include "Wrapper.h"

Wrapper::Wrapper() {
    
}

void Wrapper::load(string audioPath) {
 
    NSString *oldFilePath = [NSString stringWithCString:audioPath.c_str()
                                            encoding:[NSString defaultCStringEncoding]];
    
    NSString *filePath = [oldFilePath substringToIndex:[oldFilePath length]-4]; // this removes the .wav
    
        [[SimpleAudioPlayer sharedInstance] preloadAudioSample:filePath];

}

void Wrapper::play(string name, float volume, float pan) {
    
    NSString *oldFileName = [NSString stringWithCString:name.c_str()
                                                encoding:[NSString defaultCStringEncoding]];
    
    NSString *fileName = [oldFileName substringToIndex:[oldFileName length]-4];
    
    [[SimpleAudioPlayer sharedInstance] playAudioSample:fileName gain:volume pan:pan];
}
