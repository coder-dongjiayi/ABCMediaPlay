//
//  ABCPcmToWav.cpp
//  ABCMediaPlay
//
//  Created by 董家祎 on 2021/4/1.
//

#include "ABCPcmToWav.hpp"
#include <fstream>

#include <iostream>
using namespace::std;

void ABCPcmToWav::pcmToWav(){
    cout << "这是home路径:" <<getenv ("HOME")<< endl;
    
    
    //2748416
    WAVHeader header;
    header.riffChunkDataSize = 2748452;
    header.sampleRate = 44100;
    header.bitPerSample = 16;
    header.numChannels = 2;
    header.blockAlign = header.bitPerSample * header.numChannels >> 3;
    header.byteRate = header.sampleRate * header.blockAlign;
    header.dataChunkDataSize = 2748416;
    
    _pcmToWav(header, "out.pcm", "out.wav");
    
}

void ABCPcmToWav::_pcmToWav(WAVHeader &header, const char *pcmFilename, const char *wavFileName){
    
    //打开pcm文件
    ifstream pcmFile;
    pcmFile.open(pcmFilename);
    if(pcmFile.is_open() == false){
        cout << "pcm 文件打开失败"<< endl;
        return;
    }
    
    //打开wav文件
    ofstream wavFile;
    wavFile.open(wavFileName);
    if(wavFile.is_open() == false){
        pcmFile.close();
        cout << "WAV 文件打开失败" << endl;
        return;
    }
    
    //写入头部数据
    wavFile.write((const char*)&header, sizeof(WAVHeader));
    
    //写入pcm数据
    char buf[1024];
    int size;
    while ((size = (int)pcmFile.read(buf, sizeof(buf)).gcount()) > 0) {
        wavFile.write(buf, size);
    }
    pcmFile.close();
    wavFile.close();
    cout << "pcm转wav完成" << endl;
}
