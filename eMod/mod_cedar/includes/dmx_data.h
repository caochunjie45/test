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
* File   : dmx_data.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-12-21
* Description:
    用来定义dmx和后面的player各插件之间传输数据的数据结构
********************************************************************************
*/
#ifndef _DMX_DATA_H_
#define _DMX_DATA_H_

#define PES_PRIVATE_DATA_LEN 16


typedef struct __MEDIA_BUFFER
{
    __u8*   buf;
    __u32   bufSize;
}__md_buf_t;


typedef struct __MEDIA_DATA_INFO
{
    __s64   pts;    //dmx传输过来的数据,时间单位为ms
    __u32   dataLen;
    __u32   ctrlBits;   //PTS_VALID_BIT等
    __u8    privateData[PES_PRIVATE_DATA_LEN];
}__md_data_info_t;

//潜规则:必须和psr_2_vdec.h以及psr_2_adec.h的定义值一样.否则出错.现在正好是一样的
#define PTS_VALID_BIT           (1<<1)  //* whether pts is valid.
#define FIRST_PART_BIT          (1<<3)  //* media data is the first part of a new frame.
#define LAST_PART_BIT           (1<<4)  //* media data is the last part of a frame.
#define RANDOM_ACCESS_FRAME_BIT (1<<14)     //* random access point of stream.原本设计给h264 decode使用,表示本标记位置1的数据为I帧,可以从这里开始解码.目前没有使用

#endif  /* _DMX_DATA_H_ */

