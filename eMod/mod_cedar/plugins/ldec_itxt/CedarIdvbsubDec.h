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
    CedarIsubtitleDec��������,��ʵ��dvbsubDec��������
    ����Dvbsub�Ľ���.
********************************************************************************
*/

#ifndef _CEDARIDVBSUBDEC_H_
#define _CEDARIDVBSUBDEC_H_

#if PLAY_DVBT_DVBSUBTITLE

typedef struct tag_CedarIdvbsubDec
{
    CedarIsubtitleDec       base;
    __cedar_subtitle_item_t outdvbsubItem; //ר�Ŵ洢Ҫ������Ļ֡,�ṩ��app, pSubtitle���Ⱦͷ����.���Ⱦ͸���nOutputSubtitleSize����.
    __s32                   nOutputSubtitleSize;    //ָʾoutputItem->pSubtitle�ĳ���
    CCedarSubtitleFrame     outputSubtitleFrame; //ר�Ŵ洢Ҫ������Ļ֡,�ṩ��app,ƥ��ڶ���Ҫ֡��ʽ,subData[10]���Ⱦͷ����.���Ⱦ͸���nOutputSubtitleSize����.������outputItem�����ڴ�.
    __ldec_idvbsub_t        *pdvbsubDec;
}CedarIdvbsubDec;
extern __s32 CedarIdvbsubDec_Init(CedarIdvbsubDec *thiz);   //LYRIC_ITXT��
extern __s32 CedarIdvbsubDec_Exit(CedarIdvbsubDec *thiz);
extern CedarIdvbsubDec* newCedarIdvbsubDec(void);   //LYRIC_ITXT��
extern void deleteCedarIdvbsubDec(CedarIdvbsubDec *thiz);

#endif  /* _CEDARIISDBDEC_H_ */

#endif
