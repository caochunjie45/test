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
    GraphicSetsDesignationMode_FullSeg = 0, //fullseg的字幕,默认G0:kanji,G1:alphanumeric,G2:hiragana,G3:macro code
    GraphicSetsDesignationMode_OneSeg,      //oneseg的字幕,默认G0:DRCS-1,G1:alphanumeric,G2:kanji,G3:macro code
    GraphicSetsDesignationMode_SI,          //SI的字符串,默认G0:kanji,G1:alphanumeric,G2:hiragana,G3:katakana
}GraphicSetsDesignationMode;
/*******************************************************************************
Function name: arib2utf8
Description: 
    默认output的buffer的长度一定够用.
    这个函数会考虑\0因素,转换完之后在字符串末尾会再加一个\0字节.所以输出output要考虑
    \0的因素.
Parameters: 
    input:   source string；
    ilen:    source string的长度;默认要求不包括\0
    output:  dst string的起始地址；
    olen:    dst string的最大长度；
    mode:    字符转码模式: 0: fullseg; 1: oneseg;
Return: 
    转换之后的存储在output的字节长度;不包括\0
Time: 2011/2/16
*******************************************************************************/
extern int conv_arib_to_utf8(char *input, int ilen, char *output, int olen, GraphicSetsDesignationMode mode);

#endif

