//
//  simpleAudioPlayer.h
//  simpleAudioPlayer
//
//  Created by Oscar Rodrigues on 08/09/16.
//  Copyright © 2016 Oscar Rodrigues. All rights reserved.
//

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
