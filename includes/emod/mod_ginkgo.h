/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                               mod_mmp sub-system
*
*                                      (c) Copyright 2008-2009, kevin China
*                                               All Rights Reserved
*
* File   : mod_ginkgo.h
* Version: V1.0
* By     : kevin
* Date   : 2009-3-21 17:53
*********************************************************************************************************
*/
#ifndef _MOD_GINKGO_H_
#define _MOD_GINKGO_H_

#include "typedef.h"
#include "cfgs.h"

#include "mod_cedar.h"
#define GINKGO_SPECTRUM_LEVEL           (32)    //spectrum level, __u16 for save every level

typedef enum __GINKGO_CMD_SET           /* ý��¼���м�������б�(IOCtrol)                              */
{
    GINKGO_SET_MEDIAFILE=0x0100,        /* ����¼�Ƶ�ý���ļ���Ϣaux = (__ginkgo_media_file_inf *)      */
    GINKGO_CMD_GET_MESSAGE_CHN,         /* ��ȡ������Ϣͨ����return = (__krnl_event_t *)                */
    GINKGO_CMD_GET_ERROR_TYPE,          /* ��ȡý�岥�����Ĵ�������,                                    */

    GINKGO_CMD_START=0x0200,            /* ����¼��                                                     */
    GINKGO_CMD_STOP,                    /* ֹͣ¼��                                                     */
    GINKGO_CMD_PAUSE,                   /* ��ͣ¼��                                                     */
    GINKGO_CMD_GET_STATUS,              /* ��ȡý��¼���м����ǰ״̬                                   */

    GINKGO_CMD_GET_CUR_TIME=0x0300,     /* ��ȡ��ǰ¼��ʱ�� return = current encode time(ms)            */
    GINKGO_CMD_SET_FILE_FMT,            /* ����Ҫ¼�Ƶ�ý���ļ��ĸ�ʽ��aux = __ginkgo_media_file_fmt_t  */

    GINKGO_CMD_SET_ABSTYPE=0x0400,      /* ������Ƶ�������ʽ, aux = __ginkgo_audio_fmt_t, return = new abs type    */
    GINKGO_CMD_GET_ABSTYPE,             /* ��ȡ��Ƶ�������ʽ return = __ginkgo_audio_fmt_t             */
    GINKGO_CMD_SET_AUDBPS,              /* ������Ƶ�������� aux = bps to be set, return = bps after set */
    GINKGO_CMD_GET_AUDBPS,              /* ��ȡ��Ƶ�������� return = bps                                */
    GINKGO_CMD_SET_SAMPRATE,            /* ������Ƶ������   aux = sample rate, return = new sample rate after set   */
    GINKGO_CMD_GET_SAMPRATE,            /* ��ȡ��Ƶ������   return = sample rate                        */
    GINKGO_CMD_SET_SAMPWIDTH,           /* ������Ƶ�������   aux = __ginkgo_audio_samp_width_t, return = new value */
    GINKGO_CMD_GET_SAMPWIDTH,           /* ��ȡ��Ƶ�������   return = __ginkgo_audio_samp_width_t      */
    GINKGO_CMD_SET_CHN,                 /* ����ͨ������     aux = __ginkgo_audio_chn_t, return = new channel        */
    GINKGO_CMD_GET_CHN,                 /* ��ȡͨ������     return = __ginkgo_audio_chn_t               */
    GINKGO_CMD_SET_VOL,                 /* ����¼������     aux = MIN_VOLUME_VAL~MAX_VOLUME_VAL, return = new vol   */
    GINKGO_CMD_GET_VOL,                 /* ��ȡ¼������     return = volume                             */
    GINKGO_CMD_SET_AUDIO_SOURCE,        /* ����¼����Դ     aux = __ginkgo_audio_src_t                  */
    GINKGO_CMD_GET_AUDIO_SOURCE,        /* ��ȡ¼����Դ     return = __ginkgo_audio_src_t               */
    GINKGO_CMD_SET_SPECTRUM,            /* �����Ƿ��Ƶ�׽����� 0-disable; !0-enable                  */
    GINKGO_CMD_GET_SPECTRUM,            /* ��ȡʵʱspectrum (i/o)pbuffer = spectrum (return ok or fail) */
    GINKGO_CMD_SET_AUDIOGAIN,             //�������¼��ʱ������ֵ, aux = -20~20dB

    GINKGO_CMD_SET_VBSTYPE=0x0500,      /* ������Ƶ�������ʽ                                           */
    GINKGO_CMD_GET_VBSTYPE,             /* ��ȡ��Ƶ�������ʽ                                           */
    GINKGO_CMD_SET_VIDBITRATE,          /* ������Ƶ��������                                             */
    GINKGO_CMD_GET_VIDBITRATE,          /* ��ȡ��Ƶ��������                                             */
    GINKGO_CMD_SET_VIDFPS,              /* ����ͼ��֡��     return = frame rate                         */
    GINKGO_CMD_GET_VIDFPS,              /* ��ȡͼ��֡��     return = frame rate                         */
    GINKGO_CMD_SET_FRAMESIZE,           /* ����ͼ���С     aux = (width << 16) + height                */
    GINKGO_CMD_GET_FRAMESIZE,           /* ��ȡͼ���С     return = (width << 16) + height             */
    GINKGO_CMD_SET_VIDEO_SOURCE,        /* ����¼����ƵԴ     aux = __ginkgo_video_src_t                */
    GINKGO_CMD_GET_VIDEO_SOURCE,        /* ��ȡ¼����ƵԴ     return = __ginkgo_audio_src_t             */

    GINKGO_CMD_SET_USER_FILEOP = 0x0600,  /*  �����û���context,��ص�����,�ļ����ȵ�, (pbuffer = __cedar_usr_file_op_t *)                                */

    GINKGO_CMD_

} __ginkgo_cmd_set_t;


typedef enum __GINKGO_FEEDBACK_MSG          /* ����ý��¼���м��������Ӧ�ó������Ϣ����               */
{
    GINKGO_ERROR_NOP                 =   0, /* ����Ϣ                                                   */

    //ginkgo feed back error type
    GINKGO_ERROR_CREATE_FILE_FAIL    = -20, /* ����ý���ļ�ʧ��                                         */
    GINKGO_ERROR_FILE_FMT_UNKNOWN,          /* �޷�ʶ��ǰý���ļ���ʽ                                 */
    GINKGO_ERROR_FILE_NOT_SUPPORT,          /* ��֧�ֵ�ǰý���ļ���ʽ¼��                               */
    GINKGO_ERROR_NO_FILE_INPUT,             /* δ����Ҫ¼�Ƶ�ý���ļ�                                   */
    GINKGO_ERROR_WRITE_FILE_FAIL,           /* д���ļ�����ʧ��                                         */
    GINKGO_ERROR_DISK_FULL,                 /* ������                                                   */

    GINKGO_ERROR_START_PLG_FAIL      = -40, /* �������ʧ��                                             */
    GINKGO_ERROR_AENC_ERROR,                /* ��Ƶ����������                                           */
    GINKGO_ERROR_VENC_ERROR,                /* ��Ƶ����������                                           */

    GINKGO_ERROR_UNKOWN              = -60, /* δ֪����                                                 */
    GINKGO_ERROR_REQ_MEMORY_FAIL,           /* �����ڴ�ʧ��                                             */

    //ginkgo feed back status type
    GINKGO_FEDBAK_NO_ERROR           =  1,  /* ����Ϣ                                                   */

    GINKGO_FEDBAK_

} __ginkgo_feedback_msg_t;


typedef enum __GINKGO_STATUS                /* the status of ginkgo             */
{
    GINKGO_STAT_IDLE=0,                     /* idle                             */
    GINKGO_STAT_RECORD,                     /* record                           */
    GINKGO_STAT_PAUSE,                      /* pause                            */

    GINKGO_STAT_

} __ginkgo_status_t;


typedef enum __GINKGO_MEDIA_FILE_FMT
{
    GINKGO_MEDIA_FILE_FMT_NONE = CEDAR_MEDIA_FILE_FMT_NONE,

    //define audio file format
    GINKGO_MEDIA_FILE_FMT_MP3   = CEDAR_MEDIA_FILE_FMT_MP3, //0x0010,    /* *.mp3, ��Ƶ�ļ�                      */
    GINKGO_MEDIA_FILE_FMT_WAV   = CEDAR_MEDIA_FILE_FMT_WAV,           /* *.wav, ��Ƶ�ļ�                      */
    GINKGO_MEDIA_FILE_FMT_WMA   = CEDAR_MEDIA_FILE_FMT_WMA,           // *.WMA, ��Ƶ�ļ�

    //define video file file format
    GINKGO_MEDIA_FILE_FMT_AVI = CEDAR_MEDIA_FILE_FMT_AVI, //0x0020,  /* *.avi, ��Ӱ��Ƶ�ļ�                  */
    GINKGO_MEDIA_FILE_FMT_MP4 = CEDAR_MEDIA_FILE_FMT_MP4, /* *.mp4, mpeg-4ӰƬ��Ƶ�ļ�            */

    GINKGO_MEDIA_FILE_FMT_

} __ginkgo_media_file_fmt_t;


typedef enum __GINKGO_AUDIO_FMT
{
    GINKGO_ABS_TYPE_NONE            = CEDAR_ABS_TYPE_NONE, //0,           /* û����Ƶ��                           */

    GINKGO_ABS_TYPE_ADPCM           = CEDAR_ABS_TYPE_ADPCM,              /* adpcm                                */
    GINKGO_ABS_TYPE_MPEG12_LAYER3   = CEDAR_ABS_TYPE_MPEG12_LAYER3,      /* mp3                                  */
    GINKGO_ABS_TYPE_MPEG_AAC_LC     = CEDAR_ABS_TYPE_MPEG_AAC_LC,        /* aac                                  */
    GINKGO_ABS_TYPE_WMA_STANDARD    = CEDAR_ABS_TYPE_WMA_STANDARD,       /* wma                                  */

    GINKGO_ABS_TYPE_

} __ginkgo_audio_fmt_t;


typedef enum __GINKGO_VIDEO_FMT
{
    GINKGO_VBS_TYPE_NONE = CEDAR_VBS_TYPE_NONE,           /* û����Ƶ��                           */

    GINKGO_VBS_TYPE_MJPG = CEDAR_VBS_TYPE_MJPEG,               /* motion jpeg                          */
    GINKGO_VBS_TYPE_XVID = CEDAR_VBS_TYPE_XVID,               /* mpeg4-xvid                           */
    GINKGO_VBS_TYPE_H264 = CEDAR_VBS_TYPE_H264,               /* h264                                 */

    GINKGO_VBS_TYPE_

} __ginkgo_video_fmt_t;


typedef enum __GINKGO_AUDIO_CHN         /* audio channel type                   */
{
    GINKGO_AUDIO_CHN_NONE=0,            /* none audio channel                   */

    GINKGO_AUDIO_CHN_MONO,              /* mono                                 */
    GINKGO_AUDIO_CHN_STEREO,            /* stereo                               */

    GINKGO_AUDIO_CHN_

} __ginkgo_audio_chn_t;


typedef enum __GINKGO_AUDIO_SAMP_WIDTH
{
    //NOTE:we only support 16bits right now!!!
    GINKGO_AUDIO_SAMP_WIDTH_0   = 0,      /* invalid audio sample width         */

    GINKGO_AUDIO_SAMP_WIDTH_8   = 8,      /* 8 bits audio sample                */
    GINKGO_AUDIO_SAMP_WIDTH_16  =16,      /* 16 bits audio sample               */
    GINKGO_AUDIO_SAMP_WIDTH_24  =24,      /* 24 bits audio sample               */
    GINKGO_AUDIO_SAMP_WIDTH_32  =32,      /* 32 bits audio sample               */

} __ginkgo_audio_samp_width_t;


typedef enum __GINKGO_AUDIO_SRC         /* audio source type                    */
{
    GINKGO_AUDIO_SRC_NONE=0,            /* none audio source                    */

    GINKGO_AUDIO_SRC_MIC,               /* microphone record                    */
    GINKGO_AUDIO_SRC_LINEIN,            /* line-in record                       */
    GINKGO_AUDIO_SRC_FM,                /* fm record                            */

    GINKGO_AUDIO_SRC_

} __ginkgo_audio_src_t;


typedef enum __GINKGO_VIDEO_SRC         /* video source type                    */
{
    GINKGO_VIDEO_SRC_NONE=0,            /* none video source                    */

    GINKGO_VIDEO_SRC_SENSOR,            /* sensor record                        */
    GINKGO_VIDEO_SRC_TVIN,              /* TV-in record                         */

    GINKGO_VIDEO_SRC_

} __ginkgo_video_src_t;


typedef struct __GINKGO_AUDIO_INF
{
    __u32       uAbsFmt;                /* ��Ƶ�������ͣ�if(uAbsFmt == GINKGO_ABS_TYPE_NONE), ��
                                           �������������Ƶ����                             */
    __u32       uVolume;                /* ¼��������ֵ��������÷Ƿ��������defaultֵ      */
    __u32       uChn;                   /* ¼��������ģʽ��������÷Ƿ��������defaultֵ, __ginkgo_audio_chn_t    */
    __u32       uSampWidth;             /* ¼���������λ��������÷Ƿ��������defaultֵ  */
    __u32       uAudioBps;              /* ��Ƶ���������ʣ�������÷Ƿ��������defaultֵ    */
    __u32       uAudioSampRate;         /* ��Ƶ�����Ĳ����ʣ�������÷Ƿ��������defaultֵ  */
    __u32       uAudioSource;           /* ��ƵԴ��������÷Ƿ��������defaultֵ, GINKGO_AUDIO_SRC_MIC */

} __ginkgo_audio_inf_t;


typedef struct __GINKGO_VIDEO_INF
{
    __u32       uVbsFmt;                /* ��Ƶ�����ͣ�if(uVbsFmt == GINKGO_VBS_TYPE_NONE), ��
                                           ����������ص���Ƶ����                           */
    __u32       uVideoBps;              /* ��Ƶ����������,������÷Ƿ��������defaultֵ     */
    __u32       uVideoFrmRate;          /* ��Ƶ������֡��,������÷Ƿ��������defaultֵ     */
    __u32       uVideoFrmSize;          /* ��Ƶͼ��Ĵ�С,������÷Ƿ��������defaultֵ     */
    __u32       uVideoSource;           /* ��ƵԴ��������÷Ƿ��������defaultֵ            */

} __ginkgo_video_inf_t;


//the information of the media file which will be input to play
typedef struct __GINKGO_MEDIA_FILE_INF
{
    //Ҫ¼�Ƶ��ļ���ʽ
    __u32       uFileFmt;               /* Ҫ¼�Ƶ��ļ���ʽ, __ginkgo_media_file_fmt_t  */
    char        *file_path;             /* ¼�Ƶ�ý���ļ�����ľ���·���������δ���ã��򱣴浽default·��  */

    //��Ƶ�����Ϣ
    __ginkgo_audio_inf_t    audio_inf;  /* ��Ƶ¼�Ƹ�ʽ��Ϣ                     */

    //��Ƶ�����Ϣ
    __ginkgo_video_inf_t    video_inf;  /* ��Ƶ¼�Ƹ�ʽ��Ϣ                     */

} __ginkgo_media_file_inf;



#endif  /* _MOD_GINKGO_H_ */


