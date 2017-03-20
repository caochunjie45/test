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
    GINKGO_AENC_CMD_START=0x00,         /* ��������                             */
    GINKGO_AENC_CMD_STOP,               /* ֹͣ����                             */

    GINKGO_AENC_CMD_SET_ABSSKT=0x20,    /* ������Ƶ�������ͨ��                 */
    GINKGO_AENC_CMD_GET_PCMSKT,         /* ��ȡ��Ƶ��������ͨ��                 */
    GINKGO_AENC_CMD_CHKSPT,
    GINKGO_AENC_CMD_GET_ABSHDR,         // ��ȡ��Ƶ����ͷ��Ϣ, pbuffer = pAbsHdr, aux = bufsize, ret = validlen

    GINKGO_AENC_CMD_GET_TIME=0x40,      /* ��ȡ��ǰ����ʱ��                     */
    GINKGO_AENC_CMD_SET_AUDIOINF,       /* ������Ƶ����                         */
    GINKGO_AENC_CMD_SET_MSGQ,           /* ������Ϣͨ��                         */

    GINKGO_AENC_CMD_OPEN = 0x60,         //������Դ                              */
    GINKGO_AENC_CMD_CLOSE,               //�ͷ���Դ                              */
    GINKGO_AENC_CMD_SET_ABSTYPE,         //����abstype                           */
    GINKGO_AENC_CMD_GET_ADRV_STATUS,     /* �õ�adrvģ���״̬, aux = 1:��ʾһ��Ҫ�����ȶ�״̬; aux=0��ʾ��������.*/
       
    GINKGO_AENC_CMD_

} __audio_encoder_command_t;


#endif /*_AENC_H_*/
