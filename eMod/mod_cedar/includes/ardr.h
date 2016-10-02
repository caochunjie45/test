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
    //__s32                       pts;            // pcm data time stamp，取消，因为__ardr_2_aply_msg_t有了该变量了
    __s32                       speed;          // 参考 PostProcessSt->VPS 的数值含义，与其完全一样. -40~100. audio variable speed, original_sample_cnt/(1+speed/100) = cur_sample_cnt.规定:一次传送的数据只能有一种变速值.
    __s32                       nOriginalSamples;     //该笔数据变速处理前的sample数，误差不累积
} __audio_vps_parameter_t;


typedef enum __AUDIO_RENDER_COMMAND
{
    CEDAR_ARDR_CMD_START=0x00,          /* start audio render                   */
    CEDAR_ARDR_CMD_STOP,                /* stop audio render                    */
    CEDAR_ARDR_CMD_PAUSE,               /* 无用. pause audio render                   */
    CEDAR_ARDR_CMD_CONTI,               /* 无用. continue audio render                */
    CEDAR_ARDR_CMD_FF,                  /* fast forward, karaoke不实现                         */
    CEDAR_ARDR_CMD_RR,                  /* fast reverse,karaoke不实现                         */
    CEDAR_ARDR_CMD_CLEAR_FFRR,          /* clear fast forward/reverse,karaoke不实现           */
    CEDAR_ARDR_CMD_JUMP,                /* jump,karaoke不实现                         */
    CEDAR_ARDR_CMD_CLEAR_JUMP,          /* clear jump,karaoke不实现           */
    CEDAR_ARDR_CMD_OPEN,                //分配资源 aux=aux_wav_buf_size
    CEDAR_ARDR_CMD_CLOSE,               //释放资源,并恢复到初始化状态.

    CEDAR_ARDR_CMD_CHKSPT=0x20,         /* check if support,ret=1支持, karaoke不实现                     */
    CEDAR_ARDR_CMD_SET_PCMSKTIN,        /* set pcm input socket                 */
    CEDAR_ARDR_CMD_GET_PCMSKTOUT,       /* get pcm output socket                */
    CEDAR_ARDR_CMD_SET_AVSHDL,          /* set avsync driver handle             */
    CEDAR_ARDR_CMD_SET_ADRVHDL,         /* set audio decode driver handle, 准备删除       */
    CEDAR_ARDR_CMD_SET_EQ,              /* set eq type,                          */
    CEDAR_ARDR_CMD_GET_EQ,              /* get eq type                          */
    CEDAR_ARDR_CMD_SET_SPECTRUM,        /* enable or disable spectrum,karaoke不实现           */
    CEDAR_ARDR_CMD_SET_SWGAIN,		 /* 设置软件增益 aux = -20 ~ 20*/
    CEDAR_ARDR_CMD_GET_SPECTRUM,        /* get spectrum,karaoke不实现                         */
    CEDAR_ARDR_CMD_SET_VPS,             /* set play speed,karaoke不实现, aux = -40~100                       */
    CEDAR_ARDR_CMD_GET_VPS,             /* get play speed,karaoke不实现                       */
    CEDAR_ARDR_CMD_GET_SWGAIN,		 /* 获取软件增益*/

    CEDAR_ARDR_CMD_GET_AUDIO_CHANNEL_MODE = 0x40,  //设置是否左右声道覆盖, aux = AudioChanOutputMode, karaoke实现, cedar不实现
    CEDAR_ARDR_CMD_SET_AUDIO_CHANNEL_MODE,
    CEDAR_ARDR_CMD_RESAMPLE,                //设置重采样率, aux = resample rate, karaoke实现, cedar不实现

    CEDAR_ARDR_CMD_PLAY_WAV_BUFFER = 0x50,/*用于在音视频播放的时候播放一些附加的wav声音数据，该数据跟解码音频数据混音, aux=__cedar_play_aux_wav_mode_t, pbuffer=(__cedar_pcm_info_t*) */

    CEDAR_ARDR_CMD_

} __audio_render_command_t;

/*
=========================================================================================================
                                        音效频段中心频率缩放系数表
=========================================================================================================
频段(Index) ：  1    |   2    |   3    |   4    |   5    |   6    |   7    |   8    |   9    |   10     |
=========================================================================================================
中心频率(hz):   31   |   62   |   125  |   250  |   500  |   1000 |   2000 |   4000 |   8000 |   16000  |
=========================================================================================================
low/重低音  :        |        |        |        |        |        |        |        |        |          |
---------------------------------------------------------------------------------------------------------
pop/流行    :   -1   |   -1   |   0    |   3    |   6    |   4    |   2    |   -1   |   -2   |   -2     |
---------------------------------------------------------------------------------------------------------
rock/摇滚   :   3    |   2    |   -1   |   -5   |   -8   |   -3   |   -1   |   2    |   3    |   3      |
---------------------------------------------------------------------------------------------------------
class/古典  :   0    |   6    |   6    |   3    |   0    |   0    |   0    |   0    |   2    |   2      |
---------------------------------------------------------------------------------------------------------
jazz/爵士   :   0    |   0    |   0    |   3    |   3    |   3    |   0    |   2    |   4    |   4      |
---------------------------------------------------------------------------------------------------------
vocal/语言  :   -2   |   0    |   2    |   1    |   0    |   0    |   0    |   0    |   -2   |   -5     |
---------------------------------------------------------------------------------------------------------
dance/舞曲  :   -1   |   4    |   5    |   1    |   -1   |   -1   |   0    |   0    |   4    |   4      |
---------------------------------------------------------------------------------------------------------
soft/柔和   :   3    |   2    |   1    |   0    |   -1   |   0    |   1    |   3    |   5    |   6      |
---------------------------------------------------------------------------------------------------------
*/


#endif /*_ARDR_H_*/

