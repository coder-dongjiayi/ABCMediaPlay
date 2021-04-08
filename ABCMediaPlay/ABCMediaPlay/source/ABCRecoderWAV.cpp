//
//  ABCRecoderWAV.cpp
//  ABCMediaPlay
//
//  Created by 董家祎 on 2021/4/7.
//

#include "ABCRecoderWAV.hpp"
#include <thread>
#include <iostream>
#include <fstream>
extern "C" {


#include "libavcodec/avcodec.h"
#include "libavdevice/avdevice.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"

}
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

using namespace std;
//一边录制pcm 一边转化为wav
ABCRecoderWAV::ABCRecoderWAV(){
    avdevice_register_all();
    cout << "初始化ABCRecoderWAV" << endl;
}

ABCRecoderWAV::~ABCRecoderWAV(){
    cout << "ABCRecoderWAV释放"<< endl;
}
//开始录制pcm
void _start(bool &isStop){
    cout << "这是home路径:" <<getenv ("HOME") << endl;
    
    const char * fmtName = "avfoundation";
    AVInputFormat *fmt =  av_find_input_format(fmtName);
    if(fmt == nullptr){
        cout << "获取设备输入格式失败" << endl;
        return;
    }
    const char * deviceName = ":0";
    AVFormatContext * context = nullptr;
    AVDictionary * options = nullptr;
    
    int ret = avformat_open_input(&context, deviceName, fmt, &options);
    if(ret < 0){
        char errorbuf[1024] = {0};
        av_strerror(ret, errorbuf, sizeof(errorbuf));
        cout << "设备打开失败" << ret << endl;
        return;
    }
    
    ofstream wavFile;
   
    wavFile.open("recoderWav.wav");

    if(wavFile.is_open() == false){
        cout << "recoderWav.wav文件打开失败" << endl;
        return;
    }
    WAVHeader header;
   
    wavFile.write((char*)&header, sizeof(WAVHeader));

  
   
    
    AVPacket pkt;
    while (isStop == false) {
        ret = av_read_frame(context, &pkt);
        if(ret == AVERROR(EAGAIN)){
            continue;
        }
        if(ret == 0){

            wavFile.write((const char*)pkt.data, pkt.size);
        }else{
            char errbuf[1024];
            av_strerror(ret, errbuf, sizeof(errbuf));
            cout << "pkt数据包写入失败"<< endl;
            break;
        }

    }

        
        wavFile.seekp(sizeof(WAVHeader),wavFile.end);

        uint32_t fileSize = (uint32_t)wavFile.tellp();
        uint32_t diff = 36;
    
        wavFile.seekp(0,wavFile.end);
        uint32_t fileSize2 = (uint32_t)wavFile.tellp();

        header.riffChunkDataSize = fileSize + diff;
        header.sampleRate = 44100;
        header.bitPerSample = 16;
        header.numChannels = 2;
        header.blockAlign = header.bitPerSample * header.numChannels >> 3;
        header.byteRate =  header.sampleRate * header.blockAlign;

        header.dataChunkDataSize = fileSize;

        wavFile.seekp(0);
        wavFile.write((char*)&header, sizeof(WAVHeader));

        wavFile.seekp(0,wavFile.end);
        uint32_t fileSize3 = (uint32_t)wavFile.tellp();
    
        wavFile.close();
        avformat_close_input(&context);
        cout << "录音结束"<< endl;
   
}



void ABCRecoderWAV::startRecoder(){
    _isStop = false;
    thread _t(_start,ref(_isStop));
    
    _t.detach();
}

void ABCRecoderWAV::stopRecoder(){
    _isStop = true;
}
