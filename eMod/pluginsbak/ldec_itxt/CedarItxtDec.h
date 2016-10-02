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
* File   : CedarITxtDec.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-10-16
* Description:
    CedarIsubtitleDec的派生类,其实是ItxtDec的适配类
    负责issa, itext, isub的解码.
********************************************************************************
*/
#ifndef _CEDARITXTDEC_H_
#define _CEDARITXTDEC_H_
typedef struct tag_CedarItxtDec
{
    CedarIsubtitleDec       base;
    __cedar_subtitle_item_t outputItem; //专门存储要到的字幕帧,提供给app, pSubtitle事先就分配好.长度就根据nOutputSubtitleSize来定.
    __s32                   nOutputSubtitleSize;    //指示outputItem->pSubtitle的长度
    CCedarSubtitleFrame     outputSubtitleFrame; //专门存储要到的字幕帧,提供给app,匹配第二种要帧方式,pSubtitle事先就分配好.长度就根据nOutputSubtitleSize来定.决定和outputItem共用内存.
    ItxtDec                 *pItxtDec;
}CedarItxtDec;
extern __s32 CedarItxtDec_Init(CedarItxtDec *thiz, __s32 nFileFmt);   //LYRIC_ITXT等
extern __s32 CedarItxtDec_Exit(CedarItxtDec *thiz);
extern CedarItxtDec* newCedarItxtDec(__s32 nFileFmt);   //LYRIC_ITXT等
extern void deleteCedarItxtDec(CedarItxtDec *thiz);

#endif  /* _CEDARITXTDEC_H_ */

