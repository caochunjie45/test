/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                           mod_cedarcore sub-system
*
*                                (c) Copyright 2009-20010, Clif.Chen China
*                                               All Rights Reserved
*
* File   : play_ctrl.h
* Version: V1.0
* By     : clif.c
* update : 
*********************************************************************************************************
*/

#ifndef _PLAY_CTRL_H
#define _PLAY_CTRL_H

//#include "mod_dvb_i.h"
#include "mod_cedar_i.h"
#include "audio_plybk.h"
#include "video_plybk.h"
#include "subtitle_plybk.h"
//#include "psr_2_ldec.h"

#define DVBC_CMDQ_SIZE             (8)  //play_ctrl内部的cmdqueue队列的size
#define DVBC_MSGQ_SIZE             (4)  //对上层的feedback msgQ的size
#define DVBC_TMP_STATUS_FLAG       (1<<7)
#define DVBC_TMP_STATUS_RESET_FALG (1<<6)

#define DEBUG_SAVE_VIDEO_STREAM		(0)
#define DEBUG_SAVE_AUDIO_STREAM		(0)
#define DEBUG_SAVE_SUBTITLE_STREAM  (0)

#define PLAY_VIDEO_STREAM			(1) //内部调试,不影响dmx
#define PLAY_AUDIO_STREAM			(1)
#define PLAY_SUBTITLE_STREAM        (1)
#define DEBUG_TIME					(0)
#define DRAW_RADIO_BACKGROUND       (0)


#define SYSTEM_CLOCK_START_TIME		(10000)		//* start point in ms, 也就是说:不管dmx送来的数据的pts是多少,统一从10s开始播放,这是内部时间，并且一直不回头.
#define TIME_BEFORE_PLAY			(1000)

#define DVBC_TMP_STATUS_BIT			(0x8000)

//数字电视相关设置
#define DTV_DELAY_TIME (300)   //定义在cedar插件里面的设置的缓冲时间.不包括前端DMX/TSC的缓冲时间.单位ms,处理PTS回头时使用

//* the setting information for cedarcore sub-system
typedef struct __DVBC_SETTING_INF
{
    //* 音频播放的相关设置
    __u32       volume;                     //* 音量值
    __u32       eq;                         //* 音效类型
    __u32       channel;                    //* 声道模式
    __u32       vpsspeed;                   //* 播放速度
    __u32       ab_loopcnt;                 //* A-B播放循环次数
    __u32       spectrum;                   //* 是否使能频谱解析
    __s32	      swgain;
    __s8        usr_eq_filter[USR_EQ_BAND_CNT];

    //* 用户自定义音效的滤波器频段的滤波系数
    __u8        volume_flag;                //* volume参数是否有效的标志
    __u8        eq_flag;                    //* eq参数是否有效的标志
    __u8        channel_flag;               //* channel参数是否有效的标志
    __u8        vpsspeed_flag;              //* vpsspeed参数是否有效的标志
    __u8        ab_loopcnt_flag;            //* ab_loopcnt参数是否有效的标志
    __u8        spectrum_flag;              //* spectrum参数是否有效的标志
    __s32       aud_suffix;                 //* 当前播放的音频流在音频流数组的下标

    //* 视频播放的相关设置
    __s32       vid_layer_hdl;              //* 视频物理图层句柄
    __rect_t    vid_window;                 //* 视频图像窗口属性
    __s32       vid_show_mode;              //* 视频图像显示比例模式
    __u32       frpic_hold_time;            //* 快进、快退状态下视频图片的停留时间
    __s32       nCkEnableValue;             //* VirtualDE_DispLayerInfo->ck_enable是否置1,置1表示视频图层允许colorKey的透过.这个参数由app根据需要配置,默认为0.

    __s8        vid_layer_hdl_flag;         //* vid_layer_hdl参数是否有效的标志
    __s8        vid_window_flag;            //* vid_window参数是否有效的标志
    __s8        vid_show_mode_flag;         //* vid_show_mode参数是否有效的标志
    __s8        frpic_hold_time_flag;       //* frpic_hold_time参数是否有效的标志

    //* 音、视频播放共享的相关设置
    __u32       frspeed;                    //* 快进、快退速度
    __u8        frspeed_flag;               //* frspeed参数是否有效的标志
    
    __u8        vid_layer_alpha;
    __u8        alpha_blending_on;

} __dvbc_setting_inf_t;


typedef struct __DVB_CTRL_BLK                   //* 媒体播放中间件句柄
{
    __u32               mid;
    __u8                maintskprio;
    __u8                status;                 //* 媒体播放器当前状态, __cedar_status_t

    __md_src_t          md_src;                 //* media source type;

    __dvbc_setting_inf_t     setting_inf;            //* 媒体播放器的设置信息
    __s32           nOneSegFlag;        //当前端调制模式是isdb时,指示本service是oneseg还是fullseg传输的.其他调制模式如dvb,atsc,本变量无意义.置0即可.  
    __s32           nMafEnable;                 //记录maf是否要打开. 2:leve2的deinterlace, 解码驱动会开始做处理了,de驱动会选择maf模式,带宽占用最大;1:level1的deinterlace,de驱动应该会选择maf_bob模式,带宽占用稍大一些. 0:关闭.默认关闭.
    __pCBK_t        VplyCB_SetSrcScnWinPara;    //设置显示模式nVidShowMode,如果是CEDAR_VID_WINDOW_USERDEF模式,就需要接收app传下来的callback.
    __pCBK_t        CB_ShowDraw;                // subtitle moudle callback function;

    __krnl_event_t          *pFeedBackMsgQ;      /* 向上层控制模块的反馈消息通道, 由maple设置 */
    
    //__krnl_event_t      *CmdQ;                  //* cedarcore接受命令的队列
    MsgQueue            *CedarCmdQ;                 /* msg的数据结构是CedarMsg, mmplayer接受命令的队列，除了接收cedar模块发送的消息之外，现在也接收vply和aply发送的ab播放已到B点的消息           */
    __s8                pic_show_flag;          //* 标志是否需要打开视频图层

    __hdle              pABSSktout;             //* 音频流插座, 自己申请.
    __hdle              pVBSSktout;             //* 视频流插座, 自己申请.
    __hdle              pLBSSktout;             //* 字幕流插座，自己申请.
    
    __krnl_sktfrm_t     *vbs_frm;   //存储socket, request_mem()用到
    __krnl_sktfrm_t     *abs_frm;
    __krnl_sktfrm_t     *sbs_frm;   //

    __ad_codec_fmt_t    adFmt;      //当maple主控调用DVB_CMD_SET_MEDIA_INFO时设置
    __vd_codec_fmt_t    vdFmt;      //同adFmt.
    __subt_codec_fmt_t  subtFmt;    //同adFmt.
    __s8                getAdFmt;   //当maple主控调用DVB_CMD_SET_MEDIA_INFO设置audio信息时会置此标记位.
    __s8                getVdFmt;   //同getAdFmt
    __s8                getSubtFmt; //同getAdFmt

    CedarLock           *pLockStreamPts;   //操作StreamStartPts和AccumulateStartPts的信号量,起互斥作用.
    __s64               StreamStartPts; //一个stream中的起始的pts,取原始值. 一般取video或者audio的一个,优先video.因为video clock是要作为stream clock的,不能出错.
    __s64               AccumulateStartPts; //累积起始时间,每次回头以后，需要修改.该时间优先以video为准,并且需要减去ptsBase, 考虑回头.
    __s8                ptsBaseGet; //表示是否拿到并确定了总的basetime,在update_data()中会赋值为1.
    __s64               ptsBase;    //单位ms.可以为负值.总的basetime,即使回头也不会更改,内部的时间是减去这个basetime的,这样可以从很小的数值开始计数,即使不回头,__display_info_t的__u32也可以坚持20多天.
    __s64               nAVPtsInterval;    //音视频的时间差, 第一个拿到的视频的pts和音频pts的差值.videoPts - audioPts. 为正表示video领先.需要audio加速追video.

	__s8				hasVideo;   //同hasAudio
	__s8				nGetVdFlag; //标记是否拿到了视频数据
	__s64				vdStreamStartPts; //vdPtsBase; 这是stream中的第一个video的原始值,现在只是记录,没有用到.我们认为的ts stream是一段pts不断递增的stream,如果pts回头,就认为是另一个stream. video存在,就以video为准.
    __s64				vdStreamPts;    //也是原始值,记录当前送到后面去的pts,下次来的时候,和它比较,看是否回头.单位ms
	__s64				vdPts;          //记录准备送到后面的vdec的值,减去ptsBase,并且还要考虑回头.如果ptsBase=0,就是从第一个pts开始计时.
	//__s64				firstVdPts;
	__s64               preVdPts[2];    //也是转换pts用的,现在废弃了.
	
	__s8				hasAudio;   //当maple主控调用DVB_CMD_SET_MEDIA_INFO设置audio信息时会置此标记位.表示有audio数据
	__s8				nGetAdFlag; //标记是否拿到了音频数据
	__s64				adStreamStartPts;   //adPtsBase;同vdStreamStartPts
	__s64				adStreamPts;    //也是原始值,记录当前送到后面去的pts,下次来的时候,和它比较,看是否回头, 同vdStreamPts
	__s64				adPts;          //同vdPts
	//__s64				firstAdPts;
	__s64               preAdPts[2];    //同preVdPts[]
	
//#if PLAY_SUBTITLE_STREAM
	__s8                hasSubtitle;    //同hasAudio
	__s8                nGetSubFlag; //标记是否拿到字幕数据
	__s64				subStreamStartPts;   //subPtsBase;同vdStreamStartPts
	__s64				subStreamPts;    //码流原始值, 同vdStreamPts
	__s64				subPts;          //同vdPts
	__s64               preSubPts[2];    //同preVdPts[]
	
//	__s64               subtPtsBase;
//	__s64               subtPts;
//	__s64               firstSubtPts;
//#endif
	__u32				pcrpts;
	__u32				prepcrpts[2];
    //* plug-in module id
    __u32               adec_mid;
    __u32               ardr_mid;
    __u32               aply_mid;
    __mp                *adec;
    __mp                *aply;
    
    __u32               vdec_mid;
    __u32               vply_mid;
    __mp                *vdec;
    __mp                *ardr;
    __mp                *vply;
    
#if PLAY_SUBTITLE_STREAM
    __u32               sdec_mid;
    __u32               sply_mid;
    __mp                *sdec;
    __mp                *sply;
#endif
    
    __u32               avsync_mid;
    __mp                *avsync;

#if DEBUG_TIME
	__u32 				time;
	__u32				newTime;
#endif

#if	DEBUG_SAVE_VIDEO_STREAM
	ES_FILE*            fpVideoStream;  //同fpAudioStream
#endif

#if DEBUG_SAVE_AUDIO_STREAM
	ES_FILE*            fpAudioStream;  //保存dmx输入过来的audio数据到一个文件
#endif

#if DEBUG_SAVE_SUBTITLE_STREAM
	ES_FILE*            fpSubtitleStream;  //保存dmx输入过来的subtitle数据到一个文件
#endif


#if ((PLAY_AUDIO_STREAM == 0) || (PLAY_VIDEO_STREAM == 0))
	__u8*				tmpBuf; //用于欺骗dmx的,如果禁止video,那么就没有vdec的buf给dmx了,那么就要用一个buf顶替.这是用于player内部debug的.
#endif

#if (PLAY_SUBTITLE_STREAM == 0)
	__u8*				tmpSubtitleBuf; //用于欺骗dmx的,如果禁止subtitle,那么就要用一个buf顶替.这是用于player内部debug的.
#endif

#if DRAW_RADIO_BACKGROUND
    __u8*               radio_bg;
    ES_FILE*            disFile;
	__u32               disLayer;
#endif

    __s32           aud_raw_data_mode;   //0:不是raw data, 1:是raw data模式。
    
    //用户的配置脚本信息
    //CedarINI        CustomCedarINI;   //客户定制的配置信息
    CMapleINI       *pMapleINI;

    __u32           nReservedMemSize;   //当告诉vdeclib可以使用的内存数量时,预留的内存数量,默认为600*1024
    __s32           nStopMode;              //CEDAR_STOP_MODE_KEEP_PLUGINS等
    __s32           nFileSwitchVplyMode;    //CEDAR_FILE_SWITCH_VPLY_MODE_GAP等
    __u8						tvMode;								//1:tvmode,0:本地媒体模式，针对TV TS流格式录制成本地媒体播放时的兼容
    __s32           rotate_mode;    //设置视频解码模块解码图像的角度. 0:默认为原图,1:顺时针90度,2:180度,3:270度,4:水平镜像,5:垂直镜像
} __dvbc_t;

extern __dvbc_t *dvbc;
//extern __cedar_vply_t *cedar_vply;
//extern __cedar_aply_t *cedar_aply;
__s32 FileStop_process(__dvbc_t * dvbc);
void uninstall_plugins(__dvbc_t *dvbc);

__s32 play_media(__dvbc_t *dvbc);

extern __s32 CB_CedarMsgQPost(void *arg);   //函数原型__pCBK_t

#endif  //* _DVB_CTRL_H

