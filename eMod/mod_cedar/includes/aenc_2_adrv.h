/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, kevin China
*                                              All Rights Reserved
*
* File   : aenc_2_adrv.h
* Version: V1.0
* By     : kevin.z
* Date   : 2009-3-22 10:55
*********************************************************************************************************
*/
#ifndef _AENC_2_ADRV_H_
#define _AENC_2_ADRV_H_

//#include "mod_ginkgo_i.h"


typedef struct __AENC_2_ADRV_MSG
{
    __u32       nMsgType;
    void        *pMsg;

} __aenc_2_adrv_msg_t;



typedef enum __AENC_2_ADRV_MSG_TYPE
{
    AENC2ADRV_MSG_TYPE_NONE=0,          // no message

    AENC2ADRV_MSG_TYPE_DATA_END = GINKGO_AENC2ADRV_MSG_BASE,
    AENC2ADRV_MSG_TYPE_FATAL_ERR,       // fatal error

    AENC2ADRV_MSG_TYPE_

} __aenc_2_adrv_msg_type_t;



#endif /* _AENC_2_ADRV_H_ */
