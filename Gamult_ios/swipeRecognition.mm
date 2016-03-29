//
//  UISwipeGestureRecognizer+swipeRecognition.m
//  Gamult_ios
//
//  Created by Oscar Rodrigues on 29/03/16.
//
//

#import "swipeRecognition.h"

#include "ofApp.h"

@implementation UISwipeGestureRecognizer (swipeRecognition)

-(void)viewDidLoad
{
    
    NSLog(@"STARTING viewDidLoad");
    

   /* UISwipeGestureRecognizer *swipeLeft = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(didSwipe:)];
    swipeLeft.direction = UISwipeGestureRecognizerDirectionLeft;
    [self.view addGestureRecognizer:swipeLeft];

    UISwipeGestureRecognizer *swipeRight = [[UISwipeGestureRecognizer alloc] initWithTarget:self  action:@selector(didSwipe:)];
    swipeRight.direction = UISwipeGestureRecognizerDirectionRight;
    [self.view addGestureRecognizer:swipeRight];*/
}

/*- (void)didSwipe:(UISwipeGestureRecognizer*)swipe{
    
    if (swipe.direction == UISwipeGestureRecognizerDirectionLeft) {
        
    } else if (swipe.direction == UISwipeGestureRecognizerDirectionRight) {
        
    }
}*/

- (void) didSwipe:(UISwipeGestureRecognizer *)recognizer{
    if([recognizer direction] == UISwipeGestureRecognizerDirectionLeft){
        NSLog(@"Swipe Left");
    }else{
        NSLog(@"Swipe Right");
    }
}

@end
