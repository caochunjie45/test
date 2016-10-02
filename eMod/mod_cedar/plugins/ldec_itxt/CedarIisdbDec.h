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
* File   : CedarIisdbDec.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2012-06-05
* Description:
    CedarIsubtitleDec的派生类,其实是isdbDec的适配类
    负责Isdb的解码.
********************************************************************************
*/
#ifndef _CEDARIISDBDEC_H_
#define _CEDARIISDBDEC_H_

#if PLAY_ISDBT_CLOSECAPTION
typedef struct tag_CedarIisdbDec
{
    CedarIsubtitleDec       base;
    __cedar_subtitle_item_t outputItem; //专门存储要到的字幕帧,提供给app, pSubtitle事先就分配好.长度就根据nOutputSubtitleSize来定.
    __s32                   nOutputSubtitleSize;    //指示outputItem->pSubtitle的长度
    CCedarSubtitleFrame     outputSubtitleFrame; //专门存储要到的字幕帧,提供给app,匹配第二种要帧方式,subData[10]事先就分配好.长度就根据nOutputSubtitleSize来定.决定和outputItem共用内存.
    IsdbDec                 *pIsdbDec;
}CedarIisdbDec;
extern __s32 CedarIisdbDec_Init(CedarIisdbDec *thiz);   //LYRIC_ITXT等
extern __s32 CedarIisdbDec_Exit(CedarIisdbDec *thiz);
extern CedarIisdbDec* newCedarIisdbDec(void);   //LYRIC_ITXT等
extern void deleteCedarIisdbDec(CedarIisdbDec *thiz);

#endif
#endif  /* _CEDARIISDBDEC_H_ */

