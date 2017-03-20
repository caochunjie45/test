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
    CedarIsubtitleDec��������,��ʵ��ItxtDec��������
    ����issa, itext, isub�Ľ���.
********************************************************************************
*/
#ifndef _CEDARITXTDEC_H_
#define _CEDARITXTDEC_H_
typedef struct tag_CedarItxtDec
{
    CedarIsubtitleDec       base;
    __cedar_subtitle_item_t outputItem; //ר�Ŵ洢Ҫ������Ļ֡,�ṩ��app, pSubtitle���Ⱦͷ����.���Ⱦ͸���nOutputSubtitleSize����.
    __s32                   nOutputSubtitleSize;    //ָʾoutputItem->pSubtitle�ĳ���
    CCedarSubtitleFrame     outputSubtitleFrame; //ר�Ŵ洢Ҫ������Ļ֡,�ṩ��app,ƥ��ڶ���Ҫ֡��ʽ,pSubtitle���Ⱦͷ����.���Ⱦ͸���nOutputSubtitleSize����.������outputItem�����ڴ�.
    ItxtDec                 *pItxtDec;
}CedarItxtDec;
extern __s32 CedarItxtDec_Init(CedarItxtDec *thiz, __s32 nFileFmt);   //LYRIC_ITXT��
extern __s32 CedarItxtDec_Exit(CedarItxtDec *thiz);
extern CedarItxtDec* newCedarItxtDec(__s32 nFileFmt);   //LYRIC_ITXT��
extern void deleteCedarItxtDec(CedarItxtDec *thiz);

#endif  /* _CEDARITXTDEC_H_ */

