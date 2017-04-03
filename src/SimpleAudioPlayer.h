#import <Foundation/Foundation.h>

#import <OpenAl/al.h>
#import <OpenAl/alc.h>
#include <AudioToolbox/AudioToolbox.h>

@interface SimpleAudioPlayer : NSObject

+ (SimpleAudioPlayer*) sharedInstance;

-(void) preloadAudioSample:(NSString *)sampleName;

-(void) playAudioSample:(NSString *)sampleName;

-(void) playAudioSample:(NSString *)sampleName gain:(float)gain;

-(void) playAudioSample:(NSString *)sampleName gain:(float)gain pan:(float)pan;

-(void) shutdownAudioSamplePlayer;

@end
