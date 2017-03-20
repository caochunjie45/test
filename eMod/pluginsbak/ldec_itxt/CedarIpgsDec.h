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
    CedarIsubtitleDec��������,��ʵ��pgsDec��������
    ����pgs��Ļ�Ľ���.
********************************************************************************
*/
#ifndef _CEDARIPGSDEC_H_
#define _CEDARIPGSDEC_H_

typedef struct tag_CedarIpgsDec
{
    CedarIsubtitleDec       base;
    __cedar_subtitle_item_t outItem; //ר�Ŵ洢Ҫ������Ļ֡,�ṩ��app, pSubtitle���Ը�ֵΪpBitmapData��Ҳ������pPgsDec�ڲ���buffer,�����.
    __u8*                   pBitmapData;    //malloc�ڴ�, �洢һ֡��scaledown֮�������,���Ҫ��scaledown�Ļ�,��������,����������,�������ڴ�.
    __s32                   nBitmapDataSize;    //ָʾoutputItem->pSubtitle�ĳ���
    __s32                   screenWidth;    //��ʾ���ڵĿ��, g_oalCtx->screenWidth  = aux>>16;
    __s32                   screenHeight;
    CCedarSubtitleFrame     outputSubtitleFrame; //ר�Ŵ洢Ҫ������Ļ֡,�ṩ��app,ƥ��ڶ���Ҫ֡��ʽ,pSubtitle��outItem����һ�¾Ϳ�����.
    PgsDecInterface         *pPgsDec;
}CedarIpgsDec;
extern __s32 CedarIpgsDec_Init(CedarIpgsDec *thiz);   //LYRIC_ITXT��
extern __s32 CedarIpgsDec_Exit(CedarIpgsDec *thiz);
extern CedarIpgsDec* newCedarIpgsDec(void);   //LYRIC_ITXT��
extern void deleteCedarIpgsDec(CedarIpgsDec *thiz);

#endif  /* _CEDARIPGSDEC_H_ */

