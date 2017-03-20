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
#define MACROBLOCK_WIDTH    (16)    //��λ:����
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
    CEDARLIB_FILE_FMT_MP3=0x0001,      //CEDAR_MEDIA_FILE_FMT_MP3=0x0001,              /* *.mp3, ��Ƶ�ļ�                      */
    CEDARLIB_FILE_FMT_WAV,             //CEDAR_MEDIA_FILE_FMT_WAV,                     /* *.wav, ��Ƶ�ļ�                      */
    CEDARLIB_FILE_FMT_WMA,             //CEDAR_MEDIA_FILE_FMT_WMA,                     /* *.wmv, Windows media��/��Ƶ�ļ�      */
    CEDARLIB_FILE_FMT_APE,             //CEDAR_MEDIA_FILE_FMT_APE,                     /* *.ape, APE������Ƶ                   */
    CEDARLIB_FILE_FMT_FLAC,            //CEDAR_MEDIA_FILE_FMT_FLAC,                    /* *.flac, FLAC��Ƶ                     */
    CEDARLIB_FILE_FMT_OGG,             //CEDAR_MEDIA_FILE_FMT_OGG,                     /* *.ogg, OGG Vorbis��Ƶ�ļ�            */
    CEDARLIB_FILE_FMT_RA,              //CEDAR_MEDIA_FILE_FMT_RA,                      /* *.ra, Real��Ƶ                       */
    CEDARLIB_FILE_FMT_MP1,             //CEDAR_MEDIA_FILE_FMT_MP1,                     /* *.mp1, mp2/mpa��Ƶ                   */
    CEDARLIB_FILE_FMT_MP2,             //CEDAR_MEDIA_FILE_FMT_MP2,                     /* *.mp2, mp2/mpa��Ƶ                   */
    CEDARLIB_FILE_FMT_AAC,             //CEDAR_MEDIA_FILE_FMT_AAC,                     /* *.aac, MPEG4��Ƶ                     */
    CEDARLIB_FILE_FMT_AC3,             //CEDAR_MEDIA_FILE_FMT_AC3,                     /* *.ac3, ��Ƶ�ļ�                      */
    CEDARLIB_FILE_FMT_DTS,             //CEDAR_MEDIA_FILE_FMT_DTS,                     /* *.dts, DVD��Ƶ                       */
    CEDARLIB_FILE_FMT_AIF,             //CEDAR_MEDIA_FILE_FMT_AIF,                     /* *.aif, Mac��Ƶ                       */
    CEDARLIB_FILE_FMT_AIFF,            //CEDAR_MEDIA_FILE_FMT_AIFF,                    /* *.aiff, Mac��Ƶ                      */
    CEDARLIB_FILE_FMT_AIFC,            //CEDAR_MEDIA_FILE_FMT_AIFC,                    /* *.aifc, Mac��Ƶ                      */
    CEDARLIB_FILE_FMT_AMR,             //CEDAR_MEDIA_FILE_FMT_AMR,                     /* *.amr, �ƶ���Ƶ                      */
    CEDARLIB_FILE_FMT_MAC,             //CEDAR_MEDIA_FILE_FMT_MAC,                     /* *.mac, APE������Ƶ                   */
    CEDARLIB_FILE_FMT_TTA,             //CEDAR_MEDIA_FILE_FMT_TTA,                     /* *.tta, TTA��Ƶ                       */
    CEDARLIB_FILE_FMT_M4A,             //CEDAR_MEDIA_FILE_FMT_M4A,                     /* *.m4a, MPEG4��Ƶ                     */
    CEDARLIB_FILE_FMT_CDA,             //CEDAR_MEDIA_FILE_FMT_CDA,                     /* *.cda, CD��Ƶ                        */
    CEDARLIB_FILE_FMT_AU,              //CEDAR_MEDIA_FILE_FMT_AU,                      /* *.au, Sunѹ����Ƶ                    */
    CEDARLIB_FILE_FMT_ACC,             //CEDAR_MEDIA_FILE_FMT_ACC,                     /* *.acc, ��Ƶ                          */
    CEDARLIB_FILE_FMT_MIDI,            //CEDAR_MEDIA_FILE_FMT_MIDI,                    /* *.midi, mid��Ƶ�ļ�                  */
    CEDARLIB_FILE_FMT_MID,             //CEDAR_MEDIA_FILE_FMT_MID,                     /* *.mid, mid��Ƶ�ļ�                   */
    CEDARLIB_FILE_FMT_RMI,             //CEDAR_MEDIA_FILE_FMT_RMI,                     /* *.rmi, mid��Ƶ�ļ�                   */
    CEDARLIB_FILE_FMT_MP5,             //CEDAR_MEDIA_FILE_FMT_MP5,                     /* *.mp5, ��Ƶ�ļ�                      */
    CEDARLIB_FILE_FMT_MPA,             //CEDAR_MEDIA_FILE_FMT_MPA,                     /* *.mpa, mpeg��Ƶ�ļ�                  */
    CEDARLIB_FILE_FMT_MPGA,            //CEDAR_MEDIA_FILE_FMT_MPGA,                    /* *.mpga, mp3��Ƶ�ļ�                  */
    CEDARLIB_FILE_FMT_ACT,             //CEDAR_MEDIA_FILE_FMT_ACT,                     /* *.act, ��Ƶ�ļ�                      */
    CEDARLIB_FILE_FMT_ATRC,            //CEDAR_MEDIA_FILE_FMT_ATRC,                    /* *.omg,oma,.aa3, sony openMG��Ƶ�ļ�  */
	CEDARLIB_FILE_FMT_ALAC,            //CEDAR_MEDIA_FILE_FMT_ALAC,                    /* *.caf, alac��Ƶ�ļ�                  */

    //define video file file format               
    CEDARLIB_FILE_FMT_AVI = 0x1000,    //CEDAR_MEDIA_FILE_FMT_AVI = 0x1000,            /* *.avi, ��Ӱ��Ƶ�ļ�                  */
    CEDARLIB_FILE_FMT_FLV,             //CEDAR_MEDIA_FILE_FMT_FLV,                     /* *.flv, flash video��Ƶ�ļ�           */
    CEDARLIB_FILE_FMT_MPEG,            //CEDAR_MEDIA_FILE_FMT_MPEG,                    /* *.mpeg, mpegӰƬ��Ƶ�ļ�             */
    CEDARLIB_FILE_FMT_MPG,             //CEDAR_MEDIA_FILE_FMT_MPG,                     /* *.mpg, mpegӰƬ��Ƶ�ļ�              */
    CEDARLIB_FILE_FMT_VOB,             //CEDAR_MEDIA_FILE_FMT_VOB,                     /* *.vob, mpegӰƬ��Ƶ�ļ�              */
    CEDARLIB_FILE_FMT_DAT,             //CEDAR_MEDIA_FILE_FMT_DAT,                     /* *.dat, VCD��Ƶ�ļ�                   */
    CEDARLIB_FILE_FMT_MP4,             //CEDAR_MEDIA_FILE_FMT_MP4,                     /* *.mp4, mpeg-4ӰƬ��Ƶ�ļ�            */
    CEDARLIB_FILE_FMT_3GP,             //CEDAR_MEDIA_FILE_FMT_3GP,                     /* *.3gp, �ƶ���Ƶ                      */
    CEDARLIB_FILE_FMT_MOV,             //CEDAR_MEDIA_FILE_FMT_MOV,                     /* *.mov, QuickTimeӰƬ                 */
    CEDARLIB_FILE_FMT_WMV,             //CEDAR_MEDIA_FILE_FMT_WMV,                     /* *.wmv, Windows media��/��Ƶ�ļ�      */
    CEDARLIB_FILE_FMT_ASF,             //CEDAR_MEDIA_FILE_FMT_ASF,                     /* *.asf, Windows media��/��Ƶ�ļ�      */
    CEDARLIB_FILE_FMT_RM,              //CEDAR_MEDIA_FILE_FMT_RM,                      /* *.rm, realӰƬ                       */
    CEDARLIB_FILE_FMT_RMVB,            //CEDAR_MEDIA_FILE_FMT_RMVB,                    /* *.rmvb, realӰƬ                     */
    CEDARLIB_FILE_FMT_RAM,             //CEDAR_MEDIA_FILE_FMT_RAM,                     /* *.ram, realӰƬ                      */
    CEDARLIB_FILE_FMT_SCM,             //CEDAR_MEDIA_FILE_FMT_SCM,                     /* *.scm, realӰƬ                      */
    CEDARLIB_FILE_FMT_DIVX,            //CEDAR_MEDIA_FILE_FMT_DIVX,                    /* *.divx, ��Ӱ��Ƶ                     */
    CEDARLIB_FILE_FMT_MKV,             //CEDAR_MEDIA_FILE_FMT_MKV,                     /* *.mkv, matroskaӰƬ��Ƶ�ļ�          */
    CEDARLIB_FILE_FMT_OGM,             //CEDAR_MEDIA_FILE_FMT_OGM,                     /* *.ogm, oggӰƬ��Ƶ�ļ�               */
    CEDARLIB_FILE_FMT_M4V,             //CEDAR_MEDIA_FILE_FMT_M4V,                     /* *.m4v, mpeg-4ӰƬ��Ƶ                */
    CEDARLIB_FILE_FMT_M4P,             //CEDAR_MEDIA_FILE_FMT_M4P,                     /* *.m4p, mpeg-4ӰƬ��Ƶ                */
    CEDARLIB_FILE_FMT_M4B,             //CEDAR_MEDIA_FILE_FMT_M4B,                     /* *.m4b, mpeg-4ӰƬ��Ƶ                */
    CEDARLIB_FILE_FMT_TP,              //CEDAR_MEDIA_FILE_FMT_TP,                      /* *.tp, mpeg-2ӰƬ��Ƶ�ļ�             */
    CEDARLIB_FILE_FMT_TPR,             //CEDAR_MEDIA_FILE_FMT_TPR,                     /* *.tpr, mpegӰƬ��Ƶ�ļ�              */
    CEDARLIB_FILE_FMT_TS,              //CEDAR_MEDIA_FILE_FMT_TS,                      /* *.ts, mpeg-2ӰƬ��Ƶ�ļ�             */
    CEDARLIB_FILE_FMT_PVA,             //CEDAR_MEDIA_FILE_FMT_PVA,                     /* *.pva, mpegӰƬ��Ƶ�ļ�              */
    CEDARLIB_FILE_FMT_PSS,             //CEDAR_MEDIA_FILE_FMT_PSS,                     /* *.pss, mpegӰƬ��Ƶ�ļ�              */
    CEDARLIB_FILE_FMT_MPE,             //CEDAR_MEDIA_FILE_FMT_MPE,                     /* *.mpe, mpegӰƬ��Ƶ�ļ�              */
    CEDARLIB_FILE_FMT_WV,              //CEDAR_MEDIA_FILE_FMT_WV,                      /* *.wv, mpegӰƬ��Ƶ�ļ�               */
    CEDARLIB_FILE_FMT_M2TS,            //CEDAR_MEDIA_FILE_FMT_M2TS,                    /* *.m2ts, m2ts��Ƶ                     */
    CEDARLIB_FILE_FMT_EVO,             //CEDAR_MEDIA_FILE_FMT_EVO,                     /* *.evo, HD-DVD��Ƶ                    */
    CEDARLIB_FILE_FMT_RPM,             //CEDAR_MEDIA_FILE_FMT_RPM,                     /* *.rpm, RealӰƬ                      */
    CEDARLIB_FILE_FMT_3GPP,            //CEDAR_MEDIA_FILE_FMT_3GPP,                    /* *.3gp, �ƶ���Ƶ                      */
    CEDARLIB_FILE_FMT_3G2,             //CEDAR_MEDIA_FILE_FMT_3G2,                     /* *.3gp, �ƶ���Ƶ                      */
    CEDARLIB_FILE_FMT_3GP2,            //CEDAR_MEDIA_FILE_FMT_3GP2,                    /* *.3gp, �ƶ���Ƶ                      */
    CEDARLIB_FILE_FMT_QT,              //CEDAR_MEDIA_FILE_FMT_QT,                      /* *.qt, QuickTimeӰƬ                  */
    CEDARLIB_FILE_FMT_WMP,             //CEDAR_MEDIA_FILE_FMT_WMP,                     /* *.wmp, Windows media��/��Ƶ�ļ�      */
    CEDARLIB_FILE_FMT_WM,              //CEDAR_MEDIA_FILE_FMT_WM,                      /* *.wm, Windows media��/��Ƶ�ļ�       */
    CEDARLIB_FILE_FMT_AMV,             //CEDAR_MEDIA_FILE_FMT_AMV,                     /* *.avm, ѹ����Ƶ                      */
    CEDARLIB_FILE_FMT_DSM,             //CEDAR_MEDIA_FILE_FMT_DSM,                     /* *.dsm, NDS��Ƶ                       */
    CEDARLIB_FILE_FMT_M1V,             //CEDAR_MEDIA_FILE_FMT_M1V,                     /* *.m1v, ӰƬ��Ƶ                      */
    CEDARLIB_FILE_FMT_M2V,             //CEDAR_MEDIA_FILE_FMT_M2V,                     /* *.m2v, ӰƬ��Ƶ                      */
    CEDARLIB_FILE_FMT_PMP,             //CEDAR_MEDIA_FILE_FMT_PMP,                     /* *.pmp, ӰƬ��Ƶ                      */
    CEDARLIB_FILE_FMT_SMK,             //CEDAR_MEDIA_FILE_FMT_SMK,                     /* *.smk, ӰƬ��Ƶ                      */
    CEDARLIB_FILE_FMT_BIK,             //CEDAR_MEDIA_FILE_FMT_BIK,                     /* *.bik, BinkӰƬ��Ƶ                  */
    CEDARLIB_FILE_FMT_RAT,             //CEDAR_MEDIA_FILE_FMT_RAT,                     /* *.rat, ratDVDӰƬ��Ƶ                */
    CEDARLIB_FILE_FMT_VG2,             //CEDAR_MEDIA_FILE_FMT_VG2,                     /* *.vg2, XVDӰƬ��Ƶ                   */
    CEDARLIB_FILE_FMT_IVF,             //CEDAR_MEDIA_FILE_FMT_IVF,                     /* *.ivf, IndeoӰƬ��Ƶ                 */
    CEDARLIB_FILE_FMT_VP6,             //CEDAR_MEDIA_FILE_FMT_VP6,                     /* *.vp6, VP6ӰƬ��Ƶ                   */
    CEDARLIB_FILE_FMT_VP7,             //CEDAR_MEDIA_FILE_FMT_VP7,                     /* *.vp7, VP7ӰƬ��Ƶ                   */
    CEDARLIB_FILE_FMT_D2V,             //CEDAR_MEDIA_FILE_FMT_D2V,                     /* *.d2v, DVD2AVI ProjectӰƬ��Ƶ       */
    CEDARLIB_FILE_FMT_MOD,             //CEDAR_MEDIA_FILE_FMT_MOD,                     /* *.mod, �������ʽ                    */
    CEDARLIB_FILE_FMT_M2P,             //CEDAR_MEDIA_FILE_FMT_M2P,                     /* *.m2p, ӰƬ��Ƶ                      */
    CEDARLIB_FILE_FMT_VID,             //CEDAR_MEDIA_FILE_FMT_VID,                     /* *.vid, mp4�ֻ�����ӰƬ��Ƶ           */
    CEDARLIB_FILE_FMT_PMP2,            //CEDAR_MEDIA_FILE_FMT_PMP2,                    /* *.pmp2, ӰƬ��Ƶ                     */
    CEDARLIB_FILE_FMT_MTS,             //CEDAR_MEDIA_FILE_FMT_MTS,                     /* *.mts, DV��Ƶ                        */
    CEDARLIB_FILE_FMT_F4V,             //CEDAR_MEDIA_FILE_FMT_F4V,                     /* *.f4v, flash video��Ƶ�ļ�           */ 
    
    //user define format type                     
    CEDARLIB_FILE_FMT_PSR = 0x2000,    //CEDAR_MEDIA_FILE_FMT_PSR = 0x2000,            /* *.psr, �û��Զ����ý���ļ���ʽ      */
    CEDARLIB_FILE_FMT_DEV = 0x2001,    //CEDAR_MEDIA_FILE_FMT_DEV = 0x2001,            /* *.dev, ���豸�ļ�*/

    //�û��Զ���3D�ļ�
    CEDARLIB_FILE_FMT_3DV = 0x2200,        /* .3dv, 3DV video��Ƶ�ļ�, ά���Զ���3d��Ƶ��ʽ */
    //define unknown format type
    CEDARLIB_FILE_FMT_UNKNOWN = 0xffff,//CEDAR_MEDIA_FILE_FMT_UNKNOWN = 0xffff,        /* ý���ļ���ʽ�޷�ʶ��              */
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
    VIDEO_SUB_NONE = 0, //�����Ͳ�����
    VIDEO_VC1_SUB_PROFILEMAIN,      //VC1��������
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
    unsigned int       codec_type_sub;      //video codec type��������, CedarlibVideoSubCodecType, VIDEO_VC1_SUB_PROFILEMAIN��

    unsigned char        ifrm_pts_credible;  // flag marks that if pts for i frame is credible
    unsigned char        pfrm_pts_credible;  // flag marks that if pts for p frame is credible
    unsigned char        bfrm_pts_credible;  // flag marks that if pts for b frame is credible

    unsigned short       width;              // picture width
    unsigned short       height;             // picture height
    unsigned int       frame_rate;         // frame rate, ��ֵ�Ŵ�1000����
    unsigned int       mic_sec_per_frm;    // frame duration, us. 

    int       avg_bit_rate;       // average bit rate
    int       max_bit_rate;       // maximum bit rate

    //define private information for video decode
    //ֻ����CEDARLIB_FILE_FMT_ASF, CEDARLIB_FILE_FMT_AVI, CEDARLIB_FILE_FMT_FLV, CEDARLIB_FILE_FMT_MKV, CEDARLIB_FILE_FMT_MPG, , CEDARLIB_FILE_FMT_PMP, CEDARLIB_FILE_FMT_TS 7��ֵ
    unsigned short       video_bs_src;       // video bitstream source, 0:������ļ�; 1:asf,avi,mkv; 2:flv,ts����ͬvdrv�Ĺ涨��ͬ. Ŀǰ�Ѿ�ͳһΪ __cedarlib_file_fmt_t , CEDARLIB_FILE_FMT_AVI, 0��ʾ���������ļ�.
    void        *private_inf;       // video bitstream private information pointer��һ���Ǳ���ֵ�ģ������Լ�malloc����AVI_PSR
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
    unsigned int       codec_type;         // audio codec type, AUDIO_MP3��, 
    unsigned int       codec_type_sub;     // audio codec sub type, the highest bit mark endian type
                                    // bit0-bit15, sub type of the audio codec
                                    // bit16,      endian flag: 0, little ending; 1, big ending;
                                    // other bits, reserved

    //define audio bitstream format
    unsigned short       channels;           // channel count
    unsigned short       bits_per_sample;    // bits per sample, ָһ�������ı���ʱռ�õı���������ϵͳһ��Ϊ16bit.
    unsigned int       sample_per_second;  // sample count per second
    unsigned int       avg_bit_rate;       // average bit rate,bit/s
    unsigned int       max_bit_rate;       // maximum bit rate
    
    unsigned int       file_length;       // �ļ��ܳ���
    //define private information for audio decode
    unsigned short       audio_bs_src;       // audio bitstream source, __cedarlib_file_fmt_t , CEDARLIB_FILE_FMT_AVI, <==>CEDAR_MEDIA_FILE_FMT_AVI��
    int       is_raw_data_output; //0:pcm output, 1:raw data 
    short       priv_inf_len;       // audio bitstream private information length
    void        *private_inf;       // audio bitstream private information pointer. ����malloc��һ�㲻��Ҫmalloc�ڴ棬ָ�븳ֵ������AVI_PSR��set_FILE_PARSER_audio_info()

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
    ���enum������ָ:��Ļ��Ŀ�ı����ʽ. ����Ҳ����ָʾ������Ļ����Ϣ���ַ����ı����ʽ.

    ����:��Ļ�ļ���ʽΪLYRIC_SRT,������ļ��ڵ����ֱ����ʽ����ΪLYRIC_SUB_ENCODE_UTF8, LYRIC_SUB_ENCODE_GB2312,
    LYRIC_SUB_ENCODE_BIG5�ȵ�.


    ����,Ϊ��ʡ��,Ҳ�����enum����ָʾ������Ļ��Ϣ���ַ����ı����ʽ.�������ʹ����Ҫ�Լ�������ת��һ��.
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
    LYRIC_SUB_ENCODE_ARIBJIS        = 39,       //EPDK_CHARSET_ENM_JIS, �ձ�isdb���ӹ㲥ʹ�õ�aribjis�ַ�����
    
    LYRIC_SUB_ENCODE_UNKNOWN = -1,  //CEDAR_SUB_ENCODE_UNKNOWN
};

typedef struct  SUBTITLE_CODEC_FORMAT
{
    unsigned int       codec_type;         //__CEDARLIB_LYRIC_FILE_FMT, LYRIC_ITXT��, <==>CEDAR_LRC_TYPE_SRT,��Ļ����
    unsigned int       data_encode_type;   //__CEDARLIB_LYRIC_ENCODE_TYPE(LYRIC_SUB_ENCODE_UTF8) <==>__cedar_subtitle_encode_t,ָ������Ļ�����ݵ�����,�����Ļ�����ı�,һ������ı����ַ���������;�����ͼ��,һ�����LYRIC_SUB_ENCODE_BITMAP
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
    unsigned short       subtitle_bs_src;       // Ŀǰ�õ�����:CEDARLIB_FILE_FMT_MKV, CEDARLIB_FILE_FMT_VOB, CEDARLIB_FILE_FMT_TS, ��ʾ������Ļ��Դ
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
    MEDIA_STATUS_IDLE = 0,  //��MEDIA_STATUS_STOP��ͬ,һ�㲻����
    MEDIA_STATUS_PAUSE, //ʹ��
    MEDIA_STATUS_STEP,
    MEDIA_STATUS_PLAY,  //ʹ��
    MEDIA_STATUS_FORWARD,   //ʹ��
    MEDIA_STATUS_BACKWARD,  //ʹ��
    MEDIA_STATUS_STOP,      //ʹ��
    MEDIA_STATUS_ABORT,
    MEDIA_STATUS_JUMP,      //audio pDevʹ��, aplyʹ��

    MEDIA_STATUS_RECORD,    //¼�������ģ��ʹ��

    MEDIA_STATUS_
};


enum BITSTREAM_TYPE
{
    BITSTREAM_TYPE_NONE = 0,    //no bitstream, default
    BITSTREAM_TYPE_AUDIO,       //audio bitstream
    BITSTREAM_TYPE_VIDEO,       //video bitstream
    BITSTREAM_TYPE_LYRIC,       //subtitle bitstream
    BITSTREAM_TYPE_MIXED,       //��ʾ�¼�psrģ�������chunk�к���audio,video�Ļ������
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
    unsigned short       eAspectRatio;   //the source picture aspect ratio, ��߱�, 1000Ϊ��λ������1:1,����֮���ratio,������ve��ѹ��
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
    __vdrv_rect_t       src_rect;           // source valid size,������, ����frame buf�Ŀ��,vdrv��frame buf�Ŀ����ż�������Ϊ��λ.���Ǳ���ʱ�Ŀ��,�����������������Ϊ��λ������ʱ���������ľ�����ʵͼ��Ŀ�ߣ���ϵͳ��IPĬ��2�������߽�,���Բ������
    __vdrv_rect_t       dst_rect;           // source display size,������ͼ��Ŀ��.�������ת��,����������ת��Ŀ�߶�����ԭͼ��.�����ǽ�������ʵ���,ѹ��֮��.
    unsigned char              top_index;          // frame buffer index containing the top field
    unsigned int               top_y;              // the address of frame buffer, which contains top field luminance
    unsigned int               top_c;              // the address of frame buffer, which contains top field chrominance

    //the following is just for future
    unsigned char              bottom_index;       // frame buffer index containing the bottom field
    unsigned int               bottom_y;           // the address of frame buffer, which contains bottom field luminance
    unsigned int               bottom_c;           // the address of frame buffer, which contains bottom field chrominance

    //time stamp of the frame
    unsigned int               uPts;               // time stamp of the frame, ms
    //long long                  uPts;            //��չΪ__s64,��dtv�ĽǶȳ���,��clock������ͷ

    //scale param
    unsigned char       horizontal_scale_ratio; //* what ratio this picture has been scaled down at horizon size, 0: 1/1, 1: 1/2, 2: 1/4, 3: 1/8; 0xff:��Ч
    unsigned char       vertical_scale_ratio;   //* what ratio this picture has been scaled down at vetical size, 0: 1/1, 1: 1/2, 2: 1/4, 3: 1/8; 0xff:��Ч

    //MAF
    __s32               maf_valid;  //maf_valid��ʾmaf buffer�Ƿ�ʹ��
    void*               flag_addr;  //maf buffer����ʼ��ַ
    __u32               flag_stride;    //0x40.
    __s32               pre_frame_valid;    //ǰһ֡�Ƿ��������deinterlace
} __display_info_t;

//the information from media file
#define CEDARLIB_MAX_AUDIO_STREAM_NUM    (16)
#define MAX_VIDEO_STREAM_NUM    (1)
#define MAX_SUBTITLE_STREAM_NUM (32)                //SUBTITLE_MAX_NUM����Ե��ļ�(��Ƶ�ļ�������Ļ�����������Ļ�ļ�)
#define ID3_INFO_BUF_SIZE       (64)
#define CEDARLIB_MAX_LANG_CHAR_SIZE  (32)   //�������MAX_LANG_CHAR_SIZE
typedef struct  CEDARLIB_AUDIO_BS_INFO
{
    int       codec_type;     //enum __CEDARLIB_AUDIO_CODEC_TYPE,AUDIO_MP3��   <==>__cedar_audio_fmt_t
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
    unsigned short       codec_type; //ʹ��cedarlib�����enum __CEDARLIB_VIDEO_CODEC_TYPE, VIDEO_XVID<==>__cedar_video_fmt_t
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
    int   palette_ary[CEDARLIB_PALETTE_CNT];//RGB��ʽ�ĵ�ɫ��
}__cedarlib_palette_t;
//subtitle format information
typedef struct {
    int       subtitle_coming_type;  //embeded subtitle or extern subtitle, __subtitle_source_t.
    int       subtitle_type;  // srt, sub, etc. enum __CEDARLIB_LYRIC_FILE_FMT  <==> __cedar_lyric_fmt_t
    __cedarlib_palette_t palette_info;   //�����SUB��ʽ����Ļ����Ҫ����Ϣ�������__palette_t����һ��
    int       stream_idx;     //in one file(e.g,sub),there will exist several streams, but in srt file, stream_idx means nothing.����psr����Ļ�����ڲ����,��ѡ��Ļʱ�õ��������±�
    int       data_encode_type;    //��ʾlang[]���ַ���������, enum __CEDARLIB_LYRIC_ENCODE_TYPE(LYRIC_SUB_ENCODE_UTF8),  ��ΧתΪ__cedar_subtitle_encode_t
    unsigned char        lang[CEDARLIB_MAX_LANG_CHAR_SIZE];
} __cedarlib_subtitle_stream_info_t;
//extern __s32 subtitle_stream_info_lib2cedar(__subtitle_stream_info_t *pDes, __cedarlib_subtitle_stream_info_t *pSrc),��ת�ĺ���


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
    char        nHasAudio;          //ý���ļ��Ƿ�����Ƶ�����ж��ٸ���Ƶ��, nHasAudio�͵��ڼ�,����mkv. Ҳ�е�parserֻ���Է�0��ʾ��
    char        nHasVideo;          //ý���ļ��Ƿ�����Ƶ��, ��Щpsr����ֵ������Ƶ��������������mkv.
    char        nHasLrc;            //ý���ļ��Ƿ�����Ļ��,Ӧ����רָ������Ļ.��Щpsr����ֵ������Ļ��������������mkv.
    char        nUseExtLrc;         //ý���ļ��Ƿ�����ʹ�������Ļ��������,û���õ�.�Ժ���ֵӦ��ֻ�����0.

    unsigned char        nStreamNum;         //ý���ļ�����������
    unsigned char        nAudStrmNum;        //ý���ļ�����Ƶ���ĸ���
    unsigned char        nVidStrmNum;        //ý���ļ�����Ƶ���ĸ���
    unsigned char        nLrcStrmNum;        //ý���ļ�����Ļ���ĸ���

    //__subtitle_profile_t    LyricInfo;       //ý���ļ���Ƕ��ÿ����Ļ������Ϣ

    unsigned int       nDuration;          //ý���ļ��ĳ�����ʱ��
    
    DisplayModeInfo   display_mode_info;  //display mode information, 2D/3D

    __cedarlib_audio_bs_info_t           AudStrmList[CEDARLIB_MAX_AUDIO_STREAM_NUM];  //��Ƶ����Ϣ���е�ͷָ�룬�����__audio_bs_info_tһ��
    __cedarlib_video_bs_info_t           VidStrmList[MAX_VIDEO_STREAM_NUM];  //��Ƶ����Ϣ���е�ͷָ��, �����__video_bs_info_tһ��
    __cedarlib_subtitle_stream_info_t    SubtitleStreamList[MAX_SUBTITLE_STREAM_NUM];   //SUBTITLE_MAX_NUM, ֻ��¼������Ļ�������__subtitle_stream_info_tһ��

    unsigned char        Author[ID3_INFO_BUF_SIZE];                  //���߱�ǩ
    int       nAuthorLen;                                 //���߱�ǩ����
    unsigned char        CopyRight[ID3_INFO_BUF_SIZE];               //��Ȩ��ǩ
    int       nCopyRightLen;                              //��Ȩ��ǩ����
    unsigned char        Title[ID3_INFO_BUF_SIZE];                   //�����ǩ
    int       nTitleLen;                                  //�����ǩ����
    unsigned char        Genre[ID3_INFO_BUF_SIZE];                   //���ɱ�ǩ
    int       nGenreLen;                                  //���ɱ�ǩ����

} __media_file_info_internal_t;

#endif  //_MEDIA_TYPE_H

