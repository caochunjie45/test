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
* Date   : 2009-3-22 11:19
*********************************************************************************************************
*/
#ifndef _AENC_2_AREC_H_
#define _AENC_2_AREC_H_

//#include "mod_ginkgo_i.h"

typedef union __AENC_2_AREC_MSG
{
    __u32   frm_size;

} __aenc_2_arec_msg_t;


typedef enum __AENC_2_AREC_MSG_TYPE
{
    AENC2AREC_MSG_TYPE_NONE=0,      //no message

    AENC2AREC_MSG_TYPE_FATAL_ERR = GINKGO_AENC2AREC_MSG_BASE,
                                    //fatal error
    AENC2AREC_MSG_TYPE_

} __aenc_2_arec_msg_type_t;


#endif /*_AENC_2_AREC_H_*/
