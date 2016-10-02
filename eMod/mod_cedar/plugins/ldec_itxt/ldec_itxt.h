
/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_mmp sub-system
*                           ldec_txt decoder module
*
*          (c) Copyright 2009-2010, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : ldec_itxt_i.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2009-6-24
********************************************************************************
*/


#ifndef _LDEC_ITXT_H_
#define _LDEC_ITXT_H_

//#include "mod_cedar_i.h"
//#include "psr_2_ldec.h"
//#include "ldec_2_lrdr.h"
//#include "ldec_lib_cfg.h"
//
//#ifndef EPDK_DEBUG_LEVEL
//#define EPDK_DEBUG_LEVEL            EPDK_DEBUG_LEVEL_LOG_ALL
//#endif

//#define SWITCH_PRINT_LRC            (0)

//#define TV_SYSTEM_PAL       0
//#define TV_SYSTEM_NTSC      1
//
//#define TOP_FIELD_FLAG      0
//#define BOTTOM_FIELD_FLAG   1
//
//#define LDEC_MAX_TXT_BUF_SIZE     (1024 * 128)  //lbsbufsize
//#define LDEC_MAX_TXT_DATA_SIZE    (1024 * 200)
//#define LDEC_MAX_TXT_FRM_SIZE     (1024 * 64)   //一帧的长度,这是以sub为标准,文本类型不会malloc这么大的.
#define LDEC_MAX_TXT_COPY_SIZE    (1024 * 32)     //专门用于和psr传输数据的buffer,目的是一次装完psr要装的数据,有些psr例如mkv对传输字幕的要求就是一次传完.
//#define LDEC_MAX_TXT_ANCI_SIZE    (1024 * 6)     //防止解码时数据回头
//
//#define LDEC_MAX_TXT_FRM_NUM       (4)  //frame的个数
//#define LDEC_MAX_TXT_DATA_NUM      (64) //lbs分块的个数

//typedef enum __SUBTITLE_DEC_RESULT
//{
//    LDEC_ERR_NONE               =  0,       // decode one subtitle successed
//    LDEC_ERR_LBS_UNDERFLOW      =  1,       // subtitle bitstream underflow
//    LDEC_ERR_NO_FRAMEBUFFER     =  2,       // there is no free subtitle buffer for decoder
//    LDEC_ERR_DEC_SUBTITLE       = -1,       // some error has happend when decode subtitle
//    LDEC_ERR_LFMT_ERR           = -2,       // the format of the video is not supported, fatal error
//    LDEC_ERR_
//} __subtitle_dec_result;


//*****************************************************************************//
typedef struct __LDEC_ITXT LdecItxt;
//实成员函数
typedef __s32 (*LdecItxt_Open)(LdecItxt *thiz);
typedef __s32 (*LdecItxt_Close)(LdecItxt *thiz);
typedef struct __LDEC_ITXT
{
    __u32               mid;          //lyric decoder
    __u8                socket_tsk_prio;   //priority of the main task
    __u8                dec_tsk_prio;      //priority of the main task
    __s32               IOOpenFlag;     //表示是否调用了IO_Open.

    __s8                stop_flag;          //stop flag for main task
    //__s8                decFlag;            //解码flag, 表示是否正在解码,起互斥作用的.
    //__s8                useAnciBuf;     //是否使用了ancibuf,来处理回头.
    __hdle              pLSktin;            //socket for input lrc bitstream
    __hdle              pLSktout;           //socket for ouput lrc item, not use now.
    __mp                *hAvs;              //av-sync driver handle

    __s32               nOneSegFlag;        //当前端调制模式是isdb时,指示本service是oneseg还是fullseg传输的.其他调制模式如dvb,atsc,本变量无意义.置0即可.  

    __subtitle_codec_format_t   SubtitleCodecInfo;  //用于接收内置字幕的psr模块传来的字幕信息,实际上,codec_type和data_encode_type是不看的,ts_psr传来的也未必正确,只有subtitle_bs_src有用.codec_type另有接口CEDAR_LDEC_CMD_SET_LBSTYPE专门设置,设置给ldec_itxt->subtitle_type.
    __s32               subtitle_type;      // srt, sub, etc.__cedar_lyric_fmt_t, 字幕文件类型
    __s32               frm_rate;           //video frame rate. be careful : 数值放大了1000倍!, for sub decode
    __s32               width;              //for sub decode
    __s32               height;             //for sub decode
    __s32               screenWidth;    //显示窗口的宽高
    __s32               screenHeight;
//    __s32               tvType;             //sub字幕的解码，需要tvSystemType, TV_SYSTEM_PAL
//    __u8                lastColorCode[4];
//    __u8                lastContrast[4];

    //__ldec_lbs_buf_t    ldecLbsBuf;     //解码类
    //__ldec_manager_t    ldecManager;    //解码类
    //__cedar_subtitle_item_t outputItem;
    //__u8                *anciDataBuf;   //解码类, 解码时处理lbsbuf回头用的.
    __u8                *copyDataBuf;   //专门用于和psr传输数据的buf,ldec_itxt不是直接把lbsbuf给psr的.这是因为最初的时候,设计要求为一次必须传完整个字幕条目.而lbsbuf有回头的问题,可能一次只给少量的buf,不能传完.为兼容这种做法,干脆专门用一个大buf传输.

    __s32               nResetLdecBufFlag;    //重新初始化ldecDrv的所有buffer的标记位.
    __krnl_event_t      *LbsInputSem;            //semaphore for wakeup lyric decode task, lbsbuf_input_sema, 这是等待用的,不是互斥
    __krnl_event_t      *FrameBufSem;            //semaphore for wakeup lyric decode task, frame_release_sema.
    CedarIsubtitleDec   *pCedarIsubtitleDecoder; //内置字幕解码器,一般在start时才malloc出来.
    
    ReqSubtitleFrameMode    nRequestFrameMode;  //ReqSubtitleFrameMode_AppReq等.

    //实成员函数
    LdecItxt_Open   open;
    LdecItxt_Close  close;

} LdecItxt;

extern __s32 LdecItxt_Init(LdecItxt *thiz, __u32 mid);
extern __s32 LdecItxt_Exit(LdecItxt *thiz);
extern LdecItxt* newLdecItxt(__u32 mid);
extern void deleteLdecItxt(LdecItxt* thiz);

#endif

