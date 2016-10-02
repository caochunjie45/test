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
* Description: herb_core提供的头文件，所有基于herb_core的中间件的对外头文件都
               要包含它
********************************************************************************
*/
#ifndef _MOD_HERB_CORE_H_
#define _MOD_HERB_CORE_H_

#define HERB_FPATH_SIZE         (1024*4)    /* 媒体文件路径名长度最大值                                 */
#define HERB_SPECTRUM_LEVEL     (32)        /* spectrum level, __u16 for save every level，录音的频谱   */
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
    __u32       uAbsFmt;                /* 音频码流类型，if(uAbsFmt == HERB_ABS_TYPE_NONE), 将
                                           忽略其它相关音频参数                             */
    __u32       uVolume;                /* 录音的音量值，如果设置非法，则采用default值,MIN_VOLUME_VAL~MAX_VOLUME_VAL      */
    __u32       uChn;                   /* 录音的声道模式，如果设置非法，则采用default值,HERB_AUDIO_CHN_MONO    */
    __u32       uSampWidth;             /* 录音样本点的位宽，如果设置非法，则采用default值  */
    __u32       uAudioBps;              /* 编码后的音频码流的码率，如果设置非法，则采用default值，单位:bit/s    */
    __u32       uAudioSampRate;         /* 音频码流的采样率，如果设置非法，则采用default值  */
    __u32       uAudioSource;           /* 音频源，如果设置非法，则采用default值            */
    __s32       frame_style;            /* for 0:add head,1:raw data                        */

    //额外的不需要应用程序填写的信息,这些信息由herb内部填写，交给内部的其他模块。
    __s32       sample_cnt_per_frame;   /* 每个audio frame包含的sample的数量 */
} __herb_audio_inf_t;

typedef struct __HERB_VIDEO_INF
{
    __u32       uVbsFmt;                /* 视频流类型, if(uVbsFmt == SW_CODEC_TYPE_NONE), 将忽略其它相关的视频参数, __herb_codec_t */
    __s32       nWidth;                 /* 要编码的视频图像的宽度 */
    __s32       nHeight;                /* 要编码的视频图像的高度 */
    __u32       uVideoFrmRate;          /* 视频码流的帧率,如果设置非法，则采用default值, 数值放大1000倍     */
    __s32       nVideoBps;              /* 视频码流的码率,如果设置非法，则采用default值，单位:bit/s     */

    __s32       color_format;           /* yuv, bayerRGB(4种), PIXEL_YUV420*/
    __s32       color_space;            /* BT601, bt709*/

    __s16       qp_max;
    __s16       qp_min;
    __s32       maxKeyInterval;         /*keyframe Interval，指2个关键帧之间的P帧的最大数量             */

    //__u32       uVideoSource;         /* 视频源，如果设置非法，则采用default值                      */
} __herb_video_inf_t;
//the information of the media file which will be input to play
typedef struct __HERB_MEDIA_FILE_INF
{
    //要录制的文件格式
    __u32       uFileFmt;                   /* 要录制的文件格式,__herb_media_file_fmt_t                         */
    char        file_path[HERB_FPATH_SIZE]; /* 录制的媒体文件保存的绝对路径名，如果未设置，则保存到default路径  */

    //音频相关信息
    __herb_audio_inf_t    audio_inf;        /* 音频录制格式信息                                         */

    //视频相关信息
    __herb_video_inf_t    video_inf;        /* 视频录制格式信息,参考__video_encode_format_t             */

    //标题，时间等
    char        title[HERB_FILE_TITLE_SIZE];
    __s32       create_time;                /* 数字的规格定义，由mux决定，应用程序遵守                  */
} __herb_media_file_inf;

#endif  /* _MOD_HERB_CORE_H_ */

