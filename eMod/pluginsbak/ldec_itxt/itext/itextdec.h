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
* File   : itextdec.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-9-25
* Description:
    普通的内置文本字幕解码器,一般是utf8编码.
********************************************************************************
*/
#ifndef _ITEXTDEC_H_
#define _ITEXTDEC_H_

#define ITEXTDEC_MAX_LBSBUF_SIZE    (1024 * 2)  //lbsbufsize, 不同的字幕解码器,值不一样
#define ITEXTDEC_MAX_FRM_SIZE       (1024 * 1)  //一帧的长度,对于itext来说,1024字节足够了
#define ITEXTDEC_MAX_ANCI_SIZE      (1024 * 1)  //处理lbsbuffer回头的buffer,对于字符编码的字幕,和frame一样大就够了.


typedef struct tag_ItextDec
{
    ItxtDec base;
}ItextDec;
extern __s32 ItextDec_Init(ItextDec *thiz);
extern __s32 ItextDec_Exit(ItextDec *thiz);
extern ItextDec *newItextDec(void);
extern void deleteItextDec(ItextDec *thiz);

#endif  /* _ITEXTDEC_H_ */

