
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
//    __u32   codec_type;                 //* 音频编码器类型
//    __u32   codec_type_sub;             //* 音频编码器类型次级描述
//    __u16   channels;                   //* 音频通道数
//    __u16   bits_per_sample;            //* 每音频采样的比特数
//    __u32   sample_per_second;          //* 音频采样率
//    __u32   avg_bit_rate;               //* 平均码率
//    __u32   max_bit_rate;               //* 最大码率
//
//    unsigned int       file_length;       // 文件总长度
//    __u32   audio_bs_src;               //* 标志音频流从哪个demuxer送出
//    int     is_raw_data_output; //0:pcm output, 1:raw data 
//    __u32   priv_inf_len;               //* 额外信息数据长度
//    void*   private_inf;                //* 额外信息数据起始地址
//}__ad_codec_fmt_t;
//typedef __audio_codec_format_t __ad_codec_fmt_t;

//typedef struct __VIDEO_CODEC_FORMAT
//{
//    __u32   codec_type;                 //* 音频编码器类型
//    __u8    ifrm_pts_credible;          //* I帧PTS是否可信
//    __u8    pfrm_pts_credible;          //* P帧PTS是否可信
//    __u8    bfrm_pts_credible;          //* B帧PTS是否可信
//    __u16   width;                      //* 视频源高度
//    __u16   height;                     //* 视频源宽度
//    __u32   frame_rate;                 //* 帧率，（每1000秒播放的视频帧数）
//    __u32   mic_sec_per_frm;            //* 每帧播放时间，从帧率推算
//    __s32   avg_bit_rate;               //* 平均码率
//    __s32   max_bit_rate;               //* 最大码率
//    __u16   video_bs_src;               //* 标志视频流从哪个demuxer送出，TS流设为2表示流格式， CMMB设为0表示帧格式
//    __s16   priv_inf_len;               //* 额外信息数据长度
//    void*   private_inf;                //* 额外信息数据起始地址
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

    //以下是各模块的反馈消息
    DVBC_MAINTASK_CMD_APLYMSG_AB_B_DONE  = _CEDAR_CMD_APLYMSG_AB_B_DONE,    //aply模块在ab播放时，遇到了B点
    DVBC_MAINTASK_CMD_APLYMSG_STOP  = _CEDAR_CMD_APLYMSG_STOP,
    
    DVBC_MAINTASK_CMD_VPLYMSG_AB_B_DONE = _CEDAR_CMD_VPLYMSG_AB_B_DONE,      //vply模块在ab播放时，遇到了B点
    DVBC_MAINTASK_CMD_VPLYMSG_STOP  = _CEDAR_CMD_VPLYMSG_STOP,

    DVBC_MAINTASK_CMD_DISK_FULL     = _CEDAR_CMD_DISK_FULL, //录制时,muxer模块发现磁盘满,发送的消息
    
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


typedef enum __DVB_CMD_SET        //* 媒体播放中间件命令列表(IOCtrol)
{
    DVB_CMD_GET_MESSAGE_CHN,             //* 获取dvbc的反馈消息通道,不实现.
    DVB_CMD_SET_FEEDBACK_MSG_QUEUE,      // 设置给DVBC的消息通道,如果dvbc有消息要反馈,直接通过它发送, pbuffer = __krnl_event_t*
    DVB_CMD_PLAY,                        //* 启动播放, aux = 0:fullseg, 1:oneseg; 对于非isdb格式,一般是0.
    DVB_CMD_STOP,                        //* 停止播放
    DVB_CMD_GET_STATUS,                  //* 获取当前状态
    DVB_CMD_SET_PCR,                     //* 设置pcr值, 获取pcrpts后,立刻调用该接口校准video clock,本中间件用video clock模拟stream clock. pbuffer = pcrpts,单位ms, __u32
    DVB_CMD_GET_TIME,                    //* 获取当前播放时间, 以最初的pts为基础累加,不回头.
    DVB_CMD_SET_MEDIA_INFO,              //* 设置音频、视频格式信息
    DVB_CMD_REQUEST_BUFFER,              //* 请求媒体数据buffer
    DVB_CMD_UPDATE_DATA,                 //* 更新媒体数据
    DVB_CMD_SET_VOL,                     //* 设置音量
    DVB_CMD_GET_VOL,                     //* 获取当前音量
    DVB_CMD_VOLUP,                       //* 提高音量
    DVB_CMD_VOLDOWN,                     //* 降低音量
    DVB_CMD_SET_VIDEO_LAYER,             //* 设置视频播放图层句柄
    DVB_CMD_SET_VIDEO_WINDOW,            //* 设置视频播放窗口
    DVB_CMD_GET_VIDEO_WINDOW,            //* 获取视频播放窗口坐标信息
    DVB_CMD_SET_VIDEO_SHOW_MODE,         //* 设置视频播放画面比例, 设置视频图象显示的模式，aux = __vd_win_ratio_mode_t, return = 设置后的当前显示模式, 如果 aux = VID_WINDOW_USERDEF, pbuffer = __pCBK_t, 参数类型是SrcScnWinPara,参看mod_cedar.h
    DVB_CMD_SET_VIDEO_LYR_ALPHA,         //* 设置alpha blending 值, 0-255, 255为不透明
    DVB_CMD_SET_VIDEO_LYR_ALPHA_ON,      //* 开启alpha blending
    DVB_CMD_SET_VIDEO_LYR_ALPHA_OFF,     //* 关闭alpha blending
    DVB_CMD_CONFIG_VIDEO_LYR_CK_ENABLE,     // 配置设置给maple的视频图层的ck_enable参数的值, aux = 1:enable; aux = 0:disable
    DVB_CMD_ENABLE_DEINTERLACE,         //配置deinterlace功能, aux = 1:enable; aux=0:disable;
    DVB_CMD_GET_FRAMESIZE,              //* frame size       return = (width << 16) + height
    DVB_CMD_SET_CLOSE_CAPTION_CB,        //* Set call back function for Close Caption;

    DVB_CMD_SET_MAPLE_INI,              //把maple的配置设置给playctrl, pbuffer = CMapleINI*
    DVB_CMD_SET_SPLY_DRAW_CB,                 //
		DVB_CMD_GET_VE_STATUS,						//弱信号状态下获取解码是否成功的标记
    DVB_CMD_CHECK_PICTURE_OUT, 					//ret = EPDK_TRUE,显示.当播放视频时，查询第一帧图像是否已经显示出来;如果是音频,则返回值始终为EPDK_FALSE.
		
    DVB_CMD_SET_SWGAIN,
    DVB_CMD_GET_SWGAIN,
    DVB_CMD_SET_RESERVED_MEM_SIZE, 			//设置预留的内存数量,当告诉解码器可用的内存数量时,把这部分内存预留下来.
		DVB_CMD_SET_TV_MODE,								//设置1:TV MODE 0:本地媒体播放
		DVB_CMD_SET_ROTATE,							    // 设置解码图像旋转的角度.aux = 0:默认为原图,1:顺时针90度,2:180度,3:270度,4:水平镜像,5:垂直镜像,注意事项:只允许在文件播放之前设置，播放过程中(即解码过程中)设置无效
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


//* 用户自定义音效滤波器的频段数，用户需要自行设定各频段的滤波系数
#define USR_EQ_BAND_CNT                 (10)
#define USR_EQ_NEGATIVE_PEAK_VALUE      (-12)
#define USR_EQ_POSITIVE_PEAK_VALUE      (+12)

typedef enum __AUDIO_EQ       //* 音效类型定义
{
    AUD_EQ_TYPE_NORMAL=0,            //* 自然
    AUD_EQ_TYPE_DBB,                 //* 重低音
    AUD_EQ_TYPE_POP,                 //* 流行
    AUD_EQ_TYPE_ROCK,                //* 摇滚
    AUD_EQ_TYPE_CLASSIC,             //* 古典
    AUD_EQ_TYPE_JAZZ,                //* 爵士
    AUD_EQ_TYPE_VOCAL,               //* 语言
    AUD_EQ_TYPE_DANCE,               //* 舞曲
    AUD_EQ_TYPE_SOFT,                //* 柔和
    AUD_EQ_TYPE_USR_MODE=0xff,       //* 用户模式, pbuffer = __s8 UsrEqArray[USR_EQ_BAND_CNT],
                                     //* 每个点的值为: USR_EQ_POSITIVE_PEAK_VALUE~USR_EQ_NEGATIVE_PEAK_VALUE
    AUD_EQ_TYPE_

} __audio_eq_t;

typedef enum __VIDEO_WINDOW_RATIO_MODE
{
    VID_WINDOW_NOTCARE  = CEDAR_VID_WINDOW_NOTCARE,       //* 不关心图片显示比例，以当前设置的比例
    VID_WINDOW_ORIGINAL = CEDAR_VID_WINDOW_ORIGINAL,             //* 以图片原始大小在窗口内显示，不能溢出窗口
    VID_WINDOW_BESTSHOW = CEDAR_VID_WINDOW_BESTSHOW,             //* 以图片本身的比例缩放至满窗口显示，图片不变形
    VID_WINDOW_FULLSCN  = CEDAR_VID_WINDOW_FULLSCN,              //* 以窗口的比例缩放图片至满窗口显示，可能会变形
    VID_WINDOW_4R3MODE  = CEDAR_VID_WINDOW_4R3MODE,              //* 强制以4:3的模式满屏显示视频图像，图像会变形
    VID_WINDOW_16R9MODE = CEDAR_VID_WINDOW_16R9MODE,             //* 强制以16:9的模式满屏显示视频图像，图像会变形
    VID_WINDOW_CUTEDGE  = CEDAR_VID_WINDOW_CUTEDGE,
    VID_WINDOW_USERDEF  = CEDAR_VID_WINDOW_USERDEF,

    VID_WINDOW_NUM_

}__vd_win_ratio_mode_t;


#endif

