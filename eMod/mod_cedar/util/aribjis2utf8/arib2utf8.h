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
* File   : arib2utf8.h
* Version: V1.0
* By     : Michael_LEE
* Date   : 2012-4-10
* Description:
********************************************************************************
*/

#ifndef _ARIB_2_UTF8_H
#define _ARIB_2_UTF8_H

typedef enum tag_GraphicSetsDesignationMode
{
    GraphicSetsDesignationMode_FullSeg = 0, //fullseg����Ļ,Ĭ��G0:kanji,G1:alphanumeric,G2:hiragana,G3:macro code
    GraphicSetsDesignationMode_OneSeg,      //oneseg����Ļ,Ĭ��G0:DRCS-1,G1:alphanumeric,G2:kanji,G3:macro code
    GraphicSetsDesignationMode_SI,          //SI���ַ���,Ĭ��G0:kanji,G1:alphanumeric,G2:hiragana,G3:katakana
}GraphicSetsDesignationMode;
/*******************************************************************************
Function name: arib2utf8
Description: 
    Ĭ��output��buffer�ĳ���һ������.
    ��������ῼ��\0����,ת����֮�����ַ���ĩβ���ټ�һ��\0�ֽ�.�������outputҪ����
    \0������.
Parameters: 
    input:   source string��
    ilen:    source string�ĳ���;Ĭ��Ҫ�󲻰���\0
    output:  dst string����ʼ��ַ��
    olen:    dst string����󳤶ȣ�
    mode:    �ַ�ת��ģʽ: 0: fullseg; 1: oneseg;
Return: 
    ת��֮��Ĵ洢��output���ֽڳ���;������\0
Time: 2011/2/16
*******************************************************************************/
extern int conv_arib_to_utf8(char *input, int ilen, char *output, int olen, GraphicSetsDesignationMode mode);

#endif

