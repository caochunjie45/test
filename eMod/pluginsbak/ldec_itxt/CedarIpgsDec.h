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
* File   : CedarIpgsDec.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-10-18
* Description:
    CedarIsubtitleDec的派生类,其实是pgsDec的适配类
    负责pgs字幕的解码.
********************************************************************************
*/
#ifndef _CEDARIPGSDEC_H_
#define _CEDARIPGSDEC_H_

typedef struct tag_CedarIpgsDec
{
    CedarIsubtitleDec       base;
    __cedar_subtitle_item_t outItem; //专门存储要到的字幕帧,提供给app, pSubtitle可以赋值为pBitmapData，也可以是pPgsDec内部的buffer,随机的.
    __u8*                   pBitmapData;    //malloc内存, 存储一帧的scaledown之后的内容,如果要做scaledown的话,就是用它,否则还是用它,但交换内存.
    __s32                   nBitmapDataSize;    //指示outputItem->pSubtitle的长度
    __s32                   screenWidth;    //显示窗口的宽高, g_oalCtx->screenWidth  = aux>>16;
    __s32                   screenHeight;
    CCedarSubtitleFrame     outputSubtitleFrame; //专门存储要到的字幕帧,提供给app,匹配第二种要帧方式,pSubtitle和outItem保持一致就可以了.
    PgsDecInterface         *pPgsDec;
}CedarIpgsDec;
extern __s32 CedarIpgsDec_Init(CedarIpgsDec *thiz);   //LYRIC_ITXT等
extern __s32 CedarIpgsDec_Exit(CedarIpgsDec *thiz);
extern CedarIpgsDec* newCedarIpgsDec(void);   //LYRIC_ITXT等
extern void deleteCedarIpgsDec(CedarIpgsDec *thiz);

#endif  /* _CEDARIPGSDEC_H_ */

