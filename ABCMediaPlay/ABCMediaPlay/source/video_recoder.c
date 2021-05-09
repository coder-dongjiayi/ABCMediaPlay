//
//  recode_video.c
//  ABCMediaPlay
//
//  Created by 董家祎 on 2021/5/8.
//

#include "video_recoder.h"

#include "libavcodec/avcodec.h"
#include "libavdevice/avdevice.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavutil/imgutils.h"

#include <pthread/pthread.h>

int *run(void){
    //获取输入格式对象
    AVInputFormat *fmt =  av_find_input_format("avfoundation");
    if(!fmt){
        printf("fmt获取失败");
        return  (void *)0;
    }
    //定义操作上下文
    AVFormatContext * ctx = NULL;
    
    //录制参数
    AVDictionary * options = NULL;
    av_dict_set(&options, "video_size", "640x480", 0);
    av_dict_set(&options,"pixel_format","yuyv422",0);
    av_dict_set(&options,"framerate","30",0);
    
    //打开设备
    int ret =  avformat_open_input(&ctx, "0", fmt, &options);
    
    if(ret < 0){
        printf("设备打开失败");
        return  (void *)0;
    }
    FILE *inFile = fopen("video.yuv", "wt");
    if (inFile == NULL) {
        printf("文件打开失败");
        avformat_close_input(&ctx);
        return  (void *)0;
    }
    
    // 计算一帧的大小
    AVCodecParameters *parametes = ctx->streams[0]->codecpar;
    
    enum AVPixelFormat pixFormat = parametes->format;
    
   //int imgSize2 = av_image_get_buffer_size(pixFormat, parametes->width, parametes->height, 1);
    
    int pixSize = av_get_bits_per_pixel(av_pix_fmt_desc_get(pixFormat)) >> 3;
    int pixNumber = parametes->width * parametes->height;
    
    AVPacket * pkt = av_packet_alloc();
    while (1) {
       ret = av_read_frame(ctx, pkt);
        //读取成功
        if (ret == 0) {
            fwrite((const char*)pkt->data, pixSize, pixNumber, inFile);
            av_packet_unref(pkt);
        }else if (ret == AVERROR(EAGAIN)){
            continue;
        }else { // 其他错误
            printf("其他错误");
           
            break;
        }
    }
    
    av_packet_free(&pkt);
    fclose(inFile);
    avformat_close_input(&ctx);
    return (void *)0;
}

void  recoder_video(){
    avdevice_register_all();
    pthread_t t1;
    pthread_create(&t1, NULL, (void*)run, NULL);
    pthread_detach(t1);

    printf("%s",getenv ("HOME"));
    
    
}

