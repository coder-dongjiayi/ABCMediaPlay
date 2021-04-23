//
//  pcmencodeaac.c
//  ABCMediaPlay
//
//  Created by 董家祎 on 2021/4/21.
//

#include "pcmencodeaac.h"

static int check_sample_fmt(const AVCodec *codec,enum AVSampleFormat sample_fmt){
    
    const enum AVSampleFormat *p = codec->sample_fmts;
    while (*p != AV_SAMPLE_FMT_NONE) {
        if(*p == sample_fmt) return  1;
        p++;
    }

    return 0;
}
static int encode(AVCodecContext *ctx,
                  AVFrame *frame,
                  AVPacket *pkt,
                  FILE *file){
    
    int ret  = avcodec_send_frame(ctx, frame);
    if(ret < 0){
        
        printf("发送数据失败");
        return ret;
    }
    while (1) {
        //获取编码后的数据
        ret = avcodec_receive_packet(ctx, pkt);
        //如果编码出现错误或者全部编码完成那么返回
        if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
            return 0;
        }else if (ret < 0){
            printf("编码失败，出现其他错误");
            return ret;
        }
        
        fwrite((char*)pkt->data, 1, pkt->size, file);
        
       av_packet_unref(pkt);
    }
    
    
    return 0;
}

int aac_encode(void){
    
    AudioEncodeSpec audioEncodeSpec;
    audioEncodeSpec.chLayout = AV_CH_LAYOUT_STEREO;
    audioEncodeSpec.filename = "44100_s16le_2.pcm";
    audioEncodeSpec.smapleRate = 44100;
    audioEncodeSpec.sampleFmt = AV_SAMPLE_FMT_S16;
    
    printf("home路径%s\n",getenv ("HOME"));
    
    int ret = 0;
    //编码器
    AVCodec *codec = avcodec_find_encoder_by_name("libfdk_aac");
    if(codec == NULL){
        printf("获取解码器失败");
        return -1;
    }
    //检查采样格式 是否符合aac编码标准
    int isFmt = check_sample_fmt(codec, audioEncodeSpec.sampleFmt);
    if( isFmt != 1){
        printf("采样格式错误");
        return  -2;
    }
    
    //上下文
    AVCodecContext *ctx = avcodec_alloc_context3(codec);
    if(ctx == NULL){
        printf("获取编码器上下文失败");
        return -3;
    }
    ctx->sample_rate = audioEncodeSpec.smapleRate;
    ctx->sample_fmt = audioEncodeSpec.sampleFmt;
    ctx->channel_layout = audioEncodeSpec.chLayout;
    
    //比特率
    ctx->bit_rate = 32000;
    //规格
    ctx->profile = FF_PROFILE_AAC_HE_V2;
    
    // 打开编码器
//    AVDictionary *options = nullptr;
//    av_dict_set(&options, "vbr", "5", 0);
//    ret = avcodec_open2(ctx, codec, &options);
    ret = avcodec_open2(ctx, codec, NULL);
    
    if(ret < 0){
        printf("编码器打开失败");
        return -4;
      
    }
    FILE *inFile = fopen(audioEncodeSpec.filename, "r");
    
    FILE *outFile = fopen("in.aac", "w");
    
    if(inFile == NULL){
        printf("inFile 打开失败");
        goto end;
        
    }
    if(outFile == NULL){
        printf("outFile 打开失败");
        goto end;
    }
    
    
    //用来存放编码前的数据
    AVFrame *frame = av_frame_alloc();
    
    if(frame == NULL){
        printf("AVFrame 初始化失败");
        goto end;
        
    }
    frame->nb_samples = ctx->frame_size;
    frame->format = ctx->sample_fmt;
    frame->channel_layout = ctx->channel_layout;
    
    //创建frame缓冲区
    ret = av_frame_get_buffer(frame, 0);
    if(ret < 0){
        printf("缓冲区创建失败");
        goto end;
      
    }
    
    //用来存放编码之后的数据
    AVPacket *pkt = av_packet_alloc();
    if(pkt == NULL){
       
        printf("AVPacket 初始化失败");
        goto end;
       
    }
    
    //从 in.pcm中读取采样数据 把每一个样本进行编码
   
    
    while (( ret = (int)fread((char*)frame->data[0], sizeof(frame->data[0][0]), frame->linesize[0], inFile)) > 0) {
       
//        if(ret < frame->linesize[0]){
//
//
//            int chs = av_get_channel_layout_nb_channels(frame->channel_layout);
//            int bytes = av_get_bytes_per_sample(audioEncodeSpec.sampleFmt);
//            frame->nb_samples = ret / (chs * bytes);
//        }
        
        if(encode(ctx, frame, pkt, outFile) < 0){
            goto end;
        }
    }
    //刷新编码器
    encode(ctx, NULL, pkt, outFile);
    
   
    
end:
  
    fclose(inFile);
    fclose(outFile);
    av_frame_free(&frame);
    av_packet_free(&pkt);
    avcodec_free_context(&ctx);

    return ret;
    
}

