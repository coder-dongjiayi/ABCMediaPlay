//
//  ViewController.m
//  ABCMediaPlay
//
//  Created by 董家祎 on 2021/3/30.
//

#import "ViewController.h"
#include "ABCPcmToWav.hpp"
#include "ABCAduioResample.hpp"
@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

   
    _recoder = ABCRecoder();
    _audioPlayer = ABCAudioPlay();
}

- (IBAction)playAudioAction:(id)sender {
    _audioPlayer.play();
}

- (IBAction)audioResampleAction:(id)sender {
    ABCAudioResample().resample();
    
}


- (IBAction)startRecoderAction:(id)sender {
    
    _recoder.startRecoder();
}

- (IBAction)endRecoderAction:(id)sender {
    
    _recoder.stopRecoder();
}
- (IBAction)pcmtowavAction:(id)sender {
    ABCPcmToWav::pcmToWav();
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}


@end
