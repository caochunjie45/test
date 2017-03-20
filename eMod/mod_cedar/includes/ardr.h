/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, Kevin.Z China
*                                              All Rights Reserved
*
* File   : ardr.h
* Version: V1.0
* By     : Kevin.Z
*********************************************************************************************************
*/
#ifndef _ARDR_H_
#define _ARDR_H_


#define SPECTRUM_HIGH_THRESHOLD         (100)
#define SPECTRUM_LOW_THRESHOLD          (30)
//switch for spectrum time control
#define SPECTRUM_TIME_CTL_SWITCH        (1)


typedef struct __AUDIO_VPS_PARAMETER
{
    //__s32                       pts;            // pcm data time stamp��ȡ������Ϊ__ardr_2_aply_msg_t���˸ñ�����
    __s32                       speed;          // �ο� PostProcessSt->VPS ����ֵ���壬������ȫһ��. -40~100. audio variable speed, original_sample_cnt/(1+speed/100) = cur_sample_cnt.�涨:һ�δ��͵�����ֻ����һ�ֱ���ֵ.
    __s32                       nOriginalSamples;     //�ñ����ݱ��ٴ���ǰ��sample�������ۻ�
} __audio_vps_parameter_t;


typedef enum __AUDIO_RENDER_COMMAND
{
    CEDAR_ARDR_CMD_START=0x00,          /* start audio render                   */
    CEDAR_ARDR_CMD_STOP,                /* stop audio render                    */
    CEDAR_ARDR_CMD_PAUSE,               /* ����. pause audio render                   */
    CEDAR_ARDR_CMD_CONTI,               /* ����. continue audio render                */
    CEDAR_ARDR_CMD_FF,                  /* fast forward, karaoke��ʵ��                         */
    CEDAR_ARDR_CMD_RR,                  /* fast reverse,karaoke��ʵ��                         */
    CEDAR_ARDR_CMD_CLEAR_FFRR,          /* clear fast forward/reverse,karaoke��ʵ��           */
    CEDAR_ARDR_CMD_JUMP,                /* jump,karaoke��ʵ��                         */
    CEDAR_ARDR_CMD_CLEAR_JUMP,          /* clear jump,karaoke��ʵ��           */
    CEDAR_ARDR_CMD_OPEN,                //������Դ aux=aux_wav_buf_size
    CEDAR_ARDR_CMD_CLOSE,               //�ͷ���Դ,���ָ�����ʼ��״̬.

    CEDAR_ARDR_CMD_CHKSPT=0x20,         /* check if support,ret=1֧��, karaoke��ʵ��                     */
    CEDAR_ARDR_CMD_SET_PCMSKTIN,        /* set pcm input socket                 */
    CEDAR_ARDR_CMD_GET_PCMSKTOUT,       /* get pcm output socket                */
    CEDAR_ARDR_CMD_SET_AVSHDL,          /* set avsync driver handle             */
    CEDAR_ARDR_CMD_SET_ADRVHDL,         /* set audio decode driver handle, ׼��ɾ��       */
    CEDAR_ARDR_CMD_SET_EQ,              /* set eq type,                          */
    CEDAR_ARDR_CMD_GET_EQ,              /* get eq type                          */
    CEDAR_ARDR_CMD_SET_SPECTRUM,        /* enable or disable spectrum,karaoke��ʵ��           */
    CEDAR_ARDR_CMD_SET_SWGAIN,		 /* ����������� aux = -20 ~ 20*/
    CEDAR_ARDR_CMD_GET_SPECTRUM,        /* get spectrum,karaoke��ʵ��                         */
    CEDAR_ARDR_CMD_SET_VPS,             /* set play speed,karaoke��ʵ��, aux = -40~100                       */
    CEDAR_ARDR_CMD_GET_VPS,             /* get play speed,karaoke��ʵ��                       */
    CEDAR_ARDR_CMD_GET_SWGAIN,		 /* ��ȡ�������*/

    CEDAR_ARDR_CMD_GET_AUDIO_CHANNEL_MODE = 0x40,  //�����Ƿ�������������, aux = AudioChanOutputMode, karaokeʵ��, cedar��ʵ��
    CEDAR_ARDR_CMD_SET_AUDIO_CHANNEL_MODE,
    CEDAR_ARDR_CMD_RESAMPLE,                //�����ز�����, aux = resample rate, karaokeʵ��, cedar��ʵ��

    CEDAR_ARDR_CMD_PLAY_WAV_BUFFER = 0x50,/*����������Ƶ���ŵ�ʱ�򲥷�һЩ���ӵ�wav�������ݣ������ݸ�������Ƶ���ݻ���, aux=__cedar_play_aux_wav_mode_t, pbuffer=(__cedar_pcm_info_t*) */

    CEDAR_ARDR_CMD_

} __audio_render_command_t;

/*
=========================================================================================================
                                        ��ЧƵ������Ƶ������ϵ����
=========================================================================================================
Ƶ��(Index) ��  1    |   2    |   3    |   4    |   5    |   6    |   7    |   8    |   9    |   10     |
=========================================================================================================
����Ƶ��(hz):   31   |   62   |   125  |   250  |   500  |   1000 |   2000 |   4000 |   8000 |   16000  |
=========================================================================================================
low/�ص���  :        |        |        |        |        |        |        |        |        |          |
---------------------------------------------------------------------------------------------------------
pop/����    :   -1   |   -1   |   0    |   3    |   6    |   4    |   2    |   -1   |   -2   |   -2     |
---------------------------------------------------------------------------------------------------------
rock/ҡ��   :   3    |   2    |   -1   |   -5   |   -8   |   -3   |   -1   |   2    |   3    |   3      |
---------------------------------------------------------------------------------------------------------
class/�ŵ�  :   0    |   6    |   6    |   3    |   0    |   0    |   0    |   0    |   2    |   2      |
---------------------------------------------------------------------------------------------------------
jazz/��ʿ   :   0    |   0    |   0    |   3    |   3    |   3    |   0    |   2    |   4    |   4      |
---------------------------------------------------------------------------------------------------------
vocal/����  :   -2   |   0    |   2    |   1    |   0    |   0    |   0    |   0    |   -2   |   -5     |
---------------------------------------------------------------------------------------------------------
dance/����  :   -1   |   4    |   5    |   1    |   -1   |   -1   |   0    |   0    |   4    |   4      |
---------------------------------------------------------------------------------------------------------
soft/���   :   3    |   2    |   1    |   0    |   -1   |   0    |   1    |   3    |   5    |   6      |
---------------------------------------------------------------------------------------------------------
*/


#endif /*_ARDR_H_*/

