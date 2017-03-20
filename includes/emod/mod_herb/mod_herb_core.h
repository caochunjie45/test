/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_herb sub-system
*                                 herb_core module
*
*          (c) Copyright 2009-2010, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : mod_herb_core.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2010-2-22
* Description: herb_core�ṩ��ͷ�ļ������л���herb_core���м���Ķ���ͷ�ļ���
               Ҫ������
********************************************************************************
*/
#ifndef _MOD_HERB_CORE_H_
#define _MOD_HERB_CORE_H_

#define HERB_FPATH_SIZE         (1024*4)    /* ý���ļ�·�����������ֵ                                 */
#define HERB_SPECTRUM_LEVEL     (32)        /* spectrum level, __u16 for save every level��¼����Ƶ��   */
#define FRAME_RATE_BASE         1000
#define HERB_FILE_TITLE_SIZE    (1024)

typedef enum __HERB_AUDIO_CHN         /* audio channel type                   */
{
    HERB_AUDIO_CHN_NONE=0,            /* none audio channel                   */

    HERB_AUDIO_CHN_MONO,              /* mono                                 */
    HERB_AUDIO_CHN_STEREO,            /* stereo                               */

    HERB_AUDIO_CHN_

} __herb_audio_chn_t;


typedef enum __HERB_AUDIO_SAMP_WIDTH
{
    //NOTE:we only support 16bits right now!!!
    HERB_AUDIO_SAMP_WIDTH_0   = 0,      /* invalid audio sample width         */

    HERB_AUDIO_SAMP_WIDTH_8   = 8,      /* 8 bits audio sample                */
    HERB_AUDIO_SAMP_WIDTH_16  =16,      /* 16 bits audio sample               */
    HERB_AUDIO_SAMP_WIDTH_24  =24,      /* 24 bits audio sample               */
    HERB_AUDIO_SAMP_WIDTH_32  =32,      /* 32 bits audio sample               */

} __herb_audio_samp_width_t;

typedef enum __HERB_AUDIO_SRC         /* audio source type                    */
{
    HERB_AUDIO_SRC_NONE=0,            /* none audio source                    */

    HERB_AUDIO_SRC_MIC,               /* microphone record                    */
    HERB_AUDIO_SRC_LINEIN,            /* line-in record                       */
    HERB_AUDIO_SRC_FM,                /* fm record                            */

    HERB_AUDIO_SRC_

} __herb_audio_src_t;

typedef struct __HERB_AUDIO_INF
{
    __u32       uAbsFmt;                /* ��Ƶ�������ͣ�if(uAbsFmt == HERB_ABS_TYPE_NONE), ��
                                           �������������Ƶ����                             */
    __u32       uVolume;                /* ¼��������ֵ��������÷Ƿ��������defaultֵ,MIN_VOLUME_VAL~MAX_VOLUME_VAL      */
    __u32       uChn;                   /* ¼��������ģʽ��������÷Ƿ��������defaultֵ,HERB_AUDIO_CHN_MONO    */
    __u32       uSampWidth;             /* ¼���������λ��������÷Ƿ��������defaultֵ  */
    __u32       uAudioBps;              /* ��������Ƶ���������ʣ�������÷Ƿ��������defaultֵ����λ:bit/s    */
    __u32       uAudioSampRate;         /* ��Ƶ�����Ĳ����ʣ�������÷Ƿ��������defaultֵ  */
    __u32       uAudioSource;           /* ��ƵԴ��������÷Ƿ��������defaultֵ            */
    __s32       frame_style;            /* for 0:add head,1:raw data                        */

    //����Ĳ���ҪӦ�ó�����д����Ϣ,��Щ��Ϣ��herb�ڲ���д�������ڲ�������ģ�顣
    __s32       sample_cnt_per_frame;   /* ÿ��audio frame������sample������ */
} __herb_audio_inf_t;

typedef struct __HERB_VIDEO_INF
{
    __u32       uVbsFmt;                /* ��Ƶ������, if(uVbsFmt == SW_CODEC_TYPE_NONE), ������������ص���Ƶ����, __herb_codec_t */
    __s32       nWidth;                 /* Ҫ�������Ƶͼ��Ŀ�� */
    __s32       nHeight;                /* Ҫ�������Ƶͼ��ĸ߶� */
    __u32       uVideoFrmRate;          /* ��Ƶ������֡��,������÷Ƿ��������defaultֵ, ��ֵ�Ŵ�1000��     */
    __s32       nVideoBps;              /* ��Ƶ����������,������÷Ƿ��������defaultֵ����λ:bit/s     */

    __s32       color_format;           /* yuv, bayerRGB(4��), PIXEL_YUV420*/
    __s32       color_space;            /* BT601, bt709*/

    __s16       qp_max;
    __s16       qp_min;
    __s32       maxKeyInterval;         /*keyframe Interval��ָ2���ؼ�֮֡���P֡���������             */

    //__u32       uVideoSource;         /* ��ƵԴ��������÷Ƿ��������defaultֵ                      */
} __herb_video_inf_t;
//the information of the media file which will be input to play
typedef struct __HERB_MEDIA_FILE_INF
{
    //Ҫ¼�Ƶ��ļ���ʽ
    __u32       uFileFmt;                   /* Ҫ¼�Ƶ��ļ���ʽ,__herb_media_file_fmt_t                         */
    char        file_path[HERB_FPATH_SIZE]; /* ¼�Ƶ�ý���ļ�����ľ���·���������δ���ã��򱣴浽default·��  */

    //��Ƶ�����Ϣ
    __herb_audio_inf_t    audio_inf;        /* ��Ƶ¼�Ƹ�ʽ��Ϣ                                         */

    //��Ƶ�����Ϣ
    __herb_video_inf_t    video_inf;        /* ��Ƶ¼�Ƹ�ʽ��Ϣ,�ο�__video_encode_format_t             */

    //���⣬ʱ���
    char        title[HERB_FILE_TITLE_SIZE];
    __s32       create_time;                /* ���ֵĹ���壬��mux������Ӧ�ó�������                  */
} __herb_media_file_inf;

#endif  /* _MOD_HERB_CORE_H_ */

