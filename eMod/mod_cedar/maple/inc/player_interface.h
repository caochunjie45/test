
#ifndef _PLAYER_INTERFACE_H
#define _PLAYER_INTERFACE_H

//#include "maple_common.h"
//#include "media_type.h"
//#include "dmx_data.h"

#define PLAYER_PLUGIN_FILE_PATH  CEDAR_PLUGINS_DIRECTORY"dvbc.mod"

typedef enum __TV_STANDAR
{
    PLAYER_STANDAR_DVB_T,
    PLAYER_STANDAR_ISDB_T,
    PLAYER_STANDAR_CMMB,
    
    PLAYER_STANDAR_NUM
}player_tv_standar_e;

//typedef struct __MEDIA_BUFFER
//{
//    __u8*   buf;
//    __u32   bufSize;
//}__md_buf_t;
//
//
//typedef struct __MEDIA_DATA_INFO
//{
//    __s64   pts;
//    __u32   dataLen;
//    __u32   ctrlBits;
//}__md_data_info_t;


//#define PTS_VALID_BIT           (1<<1)  //* whether pts is valid.
//#define FIRST_PART_BIT          (1<<3)  //* media data is the first part of a new frame.
//#define LAST_PART_BIT           (1<<4)  //* media data is the last part of a frame.
//#define RANDOM_ACCESS_FRAME_BIT (1<<10)     //* random access point of stream.

//* video vbv buffer size is 8MB, we buffer video data for about 1~2 seconds
//* Bitrate of low resolution video is about 300 Kbps
#define LOW_RESOLUTION_VIDEO_VBV_UNDERFLOW_PERCENTAGE       (5)         //* percentage * 10
#define LOW_RESOLUTION_VIDEO_VBV_OVERFLOW_PERCENTAGE        (15)        //* percentage * 10

//* Bitrate of standar resolution video is about 3 Mbps
#define STANDAR_RESOLUTION_VIDEO_VBV_UNDERFLOW_PERCENTAGE   (50)        //* percentage * 10
#define STANDAR_RESOLUTION_VIDEO_VBV_OVERFLOW_PERCENTAGE    (100)       //* percentage * 10

//* Bitrate of high resolution video is about 9 Mbps
#define HIGH_RESOLUTION_VIDEO_VBV_UNDERFLOW_PERCENTAGE      (150)       //* percentage * 10
#define HIGH_RESOLUTION_VIDEO_VBV_OVERFLOW_PERCENTAGE       (600)       //* percentage * 10

//* audio vbv buffer is 128KB.
#define AUDIO_VBV_UNDERFLOW_PERCENTAGE                      (10)        //* percentage * 10
#define AUDIO_VBV_OVERFLOW_PERCENTAGE                       (20)        //* percentage * 10


//typedef struct __AUDIO_CODEC_FORMAT
//{
//    __u32   codec_type;                 //* ��Ƶ����������
//    __u32   codec_type_sub;             //* ��Ƶ���������ʹμ�����
//    __u16   channels;                   //* ��Ƶͨ����
//    __u16   bits_per_sample;            //* ÿ��Ƶ�����ı�����
//    __u32   sample_per_second;          //* ��Ƶ������
//    __u32   avg_bit_rate;               //* ƽ������
//    __u32   max_bit_rate;               //* �������
//
//    unsigned int       file_length;       // �ļ��ܳ���
//    __u32   audio_bs_src;               //* ��־��Ƶ�����ĸ�demuxer�ͳ�
//    int     is_raw_data_output; //0:pcm output, 1:raw data 
//    __u32   priv_inf_len;               //* ������Ϣ���ݳ���
//    void*   private_inf;                //* ������Ϣ������ʼ��ַ
//}__ad_codec_fmt_t;
//typedef __audio_codec_format_t __ad_codec_fmt_t;

//typedef struct __VIDEO_CODEC_FORMAT
//{
//    __u32   codec_type;                 //* ��Ƶ����������
//    __u8    ifrm_pts_credible;          //* I֡PTS�Ƿ����
//    __u8    pfrm_pts_credible;          //* P֡PTS�Ƿ����
//    __u8    bfrm_pts_credible;          //* B֡PTS�Ƿ����
//    __u16   width;                      //* ��ƵԴ�߶�
//    __u16   height;                     //* ��ƵԴ���
//    __u32   frame_rate;                 //* ֡�ʣ���ÿ1000�벥�ŵ���Ƶ֡����
//    __u32   mic_sec_per_frm;            //* ÿ֡����ʱ�䣬��֡������
//    __s32   avg_bit_rate;               //* ƽ������
//    __s32   max_bit_rate;               //* �������
//    __u16   video_bs_src;               //* ��־��Ƶ�����ĸ�demuxer�ͳ���TS����Ϊ2��ʾ����ʽ�� CMMB��Ϊ0��ʾ֡��ʽ
//    __s16   priv_inf_len;               //* ������Ϣ���ݳ���
//    void*   private_inf;                //* ������Ϣ������ʼ��ַ
//
//}__vd_codec_fmt_t;
//typedef __video_codec_format_t __vd_codec_fmt_t;

//typedef struct __SUBTITLE_CODEC_FORMAT
//{
//    __u32   codec_type;
//}__subt_codec_fmt_t;

//typedef __subtitle_codec_format_t __subt_codec_fmt_t;

enum tag_DVBC_MAINTASK_MSG
{
    DVBC_MAINTASK_CMD_PLAY          = _CEDAR_CMD_PLAY,              //* play
    DVBC_MAINTASK_CMD_PAUSE         = _CEDAR_CMD_PAUSE,               //* pause
    DVBC_MAINTASK_CMD_STOP          = _CEDAR_CMD_STOP,                //* stop
    DVBC_MAINTASK_CMD_SET_PLAYLIST  = _CEDAR_CMD_SET_PLAYLIST,        //* dir or playlist/playlistfile
    DVBC_MAINTASK_CMD_FF            = _CEDAR_CMD_FF,                  //* fast forward
    DVBC_MAINTASK_CMD_RR            = _CEDAR_CMD_RR,                  //* fast backward
    DVBC_MAINTASK_CMD_CLEAR_FFRR    = _CEDAR_CMD_CLEAR_FFRR,          //* clear fast forward/backword
    DVBC_MAINTASK_CMD_STOP_RECORD   = _CEDAR_CMD_STOP_RECORD,
    DVBC_MAINTASK_CMD_PLYEND        = _CEDAR_CMD_PLYEND,       //* current file play end
    DVBC_MAINTASK_CMD_STEP          = _CEDAR_CMD_STEP,
    DVBC_MAINTASK_CMD_JUMP          = _CEDAR_CMD_JUMP,
    DVBC_MAINTASK_CMD_SEL_AUDSTREAM = _CEDAR_CMD_SEL_AUDSTREAM,       //* change aud stream

    //�����Ǹ�ģ��ķ�����Ϣ
    DVBC_MAINTASK_CMD_APLYMSG_AB_B_DONE  = _CEDAR_CMD_APLYMSG_AB_B_DONE,    //aplyģ����ab����ʱ��������B��
    DVBC_MAINTASK_CMD_APLYMSG_STOP  = _CEDAR_CMD_APLYMSG_STOP,
    
    DVBC_MAINTASK_CMD_VPLYMSG_AB_B_DONE = _CEDAR_CMD_VPLYMSG_AB_B_DONE,      //vplyģ����ab����ʱ��������B��
    DVBC_MAINTASK_CMD_VPLYMSG_STOP  = _CEDAR_CMD_VPLYMSG_STOP,

    DVBC_MAINTASK_CMD_DISK_FULL     = _CEDAR_CMD_DISK_FULL, //¼��ʱ,muxerģ�鷢�ִ�����,���͵���Ϣ
    
    DVBC_MAINTASK_CMD_NOP          = _CEDAR_CMD_NOP,                 //* empty command, weekup cedar main task
    DVBC_MAINTASK_CMD_
};

typedef enum MEDIA_TYPE
{
    MEDIA_TYPE_VIDEO,
    MEDIA_TYPE_AUDIO,
    MEDIA_TYPE_SUBTITLE,
    
    MEDIA_TYPE_NUM
}__cc_md_type_t;


typedef enum __MEDIA_SOURCE
{
    MEDIA_SOURCE_FILE,
    MEDIA_SOURCE_DVB,
    MEDIA_SOURCE_CMMB,

    MEDIA_SOURCE_NUM_
}__md_src_t;


typedef enum __DVB_CMD_SET        //* ý�岥���м�������б�(IOCtrol)
{
    DVB_CMD_GET_MESSAGE_CHN,             //* ��ȡdvbc�ķ�����Ϣͨ��,��ʵ��.
    DVB_CMD_SET_FEEDBACK_MSG_QUEUE,      // ���ø�DVBC����Ϣͨ��,���dvbc����ϢҪ����,ֱ��ͨ��������, pbuffer = __krnl_event_t*
    DVB_CMD_PLAY,                        //* ��������, aux = 0:fullseg, 1:oneseg; ���ڷ�isdb��ʽ,һ����0.
    DVB_CMD_STOP,                        //* ֹͣ����
    DVB_CMD_GET_STATUS,                  //* ��ȡ��ǰ״̬
    DVB_CMD_SET_PCR,                     //* ����pcrֵ, ��ȡpcrpts��,���̵��øýӿ�У׼video clock,���м����video clockģ��stream clock. pbuffer = pcrpts,��λms, __u32
    DVB_CMD_GET_TIME,                    //* ��ȡ��ǰ����ʱ��, �������ptsΪ�����ۼ�,����ͷ.
    DVB_CMD_SET_MEDIA_INFO,              //* ������Ƶ����Ƶ��ʽ��Ϣ
    DVB_CMD_REQUEST_BUFFER,              //* ����ý������buffer
    DVB_CMD_UPDATE_DATA,                 //* ����ý������
    DVB_CMD_SET_VOL,                     //* ��������
    DVB_CMD_GET_VOL,                     //* ��ȡ��ǰ����
    DVB_CMD_VOLUP,                       //* �������
    DVB_CMD_VOLDOWN,                     //* ��������
    DVB_CMD_SET_VIDEO_LAYER,             //* ������Ƶ����ͼ����
    DVB_CMD_SET_VIDEO_WINDOW,            //* ������Ƶ���Ŵ���
    DVB_CMD_GET_VIDEO_WINDOW,            //* ��ȡ��Ƶ���Ŵ���������Ϣ
    DVB_CMD_SET_VIDEO_SHOW_MODE,         //* ������Ƶ���Ż������, ������Ƶͼ����ʾ��ģʽ��aux = __vd_win_ratio_mode_t, return = ���ú�ĵ�ǰ��ʾģʽ, ��� aux = VID_WINDOW_USERDEF, pbuffer = __pCBK_t, ����������SrcScnWinPara,�ο�mod_cedar.h
    DVB_CMD_SET_VIDEO_LYR_ALPHA,         //* ����alpha blending ֵ, 0-255, 255Ϊ��͸��
    DVB_CMD_SET_VIDEO_LYR_ALPHA_ON,      //* ����alpha blending
    DVB_CMD_SET_VIDEO_LYR_ALPHA_OFF,     //* �ر�alpha blending
    DVB_CMD_CONFIG_VIDEO_LYR_CK_ENABLE,     // �������ø�maple����Ƶͼ���ck_enable������ֵ, aux = 1:enable; aux = 0:disable
    DVB_CMD_ENABLE_DEINTERLACE,         //����deinterlace����, aux = 1:enable; aux=0:disable;
    DVB_CMD_GET_FRAMESIZE,              //* frame size       return = (width << 16) + height
    DVB_CMD_SET_CLOSE_CAPTION_CB,        //* Set call back function for Close Caption;

    DVB_CMD_SET_MAPLE_INI,              //��maple���������ø�playctrl, pbuffer = CMapleINI*
    DVB_CMD_SET_SPLY_DRAW_CB,                 //
		DVB_CMD_GET_VE_STATUS,						//���ź�״̬�»�ȡ�����Ƿ�ɹ��ı��
    DVB_CMD_CHECK_PICTURE_OUT, 					//ret = EPDK_TRUE,��ʾ.��������Ƶʱ����ѯ��һ֡ͼ���Ƿ��Ѿ���ʾ����;�������Ƶ,�򷵻�ֵʼ��ΪEPDK_FALSE.
		
    DVB_CMD_SET_SWGAIN,
    DVB_CMD_GET_SWGAIN,
    DVB_CMD_SET_RESERVED_MEM_SIZE, 			//����Ԥ�����ڴ�����,�����߽��������õ��ڴ�����ʱ,���ⲿ���ڴ�Ԥ������.
		DVB_CMD_SET_TV_MODE,								//����1:TV MODE 0:����ý�岥��
		DVB_CMD_SET_ROTATE,							    // ���ý���ͼ����ת�ĽǶ�.aux = 0:Ĭ��Ϊԭͼ,1:˳ʱ��90��,2:180��,3:270��,4:ˮƽ����,5:��ֱ����,ע������:ֻ�������ļ�����֮ǰ���ã����Ź�����(�����������)������Ч
		DVB_CMD_SET_STOP_MODE,			
		DVB_CMD_SET_SWITCH_VPLY_MODE,
		DVB_CMD_NUM_

} __dvb_cmd_set_t;


typedef enum __DVB_STATUS         //* the status oF cedarcore
{
    DVB_STAT_PLAY=0,                     //* play
    DVB_STAT_PAUSE,                      //* pause
    DVB_STAT_STOP,                       //* stop
    DVB_STAT_FF,                         //* fast forward
    DVB_STAT_RR,                         //* fast backward
    DVB_STAT_JUMP,                       //* jump

    DVB_STAT_NUM_

} __dvb_status_t;


typedef enum __DVB_AUDIO_FMT      //* define audio bit stream format
{
    DVB_ABS_TYPE_NONE           = CEDAR_ABS_TYPE_NONE,               //* no audio stream
    DVB_ABS_TYPE_UNKNOWN        = CEDAR_ABS_TYPE_UNKNOWN,                //* unknown audio stream
    
    DVB_ABS_TYPE_MPEG12_LAYER1  = CEDAR_ABS_TYPE_MPEG12_LAYER1,
    DVB_ABS_TYPE_MPEG12_LAYER2  = CEDAR_ABS_TYPE_MPEG12_LAYER2,
    DVB_ABS_TYPE_MPEG12_LAYER3  = CEDAR_ABS_TYPE_MPEG12_LAYER3,
    DVB_ABS_TYPE_MPEG_AAC_LC    = CEDAR_ABS_TYPE_MPEG_AAC_LC,
    DVB_ABS_TYPE_AC3            = CEDAR_ABS_TYPE_AC3,                    //* AC-3
    DVB_ABS_TYPE_DTS            = CEDAR_ABS_TYPE_DTS,                    //* DTS audio for DVD-Video
    DVB_ABS_TYPE_LPCM_V         = CEDAR_ABS_TYPE_LPCM_V,
    DVB_ABS_TYPE_LPCM_A         = CEDAR_ABS_TYPE_LPCM_A,
    DVB_ABS_TYPE_ADPCM          = CEDAR_ABS_TYPE_ADPCM,
    DVB_ABS_TYPE_PCM            = CEDAR_ABS_TYPE_PCM,                    //* PCM
    DVB_ABS_TYPE_WMA_STANDARD   = CEDAR_ABS_TYPE_WMA_STANDARD,           //* standard WMA audio
    DVB_ABS_TYPE_FLAC           = CEDAR_ABS_TYPE_FLAC,
    DVB_ABS_TYPE_APE            = CEDAR_ABS_TYPE_APE,
    DVB_ABS_TYPE_OGG            = CEDAR_ABS_TYPE_OGG,
    DVB_ABS_TYPE_RAAC           = CEDAR_ABS_TYPE_RAAC,
    DVB_ABS_TYPE_COOK           = CEDAR_ABS_TYPE_COOK,
    DVB_ABS_TYPE_SIPR           = CEDAR_ABS_TYPE_SIPR,
    DVB_ABS_TYPE_ATRC           = CEDAR_ABS_TYPE_ATRC,
    DVB_ABS_TYPE_AMR            = CEDAR_ABS_TYPE_AMR,
    DVB_ABS_TYPE_RA             = CEDAR_ABS_TYPE_RA,
    DVB_ABS_TYPE_MID            = CEDAR_ABS_TYPE_MID,
    DVB_ABS_TYPE_DRA            = CEDAR_ABS_TYPE_DRA,
    DVB_ABS_TYPE_NUM_

} __dvb_ad_fmt_t;

typedef enum __DVB_VIDEO_FMT      //* define video bit stream format
{
    DVB_VBS_TYPE_NONE       = CEDAR_VBS_TYPE_NONE,               //* no video stream
    DVB_VBS_TYPE_UNKNOWN    = CEDAR_VBS_TYPE_UNKNOWN,                //* unknown video stream
    
    DVB_VBS_TYPE_JPEG       = CEDAR_VBS_TYPE_JPEG,
    DVB_VBS_TYPE_MJPEG      = CEDAR_VBS_TYPE_MJPEG,
    DVB_VBS_TYPE_MPEG1_ES   = CEDAR_VBS_TYPE_MPEG1_ES,
    DVB_VBS_TYPE_MPEG2_ES   = CEDAR_VBS_TYPE_MPEG2_ES,
    DVB_VBS_TYPE_XVID       = CEDAR_VBS_TYPE_XVID,
    DVB_VBS_TYPE_DIVX3      = CEDAR_VBS_TYPE_DIVX3,
    DVB_VBS_TYPE_DIVX4      = CEDAR_VBS_TYPE_DIVX4,
    DVB_VBS_TYPE_DIVX5      = CEDAR_VBS_TYPE_DIVX5,
    DVB_VBS_TYPE_SORENSSON_H263 = CEDAR_VBS_TYPE_SORENSSON_H263,
    DVB_VBS_TYPE_RMG2       = CEDAR_VBS_TYPE_RMG2,
    DVB_VBS_TYPE_RMVB8      = CEDAR_VBS_TYPE_RMVB8,
    DVB_VBS_TYPE_RMVB9      = CEDAR_VBS_TYPE_RMVB9,
    DVB_VBS_TYPE_H264       = CEDAR_VBS_TYPE_H264,
    DVB_VBS_TYPE_VC1        = CEDAR_VBS_TYPE_VC1,
    DVB_VBS_TYPE_AVS        = CEDAR_VBS_TYPE_AVS,
    DVB_VBS_TYPE_H263       = CEDAR_VBS_TYPE_H263,
    DVB_VBS_TYPE_VP6        = CEDAR_VBS_TYPE_VP6,
    DVB_VBS_TYPE_WMV1       = CEDAR_VBS_TYPE_WMV1,
    DVB_VBS_TYPE_WMV2       = CEDAR_VBS_TYPE_WMV2,
    
    DVB_VBS_TYPE_NUM_

}__dvb_vd_fmt_t;


typedef enum __DVB_SUBTITLE_FMT
{
    DVB_SBS_TYPE_NONE = CEDAR_LRC_TYPE_NONE,
    DVB_SBS_TYPE_UNKNOWN = CEDAR_LRC_TYPE_NOTSPT,
    DVB_SBS_TYPE_LRC     = CEDAR_LRC_TYPE_LRC,
    
    DVB_SBS_TYPE_ARIBSUBTITLE    = CEDAR_LRC_TYPE_ISDB,
    
    DVB_SBS_TYPE_DVBSUBTITLE = CEDAR_LRC_TYPE_DVBSUB,
    
	DVB_SBS_TYPE_DVBTELETEXT = CEDAR_LRC_TYPE_TTXT,
    DVB_SBS_TYPE_NUM_
}__dvb_subt_fmt_t;


//* �û��Զ�����Ч�˲�����Ƶ�������û���Ҫ�����趨��Ƶ�ε��˲�ϵ��
#define USR_EQ_BAND_CNT                 (10)
#define USR_EQ_NEGATIVE_PEAK_VALUE      (-12)
#define USR_EQ_POSITIVE_PEAK_VALUE      (+12)

typedef enum __AUDIO_EQ       //* ��Ч���Ͷ���
{
    AUD_EQ_TYPE_NORMAL=0,            //* ��Ȼ
    AUD_EQ_TYPE_DBB,                 //* �ص���
    AUD_EQ_TYPE_POP,                 //* ����
    AUD_EQ_TYPE_ROCK,                //* ҡ��
    AUD_EQ_TYPE_CLASSIC,             //* �ŵ�
    AUD_EQ_TYPE_JAZZ,                //* ��ʿ
    AUD_EQ_TYPE_VOCAL,               //* ����
    AUD_EQ_TYPE_DANCE,               //* ����
    AUD_EQ_TYPE_SOFT,                //* ���
    AUD_EQ_TYPE_USR_MODE=0xff,       //* �û�ģʽ, pbuffer = __s8 UsrEqArray[USR_EQ_BAND_CNT],
                                     //* ÿ�����ֵΪ: USR_EQ_POSITIVE_PEAK_VALUE~USR_EQ_NEGATIVE_PEAK_VALUE
    AUD_EQ_TYPE_

} __audio_eq_t;

typedef enum __VIDEO_WINDOW_RATIO_MODE
{
    VID_WINDOW_NOTCARE  = CEDAR_VID_WINDOW_NOTCARE,       //* ������ͼƬ��ʾ�������Ե�ǰ���õı���
    VID_WINDOW_ORIGINAL = CEDAR_VID_WINDOW_ORIGINAL,             //* ��ͼƬԭʼ��С�ڴ�������ʾ�������������
    VID_WINDOW_BESTSHOW = CEDAR_VID_WINDOW_BESTSHOW,             //* ��ͼƬ����ı�����������������ʾ��ͼƬ������
    VID_WINDOW_FULLSCN  = CEDAR_VID_WINDOW_FULLSCN,              //* �Դ��ڵı�������ͼƬ����������ʾ�����ܻ����
    VID_WINDOW_4R3MODE  = CEDAR_VID_WINDOW_4R3MODE,              //* ǿ����4:3��ģʽ������ʾ��Ƶͼ��ͼ������
    VID_WINDOW_16R9MODE = CEDAR_VID_WINDOW_16R9MODE,             //* ǿ����16:9��ģʽ������ʾ��Ƶͼ��ͼ������
    VID_WINDOW_CUTEDGE  = CEDAR_VID_WINDOW_CUTEDGE,
    VID_WINDOW_USERDEF  = CEDAR_VID_WINDOW_USERDEF,

    VID_WINDOW_NUM_

}__vd_win_ratio_mode_t;


#endif

