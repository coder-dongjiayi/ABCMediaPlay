//
//  ViewController.m
//  ABCMediaPlay
//
//  Created by 董家祎 on 2021/3/30.
//

#import "ViewController.h"

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    _recoder = ABCRecoder();
}

- (IBAction)startRecoderAction:(id)sender {
    
    _recoder.startRecoder();
}

- (IBAction)endRecoderAction:(id)sender {
    
    _recoder.stopRecoder();
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}


@end
