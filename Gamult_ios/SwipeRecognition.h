#import <UIKit/UIKit.h>

#include "ofxiOSExtras.h"

class SwipeRecognition;

@interface ofxSwipeGestureRecognizer : NSObject {
    UISwipeGestureRecognizer *gestureUp;
    UISwipeGestureRecognizer *gestureDown;
    UISwipeGestureRecognizer *gestureLeft;
    UISwipeGestureRecognizer *gestureRight;
    SwipeRecognition *notifier;
    
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
-(void)setNotifier:     (SwipeRecognition *) _notifier;
-(void)swipeUp:         (UISwipeGestureRecognizer *)gUp;
-(void)swipeDown:       (UISwipeGestureRecognizer *)gDown;
-(void)swipeLeft:       (UISwipeGestureRecognizer *)gLeft;
-(void)swipeRight:      (UISwipeGestureRecognizer *)gRight;

@end

class SwipeRecognitionArgs{
public:
    int direction;
    int totalSwipes;
    int swipeOriginY;
};

class SwipeRecognition {
public:
    SwipeRecognition();
    void setup();
    ofxSwipeGestureRecognizer * swipeRecognizer;
    void gestureRecognized(SwipeRecognitionArgs args);
    
    ofEvent<SwipeRecognitionArgs> swipeRecognized;
};
