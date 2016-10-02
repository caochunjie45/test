/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, kevin China
*                                              All Rights Reserved
*
* File   : vdec_2_vdrv.h
* Version: V1.0
* By     : kevin.z
* Date   : 2008-6-24 16:05
*********************************************************************************************************
*/
#ifndef _VDEC_2_VDRV_H_
#define _VDEC_2_VDRV_H_

#include "mod_cedar_i.h"
#include "psr_2_vdec.h"


//message between video decoder and video decode driver
typedef struct __VDEC_2_VDRV_MSG
{
    void        *pData;
    __u32       nDataLen;

    void        *pMsg;
    __u32       nMsgType;
    __u32       nMsgLen;

}__vdec_2_vdrv_msg_t;


typedef struct __VDEC_FB_INF
{
    void        *pBuf;

}__vdec_fb_inf;


typedef enum __VDEC_2_VDRV_MSG_TYPE
{
    VDEC2VDRV_MSG_TYPE_NONE=0,
    VDEC2VDRV_MSG_TYPE_FRMSIZE_CHANGE=CEDAR_VDEC2VDRV_MSG_BASE,
                                                // frame size change
    VDEC2VDRV_MSG_TYPE_I_DECODE,                // decode an I frame
    VDEC2VDRV_MSG_TYPE_REFETCH_DATA,            // refetch video bitstream
    VDEC2VDRV_MSG_TYPE_FATAL_ERR,               // fatal err
    VDEC2VDRV_MSG_TYPE_DEC_END,                 // decode driver finish decode bitstream
    VDEC2VDRV_MSG_TYPE_DATA_FULL,               // bitstream buffer full

    VDEC2VDRV_MSG_TYPE_

} __vdec_2_vdrv_msg_type_t;

#endif /* _VDEC_2_VDRV_H_ */
