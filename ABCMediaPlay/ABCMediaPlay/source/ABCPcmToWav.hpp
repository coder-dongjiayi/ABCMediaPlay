//
//  ABCPcmToWav.hpp
//  ABCMediaPlay
//
//  Created by 董家祎 on 2021/4/1.
//

#ifndef ABCPcmToWav_hpp
#define ABCPcmToWav_hpp

#include <stdio.h>
typedef unsigned char uint8_t;
typedef unsigned int  uint32_t;
typedef unsigned short  uint16_t;
typedef struct {
    //chunkId
    uint8_t riffChunkId[4] = {'R','I','F','F'};
    
    //RIFF chunk的data大小
    uint32_t riffChunkDataSize;
    
    //fmt WAVE
    uint8_t format[4] = {'W','A','V','E'};
    
    // fmt chunk的data大小
    uint32_t fmChunkDataSize = 16;
    
    //音频编码 1标识pcm 3标识Floating Point
    uint16_t audioFormat = 1;
    
    // 声道数
    uint16_t numChannels;
    
    //采样率
    uint32_t sampleRate;
    
    //字节率
    uint32_t byteRate;
    
    // 一个样本的字节数 = bitsPerSample * numChannels >> 3
    uint16_t blockAlign;
    
    //位深度
    uint16_t bitPerSample;
    
    
    uint8_t dataChunkIf[4] = {'d','a','t','a'};

    // data chunk的data大小：音频数据的总长度，即文件总长度减去文件头的长度(一般是44)
    uint32_t dataChunkDataSize;
    
} WAVHeader;

class ABCPcmToWav {
  
private:
    static void _pcmToWav(WAVHeader &header,
                         const char * pcmFilename,
                         const char * wavFileName
                         );
public:
    static void pcmToWav();
    // pcm 转wav
   
};

#endif /* ABCPcmToWav_hpp */
