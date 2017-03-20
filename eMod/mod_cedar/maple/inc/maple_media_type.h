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
    ��ʱ��maple��[media_type.h]���Լ�������һЩý������.��Ҫ������maple���غ�play_control��ʹ��
********************************************************************************
*/
#ifndef _MAPLE_MEDIA_TYPE_H_
#define _MAPLE_MEDIA_TYPE_H_

//#include "media_type.h"

//typedef struct __AUDIO_CODEC_FORMAT
//{
//    __u32   codec_type;                 //* ��Ƶ����������
//    __u32   codec_type_sub;             //* ��Ƶ���������ʹμ�����
//    __u16   channels;                   //* ��Ƶͨ����
//    __u16   bits_per_sample;            //* ÿ��Ƶ�����ı�����
//    __u32   sample_per_second;          //* ��Ƶ������
//    __u32   avg_bit_rate;               //* ƽ������
//    __u32   max_bit_rate;               //* �������
//
//    unsigned int       file_length;       // �ļ��ܳ���
//    __u32   audio_bs_src;               //* ��־��Ƶ�����ĸ�demuxer�ͳ�
//    int     is_raw_data_output; //0:pcm output, 1:raw data 
//    __u32   priv_inf_len;               //* ������Ϣ���ݳ���
//    void*   private_inf;                //* ������Ϣ������ʼ��ַ
//}__ad_codec_fmt_t;
typedef __audio_codec_format_t __ad_codec_fmt_t;

//typedef struct __VIDEO_CODEC_FORMAT
//{
//    __u32   codec_type;                 //* ��Ƶ����������
//    __u8    ifrm_pts_credible;          //* I֡PTS�Ƿ����
//    __u8    pfrm_pts_credible;          //* P֡PTS�Ƿ����
//    __u8    bfrm_pts_credible;          //* B֡PTS�Ƿ����
//    __u16   width;                      //* ��ƵԴ�߶�
//    __u16   height;                     //* ��ƵԴ���
//    __u32   frame_rate;                 //* ֡�ʣ���ÿ1000�벥�ŵ���Ƶ֡����
//    __u32   mic_sec_per_frm;            //* ÿ֡����ʱ�䣬��֡������
//    __s32   avg_bit_rate;               //* ƽ������
//    __s32   max_bit_rate;               //* �������
//    __u16   video_bs_src;               //* ��־��Ƶ�����ĸ�demuxer�ͳ���TS����Ϊ2��ʾ����ʽ�� CMMB��Ϊ0��ʾ֡��ʽ
//    __s16   priv_inf_len;               //* ������Ϣ���ݳ���
//    void*   private_inf;                //* ������Ϣ������ʼ��ַ
//
//}__vd_codec_fmt_t;
typedef __video_codec_format_t __vd_codec_fmt_t;

//typedef struct __SUBTITLE_CODEC_FORMAT
//{
//    __u32   codec_type;
//}__subt_codec_fmt_t;

typedef __subtitle_codec_format_t __subt_codec_fmt_t;

#endif  /* _MAPLE_MEDIA_TYPE_H_ */

