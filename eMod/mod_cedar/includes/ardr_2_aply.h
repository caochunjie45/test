/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, kevin China
*                                              All Rights Reserved
*
* File   : ardr_2_aply.h
* Version: V1.0
* By     : kevin.z
* Date   : 2008-6-3 16:19
*********************************************************************************************************
*/
#ifndef _ARDR_2_APLY_H_
#define _ARDR_2_APLY_H_

#include "mod_cedar_i.h"


typedef struct __ARDR_2_APLY_MSG
{
    //put every type which need be transfer as message here
    __audio_codec_format_t  audioInf;       //audio bitstream frame head, ARDR2APLY_MSG_TYPE_ABSFMTINF时传一下
    __audio_vps_parameter_t audioVps;       //audio variable speed parameter,每帧必传
    __u32   uPts;   //ms                    //必传
    __s32   bPtsValid;                      //必传
    __s32   nStatFlag;                      //该笔蔵綢是否统计sample蔵；

} __ardr_2_aply_msg_t;


typedef enum __ARDR_2_APLY_MSG_TYPE
{
    ARDR2APLY_MSG_TYPE_NONE=0,              //no message

    ARDR2APLY_MSG_TYPE_ABSFMTINF=CEDAR_ARDR2APLY_MSG_BASE,  //第一笔数据会是这个msgtype,表示audioinf有效,其他和ARDR2APLY_MSG_TYPE_NONE没区别
                                            //audio bitstream frame head
    ARDR2APLY_MSG_TYPE_AUDIOVPS,            //audio variable play parameter，没有用到，因为aply每个socket都会查的.

    ARDR2APLY_MSG_TYPE_

} __ardr_2_aply_msg_type_t;



#endif /*_ARDR_2_APLY_H_*/
