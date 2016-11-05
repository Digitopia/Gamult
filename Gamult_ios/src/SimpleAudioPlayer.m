//
//  simpleAudioPlayer.m
//  simpleAudioPlayer
//
//  Created by Oscar Rodrigues on 08/09/16.
//  Copyright © 2016 Oscar Rodrigues. All rights reserved.
//

#import "SimpleAudioPlayer.h"

#define kSampleRate 44100

#define kMaxConcurrentSources 32
#define kMaxBuffers 256

#define kDefaultGain 1.0f
#define kDefaultDistance 25.0f

@implementation SimpleAudioPlayer

/* The device we are using */
static ALCdevice *openALDevice;

/* The context we are using */
static ALCcontext *openALContext;

static NSMutableArray *audioSampleSources;
static NSMutableDictionary *audioSampleBuffers;

#pragma mark Singleton Methods

+ (SimpleAudioPlayer *) sharedInstance
{
    static SimpleAudioPlayer *_sharedInstance;
    if(!_sharedInstance)
    {
        static dispatch_once_t oncePredicate;
        dispatch_once(&oncePredicate, ^{
            _sharedInstance = [[super allocWithZone:nil] init];
            
        });
    }
    
    return _sharedInstance;
}



+ (id)allocWithZone:(NSZone *)zone
{
    return [self sharedInstance];
}

- (id)copyWithZone:(NSZone *)zone
{
    return self;
}

- (id)init
{
    self = [super init];
    if (self)
    {
        /* Initialise audio sample buffers and sources. */
        audioSampleBuffers = [[NSMutableDictionary alloc] init];
        audioSampleSources = [[NSMutableArray alloc] init];
        
        /* Initialise OpenAL. */
        BOOL result = [self initOpenAL];
        if (result)
        {
            return self;
        }
    }
    return nil;
}

- (BOOL) initOpenAL
{
    /* Setup the Audio Session and monitor interruptions */
    AudioSessionInitialize(NULL, NULL, AudioInterruptionListenerCallback, NULL);
    
    /* Set the category for the Audio Session */
    UInt32 session_category = kAudioSessionCategory_MediaPlayback;
    AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(session_category), &session_category);
    
    /* Make the Audio Session active */
    AudioSessionSetActive(true);
    
    /* The device is a physical thing, like a sound card.
     'NULL' indicates that we want the default device
     */
    openALDevice = alcOpenDevice(NULL);
    
    if (openALDevice)
    {
        /* The context is used to track the state of OpenAL.
         We need to create a single context and associate
         it with the device.
         */
        openALContext = alcCreateContext(openALDevice, NULL);
        
        /* Set the context we just created to the current context.
         Note: we will need monitor app state to manage the current context
         */
        alcMakeContextCurrent(openALContext);
        
        /* Generate the sound sources which will be used to play concurrent sounds. */
        NSUInteger sourceID;
        for (int i = 0; i < kMaxConcurrentSources; i++) {
            /* Create a single OpenAL source */
            alGenSources(1, &sourceID);
            /* Add the source to the audioSampleSources array */
            [audioSampleSources addObject:[NSNumber numberWithUnsignedInt:sourceID]];
        }
        
        /* OpenAL has been setup successfully */
        return YES;
    }
    
    /* Something has gone wrong with our OpenAL setup */
    return NO;
}
#pragma mark Audio Interruption Handelling

/* Monitor interruptions to the application so that we can manage state.
 
 @param user_data Data that is passed back to the callback listener
 @param interruption_state The type of Audio Session interruption
 */
void AudioInterruptionListenerCallback(void* user_data, UInt32 interruption_state)
{
    if (kAudioSessionBeginInterruption == interruption_state)
    {
        /* There is no need to deactivate the Audio Session. This happens automatically.
         We do have to make sure we give up the context.
         */
        alcMakeContextCurrent(NULL);
    }
    else if (kAudioSessionEndInterruption == interruption_state)
    {
        /* After the interruption is over, reactivate our Audio Session
         and take back the context.
         */
        AudioSessionSetActive(true);
        alcMakeContextCurrent(openALContext);
    }
}

#pragma mark Preload Audio Sample Methods

- (void) preloadAudioSample:(NSString *)sampleName
{
    /* Check if the sample has already been loaded.
     If the sample exists, return.
     */
    if ([audioSampleBuffers objectForKey:sampleName])
    {
        return;
    }
    
    /* Check how many buffers have been generated.
     If there are more than the maximum allowed, return.
     */
    if ([audioSampleBuffers count] > kMaxBuffers) {
        NSLog(@"Warning: You are trying to create more than 256 buffers! This is not allowed.");
        return;
    }
    
    /* Get a reference to the audio file. Note: we are only dealing with .wav files. */
    NSString *audioFilePath = [[NSBundle mainBundle] pathForResource:sampleName ofType:@"wav"];
    
    /* Open the audio file */
    AudioFileID afid = [self openAudioFile:audioFilePath];
    
    /* Get the size of the audio data */
    UInt32 audioFileSizeInBytes = [self getSizeOfAudioComponent:afid];
    
    /* Read the audio data and place it into an output buffer. */
    void *audioData = malloc(audioFileSizeInBytes);
    /* false means we don't want the data cached.
     0 means read from the beginning.
     bytesRead will end up containing the actual number of bytes read.
     */
    OSStatus readBytesResult = AudioFileReadBytes(afid, false, 0, &audioFileSizeInBytes, audioData);
    
    if (0 != readBytesResult)
    {
        NSLog(@"An error occurred when attempting to read data from audio file %@: %ld", audioFilePath, readBytesResult);
    }
    
    /* We are done with the AudioFileID. Close it. */
    AudioFileClose(afid);
    
    /* Create a buffer to hold the audio data. */
    ALuint outputBuffer;
    alGenBuffers(1, &outputBuffer);
    
    /* Now, copy the audio data into the output buffer. */
    alBufferData(outputBuffer, AL_FORMAT_MONO16, audioData, audioFileSizeInBytes, kSampleRate);
    
    /* We can now keep a reference to our output buffer ID. */
    [audioSampleBuffers setObject:[NSNumber numberWithInt:outputBuffer] forKey:sampleName];
    
    /* Finally, do some clean up. */
    if (audioData)
    {
        free(audioData);
        audioData = NULL;
    }
}

/* Open the audio file and return an AudioFileID
 
 @param audioFilePathAsString A string representing the file path the the audio sample.
 
 @return AudioFileID An opaque data type that represents an audio file object.
 */
- (AudioFileID) openAudioFile:(NSString *)audioFilePathAsString
{
    /* Convert the string into a URL. */
    NSURL *audioFileURL = [NSURL fileURLWithPath:audioFilePathAsString];
    
    /* Open the audio file and read in the data to an AudioFileID.
     
     CFURLRef inFileRef = the file URL <- Note: __bridge is used for ARC
     SInt8 inPermissions = the permissions used for opening the file
     AudioFileTypeID inFileTypeHing = a hint for the file type. Note: '0' indicates that we are not providing a hint
     AudioFileID *outAudioFile = reference to the audio file
     */
    AudioFileID afid;
    OSStatus openAudioFileResult = AudioFileOpenURL((__bridge CFURLRef)audioFileURL, kAudioFileReadPermission, 0, &afid);
    
    /* Check to make sure the file opened properly. */
    if (0 != openAudioFileResult)
    {
        NSLog(@"An error occurred when attempting to open the audio file %@: %ld", audioFilePathAsString, openAudioFileResult);
        
    }
    return afid;
}

/* Determine the size of the audio file.
 
 @param AudioFileID A valid audio file object.
 
 @return UInt32 The size of the file in bytes.
 */
- (UInt32) getSizeOfAudioComponent:(AudioFileID)afid
{
    /* With the audio file open, get the file size. Note: the audio
     file contains a lot of information in addition to the actual
     audio. We only want to know how large the audio portion of
     the file is.
     
     when getting properties, you provide a reference to a variable
     containing the size of the property value. this variable is then
     set to the actual size of the property value.
     */
    UInt64 audioDataSize = 0;
    UInt32 propertySize = sizeof(UInt64);
    
    OSStatus getSizeResult = AudioFileGetProperty(afid, kAudioFilePropertyAudioDataByteCount, &propertySize, &audioDataSize);
    
    if (0 != getSizeResult)
    {
        NSLog(@"An error occurred when attempting to determine the size of audio file.");
    }
    
    return (UInt32)audioDataSize;
}

#pragma mark Play Audio Sample Methods

- (void) playAudioSample:(NSString *)sampleName
{
    /* Play the sample with the default pitch and gain. */
    [self playAudioSample:sampleName gain:kDefaultGain pan:0.0f];
}

- (void) playAudioSample:(NSString *)sampleName gain:(float)gain
{
    /* Play the sample with the default pitch and specified gain. */
    [self playAudioSample:sampleName gain:gain pan:0.0f];
}
- (void) playAudioSample:(NSString *)sampleName gain:(float)gain pan:(float)pan
{
    /* Buffers contain audio data, sources play the data.
     To play an audio sample, we need to attach a buffer to a source.
     Get the next available source
     */
    ALuint source = [self getNextAvailableSource];
    
    float sourcePosAL[] = {pan, kDefaultDistance, pan};
    
    /* Set the source parameters */
    alSourcef(source, AL_GAIN, gain);
    alSourcefv(source, AL_POSITION, sourcePosAL);
    
    //float listenerPosAL[] = {0., 0., 0.};
    // Move our listener coordinates
   // alListenerfv(AL_POSITION, listenerPosAL);
    
    /* Retrieve the buffer ID we generated when preloading the sample. */
    ALuint outputBuffer = (ALuint)[[audioSampleBuffers objectForKey:sampleName] intValue];
    
    alSourcef(source, AL_REFERENCE_DISTANCE, 50.0f);
    
    /* Attach the buffer to a source. */
    alSourcei(source, AL_BUFFER, outputBuffer);
    
    /* Now play the audio sample. */
    alSourcePlay(source);
}

- (ALuint) getNextAvailableSource
{
    /* Our aim is to find the first source that is not current
     play any audio sample. To do this, we will query to state
     of each source in the audioSampleSources array
     */
    
    ALint sourceState;
    for (NSNumber *sourceID in audioSampleSources) {
        alGetSourcei([sourceID unsignedIntValue], AL_SOURCE_STATE, &sourceState);
        if (sourceState != AL_PLAYING)
        {
            return [sourceID unsignedIntValue];
        }
    }
    
    /* If we do not find an unused source, use the first source in the audioSampleSources array. */
    ALuint sourceID = [[audioSampleSources objectAtIndex:0] unsignedIntegerValue];
    alSourceStop(sourceID);
    return sourceID;
}

#pragma mark OpenAL Shutdown

- (void) shutdownAudioSamplePlayer
{
    /* Stop playing any sounds and delete the sources */
    ALint source;
    for (NSNumber *sourceValue in audioSampleSources)
    {
        NSUInteger sourceID = [sourceValue unsignedIntValue];
        alGetSourcei(sourceID, AL_SOURCE_STATE, &source);
        alSourceStop(sourceID);
        alDeleteSources(1, &sourceID);
    }
    [audioSampleSources removeAllObjects];
    
    /* Delete the buffers */
    NSArray *bufferIDs = [audioSampleBuffers allValues];
    for (NSNumber *bufferValue in bufferIDs)
    {
        NSUInteger bufferID = [bufferValue unsignedIntValue];
        alDeleteBuffers(1, &bufferID);
    }
    [audioSampleBuffers removeAllObjects];
    
    /* Give up the context */
    alcDestroyContext(openALContext);
    
    /* Close the device */
    alcCloseDevice(openALDevice);
}

@end
