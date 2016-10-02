/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, kevin China
*                                              All Rights Reserved
*
* File   : lrdr_2_lply.h
* Version: V1.0
* By     : kevin.z
* Date   : 2008-7-17 9:00
*********************************************************************************************************
*/
#ifndef _LRDR_2_LPLY_H_
#define _LRDR_2_LPLY_H_

#include "mod_cedar_i.h"


typedef union __LRDR_2_LPLY_MSG
{
    //put every type which need be transfer as message here
    __u32       reserved;       //reserved

} __ldec_2_lrdr_msg_t;



typedef enum __LRDR_2_LPLY_MSG_TYPE
{
    LRDR2LPLY_MSG_TYPE_NONE=0,          //no message

    LRDR2LPLY_MSG_TYPE_LBSFH=CEDAR_LRDR2LPLY_MSG_BASE,
                                        //lyric bitstream frame head
    LRDR2LPLY_MSG_TYPE_

} __lrdr_2_lply_msg_type_t;


#endif /*_LRDR_2_LPLY_H_*/
