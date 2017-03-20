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
* File   : maple_ini.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2012-3-2
* Description:
    ��¼maple�м�����õ����ݽṹ.
********************************************************************************
*/
#ifndef _MAPLE_INI_H_
#define _MAPLE_INI_H_

/*******************************************************************************
maple.ini�ļ���������˵��:
Members: 
    AUDIO_RUN_ONLY_BUF_SIZE: audio������bufsize.��λkbytes, 128����128k
    AUDIO_WITH_VIDEO_BUF_SIZE:ͬ��
    AACSBR:�Ƿ��aac����������sbr����, 0�ر�;1��
*******************************************************************************/
typedef struct tag_CMapleINI
{
    CFEConfig   FEConfig;           //demodģ���tsc�������Ͽɵ��������ݽṹ
    CedarINI    CedarINIInstance;   //cedar���Ⱥ��ͳ���������ݽṹ
}CMapleINI;

#endif  /* _MAPLE_INI_H_ */

