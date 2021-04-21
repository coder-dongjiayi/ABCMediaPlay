//
//  ABCAduioResample.cpp
//  ABCMediaPlay
//
//  Created by 董家祎 on 2021/4/7.
//

#include "ABCAduioResample.hpp"

#include <iostream>


ABCAudioResample::ABCAudioResample(){
    cout << "这是home路径:" <<getenv ("HOME") << endl;
    this->inFilePath = "44100_s16le_2.pcm";
    this->outFilePath = "48000_f32le_1.pcm";
    
}
ABCAudioResample::~ABCAudioResample(){
    
}

void ABCAudioResample:: free(){
    cout << "释放资源" << endl;
    this->inStream.close();
    this->outStream.close();
    swr_free(&this->swrContext);
}

void ABCAudioResample::resample(){
    
    //输入参数::采样格式
    AVSampleFormat  inSampleFormat = AV_SAMPLE_FMT_S16;
    //采样率
    int insampleRate = 44100;
    //双声道
    int inChLayout = AV_CH_LAYOUT_STEREO;
    

    //输出参数::采样格式
    AVSampleFormat  outSampleFormat = AV_SAMPLE_FMT_FLT;
    //采样率
    int outsampleRate = 48000;
    //单声道
    int outChLayout = AV_CH_LAYOUT_MONO;
    
    int ret = 0;
    //1.创建重采样上下文
    this->swrContext = swr_alloc_set_opts(nullptr, outChLayout, outSampleFormat, outsampleRate, inChLayout, inSampleFormat, insampleRate, 0, nullptr);
    
    if(!this->swrContext){
        cout << "swr_alloc_set_opts 失败" << endl;
        this->free();
        return;
    }
    //2.初始化重采样上下文
    ret = swr_init(this->swrContext);
    if(ret < 0){
        cout << "swr_init 失败" << endl;
        this->free();
        return;
    }
    uint8_t **inData = nullptr;
    int in_linesize = 0;
    //转化枚举声道数量为 int
    int inChannels = av_get_channel_layout_nb_channels(inChLayout);
    
    //缓冲区样本数量
    int inSamples = 1024;
    
    // av_get_bytes_per_sample 这个方法返回的就是 字节大小 所以不需要在 除以8了
    int inBytesPerSample = inChannels * av_get_bytes_per_sample(inSampleFormat);
    
    //3.创建输入缓冲区
    ret = av_samples_alloc_array_and_samples(&inData, &in_linesize, inChannels, inSamples, inSampleFormat, 1);
    if(ret < 0){
        cout << "输入--缓冲区创建失败" << endl;
        this->free();
        return;
    }
    //4.创建输出缓冲区
    
    uint8_t **outData = nullptr;
    int outChannels = av_get_channel_layout_nb_channels(outChLayout);
    int out_linesize = 0;
    int outSamples = (int)av_rescale_rnd(outsampleRate, inSamples, insampleRate, AV_ROUND_UP);
    // av_get_bytes_per_sample 这个方法返回的就是 字节大小 所以不需要在 除以8了
    int outBytesPerSample = outChannels * av_get_bytes_per_sample(outSampleFormat);
    
    ret = av_samples_alloc_array_and_samples(&outData, &out_linesize, outChannels, outSamples, outSampleFormat, 1);
    
    if(ret < 0){
        cout << "输出--缓冲区创建失败" << endl;
        this->free();
        return;
    }
    
    //5.写入缓冲区数据 开始重采样
    this->inStream.open(this->inFilePath);
   
    if(this->inStream.is_open() == false){
        cout << "inFileName 文件打开失败" << endl;
        return;
    }
  
    this->outStream.open(this->outFilePath);
    if(this->outStream.is_open() == false){
        cout << "outFileName 文件打开失败" << endl;
        return;
    }
    //一次读取的文件大小
    int sizeByte = 0;
    
    while ((sizeByte = (int)inStream.read((char*)inData[0], in_linesize).gcount()) > 0) {
        //读取样本数量 = 样本大小/每一个样本的大小
        inSamples = sizeByte / inBytesPerSample;
        
        //重采样
       ret =  swr_convert(this->swrContext, outData, outSamples, (const uint8_t **)inData, inSamples);
        if(ret < 0){
            cout << "swr_convert 失败" << endl;
            if(inData){
                av_freep(&inData[0]);
            }
            av_freep(inData);
            if(outData){
                av_freep(&outData[0]);
            }
            av_freep(outData);
            this->free();
            return;
        }
        
        //转换成功写入文件中
        this->outStream.write((char*)outData[0], ret * outBytesPerSample);
        
    }
    
    //检查缓冲区是否还有残留样本 如果有就写入文件
    while ((ret = swr_convert(this->swrContext,outData,outSamples,nullptr,0)) > 0) {
        this->outStream.write((char*)outData[0], ret * outBytesPerSample);
    }
    //6.释放资源
    
    this->free();
    
    if(inData){
        av_freep(&inData[0]);
    }
    av_freep(inData);
    if(outData){
        av_freep(&outData[0]);
    }
    av_freep(outData);
    cout << "转换完成" << endl;
}


