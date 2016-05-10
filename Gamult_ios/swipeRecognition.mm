
#import "swipeRecognition.h"

#include "ofApp.h"

swipeRecognition::swipeRecognition() { }

//the following code is an adaptation of rjraffa's ofxGestureRecognizer addon

void swipeRecognition::setup(){
    EAGLView *view = ofxiPhoneGetGLView();
    swipeRecognizer = [[ofxSwipeGestureRecognizer alloc] initWithView:view];
    [swipeRecognizer setNotifier:this];
    
};

void swipeRecognition::gestureRecognized(swipeRecognitionArgs args){
    ofNotifyEvent(swipeRecognized, args);
};

@implementation ofxSwipeGestureRecognizer

@synthesize cancelsTouchesInView;
@synthesize numberOfTouchesRequired;

-(id)initWithView:(UIView*)view{
    if((self = [super init])){
        
        gestureUp = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeUp:)];//need to set direction
        gestureDown = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeDown:)];//need to set direction
        gestureLeft = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeLeft:)];//need to set direction
        gestureRight = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeRight:)];//direction is set by default
        
        [gestureUp setDirection:(UISwipeGestureRecognizerDirectionUp)];
        [gestureDown setDirection:(UISwipeGestureRecognizerDirectionDown)];
        [gestureLeft setDirection:(UISwipeGestureRecognizerDirectionLeft)];
        [gestureRight setDirection:(UISwipeGestureRecognizerDirectionRight)];
        
        [view addGestureRecognizer:gestureUp];
        [view addGestureRecognizer:gestureDown];
        [view addGestureRecognizer:gestureLeft];
        [view addGestureRecognizer:gestureRight];
        
        //you can specify the number of touches needed to create a swipe gesture with numberOfTouchesRequired
        gestureUp.cancelsTouchesInView = NO;
        gestureDown.cancelsTouchesInView = NO;
        gestureLeft.cancelsTouchesInView = NO;
        gestureRight.cancelsTouchesInView = NO;
        [gestureUp release];
        [gestureDown release];
        [gestureLeft release];
        [gestureRight release];        
        
    }
    return self;
}

- (void) setNotifier:(swipeRecognition *)_notifier{
    notifier = _notifier;
}


-(void)swipeUp:(UISwipeGestureRecognizer *) gUp {
    
    if (gUp.state == UIGestureRecognizerStateRecognized) {
        NSLog(@"Up Swipe received. \n");//Lets you know this method was called by gesture recognizer.
        NSLog(@"Direction is: %i \n ", gUp.direction);//Lets you know the numeric value of the gesture direction for confirmation (1=up).
        
        //do something for a UP swipe gesture
        
        directionUp = gUp.direction;
        swipes++;
        
        swipeRecognitionArgs args;
        args.direction = directionUp;
        args.totalSwipes = swipes;
        notifier->gestureRecognized(args);
    }
}

-(void)swipeDown:(UISwipeGestureRecognizer *)gDown {
    
    if (gDown.state == UIGestureRecognizerStateRecognized) {
        NSLog(@"Down Swipe received. \n");//Lets you know this method was called by gesture recognizer.
        NSLog(@"Direction is: %i \n", gDown.direction);//Lets you know the numeric value of the gesture direction for confirmation (0=right).
        
        //do something for a down swipe gesture
        directionDown = gDown.direction;
        swipes++;
        
        swipeRecognitionArgs args;
        args.direction = directionDown;
        args.totalSwipes = swipes;
        notifier->gestureRecognized(args);
        
    }
}

-(void)swipeLeft:(UISwipeGestureRecognizer *) gLeft {
    
    if (gLeft.state == UIGestureRecognizerStateRecognized) {
        NSLog(@"Left Swipe received. \n");//Lets you know this method was called by gesture recognizer.
        NSLog(@"Direction is: %i \n", gLeft.direction);//Lets you know the numeric value of the gesture direction for confirmation (1=left).
        
        //do something for a left swipe gesture
        
        EAGLView *view = ofxiPhoneGetGLView();
        CGPoint pt = [gestureLeft locationOfTouch:0 inView:view];
        
        directionLeft = gLeft.direction;
        swipes++;
        
        swipeRecognitionArgs args;
        args.direction = directionLeft;
        args.totalSwipes = swipes;
        args.swipeOriginY = pt.y;
        notifier->gestureRecognized(args);
        
    }
}

-(void)swipeRight:(UISwipeGestureRecognizer *) gRight {
    
    if (gRight.state == UIGestureRecognizerStateRecognized) {
        
        NSLog(@"Right Swipe received. \n");//Lets you know this method was called by gesture recognizer.
        NSLog(@"Direction is: %i \n", gRight.direction);//Lets you know the numeric value of the gesture direction for confirmation (1=right).
        
        //do something for a right swipe gesture.
        
        EAGLView *view = ofxiPhoneGetGLView();
        CGPoint pt = [gestureRight locationOfTouch:0 inView:view];
        directionRight = gRight.direction;
        swipes++;
        
        swipeRecognitionArgs args;
        args.direction = directionRight;
        args.totalSwipes = swipes;
        args.swipeOriginY = pt.y;
        notifier->gestureRecognized(args);
    }
}


-(void)dealloc{
    [gestureUp release];
    [gestureDown release];
    [gestureLeft release];
    [gestureRight release];
    [super dealloc];
}  

@end
