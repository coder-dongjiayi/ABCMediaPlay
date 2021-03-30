//
//  ABCRecoder.cpp
//  ABCMediaPlay
//
//  Created by 董家祎 on 2021/3/30.
//

#include <iostream>
#include <thread>
#include <fstream>
#include "ABCRecoder.hpp"

extern "C" {

#include "libavcodec/avcodec.h"
#include "libavdevice/avdevice.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"

}
using namespace std;

ABCRecoder::ABCRecoder(){
    avdevice_register_all();
}

ABCRecoder::~ABCRecoder(){
    cout << "~ABCRecoder 析构了" << endl;
}

void _startRecoder(bool &isStop){
    //mac 下是avfoundation windows 是dshow
    const char *fmtName = "avfoundation";
    
    //1.获取输入设备名称
   AVInputFormat * fmt =  av_find_input_format(fmtName);
    
    if(fmt == nullptr){
        cout << "获取输入格式失败"<< endl;
        return;
    }
    
    const char *deviceName = ":0";
    AVFormatContext * context = nullptr;
    AVDictionary * options = nullptr;
    
    //2.打开设备
   int ret =  avformat_open_input(&context, deviceName, fmt, &options);
    
    if(ret < 0){
        char errorbuf[1024] = {0};
        av_strerror(ret, errorbuf, sizeof(errorbuf));
        cout << "设备打开失败" << ret << endl;
        return;
    }
    
    cout << "这是home路径:" <<getenv ("HOME");
    
    //3. 写入文件
    ofstream file;
    file.open("out.pcm");
    if(file.is_open() == false){
        cout << "out.pcm文件打开失败" << endl;
        return;
    }
    
    
    AVPacket pkt;
    
    while (isStop == false) {
        ret = av_read_frame(context, &pkt);
        if(ret == AVERROR(EAGAIN)){
            continue;
        }
        if (ret == 0) {
            file.write((const char*)pkt.data, pkt.size);
        }else{
            char errbuf[1024];
            av_strerror(ret, errbuf, sizeof(errbuf));
            cout << "pkt数据包写入失败"<< endl;
            break;
        }
    }
    file.close();
    avformat_close_input(&context);
    cout << "录音结束"<< endl;
}

void ABCRecoder:: startRecoder(){
    this->isStop = false;
    
    thread t(_startRecoder,ref(this->isStop));
    
    t.detach();
    
}
void ABCRecoder:: stopRecoder(){
    this->isStop = true;
}
