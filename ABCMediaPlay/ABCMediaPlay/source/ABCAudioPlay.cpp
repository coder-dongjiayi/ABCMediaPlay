//
//  ABCAudioPlay.cpp
//  ABCMediaPlay
//
//  Created by 董家祎 on 2021/3/30.
//

#include "ABCAudioPlay.hpp"
#include "SDL2/SDL.h"
#include <thread>
#include <iostream>
#include <fstream>
#define SAMPLE_FORMAT AUDIO_F32
#define SAMPLE_SIZE SDL_AUDIO_BITSIZE(SAMPLE_FORMAT)
#define CHANNELS 2
// 音频缓冲区的样本数量
#define SAMPLES 1024

#define SAMPLE_RATE 44100


// 每个样本占用多少个字节
#define BYTES_PER_SAMPLE ((SAMPLE_SIZE * CHANNELS) / 8)
// 文件缓冲区的大小
#define BUFFER_SIZE (SAMPLES * BYTES_PER_SAMPLE)

using namespace std;

typedef  struct{
    int len = 0;
    
    int pullLen = 0;
    
    Uint8 *data = nullptr;
    
} AudioBuffer;

// 音频设备回调(会回调多次)
void pull_audio_data(void *userdata,
                     // 需要往stream中填充PCM数据
                     Uint8 *stream,
                     // 希望填充的大小(samples * format * channels / 8)
                     int len
                     ) {
    
    //清空strem
    SDL_memset(stream,0,len);
    
    //取出buffer
    AudioBuffer *buffer = (AudioBuffer *)userdata;
    
    if(buffer->len <= 0) return;
    
    //取出 buffer的最小值 防止指针越界
    buffer->pullLen =  (len > buffer->len) ? buffer->len : len;
    
    //填充数据
    SDL_MixAudio(stream, buffer->data, buffer->pullLen, SDL_MIX_MAXVOLUME);
    
    buffer->data += buffer->pullLen;
    buffer->len -= buffer->pullLen;
}
void _play(){
    //1.初始化SDL  audio系统
    if(SDL_Init(SDL_INIT_AUDIO)){
        cout << "SDL 初始化失败" << SDL_GetError();
        return;
    }
    //2.设置音频参数
    SDL_AudioSpec audioSpec;
    //采样率
    audioSpec.freq = SAMPLE_RATE;
    //采样格式
    audioSpec.format = SAMPLE_FORMAT;
    //声道数量
    audioSpec.channels = CHANNELS;
    //音频缓冲区的样本数量
    audioSpec.samples = SAMPLES;
    //设置音频回调
    audioSpec.callback = pull_audio_data;
    
    AudioBuffer buffer;
    audioSpec.userdata = &buffer;
    
    //3.打开设备
    if(SDL_OpenAudio(&audioSpec, nullptr)){
        
        cout << "设备打开失败" << endl;
        SDL_Quit();
        return;;
    }
    //4.打开文件
    ifstream file;
    file.open("out.pcm");
    if(file.is_open() == false){
        cout << "文件打开失败" << endl;
        SDL_CloseAudio();
        SDL_Quit();
        return;
    }
    // 0 播放 1 暂停
    SDL_PauseAudio(0);
    
    Uint8 data[BUFFER_SIZE];
    
    while (true) {
        //只要从文件中读取的音频数据还没有填充完毕 就继续
        if(buffer.len > 0) continue;;
        
        //读取data数据
        buffer.len = (int)file.read((char*)data, BUFFER_SIZE).gcount();
        
        //文件已经读取完毕
        if(buffer.len <= 0){
            int samples = buffer.pullLen / BYTES_PER_SAMPLE;
            int ms = samples * 1000 / SAMPLE_RATE;
            SDL_Delay(ms);
            break;
        }
        buffer.data = data;
        
    }
    file.close();
    
    SDL_CloseAudio();
    
    SDL_Quit();
    
}


void ABCAudioPlay::play(){
    thread t(_play);
    
    t.detach();
}
