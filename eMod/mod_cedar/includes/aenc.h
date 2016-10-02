/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                      (c) Copyright 2008-2009, kevin China
*                                              All Rights Reserved
*
* File   : aenc.h
* Version: V1.0
* By     : kevin.z
* Date   : 2009-3-22 10:34
*********************************************************************************************************
*/
#ifndef _AENC_H_
#define _AENC_H_

typedef enum __AUDIO_ENCODER_COMMAND
{
    GINKGO_AENC_CMD_START=0x00,         /* 启动编码                             */
    GINKGO_AENC_CMD_STOP,               /* 停止编码                             */

    GINKGO_AENC_CMD_SET_ABSSKT=0x20,    /* 设置音频码流输出通道                 */
    GINKGO_AENC_CMD_GET_PCMSKT,         /* 获取音频样本输入通道                 */
    GINKGO_AENC_CMD_CHKSPT,
    GINKGO_AENC_CMD_GET_ABSHDR,         // 获取音频流的头信息, pbuffer = pAbsHdr, aux = bufsize, ret = validlen

    GINKGO_AENC_CMD_GET_TIME=0x40,      /* 获取当前编码时间                     */
    GINKGO_AENC_CMD_SET_AUDIOINF,       /* 设置音频码率                         */
    GINKGO_AENC_CMD_SET_MSGQ,           /* 设置消息通道                         */

    GINKGO_AENC_CMD_OPEN = 0x60,         //分配资源                              */
    GINKGO_AENC_CMD_CLOSE,               //释放资源                              */
    GINKGO_AENC_CMD_SET_ABSTYPE,         //设置abstype                           */
    GINKGO_AENC_CMD_GET_ADRV_STATUS,     /* 得到adrv模块的状态, aux = 1:表示一定要返回稳定状态; aux=0表示立即返回.*/
       
    GINKGO_AENC_CMD_

} __audio_encoder_command_t;


#endif /*_AENC_H_*/
