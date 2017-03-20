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
    __audio_codec_format_t  audioInf;       //audio bitstream frame head, ARDR2APLY_MSG_TYPE_ABSFMTINFʱ��һ��
    __audio_vps_parameter_t audioVps;       //audio variable speed parameter,ÿ֡�ش�
    __u32   uPts;   //ms                    //�ش�
    __s32   bPtsValid;                      //�ش�
    __s32   nStatFlag;                      //�ñ��i�I�Ƿ�ͳ��sample�i��

} __ardr_2_aply_msg_t;


typedef enum __ARDR_2_APLY_MSG_TYPE
{
    ARDR2APLY_MSG_TYPE_NONE=0,              //no message

    ARDR2APLY_MSG_TYPE_ABSFMTINF=CEDAR_ARDR2APLY_MSG_BASE,  //��һ�����ݻ������msgtype,��ʾaudioinf��Ч,������ARDR2APLY_MSG_TYPE_NONEû����
                                            //audio bitstream frame head
    ARDR2APLY_MSG_TYPE_AUDIOVPS,            //audio variable play parameter��û���õ�����Ϊaplyÿ��socket������.

    ARDR2APLY_MSG_TYPE_

} __ardr_2_aply_msg_type_t;



#endif /*_ARDR_2_APLY_H_*/
