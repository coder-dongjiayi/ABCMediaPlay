//
//  pcmencodeaac.h
//  ABCMediaPlay
//
//  Created by 董家祎 on 2021/4/21.
//

#ifndef pcmencodeaac_h
#define pcmencodeaac_h

#include <stdio.h>
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
typedef struct {
    const char * filename;
    int smapleRate;
    enum AVSampleFormat sampleFmt;
    int chLayout;
    
} AudioEncodeSpec;


int aac_encode(void);

#endif /* pcmencodeaac_h */
