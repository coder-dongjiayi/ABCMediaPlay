//
//  ABCAduioResample.hpp
//  ABCMediaPlay
//
//  Created by 董家祎 on 2021/4/7.
//

#ifndef ABCAduioResample_hpp
#define ABCAduioResample_hpp
#include <fstream>
#include <stdio.h>
extern "C" {
#include "libavcodec/avcodec.h"
#include "libswresample/swresample.h"
#include "libavutil/avutil.h"
}
using namespace std;
///音频重采样
class ABCAudioResample{
    
private:
    void free();
    
    ifstream inStream;
    
    ofstream outStream;
    
    string inFilePath;
    string outFilePath;
    
    SwrContext * swrContext = nullptr;
public:
    ABCAudioResample();
    ~ABCAudioResample();
    void resample();
};



#endif /* ABCAduioResample_hpp */
