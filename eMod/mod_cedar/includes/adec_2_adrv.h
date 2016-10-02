/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, kevin China
*                                              All Rights Reserved
*
* File   : adec_2_adrv.h
* Version: V1.0
* By     : kevin.z
* Date   : 2008-9-26 11:05
*********************************************************************************************************
*/
#ifndef _ADEC_2_ADRV_H_
#define _ADEC_2_ADRV_H_

#include "mod_cedar_i.h"
#include "psr_2_adec.h"
#include "adec_2_ardr.h"


typedef struct __ADRV_2_ADEC_MSG
{
    __u32       nMsgType;
    void        *pMsg;

} __adrv_2_adec_msg_t;



typedef enum __ADEC_2_ADRV_MSG_TYPE
{
    ADEC2ADRV_MSG_TYPE_NONE=0,              // no message
    ADEC2ADRV_MSG_TYPE_DATA_END=CEDAR_ADEC2ADRV_MSG_BASE,
                                            // frame size change
    ADEC2ADRV_MSG_TYPE_FATAL_ERR,           // fatal error
    ADEC2ADRV_MSG_TYPE_REFETCH_DATA,        // need re-fetch data
    ADEC2ADRV_MSG_TYPE_STUFDATA_SIZE,       // report size of stuffing data

    ADEC2VDRV_MSG_TYPE_

} __adec_2_adrv_msg_type_t;



#endif /* _ADEC_2_ADRV_H_ */
