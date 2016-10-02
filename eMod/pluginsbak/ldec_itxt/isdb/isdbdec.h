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
* File   : isdbdec.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-9-25
* Description:
    普通的内置文本字幕解码器,一般是utf8编码.
********************************************************************************
*/
#ifndef _ISDBDEC_H_
#define _ISDBDEC_H_

#define ISDB_FRAME_MAX_DURATION (60000) // 单位:ms. isdb字幕项不一样给出持续时间, 如果没给,就认为持续时间无限长,这个无限长定为60秒
typedef enum MAPLE_SUBTITLE_INVOKE_CODE
{
    MAPLE_SUB_INVOKE_NONE             = 0x00,
    MAPLE_SUB_INVOKE_LS0              = 0x01,  
    MAPLE_SUB_INVOKE_LS1              = 0x02,  
    MAPLE_SUB_INVOKE_LS2              = 0x03,  
    MAPLE_SUB_INVOKE_LS3              = 0x04,  
    MAPLE_SUB_INVOKE_LS1R             = 0x05,  
    MAPLE_SUB_INVOKE_LS2R             = 0x06,  
    MAPLE_SUB_INVOKE_LS3R             = 0x07,  
    //MAPLE_SUB_INVOKE_SS2              = 0x08,  
    //MAPLE_SUB_INVOKE_SS3              = 0x09,  

    MAPLE_SUB_INVOKE_UNKNOW
}maple_subtitle_invoke_code_t;



//* Control function character set code table

//* C0 area
#define NUL        0x00
#define BEL        0x07
#define APB        0x08
#define APF        0x09
#define APD        0x0a
#define APU        0x0b
#define CS         0x0c
#define APR        0x0d
#define LS1        0x0e
#define LS0        0x0f

#define PAPF       0x16
#define CAN        0x18
#define SS2        0x19
#define ESC        0x1b
#define APS        0x1c
#define SS3        0x1d
#define RS         0x1e
#define US         0x1f

#define SP         0x20
#define DEL        0x7f

//* C1 area
#define BKF        0x80
#define RDF        0x81
#define GRF        0x82
#define YLF        0x83
#define BLF        0x84
#define MGF        0x85
#define CNF        0x86
#define WHF        0x87
#define SSZ        0x88
#define MSZ        0x89
#define NSZ        0x8a
#define SZX        0x8b

#define COL        0x90
#define FLC        0x91
#define CDC        0x92
#define POL        0x93
#define WMM        0x94
#define MACRO      0x95

#define HLC        0x97
#define RPC        0x98
#define SPL        0x99
#define STL        0x9a
#define CSI        0x9b

#define TIME       0x9d

extern __s32 ldec_isdb_decode(IsdbDec *thiz, CCedarSubtitleFrame *tmpItemInf, __u8 *curDataPtr, __u32 *usedBytes, __u8 *privateData);

#endif  /* _ISDBDEC_H_ */

