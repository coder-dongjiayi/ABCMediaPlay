//
//  ViewController.m
//  ABCMediaPlay
//
//  Created by 董家祎 on 2021/3/30.
//

#import "ViewController.h"
//#include "ABCPcmToWav.hpp"
//#include "ABCAduioResample.hpp"
#include "pcmencodeaac.h"
@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

   
//    _recoder = ABCRecoder();
//    _audioPlayer = ABCAudioPlay();
//    _recoderWav = ABCRecoderWAV();
    
}
- (IBAction)pcmEncodeAccAction:(id)sender {
   int ret =  aac_encode();
    if(ret == 0){
        NSLog(@"aac 编码完成");
    }
    
}


- (IBAction)startWavAction:(id)sender {
  //  _recoderWav.startRecoder();
}

- (IBAction)stopWavAction:(id)sender {
   // _recoderWav.stopRecoder();
}


- (IBAction)playAudioAction:(id)sender {
   // _audioPlayer.play();
}

- (IBAction)audioResampleAction:(id)sender {
   // ABCAudioResample().resample();
    
}


- (IBAction)startRecoderAction:(id)sender {
    
    //_recoder.startRecoder();
}

- (IBAction)endRecoderAction:(id)sender {
    
   // _recoder.stopRecoder();
}
- (IBAction)pcmtowavAction:(id)sender {
    //ABCPcmToWav::pcmToWav();
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}


@end
