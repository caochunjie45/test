/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, kevin China
*                                              All Rights Reserved
*
* File   : vdec_2_vply.h
* Version: V1.0
* By     : kevin.z
* Date   : 2008-6-3 16:19
*********************************************************************************************************
*/
#ifndef _VDEC_2_VPLY_H_
#define _VDEC_2_VPLY_H_

#include "mod_cedar_i.h"


typedef struct __DVDEC_FB_INFO
{                                   //frame buffer information, used for exchange information...
                                    //...between video decode driver and video play-back
    void            *buf_addr;
    __u32           buf_size;

} __dvdec_fb_info;


typedef union __VDEC_2_VPLY_MSG
{
    //put every type which need be transfer as message here
    __dvdec_fb_info     frmbufinf;


} __vdec_2_vply_msg_t;

typedef enum __VDEC_2_VPLY_MSG_TYPE
{
    VDEC2VPLY_MSG_TYPE_NONE=0,              // no message

    VDEC2VPLY_MSG_TYPE_FRMBUF=CEDAR_VDEC2VPLY_MSG_BASE,
                                            // frame buffer message
    VDEC2VPLY_MSG_TYPE_VDRV_HANDLE,         // video driver handle
    VDEC2VPLY_MSG_TYPE_VDEC_END,            // video decode end
    VDEC2VPLY_MSG_TYPE_FATAL_ERR,           // video decode fatal error
    VDEC2VPLY_MSG_TYPE_

} __vdec_2_vply_msg_type_t;



#endif /*_VDEC_2_VPLY_H_*/
