//
//  ViewController.h
//  ABCMediaPlay
//
//  Created by 董家祎 on 2021/3/30.
//

#import <Cocoa/Cocoa.h>
#include "ABCRecoder.hpp"
#include "ABCAudioPlay.hpp"
@interface ViewController : NSViewController
{
    ABCRecoder _recoder;
    
    ABCAudioPlay _audioPlayer;
}

@end

