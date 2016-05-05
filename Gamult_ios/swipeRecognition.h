
#import <UIKit/UIKit.h>

#include "ofxiOSExtras.h"

class swipeRecognition;

@interface ofxSwipeGestureRecognizer : NSObject {
    UISwipeGestureRecognizer *gestureUp;
    UISwipeGestureRecognizer *gestureDown;
    UISwipeGestureRecognizer *gestureLeft;
    UISwipeGestureRecognizer *gestureRight;
    swipeRecognition *notifier;
    
@public
    NSInteger              directionUp;
    NSInteger              directionDown;
    NSInteger              directionLeft;
    NSInteger              directionRight;
    NSInteger              swipes;
}

@property(assign,nonatomic) BOOL cancelsTouchesInView;
@property(nonatomic) NSUInteger numberOfTouchesRequired;

-(id)initWithView:(UIView*)view;
-(void)setNotifier:     (swipeRecognition *) _notifier;
-(void)swipeUp:         (UISwipeGestureRecognizer *)gUp;
-(void)swipeDown:       (UISwipeGestureRecognizer *)gDown;
-(void)swipeLeft:       (UISwipeGestureRecognizer *)gLeft;
-(void)swipeRight:      (UISwipeGestureRecognizer *)gRight;

@end

class swipeRecognitionArgs{
public:
    int direction;
    int totalSwipes;
    int swipeOriginY;
};

class swipeRecognition {
public:
    swipeRecognition();
    void setup();
    ofxSwipeGestureRecognizer * swipeRecognizer;
    void gestureRecognized(swipeRecognitionArgs args);
    
    ofEvent<swipeRecognitionArgs> swipeRecognized;
};