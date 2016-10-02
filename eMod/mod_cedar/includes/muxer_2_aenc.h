/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, kevin China
*                                              All Rights Reserved
*
* File   : muxer_2_aenc.h
* Version: V1.0
* By     : kevin.z
* Date   : 2009-3-22 11:16
*********************************************************************************************************
*/
#ifndef _MUXER_2_AENC_H_
#define _MUXER_2_AENC_H_

//#include "mod_ginkgo_i.h"

typedef union __MUXER_2_AENC_MSG
{
    __pcm_header_inf_t  pcminf;

} __muxer_2_aenc_msg_t;


typedef enum __MUXER_2_AENC_MSG_TYPE
{
    MUXER2AENC_MSG_TYPE_NONE=0,         // no message

    MUXER2AENC_MSG_TYPE_FATAL_ERR = GINKGO_MUXER2AENC_MSG_BASE,
                                        // fatal error
    MUXER2AENC_MSG_TYPE_ABSHDR,         //audio bitstream header

    MUXER2AENC_MSG_TYPE_

} __muxer_2_aenc_msg_type_t;


#endif /* _PSR_2_ADEC_H_ */
