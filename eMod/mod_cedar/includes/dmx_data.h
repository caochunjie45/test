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
    ��������dmx�ͺ����player�����֮�䴫�����ݵ����ݽṹ
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
    __s64   pts;    //dmx�������������,ʱ�䵥λΪms
    __u32   dataLen;
    __u32   ctrlBits;   //PTS_VALID_BIT��
    __u8    privateData[PES_PRIVATE_DATA_LEN];
}__md_data_info_t;

//Ǳ����:�����psr_2_vdec.h�Լ�psr_2_adec.h�Ķ���ֵһ��.�������.����������һ����
#define PTS_VALID_BIT           (1<<1)  //* whether pts is valid.
#define FIRST_PART_BIT          (1<<3)  //* media data is the first part of a new frame.
#define LAST_PART_BIT           (1<<4)  //* media data is the last part of a frame.
#define RANDOM_ACCESS_FRAME_BIT (1<<14)     //* random access point of stream.ԭ����Ƹ�h264 decodeʹ��,��ʾ�����λ��1������ΪI֡,���Դ����￪ʼ����.Ŀǰû��ʹ��

#endif  /* _DMX_DATA_H_ */

