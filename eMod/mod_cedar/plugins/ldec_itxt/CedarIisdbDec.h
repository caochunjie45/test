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
    CedarIsubtitleDec��������,��ʵ��isdbDec��������
    ����Isdb�Ľ���.
********************************************************************************
*/
#ifndef _CEDARIISDBDEC_H_
#define _CEDARIISDBDEC_H_

#if PLAY_ISDBT_CLOSECAPTION
typedef struct tag_CedarIisdbDec
{
    CedarIsubtitleDec       base;
    __cedar_subtitle_item_t outputItem; //ר�Ŵ洢Ҫ������Ļ֡,�ṩ��app, pSubtitle���Ⱦͷ����.���Ⱦ͸���nOutputSubtitleSize����.
    __s32                   nOutputSubtitleSize;    //ָʾoutputItem->pSubtitle�ĳ���
    CCedarSubtitleFrame     outputSubtitleFrame; //ר�Ŵ洢Ҫ������Ļ֡,�ṩ��app,ƥ��ڶ���Ҫ֡��ʽ,subData[10]���Ⱦͷ����.���Ⱦ͸���nOutputSubtitleSize����.������outputItem�����ڴ�.
    IsdbDec                 *pIsdbDec;
}CedarIisdbDec;
extern __s32 CedarIisdbDec_Init(CedarIisdbDec *thiz);   //LYRIC_ITXT��
extern __s32 CedarIisdbDec_Exit(CedarIisdbDec *thiz);
extern CedarIisdbDec* newCedarIisdbDec(void);   //LYRIC_ITXT��
extern void deleteCedarIisdbDec(CedarIisdbDec *thiz);

#endif
#endif  /* _CEDARIISDBDEC_H_ */

