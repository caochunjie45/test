/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, kevin China
*                                              All Rights Reserved
*
* File   : ldec_2_lrdr.h
* Version: V1.0
* By     : kevin.z
* Date   : 2008-7-17 9:04
*********************************************************************************************************
*/
#ifndef _LDEC_2_LRDR_H_
#define _LDEC_2_LRDR_H_

#include "mod_cedar_i.h"

#define CEDAR_DEC_LYRIC_ITEM_QSIZE      (1)
#define CEDAR_DEC_LYRIC_ITEM_BUFSIZE    (1024 * 4)

typedef union __LDEC_2_LRDR_MSG
{
    //put every type which need be transfer as message here
    __u32       reserved;       //reserved

} __ldec_2_lrdr_msg_t;


typedef enum __LDEC_2_LRDR_MSG_TYPE
{
    LDEC2LRDR_MSG_TYPE_NONE=0,          //no message

    LDEC2LRDR_MSG_TYPE_ABSFH=CEDAR_LDEC2LRDR_MSG_BASE,
                                        //audio bitstream frame head

    LDEC2LPLY_MSG_TYPE_

} __ldec_2_lrdr_msg_type_t;


#endif /*_LDEC_2_LRDR_H_*/
