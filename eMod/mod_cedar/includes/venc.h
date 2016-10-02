/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, kevin.z China
*                                              All Rights Reserved
*
* File   : venc.h
* Version: V1.0
* By     : kevin.z
* Date   : 2009-3-22 10:30
*********************************************************************************************************
*/
#ifndef _VENC_H_
#define _VENC_H_

typedef enum __VIDEO_ENCODER_COMMAND
{
    GINKGO_VENC_CMD_START=0x00,     /* 启动编码                                 */
    GINKGO_VENC_CMD_STOP,           /* 停止编码                                 */

    GINKGO_VENC_CMD_CHKSPT=0x20,
    GINKGO_VENC_CMD_SET_VBSSKT,     /* 设置视频码流输出通道                     */
    GINKGO_VENC_CMD_GET_PICSKT,     /* 获取视频图片输入通道                     */
    GINKGO_VENC_CMD_SET_MSGQ,       /* 设置消息通道                             */

    GINKGO_VENC_CMD_

} __video_encoder_command_t;


#endif /*_VENC_H_*/
