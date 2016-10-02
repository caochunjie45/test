/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, kevin China
*                                              All Rights Reserved
*
* File   : media_type.h
* Version: V1.0
* By     : kevin
* Date   : 2008-6-26 14:18
    [cedarlib]

*********************************************************************************************************
*/
#ifndef _MEDIA_TYPE_H
#define _MEDIA_TYPE_H

//#include "epdk.h"
#define MACROBLOCK_WIDTH    (16)    //单位:像素
#define MACROBLOCK_HEIGHT   (16)
#define FRAME_RATE_BASE     1000

//Position   (x,y)
typedef struct Position
{
    short       uX;         // Horizontal position .
    short       uY;         // Vertical position.

} __position_t;

//rectangle size
typedef struct RectSize
{
    short       uWidth;     // Horizontal size.
    short       uHeight;    // Vertical size.

} __rect_size_t;

//Rectangle
typedef struct Rect
{
    short       uStartX;    // Horizontal start point.
    short       uStartY;    // Vertical start point.
    short       uWidth;     // Horizontal size.
    short       uHeight;    // Vertical size.

} __vdrv_rect_t;

//define the type base number of message  which taken with socket
#define CEDAR_MSGTYPE_NONE          (0<<16)     /* none message                                                     */
#define CEDAR_PSR2ADEC_MSG_BASE     (1<<16)     /* message base number between parser and audio deocder             */
#define CEDAR_PSR2VDEC_MSG_BASE     (2<<16)     /* message base number between parser and video deocder             */
#define CEDAR_PSR2LDEC_MSG_BASE     (3<<16)     /* message base number between parser and lyric deocder             */
#define CEDAR_ADEC2ARDR_MSG_BASE    (4<<16)     /* message base number between audio decoder and render             */
#define CEDAR_ARDR2APLY_MSG_BASE    (5<<16)     /* message base number between render and audio playback            */
#define CEDAR_VDEC2VPLY_MSG_BASE    (6<<16)     /* message base number between video decoder and video playback     */
#define CEDAR_LDEC2LRDR_MSG_BASE    (7<<16)     /* message base number between lyric decoder and lyric render       */
#define CEDAR_LRDR2LPLY_MSG_BASE    (8<<16)     /* message base number between lyric render and lyric playback      */
#define CEDAR_ADEC2ADRV_MSG_BASE    (9<<16)     /* message base number between audio decoder plug-in and driver     */
#define CEDAR_VDEC2VDRV_MSG_BASE    (10<<16)    /* message base number between video decoder plug-in and driver     */
#define CEDAR_ARDR2ADRV_MSG_BASE    (11<<16)    /* message base number between audio render plug-in and driver      */

//define the type base number of message  which taken with socket
#define GINKGO_MSGTYPE_NONE         (0<<16)     /* none message                                                 */
#define GINKGO_MUXER2AENC_MSG_BASE  (1<<16)     /* message base number between muxer and audio encoder          */
#define GINKGO_MUXER2VENC_MSG_BASE  (2<<16)     /* message base number between muxer and video encoder          */
#define GINKGO_AENC2AREC_MSG_BASE   (3<<16)     /* message base number between audio encoder and audio recorder */
#define GINKGO_VENC2VREC_MSG_BASE   (4<<16)     /* message base number between video encoder and video recorder */
#define GINKGO_AENC2ADRV_MSG_BASE   (5<<16)     /* message base number between audio encoder plug-in and driver */
#define GINKGO_VENC2VDRV_MSG_BASE   (6<<16)     /* message base number between video encoder plug-in and driver */

/*
================================================================================
                            file format definition
================================================================================
*/
typedef enum __CEDARLIB_FILE_FMT
{
    CEDARLIB_FILE_FMT_NONE = 0x0000,   //CEDAR_MEDIA_FILE_FMT_NONE = 0x0000,

    //define audio file format
    CEDARLIB_FILE_FMT_MP3=0x0001,      //CEDAR_MEDIA_FILE_FMT_MP3=0x0001,              /* *.mp3, 音频文件                      */
    CEDARLIB_FILE_FMT_WAV,             //CEDAR_MEDIA_FILE_FMT_WAV,                     /* *.wav, 音频文件                      */
    CEDARLIB_FILE_FMT_WMA,             //CEDAR_MEDIA_FILE_FMT_WMA,                     /* *.wmv, Windows media音/视频文件      */
    CEDARLIB_FILE_FMT_APE,             //CEDAR_MEDIA_FILE_FMT_APE,                     /* *.ape, APE无损音频                   */
    CEDARLIB_FILE_FMT_FLAC,            //CEDAR_MEDIA_FILE_FMT_FLAC,                    /* *.flac, FLAC音频                     */
    CEDARLIB_FILE_FMT_OGG,             //CEDAR_MEDIA_FILE_FMT_OGG,                     /* *.ogg, OGG Vorbis音频文件            */
    CEDARLIB_FILE_FMT_RA,              //CEDAR_MEDIA_FILE_FMT_RA,                      /* *.ra, Real音频                       */
    CEDARLIB_FILE_FMT_MP1,             //CEDAR_MEDIA_FILE_FMT_MP1,                     /* *.mp1, mp2/mpa音频                   */
    CEDARLIB_FILE_FMT_MP2,             //CEDAR_MEDIA_FILE_FMT_MP2,                     /* *.mp2, mp2/mpa音频                   */
    CEDARLIB_FILE_FMT_AAC,             //CEDAR_MEDIA_FILE_FMT_AAC,                     /* *.aac, MPEG4音频                     */
    CEDARLIB_FILE_FMT_AC3,             //CEDAR_MEDIA_FILE_FMT_AC3,                     /* *.ac3, 音频文件                      */
    CEDARLIB_FILE_FMT_DTS,             //CEDAR_MEDIA_FILE_FMT_DTS,                     /* *.dts, DVD音频                       */
    CEDARLIB_FILE_FMT_AIF,             //CEDAR_MEDIA_FILE_FMT_AIF,                     /* *.aif, Mac音频                       */
    CEDARLIB_FILE_FMT_AIFF,            //CEDAR_MEDIA_FILE_FMT_AIFF,                    /* *.aiff, Mac音频                      */
    CEDARLIB_FILE_FMT_AIFC,            //CEDAR_MEDIA_FILE_FMT_AIFC,                    /* *.aifc, Mac音频                      */
    CEDARLIB_FILE_FMT_AMR,             //CEDAR_MEDIA_FILE_FMT_AMR,                     /* *.amr, 移动音频                      */
    CEDARLIB_FILE_FMT_MAC,             //CEDAR_MEDIA_FILE_FMT_MAC,                     /* *.mac, APE无损音频                   */
    CEDARLIB_FILE_FMT_TTA,             //CEDAR_MEDIA_FILE_FMT_TTA,                     /* *.tta, TTA音频                       */
    CEDARLIB_FILE_FMT_M4A,             //CEDAR_MEDIA_FILE_FMT_M4A,                     /* *.m4a, MPEG4音频                     */
    CEDARLIB_FILE_FMT_CDA,             //CEDAR_MEDIA_FILE_FMT_CDA,                     /* *.cda, CD音频                        */
    CEDARLIB_FILE_FMT_AU,              //CEDAR_MEDIA_FILE_FMT_AU,                      /* *.au, Sun压缩音频                    */
    CEDARLIB_FILE_FMT_ACC,             //CEDAR_MEDIA_FILE_FMT_ACC,                     /* *.acc, 音频                          */
    CEDARLIB_FILE_FMT_MIDI,            //CEDAR_MEDIA_FILE_FMT_MIDI,                    /* *.midi, mid音频文件                  */
    CEDARLIB_FILE_FMT_MID,             //CEDAR_MEDIA_FILE_FMT_MID,                     /* *.mid, mid音频文件                   */
    CEDARLIB_FILE_FMT_RMI,             //CEDAR_MEDIA_FILE_FMT_RMI,                     /* *.rmi, mid音频文件                   */
    CEDARLIB_FILE_FMT_MP5,             //CEDAR_MEDIA_FILE_FMT_MP5,                     /* *.mp5, 音频文件                      */
    CEDARLIB_FILE_FMT_MPA,             //CEDAR_MEDIA_FILE_FMT_MPA,                     /* *.mpa, mpeg音频文件                  */
    CEDARLIB_FILE_FMT_MPGA,            //CEDAR_MEDIA_FILE_FMT_MPGA,                    /* *.mpga, mp3音频文件                  */
    CEDARLIB_FILE_FMT_ACT,             //CEDAR_MEDIA_FILE_FMT_ACT,                     /* *.act, 音频文件                      */
    CEDARLIB_FILE_FMT_ATRC,            //CEDAR_MEDIA_FILE_FMT_ATRC,                    /* *.omg,oma,.aa3, sony openMG音频文件  */
	CEDARLIB_FILE_FMT_ALAC,            //CEDAR_MEDIA_FILE_FMT_ALAC,                    /* *.caf, alac音频文件                  */

    //define video file file format               
    CEDARLIB_FILE_FMT_AVI = 0x1000,    //CEDAR_MEDIA_FILE_FMT_AVI = 0x1000,            /* *.avi, 电影视频文件                  */
    CEDARLIB_FILE_FMT_FLV,             //CEDAR_MEDIA_FILE_FMT_FLV,                     /* *.flv, flash video视频文件           */
    CEDARLIB_FILE_FMT_MPEG,            //CEDAR_MEDIA_FILE_FMT_MPEG,                    /* *.mpeg, mpeg影片视频文件             */
    CEDARLIB_FILE_FMT_MPG,             //CEDAR_MEDIA_FILE_FMT_MPG,                     /* *.mpg, mpeg影片视频文件              */
    CEDARLIB_FILE_FMT_VOB,             //CEDAR_MEDIA_FILE_FMT_VOB,                     /* *.vob, mpeg影片视频文件              */
    CEDARLIB_FILE_FMT_DAT,             //CEDAR_MEDIA_FILE_FMT_DAT,                     /* *.dat, VCD视频文件                   */
    CEDARLIB_FILE_FMT_MP4,             //CEDAR_MEDIA_FILE_FMT_MP4,                     /* *.mp4, mpeg-4影片视频文件            */
    CEDARLIB_FILE_FMT_3GP,             //CEDAR_MEDIA_FILE_FMT_3GP,                     /* *.3gp, 移动视频                      */
    CEDARLIB_FILE_FMT_MOV,             //CEDAR_MEDIA_FILE_FMT_MOV,                     /* *.mov, QuickTime影片                 */
    CEDARLIB_FILE_FMT_WMV,             //CEDAR_MEDIA_FILE_FMT_WMV,                     /* *.wmv, Windows media音/视频文件      */
    CEDARLIB_FILE_FMT_ASF,             //CEDAR_MEDIA_FILE_FMT_ASF,                     /* *.asf, Windows media音/视频文件      */
    CEDARLIB_FILE_FMT_RM,              //CEDAR_MEDIA_FILE_FMT_RM,                      /* *.rm, real影片                       */
    CEDARLIB_FILE_FMT_RMVB,            //CEDAR_MEDIA_FILE_FMT_RMVB,                    /* *.rmvb, real影片                     */
    CEDARLIB_FILE_FMT_RAM,             //CEDAR_MEDIA_FILE_FMT_RAM,                     /* *.ram, real影片                      */
    CEDARLIB_FILE_FMT_SCM,             //CEDAR_MEDIA_FILE_FMT_SCM,                     /* *.scm, real影片                      */
    CEDARLIB_FILE_FMT_DIVX,            //CEDAR_MEDIA_FILE_FMT_DIVX,                    /* *.divx, 电影视频                     */
    CEDARLIB_FILE_FMT_MKV,             //CEDAR_MEDIA_FILE_FMT_MKV,                     /* *.mkv, matroska影片视频文件          */
    CEDARLIB_FILE_FMT_OGM,             //CEDAR_MEDIA_FILE_FMT_OGM,                     /* *.ogm, ogg影片视频文件               */
    CEDARLIB_FILE_FMT_M4V,             //CEDAR_MEDIA_FILE_FMT_M4V,                     /* *.m4v, mpeg-4影片视频                */
    CEDARLIB_FILE_FMT_M4P,             //CEDAR_MEDIA_FILE_FMT_M4P,                     /* *.m4p, mpeg-4影片视频                */
    CEDARLIB_FILE_FMT_M4B,             //CEDAR_MEDIA_FILE_FMT_M4B,                     /* *.m4b, mpeg-4影片视频                */
    CEDARLIB_FILE_FMT_TP,              //CEDAR_MEDIA_FILE_FMT_TP,                      /* *.tp, mpeg-2影片视频文件             */
    CEDARLIB_FILE_FMT_TPR,             //CEDAR_MEDIA_FILE_FMT_TPR,                     /* *.tpr, mpeg影片视频文件              */
    CEDARLIB_FILE_FMT_TS,              //CEDAR_MEDIA_FILE_FMT_TS,                      /* *.ts, mpeg-2影片视频文件             */
    CEDARLIB_FILE_FMT_PVA,             //CEDAR_MEDIA_FILE_FMT_PVA,                     /* *.pva, mpeg影片视频文件              */
    CEDARLIB_FILE_FMT_PSS,             //CEDAR_MEDIA_FILE_FMT_PSS,                     /* *.pss, mpeg影片视频文件              */
    CEDARLIB_FILE_FMT_MPE,             //CEDAR_MEDIA_FILE_FMT_MPE,                     /* *.mpe, mpeg影片视频文件              */
    CEDARLIB_FILE_FMT_WV,              //CEDAR_MEDIA_FILE_FMT_WV,                      /* *.wv, mpeg影片视频文件               */
    CEDARLIB_FILE_FMT_M2TS,            //CEDAR_MEDIA_FILE_FMT_M2TS,                    /* *.m2ts, m2ts视频                     */
    CEDARLIB_FILE_FMT_EVO,             //CEDAR_MEDIA_FILE_FMT_EVO,                     /* *.evo, HD-DVD视频                    */
    CEDARLIB_FILE_FMT_RPM,             //CEDAR_MEDIA_FILE_FMT_RPM,                     /* *.rpm, Real影片                      */
    CEDARLIB_FILE_FMT_3GPP,            //CEDAR_MEDIA_FILE_FMT_3GPP,                    /* *.3gp, 移动视频                      */
    CEDARLIB_FILE_FMT_3G2,             //CEDAR_MEDIA_FILE_FMT_3G2,                     /* *.3gp, 移动视频                      */
    CEDARLIB_FILE_FMT_3GP2,            //CEDAR_MEDIA_FILE_FMT_3GP2,                    /* *.3gp, 移动视频                      */
    CEDARLIB_FILE_FMT_QT,              //CEDAR_MEDIA_FILE_FMT_QT,                      /* *.qt, QuickTime影片                  */
    CEDARLIB_FILE_FMT_WMP,             //CEDAR_MEDIA_FILE_FMT_WMP,                     /* *.wmp, Windows media音/视频文件      */
    CEDARLIB_FILE_FMT_WM,              //CEDAR_MEDIA_FILE_FMT_WM,                      /* *.wm, Windows media音/视频文件       */
    CEDARLIB_FILE_FMT_AMV,             //CEDAR_MEDIA_FILE_FMT_AMV,                     /* *.avm, 压缩视频                      */
    CEDARLIB_FILE_FMT_DSM,             //CEDAR_MEDIA_FILE_FMT_DSM,                     /* *.dsm, NDS视频                       */
    CEDARLIB_FILE_FMT_M1V,             //CEDAR_MEDIA_FILE_FMT_M1V,                     /* *.m1v, 影片视频                      */
    CEDARLIB_FILE_FMT_M2V,             //CEDAR_MEDIA_FILE_FMT_M2V,                     /* *.m2v, 影片视频                      */
    CEDARLIB_FILE_FMT_PMP,             //CEDAR_MEDIA_FILE_FMT_PMP,                     /* *.pmp, 影片视频                      */
    CEDARLIB_FILE_FMT_SMK,             //CEDAR_MEDIA_FILE_FMT_SMK,                     /* *.smk, 影片视频                      */
    CEDARLIB_FILE_FMT_BIK,             //CEDAR_MEDIA_FILE_FMT_BIK,                     /* *.bik, Bink影片视频                  */
    CEDARLIB_FILE_FMT_RAT,             //CEDAR_MEDIA_FILE_FMT_RAT,                     /* *.rat, ratDVD影片视频                */
    CEDARLIB_FILE_FMT_VG2,             //CEDAR_MEDIA_FILE_FMT_VG2,                     /* *.vg2, XVD影片视频                   */
    CEDARLIB_FILE_FMT_IVF,             //CEDAR_MEDIA_FILE_FMT_IVF,                     /* *.ivf, Indeo影片视频                 */
    CEDARLIB_FILE_FMT_VP6,             //CEDAR_MEDIA_FILE_FMT_VP6,                     /* *.vp6, VP6影片视频                   */
    CEDARLIB_FILE_FMT_VP7,             //CEDAR_MEDIA_FILE_FMT_VP7,                     /* *.vp7, VP7影片视频                   */
    CEDARLIB_FILE_FMT_D2V,             //CEDAR_MEDIA_FILE_FMT_D2V,                     /* *.d2v, DVD2AVI Project影片视频       */
    CEDARLIB_FILE_FMT_MOD,             //CEDAR_MEDIA_FILE_FMT_MOD,                     /* *.mod, 摄像机格式                    */
    CEDARLIB_FILE_FMT_M2P,             //CEDAR_MEDIA_FILE_FMT_M2P,                     /* *.m2p, 影片视频                      */
    CEDARLIB_FILE_FMT_VID,             //CEDAR_MEDIA_FILE_FMT_VID,                     /* *.vid, mp4手机无声影片视频           */
    CEDARLIB_FILE_FMT_PMP2,            //CEDAR_MEDIA_FILE_FMT_PMP2,                    /* *.pmp2, 影片视频                     */
    CEDARLIB_FILE_FMT_MTS,             //CEDAR_MEDIA_FILE_FMT_MTS,                     /* *.mts, DV视频                        */
    CEDARLIB_FILE_FMT_F4V,             //CEDAR_MEDIA_FILE_FMT_F4V,                     /* *.f4v, flash video视频文件           */ 
    
    //user define format type                     
    CEDARLIB_FILE_FMT_PSR = 0x2000,    //CEDAR_MEDIA_FILE_FMT_PSR = 0x2000,            /* *.psr, 用户自定义的媒体文件格式      */
    CEDARLIB_FILE_FMT_DEV = 0x2001,    //CEDAR_MEDIA_FILE_FMT_DEV = 0x2001,            /* *.dev, 流设备文件*/

    //用户自定义3D文件
    CEDARLIB_FILE_FMT_3DV = 0x2200,        /* .3dv, 3DV video视频文件, 维尚自定义3d视频格式 */
    //define unknown format type
    CEDARLIB_FILE_FMT_UNKNOWN = 0xffff,//CEDAR_MEDIA_FILE_FMT_UNKNOWN = 0xffff,        /* 媒体文件格式无法识别              */
    CEDARLIB_FILE_FMT_                 //CEDAR_MEDIA_FILE_FMT_

} __cedarlib_file_fmt_t;

/*
================================================================================
                            video & color properties
================================================================================
*/

//define the type of video bitstream
enum __CEDARLIB_VIDEO_CODEC_TYPE    // <==>__cedar_video_fmt_t
{
    VIDEO_NONE              = 0,  //CEDAR_VBS_TYPE_NONE = 0
    VIDEO_UNKNOWN           = 1,  //CEDAR_VBS_TYPE_UNKNOWN = 1
    VIDEO_JPEG              = 2,  //CEDAR_VBS_TYPE_JPEG = 2
    VIDEO_MJPEG             ,  //CEDAR_VBS_TYPE_MJPEG
    VIDEO_MPEG1_ES          ,  //CEDAR_VBS_TYPE_MPEG1_ES
    VIDEO_MPEG2_ES          ,  //CEDAR_VBS_TYPE_MPEG2_ES
    VIDEO_XVID              ,  //CEDAR_VBS_TYPE_XVID
    VIDEO_DIVX3             ,  //CEDAR_VBS_TYPE_DIVX3
    VIDEO_DIVX4             ,  //CEDAR_VBS_TYPE_DIVX4
    VIDEO_DIVX5             ,  //CEDAR_VBS_TYPE_DIVX5
    VIDEO_SORENSSON_H263    ,  //CEDAR_VBS_TYPE_SORENSSON_H263
    VIDEO_RMG2              ,  //CEDAR_VBS_TYPE_RMG2
    VIDEO_RMVB8             ,  //CEDAR_VBS_TYPE_RMVB8
    VIDEO_RMVB9             ,  //CEDAR_VBS_TYPE_RMVB9
    VIDEO_H264              ,  //CEDAR_VBS_TYPE_H264
    VIDEO_VC1               ,  //CEDAR_VBS_TYPE_VC1
    VIDEO_AVS               ,  //CEDAR_VBS_TYPE_AVS
    VIDEO_H263              ,  //CEDAR_VBS_TYPE_H263
    VIDEO_VP6               ,  //CEDAR_VBS_TYPE_VP6
    VIDEO_WMV1              ,  //CEDAR_VBS_TYPE_WMV1
    VIDEO_WMV2              ,  //CEDAR_VBS_TYPE_WMV2 
    VIDEO_DIVX2             ,  //CEDAR_VBS_TYPE_DIVX2   
    VIDEO_DIVX1             ,  //CEDAR_VBS_TYPE_DIVX1
};

typedef enum tag_CedarlibVideoSubCodecType
{
    VIDEO_SUB_NONE = 0, //子类型不区分
    VIDEO_VC1_SUB_PROFILEMAIN,      //VC1的子类型
    VIDEO_VC1_SUB_PROFILEADVANCED,
}CedarlibVideoSubCodecType;

//Aspect ratio
enum AspRatio
{
    ASP_RATIO_DAR_FORBIDDEN = 0,    // aspect ratio forbidden

    ASP_RATIO_SAR,                  // sample aspect ratio
    ASP_RATIO_DAR_4_3,              // Display aspect ratio 3/4
    ASP_RATIO_DAR_16_9,             // Display aspect ratio 9/16
    ASP_RATIO_DAR_221_1,            // Display aspect ratio 1/2.11

    ASP_RATIO_
}; //P126, value must NOT change!


//video frame rate classic value
typedef enum __VIDEO_FRAME_RATE_CLASSIC
{
    VIDEO_FPS_CLASSIC_15=15000,
    VIDEO_FPS_CLASSIC_23=23976,
    VIDEO_FPS_CLASSIC_24=24000,
    VIDEO_FPS_CLASSIC_25=25000,
    VIDEO_FPS_CLASSIC_29=29970,
    VIDEO_FPS_CLASSIC_30=30000,
    VIDEO_FPS_CLASSIC_50=50000,
    VIDEO_FPS_CLASSIC_59=59940,
    VIDEO_FPS_CLASSIC_60=60000,

} __video_frame_rate_classic_t;


typedef struct VIDEO_CODEC_FORMAT
{
    unsigned int       codec_type;         // video codec type, VIDEO_XVID, enum __CEDARLIB_VIDEO_CODEC_TYPE    // <==>__cedar_video_fmt_t
    unsigned int       codec_type_sub;      //video codec type的子类型, CedarlibVideoSubCodecType, VIDEO_VC1_SUB_PROFILEMAIN等

    unsigned char        ifrm_pts_credible;  // flag marks that if pts for i frame is credible
    unsigned char        pfrm_pts_credible;  // flag marks that if pts for p frame is credible
    unsigned char        bfrm_pts_credible;  // flag marks that if pts for b frame is credible

    unsigned short       width;              // picture width
    unsigned short       height;             // picture height
    unsigned int       frame_rate;         // frame rate, 数值放大1000倍。
    unsigned int       mic_sec_per_frm;    // frame duration, us. 

    int       avg_bit_rate;       // average bit rate
    int       max_bit_rate;       // maximum bit rate

    //define private information for video decode
    //只接受CEDARLIB_FILE_FMT_ASF, CEDARLIB_FILE_FMT_AVI, CEDARLIB_FILE_FMT_FLV, CEDARLIB_FILE_FMT_MKV, CEDARLIB_FILE_FMT_MPG, , CEDARLIB_FILE_FMT_PMP, CEDARLIB_FILE_FMT_TS 7个值
    unsigned short       video_bs_src;       // video bitstream source, 0:任意的文件; 1:asf,avi,mkv; 2:flv,ts，不同vdrv的规定不同. 目前已经统一为 __cedarlib_file_fmt_t , CEDARLIB_FILE_FMT_AVI, 0表示任意类型文件.
    void        *private_inf;       // video bitstream private information pointer，一般是被赋值的，不是自己malloc，如AVI_PSR
    short       priv_inf_len;       // video bitstream private information length

} __video_codec_format_t;


/*
================================================================================
                            audio properties
================================================================================
*/

//maximum audio channel number
#define MAX_AUDIO_CH_NUM        8

//define the type of audio bitstream
enum __CEDARLIB_AUDIO_CODEC_TYPE
{
    AUDIO_NONE              = 0,    //CEDAR_ABS_TYPE_NONE
    AUDIO_UNKNOWN           = 1,    //CEDAR_ABS_TYPE_UNKNOWN
    AUDIO_MP1               = 2,    //CEDAR_ABS_TYPE_MPEG12_LAYER1
    AUDIO_MP2               ,    //CEDAR_ABS_TYPE_MPEG12_LAYER2
    AUDIO_MP3               ,    //CEDAR_ABS_TYPE_MPEG12_LAYER3
    AUDIO_MPEG_AAC_LC       ,    //CEDAR_ABS_TYPE_MPEG_AAC_LC
    AUDIO_AC3               ,    //CEDAR_ABS_TYPE_AC3
    AUDIO_DTS               ,    //CEDAR_ABS_TYPE_DTS
    AUDIO_LPCM_V            ,    //CEDAR_ABS_TYPE_LPCM_V
    AUDIO_LPCM_A            ,    //CEDAR_ABS_TYPE_LPCM_A
    AUDIO_ADPCM             ,    //CEDAR_ABS_TYPE_ADPCM
    AUDIO_PCM               ,    //CEDAR_ABS_TYPE_PCM
    AUDIO_WMA_STANDARD      ,    //CEDAR_ABS_TYPE_WMA_STANDARD
    AUDIO_FLAC              ,    //CEDAR_ABS_TYPE_FLAC
    AUDIO_APE               ,    //CEDAR_ABS_TYPE_APE
    AUDIO_OGG               ,    //CEDAR_ABS_TYPE_OGG
    AUDIO_RAAC              ,    //CEDAR_ABS_TYPE_RAAC
    AUDIO_COOK              ,    //CEDAR_ABS_TYPE_COOK
    AUDIO_SIPR              ,    //CEDAR_ABS_TYPE_SIPR
    AUDIO_ATRC              ,    //CEDAR_ABS_TYPE_ATRC
    AUDIO_AMR               ,    //CEDAR_ABS_TYPE_AMR
    AUDIO_RA                ,    //CEDAR_ABS_TYPE_RA
    AUDIO_MID               ,    //CEDAR_ABS_TYPE_MID
    AUDIO_ALAC              ,    //CEDAR_ABS_TYPE_ALAC

    AUDIO_MLP               = AUDIO_UNKNOWN,    //AUDIO_UNKNOWN
    AUDIO_PPCM              = AUDIO_UNKNOWN,    //AUDIO_UNKNOWN
    AUDIO_WMA_LOSS          = AUDIO_UNKNOWN,    //AUDIO_UNKNOWN
    AUDIO_WMA_PRO           = AUDIO_UNKNOWN,    //AUDIO_UNKNOWN
    AUDIO_MP3_PRO           = AUDIO_UNKNOWN,    //AUDIO_UNKNOWN
};

#define ABS_EDIAN_FLAG_MASK         ((unsigned int)(1<<16))
#define ABS_EDIAN_FLAG_LITTLE       ((unsigned int)(0<<16))
#define ABS_EDIAN_FLAG_BIG          ((unsigned int)(1<<16))
typedef struct  AUDIO_CODEC_FORMAT
{
    //define audio codec type
    unsigned int       codec_type;         // audio codec type, AUDIO_MP3等, 
    unsigned int       codec_type_sub;     // audio codec sub type, the highest bit mark endian type
                                    // bit0-bit15, sub type of the audio codec
                                    // bit16,      endian flag: 0, little ending; 1, big ending;
                                    // other bits, reserved

    //define audio bitstream format
    unsigned short       channels;           // channel count
    unsigned short       bits_per_sample;    // bits per sample, 指一个声道的编码时占用的比特数，本系统一般为16bit.
    unsigned int       sample_per_second;  // sample count per second
    unsigned int       avg_bit_rate;       // average bit rate,bit/s
    unsigned int       max_bit_rate;       // maximum bit rate
    
    unsigned int       file_length;       // 文件总长度
    //define private information for audio decode
    unsigned short       audio_bs_src;       // audio bitstream source, __cedarlib_file_fmt_t , CEDARLIB_FILE_FMT_AVI, <==>CEDAR_MEDIA_FILE_FMT_AVI等
    int       is_raw_data_output; //0:pcm output, 1:raw data 
    short       priv_inf_len;       // audio bitstream private information length
    void        *private_inf;       // audio bitstream private information pointer. 可以malloc。一般不需要malloc内存，指针赋值，例如AVI_PSR的set_FILE_PARSER_audio_info()

} __audio_codec_format_t;

/*
================================================================================
                            lyric properties
================================================================================
*/
enum __CEDARLIB_LYRIC_FILE_FMT  //__cedar_lyric_fmt_t
{
    LYRIC_NONE    = 0,    //CEDAR_LRC_TYPE_NONE
    LYRIC_UNKNOWN = 1,    //CEDAR_LRC_TYPE_NOTSPT
    LYRIC_LRC     = 2,    //CEDAR_LRC_TYPE_LRC
    LYRIC_SRT     ,    //CEDAR_LRC_TYPE_SRT
    LYRIC_KSC     ,    //CEDAR_LRC_TYPE_KSC
    LYRIC_SMI     ,    //CEDAR_LRC_TYPE_SMI
    LYRIC_SSA     ,    //CEDAR_LRC_TYPE_SSA
    LYRIC_TXT     ,    //CEDAR_LRC_TYPE_TXT
    LYRIC_SUB     ,    //CEDAR_LRC_TYPE_SUB
    LYRIC_IDXSUB  ,    //CEDAR_LRC_TYPE_IDXSUB
    LYRIC_ITXT    ,    //= 0x100,CEDAR_LRC_TYPE_ITXT
    LYRIC_ISUB    ,    //CEDAR_LRC_TYPE_ISUB
    LYRIC_ISSA    ,    //CEDAR_LRC_TYPE_ISSA
    LYRIC_PGS     ,    //CEDAR_LRC_TYPE_PGS
    LYRIC_DVBSUB	= 0x200,	//CEDAR_LRC_TYPE_DVBSUB
    LYRIC_ISDB    = 0x201, //CEDAR_LRC_TYPE_ISDB
    LYRIC_TTXT = 0x202, //CEDAR_LRC_TYPE_TTXT
};

/*******************************************************************************
Function name: __CEDARLIB_LYRIC_ENCODE_TYPE
Description: 
    这个enum类型是指:字幕条目的编码格式. 现在也用来指示描述字幕流信息的字符串的编码格式.

    例如:字幕文件格式为LYRIC_SRT,但这个文件内的文字编码格式可能为LYRIC_SUB_ENCODE_UTF8, LYRIC_SUB_ENCODE_GB2312,
    LYRIC_SUB_ENCODE_BIG5等等.


    另外,为了省事,也用这个enum类型指示描述字幕信息的字符串的编码格式.反正库的使用者要自己在外面转换一次.
Parameters: 
    
Return: 
    
Time: 2011/7/11
*******************************************************************************/
enum __CEDARLIB_LYRIC_ENCODE_TYPE
{
    LYRIC_SUB_ENCODE_NONE   = 0,  //CEDAR_SUB_ENCODE_NONE,  __cedar_subtitle_encode_t
    LYRIC_SUB_ENCODE_BITMAP = 1,  //CEDAR_SUB_ENCODE_BITMAP
    LYRIC_SUB_ENCODE_UTF8   = 2,  //CEDAR_SUB_ENCODE_UTF8
    LYRIC_SUB_ENCODE_GB2312 = 3,  //CEDAR_SUB_ENCODE_GB2312

    LYRIC_SUB_ENCODE_ISO_8859_1     = 15,        //EPDK_CHARSET_ENM_ISO_8859_1
    LYRIC_SUB_ENCODE_ISO_8859_15    = 29,        //EPDK_CHARSET_ENM_ISO_8859_15
    LYRIC_SUB_ENCODE_ARIBJIS        = 39,       //EPDK_CHARSET_ENM_JIS, 日本isdb电视广播使用的aribjis字符编码
    
    LYRIC_SUB_ENCODE_UNKNOWN = -1,  //CEDAR_SUB_ENCODE_UNKNOWN
};

typedef struct  SUBTITLE_CODEC_FORMAT
{
    unsigned int       codec_type;         //__CEDARLIB_LYRIC_FILE_FMT, LYRIC_ITXT等, <==>CEDAR_LRC_TYPE_SRT,字幕类型
    unsigned int       data_encode_type;   //__CEDARLIB_LYRIC_ENCODE_TYPE(LYRIC_SUB_ENCODE_UTF8) <==>__cedar_subtitle_encode_t,指的是字幕项数据的类型,如果字幕项是文本,一般就是文本的字符编码类型;如果是图像,一般就是LYRIC_SUB_ENCODE_BITMAP
//    unsigned int       codec_type_sub;     // audio codec sub type, the highest bit mark endian type
                                    // bit0-bit15, sub type of the audio codec
                                    // bit16,      endian flag: 0, little ending; 1, big ending;
                                    // other bits, reserved
//
    //define audio bitstream format
//    unsigned short       channels;           // channel count
//    unsigned short       bits_per_sample;    // bits per sample
//    unsigned int       sample_per_second;  // sample count per second
//    unsigned int       avg_bit_rate;       // average bit rate
//    unsigned int       max_bit_rate;       // maximum bit rate
//
    //define private information for audio decode
    unsigned short       subtitle_bs_src;       // 目前用到的有:CEDARLIB_FILE_FMT_MKV, CEDARLIB_FILE_FMT_VOB, CEDARLIB_FILE_FMT_TS, 表示内置字幕来源
//    short       priv_inf_len;       // audio bitstream private information length
//    void        *private_inf;       // audio bitstream private information pointer

} __subtitle_codec_format_t;

/*
================================================================================
                            other define
================================================================================
*/

//define decoding status
#define MEDIA_STATUS_TMP_BIT 0x80
enum MEDIA_STATUS
{
    MEDIA_STATUS_IDLE = 0,  //和MEDIA_STATUS_STOP等同,一般不用它
    MEDIA_STATUS_PAUSE, //使用
    MEDIA_STATUS_STEP,
    MEDIA_STATUS_PLAY,  //使用
    MEDIA_STATUS_FORWARD,   //使用
    MEDIA_STATUS_BACKWARD,  //使用
    MEDIA_STATUS_STOP,      //使用
    MEDIA_STATUS_ABORT,
    MEDIA_STATUS_JUMP,      //audio pDev使用, aply使用

    MEDIA_STATUS_RECORD,    //录音的相关模块使用

    MEDIA_STATUS_
};


enum BITSTREAM_TYPE
{
    BITSTREAM_TYPE_NONE = 0,    //no bitstream, default
    BITSTREAM_TYPE_AUDIO,       //audio bitstream
    BITSTREAM_TYPE_VIDEO,       //video bitstream
    BITSTREAM_TYPE_LYRIC,       //subtitle bitstream
    BITSTREAM_TYPE_MIXED,       //表示下级psr模块读到的chunk中含有audio,video的混合数据
    BITSTREAM_TYPE_UNKNOWN,     //unkown type of bitstream
    BITSTREAM_TYPE_
};

//redefine color format
enum CEDARLIB_PIXEL_FMT
{
    PIC_CFMT_YCBCR_444 = 0x10, //PIXEL_YUV444
    PIC_CFMT_YCBCR_422 ,       //PIXEL_YUV422
    PIC_CFMT_YCBCR_420 ,       //PIXEL_YUV420
    PIC_CFMT_YCBCR_411 ,       //PIXEL_YUV411
};


typedef enum __DISPLAY_MODE_T
{
    DISP_2D_MODE_    = 0,
    DISP_3D_MODE_PLR = 1,
    DISP_3D_MODE_PRL = 2,
    DISP_3D_MODE_PTD = 3,
    DISP_3D_MODE_PDT = 4,
    DISP_3D_MODE_IAB = 5,
    DISP_3D_MODE_IBA = 6,
    DISP_3D_MODE_URL = 7,
    DISP_3D_MODE_MVC = 8,
    DISP_3D_MODE_UNKNOWN
    
}__display_mode_t;


enum RepeatField
{
    REPEAT_FIELD_NONE,          //means no field should be repeated

    REPEAT_FIELD_TOP,           //means the top field should be repeated
    REPEAT_FIELD_BOTTOM,        //means the bottom field should be repeated

    REPEAT_FIELD_
};


typedef struct Video_Info
{
    unsigned short       width;          //the stored picture width for luma because of mapping
    unsigned short       height;         //the stored picture height for luma because of mapping
    unsigned short       frame_rate;     //the source picture frame rate, FRAME_RATE_BASE     1000
    unsigned short       eAspectRatio;   //the source picture aspect ratio, 宽高比, 1000为单位，等于1:1,解码之后的ratio,考虑了ve的压缩
    unsigned short       color_format;   //the source picture color format, enum CEDARLIB_PIXEL_FMT,PIC_CFMT_YCBCR_420 <==> __pixel_yuvfmt_t(PIXEL_YUV420)

} __video_info_t;

typedef struct PanScanInfo
{
    unsigned int uNumberOfOffset;
    short HorizontalOffsets[3];

} __pan_scan_info_t;

typedef struct Display_Info
{
    char              bProgressiveSrc;    // Indicating the source is progressive or not
    char              bTopFieldFirst;     // VPO should check this flag when bProgressiveSrc is FALSE
    enum RepeatField    eRepeatField;       // enum RepeatField, only check it when frame rate is 24FPS and interlace output
    __video_info_t      pVideoInfo;         // a pointer to structure stored video information
    __pan_scan_info_t   pPanScanInfo;
    __vdrv_rect_t       src_rect;           // source valid size,宏块对齐, 不是frame buf的宽高,vdrv的frame buf的宽高以偶数个宏块为单位.这是编码时的宽高,编码是以整数个宏块为单位，但有时解码库给出的就是真实图像的宽高，而系统各IP默认2倍宏块算边界,所以不会出错
    __vdrv_rect_t       dst_rect;           // source display size,真正的图像的宽高.如果是旋转后,给出的是旋转后的宽高而不是原图的.而且是解码后的真实宽高,压缩之后.
    unsigned char              top_index;          // frame buffer index containing the top field
    unsigned int               top_y;              // the address of frame buffer, which contains top field luminance
    unsigned int               top_c;              // the address of frame buffer, which contains top field chrominance

    //the following is just for future
    unsigned char              bottom_index;       // frame buffer index containing the bottom field
    unsigned int               bottom_y;           // the address of frame buffer, which contains bottom field luminance
    unsigned int               bottom_c;           // the address of frame buffer, which contains bottom field chrominance

    //time stamp of the frame
    unsigned int               uPts;               // time stamp of the frame, ms
    //long long                  uPts;            //扩展为__s64,从dtv的角度出发,让clock永不回头

    //scale param
    unsigned char       horizontal_scale_ratio; //* what ratio this picture has been scaled down at horizon size, 0: 1/1, 1: 1/2, 2: 1/4, 3: 1/8; 0xff:无效
    unsigned char       vertical_scale_ratio;   //* what ratio this picture has been scaled down at vetical size, 0: 1/1, 1: 1/2, 2: 1/4, 3: 1/8; 0xff:无效

    //MAF
    __s32               maf_valid;  //maf_valid表示maf buffer是否使用
    void*               flag_addr;  //maf buffer的起始地址
    __u32               flag_stride;    //0x40.
    __s32               pre_frame_valid;    //前一帧是否可用来做deinterlace
} __display_info_t;

//the information from media file
#define CEDARLIB_MAX_AUDIO_STREAM_NUM    (16)
#define MAX_VIDEO_STREAM_NUM    (1)
#define MAX_SUBTITLE_STREAM_NUM (32)                //SUBTITLE_MAX_NUM，针对单文件(视频文件内置字幕，或者外挂字幕文件)
#define ID3_INFO_BUF_SIZE       (64)
#define CEDARLIB_MAX_LANG_CHAR_SIZE  (32)   //必须等于MAX_LANG_CHAR_SIZE
typedef struct  CEDARLIB_AUDIO_BS_INFO
{
    int       codec_type;     //enum __CEDARLIB_AUDIO_CODEC_TYPE,AUDIO_MP3等   <==>__cedar_audio_fmt_t
    int       channels;
    int       bits_per_sample;
    int       sample_per_second;
    int       avg_bit_rate;
    int       max_bit_rate;

    int       data_encode_type;    //enum __CEDARLIB_LYRIC_ENCODE_TYPE(LYRIC_SUB_ENCODE_UTF8), __cedar_subtitle_encode_t
    unsigned char        lang[CEDARLIB_MAX_LANG_CHAR_SIZE];
} __cedarlib_audio_bs_info_t;

//video format inforamtion
typedef struct CEDARLIB_VIDEO_BS_INFO
{
    unsigned short       codec_type; //使用cedarlib定义的enum __CEDARLIB_VIDEO_CODEC_TYPE, VIDEO_XVID<==>__cedar_video_fmt_t
    unsigned short       width;
    unsigned short       height;
    unsigned short       frame_rate; // *FRAME_RATE_BASE     1000
    int       avg_bit_rate;
    int       max_bit_rate;

} __cedarlib_video_bs_info_t;

typedef enum __SUBTITLE_SOURCE{
    SUBTITLE_POS_NONE   = 0,    //no subtitle exist
    SUBTITLE_POS_EMBED  = 1,    //embeded subtilte in video file
    SUBTITLE_POS_EXT    = 2,    //extern subtitle in subtitle file
    SUBTITLE_POS_,
} __subtitle_source_t;  //== __cedar_subtitle_source_t

#define CEDARLIB_PALETTE_CNT         (32)
typedef struct{
    char    valid_flg;
    int   palette_num;        //valid palette element
    int   palette_ary[CEDARLIB_PALETTE_CNT];//RGB格式的调色板
}__cedarlib_palette_t;
//subtitle format information
typedef struct {
    int       subtitle_coming_type;  //embeded subtitle or extern subtitle, __subtitle_source_t.
    int       subtitle_type;  // srt, sub, etc. enum __CEDARLIB_LYRIC_FILE_FMT  <==> __cedar_lyric_fmt_t
    __cedarlib_palette_t palette_info;   //如果是SUB格式的字幕，需要该信息，务必与__palette_t保持一致
    int       stream_idx;     //in one file(e.g,sub),there will exist several streams, but in srt file, stream_idx means nothing.这是psr对字幕流的内部编号,而选字幕时用的是数组下标
    int       data_encode_type;    //表示lang[]的字符编码类型, enum __CEDARLIB_LYRIC_ENCODE_TYPE(LYRIC_SUB_ENCODE_UTF8),  外围转为__cedar_subtitle_encode_t
    unsigned char        lang[CEDARLIB_MAX_LANG_CHAR_SIZE];
} __cedarlib_subtitle_stream_info_t;
//extern __s32 subtitle_stream_info_lib2cedar(__subtitle_stream_info_t *pDes, __cedarlib_subtitle_stream_info_t *pSrc),互转的函数


typedef enum __3D_OUT_MODE_T
{
    VIRTUAL_DISP_3D_OUT_MODE_LR = 0x00,//LRLR
    VIRTUAL_DISP_3D_OUT_MODE_RL = 0x01,//RLRL
    VIRTUAL_DISP_3D_OUT_MODE_MIX_1 = 0x02,//
    VIRTUAL_DISP_3D_OUT_MODE_MIX_2 = 0x03,//
}__3d_out_mode_t;

//video display mode inforamtion
typedef struct DISPLAY_MODE_INFO
{
    __display_mode_t         display_mode;
    unsigned short			 display_width;
	unsigned short			 display_height;
    unsigned short			 mvcdisplay_width;
	unsigned short			 mvcdisplay_height;

} DisplayModeInfo;

typedef struct DISPLAY_MODE_OUT
{
    __u32                   trd_display_enable;
    __3d_out_mode_t         trd_mode;
    __u32                   half_mode;

}DisplayModeOut;    



typedef struct __MEDIA_FILE_INFO_INTERNAL
{
    char        nHasAudio;          //媒体文件是否含有音频流，有多少个音频流, nHasAudio就等于几,例如mkv. 也有的parser只是以非0表示有
    char        nHasVideo;          //媒体文件是否含有视频流, 有些psr，其值等于视频流的数量，例如mkv.
    char        nHasLrc;            //媒体文件是否含有字幕流,应该是专指内置字幕.有些psr，其值等于字幕流的数量，例如mkv.
    char        nUseExtLrc;         //媒体文件是否正在使用外挂字幕及处理句柄,没有用到.以后其值应该只会等于0.

    unsigned char        nStreamNum;         //媒体文件内流的总数
    unsigned char        nAudStrmNum;        //媒体文件内音频流的个数
    unsigned char        nVidStrmNum;        //媒体文件内视频流的个数
    unsigned char        nLrcStrmNum;        //媒体文件内字幕流的个数

    //__subtitle_profile_t    LyricInfo;       //媒体文件内嵌的每个字幕流的信息

    unsigned int       nDuration;          //媒本文件的持续总时间
    
    DisplayModeInfo   display_mode_info;  //display mode information, 2D/3D

    __cedarlib_audio_bs_info_t           AudStrmList[CEDARLIB_MAX_AUDIO_STREAM_NUM];  //音频流信息队列的头指针，务必与__audio_bs_info_t一致
    __cedarlib_video_bs_info_t           VidStrmList[MAX_VIDEO_STREAM_NUM];  //视频流信息队列的头指针, 务必与__video_bs_info_t一致
    __cedarlib_subtitle_stream_info_t    SubtitleStreamList[MAX_SUBTITLE_STREAM_NUM];   //SUBTITLE_MAX_NUM, 只记录内置字幕，务必与__subtitle_stream_info_t一致

    unsigned char        Author[ID3_INFO_BUF_SIZE];                  //作者标签
    int       nAuthorLen;                                 //作者标签长度
    unsigned char        CopyRight[ID3_INFO_BUF_SIZE];               //版权标签
    int       nCopyRightLen;                              //版权标签长度
    unsigned char        Title[ID3_INFO_BUF_SIZE];                   //标题标签
    int       nTitleLen;                                  //标题标签长度
    unsigned char        Genre[ID3_INFO_BUF_SIZE];                   //流派标签
    int       nGenreLen;                                  //流派标签长度

} __media_file_info_internal_t;

#endif  //_MEDIA_TYPE_H

