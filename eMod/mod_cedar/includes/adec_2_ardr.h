/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, kevin China
*                                              All Rights Reserved
*
* File   : adec_2_ardr.h
* Version: V1.0
* By     : kevin.z
* Date   : 2008-6-3 16:19
*********************************************************************************************************
*/
#ifndef _ADEC_2_ARDR_H_
#define _ADEC_2_ARDR_H_

#include "mod_cedar_i.h"


typedef struct __ADEC_2_ARDR_MSG
{
    //put every type which need be transfer as message here
    __audio_codec_format_t      absfmtinf;      // pcm data format information
    __mp                        *pAudDrvHdl;    // audio deocde driver handle，没有用到
    __u32                       nFrmPts;        // frame time stamp
    __s32                       bPtsValid;      //表示携带的PTS是否有效

} __adec_2_ardr_msg_t;


typedef enum __ADEC_2_ARDR_MSG_TYPE
{
    ADEC2ARDR_MSG_TYPE_NONE=0,                  //no message

    ADEC2ARDR_MSG_TYPE_AFMTINF=CEDAR_ADEC2ARDR_MSG_BASE,    //也会携带PTS，如果有的话
                                                // audio format information
    ADEC2ARDR_MSG_TYPE_DATAEND,                 // pcm data end
    ADEC2ARDR_MSG_TYPE_ADRVHDL,                 // audio decode driver handle
    ADEC2ARDR_MSG_TYPE_FRMPTS,                  // pcm frame time stamp, 也将携带__audio_codec_format_t      absfmtinf.这是为纠错而定的,可能刚开始的音频会把sample rate等参数解错.
    ADEC2ARDR_MSG_TYPE_VPSCHANGE,               // audio variable speed change
    ADEC2ARDR_MSG_TYPE_FATAL_ERR,               //fatal error!

    ADEC2ARDR_MSG_TYPE_

} __adec_2_ardr_msg_type_t;


#endif /*_ADEC_2_ARDR_H_*/
