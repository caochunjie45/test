/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_cedar sub-system
*                          (module name, e.g.mpeg4 decoder plug-in) module
*
*          (c) Copyright 2010-2012, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : maple_media_type.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-12-26
* Description:
    有时候maple在[media_type.h]上自己定义了一些媒体类型.主要就是在maple主控和play_control间使用
********************************************************************************
*/
#ifndef _MAPLE_MEDIA_TYPE_H_
#define _MAPLE_MEDIA_TYPE_H_

//#include "media_type.h"

//typedef struct __AUDIO_CODEC_FORMAT
//{
//    __u32   codec_type;                 //* 音频编码器类型
//    __u32   codec_type_sub;             //* 音频编码器类型次级描述
//    __u16   channels;                   //* 音频通道数
//    __u16   bits_per_sample;            //* 每音频采样的比特数
//    __u32   sample_per_second;          //* 音频采样率
//    __u32   avg_bit_rate;               //* 平均码率
//    __u32   max_bit_rate;               //* 最大码率
//
//    unsigned int       file_length;       // 文件总长度
//    __u32   audio_bs_src;               //* 标志音频流从哪个demuxer送出
//    int     is_raw_data_output; //0:pcm output, 1:raw data 
//    __u32   priv_inf_len;               //* 额外信息数据长度
//    void*   private_inf;                //* 额外信息数据起始地址
//}__ad_codec_fmt_t;
typedef __audio_codec_format_t __ad_codec_fmt_t;

//typedef struct __VIDEO_CODEC_FORMAT
//{
//    __u32   codec_type;                 //* 音频编码器类型
//    __u8    ifrm_pts_credible;          //* I帧PTS是否可信
//    __u8    pfrm_pts_credible;          //* P帧PTS是否可信
//    __u8    bfrm_pts_credible;          //* B帧PTS是否可信
//    __u16   width;                      //* 视频源高度
//    __u16   height;                     //* 视频源宽度
//    __u32   frame_rate;                 //* 帧率，（每1000秒播放的视频帧数）
//    __u32   mic_sec_per_frm;            //* 每帧播放时间，从帧率推算
//    __s32   avg_bit_rate;               //* 平均码率
//    __s32   max_bit_rate;               //* 最大码率
//    __u16   video_bs_src;               //* 标志视频流从哪个demuxer送出，TS流设为2表示流格式， CMMB设为0表示帧格式
//    __s16   priv_inf_len;               //* 额外信息数据长度
//    void*   private_inf;                //* 额外信息数据起始地址
//
//}__vd_codec_fmt_t;
typedef __video_codec_format_t __vd_codec_fmt_t;

//typedef struct __SUBTITLE_CODEC_FORMAT
//{
//    __u32   codec_type;
//}__subt_codec_fmt_t;

typedef __subtitle_codec_format_t __subt_codec_fmt_t;

#endif  /* _MAPLE_MEDIA_TYPE_H_ */

