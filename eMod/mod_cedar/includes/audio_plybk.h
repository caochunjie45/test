/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, Kevin.Z China
*                                              All Rights Reserved
*
* File   : audio_plybk.h
* Version: V1.0
* By     : Kevin.Z
* Description:
    audio_plybk��apiͷ�ļ�
*********************************************************************************************************
*/
#ifndef _AUDIO_PLYBK_H_
#define _AUDIO_PLYBK_H_

enum
{
    CEDAR_APLY_CMD_START=0x00,          /* start                                */
    CEDAR_APLY_CMD_STOP,                /* stop                                 */
    CEDAR_APLY_CMD_PAUSE,               /* pause                                */
    CEDAR_APLY_CMD_CONTI,               /* continue playing                     */
    CEDAR_APLY_CMD_FF,                  /* fast forward                         */
    CEDAR_APLY_CMD_RR,                  /* fast reverse                         */
    CEDAR_APLY_CMD_CLEAR_FFRR,          /* clear fast forward/reverse           */
    CEDAR_APLY_CMD_GET_STATUS,          /* ��ȡAPLYģ���״̬,aux=0:���̷���, aux=1:��statusƽ�Ⱥ��ٷ���                   */
    CEDAR_APLY_CMD_JUMP,                /* jump                                 */
    CEDAR_APLY_CMD_CLEAR_JUMP,          /* clear jump                           */
    CEDAR_APLY_CMD_ENABLE_AB,           //֪ͨaplyģ�飬ab���ſ�ʼ����������£�һ������Ƶ�ļ���jump��ʽ��ab����,aplyֻ����play״̬�²Ż���b���Ƿ��ѵ�
    CEDAR_APLY_CMD_CLEAR_AB,            //֪ͨaplyģ�飬ab���Ž����������ټ��B����
    CEDAR_APLY_CMD_SET_AB_B,                //����AB����ʱ��B���PTS, aux = pts, unit:ms
    CEDAR_APLY_CMD_CLEAR_NOTIFYBDONEFLAG,   //AB����ʱ����֪ͨB�㵽��ı��λ��0.
    CEDAR_APLY_CMD_OPEN,                //������Դ
    CEDAR_APLY_CMD_CLOSE,               //�ͷ���Դ
    CEDAR_APLY_CMD_SET_CEDARINI,         //���û��Ķ�����Ϣ����audio_plybkģ��.���Խ���ָ��,��ʹ��.��Ϊ�ÿ��ڴ���cedar���ط���,�����ڼ�������ͷ�. pbuffer = CedarINI*

    CEDAR_APLY_CMD_SET_AVSHDL=0x20,     /* set A/V sync module handle, START֮ǰ����,           */
    CEDAR_APLY_CMD_SET_CEDARHDL,        //���Ա�������, ����cedar����ģ��ľ��,��Ϊ���ٵ���cedar��stop�ӿ���
    CEDAR_APLY_CMD_SET_PCMSKTIN,        /* pcm buf from audio decoder, START֮ǰ����,STOPʱ����0          */
    CEDAR_APLY_CMD_SET_BITSPERSAMPLE,   /* set bits per sample��δʵ��                  */
    CEDAR_APLY_CMD_SET_FS,              /* set fs, δʵ��                               */
    CEDAR_APLY_CMD_SET_CHN,             /* set channel number��δʵ��                   */
    CEDAR_APLY_CMD_SET_VOLUME,          /* set volume��δʵ��                           */
    CEDAR_APLY_CMD_GET_VOLUME,          /* get volume, δʵ��                         */
    CEDAR_APLY_CMD_GET_MAXVOLUME,       /* get max volume, δʵ��                       */
    CEDAR_APLY_CMD_SET_RAW_DATA_MODE,   /* aux=1,������raw data decode lib��aply�������ݣ�aux=0,������common decode lib��aply��������.
                                            һ������£� raw data decode lib�ᴫ��raw data������Ҳ�������������EAC3�����͵���pcm data.
                                            START֮ǰ����*/
    //CEDAR_APLY_CMD_SET_FFRR2PLAY_DELAY_TIME,    //�Ѿ�������. ������FFRRJUMP->PLAYʱ��Ҫdelay��ʱ��,���ڻ������ݣ� aux = time, unit:ms
    CEDAR_APLY_CMD_SET_CB_MSGQPOST,           //����ͨ��cedar����ģ��ķ�����Ϣͨ��,ab����ʱ����⵽B���ѵ���ͨ����Ϣͨ��֪ͨCEDAR����ģ��ȥ����, pbuffer = __krnl_event_t  *CmdQ
    CEDAR_APLY_CMD_NEED_VIDEOCLOCK_START,       //�Ƿ���Ҫ����video clock,aux =1:��Ҫ;��Ϊ�ڵ��ӹ㲥�������,�����pcrptsУ׼, audio clock��Ҫ��video clockͬ��,��ô��Ҫ����video clock.��start֮ǰ����
    
    CEDAR_APLY_CMD_GET_SAMPCNT = 0x30,  /* AVSYNCģ��ר��, ����ֵ��u32�ģ���ؼ�ס. ͳ��sample������ز������õ���ǰ���ų�����sample����
                                        ���뿼��dac��buffer���������,ret = cnt, ���ص�sample������ԭʼ��sample�����������˱��ٵ�Ӱ�죬
                                        Ҳ��������dac buffer�л����sample��, ����ֱ�Ӽ���,//���bit����Ϊ1,���Ե�����ҪǿתΪ__u32   */
    CEDAR_APLY_CMD_SET_SAMPCNT,         /* AVSYNCģ��ר��, ���õ�ǰ���ų�����sample��������ʼֵ. ���뿼��dac��buffer���������, 
                                        aux = cnt, ��ֹ���;��������Ϊ�˾���������̵߳���, aux = __u32 �����õ�cntҪ������ǰ�����ǰ����� */
    CEDAR_APLY_CMD_GET_CACHE_SAMPCNT,   //ת��Ϊ����ǰ��sample��,�ٷ���,ret = __s32, -1�����д���
    CEDAR_APLY_CMD_GET_CACHE_SIZE,   //�õ�dac buffer�е��ֽ�����ֱ�ӷ���, ��ת��Ϊ����ǰ.������dac buffer�е���ʵ���ֽ���. ����ֵ��__s32, -1����ʧ��
    
    CEDAR_APLY_CMD_REQUEST_SAMPLOCK,    //׼�����������sample��ص�IO�ӿڣ���ϣ����ԭ�Ӳ��������ṩ���ӿ�,ret = EPDK_OK. aux = 0:�������뵽lock���ܷ���
    CEDAR_APLY_CMD_RELEASE_SAMPLOCK,

    //CEDAR_APLY_CMD_SET_NEW_VIDEO_PTS,           //aux = ms.ֻ����Ҫ����vclockʱ,aply�Ż�����video clock.�ڵ��ӹ㲥��,�������Ƶ�㲥,��ô��pts��ͷ��,aply��Ҫ��������vclock���µ�ʱ��,���ʱ���Ǿ���ǰ���ģ�鴦�����ʱ��. vclock����Ϊstream clock������.
    CEDAR_APLY_CMD_SET_NEW_AUDIO_PTS = 0x40,           //pbuffer == __s64*. �п���Ϊ��ֵ,���Ա���__s64. ͬ��,��������audio clock��pts.������Ϊ�ǻ���ʱ����,������Ҫ����һ��.��Ҫע�����:���ú�,aply������������ʱ����,������Ϊadec�л���,aply��ȵ��������pts����Ƶ֡�����������audio clock.������������޹ؽ�Ҫ���ؼ���videoclock����������.
    CEDAR_APLY_CMD_SET_DTV_DELAY_TIME,              //�ڵ��ӹ㲥��,������cedar�����������õĻ���ʱ��.������ǰ��DMX/TSC�Ļ���ʱ��.��λms,����PTS��ͷʱʹ��
       
    CEDAR_APLY_CMD_

};

#endif /*_AUDIO_PLYBCK_H_*/

