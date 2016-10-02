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
* File   : CedarIdvbsubDec.h
* Version: V1.0
* By     : chenkeye
* Date   : 2012-11-24
* Description:
    CedarIsubtitleDec的派生类,其实是dvbsubDec的适配类
    负责Dvbsub的解码.
********************************************************************************
*/

#ifndef _CEDARIDVBSUBDEC_H_
#define _CEDARIDVBSUBDEC_H_

#if PLAY_DVBT_DVBSUBTITLE

typedef struct tag_CedarIdvbsubDec
{
    CedarIsubtitleDec       base;
    __cedar_subtitle_item_t outdvbsubItem; //专门存储要到的字幕帧,提供给app, pSubtitle事先就分配好.长度就根据nOutputSubtitleSize来定.
    __s32                   nOutputSubtitleSize;    //指示outputItem->pSubtitle的长度
    CCedarSubtitleFrame     outputSubtitleFrame; //专门存储要到的字幕帧,提供给app,匹配第二种要帧方式,subData[10]事先就分配好.长度就根据nOutputSubtitleSize来定.决定和outputItem共用内存.
    __ldec_idvbsub_t        *pdvbsubDec;
}CedarIdvbsubDec;
extern __s32 CedarIdvbsubDec_Init(CedarIdvbsubDec *thiz);   //LYRIC_ITXT等
extern __s32 CedarIdvbsubDec_Exit(CedarIdvbsubDec *thiz);
extern CedarIdvbsubDec* newCedarIdvbsubDec(void);   //LYRIC_ITXT等
extern void deleteCedarIdvbsubDec(CedarIdvbsubDec *thiz);

#endif  /* _CEDARIISDBDEC_H_ */

#endif
