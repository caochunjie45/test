/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009,Kevin.z China
*                                              All Rights Reserved
*
* File   : psr.h
* Version: V1.0
* By     : Kevin.Z
*********************************************************************************************************
*/
#ifndef _PSR_H_
#define _PSR_H_
#include "file_depack_lib.h"
//define if need print id3 information
#define PRINT_AUDIO_FILE_ID3_INF    (0)
//define when should file parser delay some time when ff/rr
//#define FFRR_DELAY_PIC_FRM_CNT      (10)
//#define FFRR_DELAY_AUD_FRM_CNT      (50)


///*******************************************************************************
//    跳播类型: 视频文件的跳播方式和音频文件的跳播方式完全不同。因此分为两种分别
//    处理。
//*******************************************************************************/
//enum JUMP_PLAY_TYPE{
//    NOT_SUPPORT_JUMP_PLAY   = -1,
//    JUMP_PLAY_TYPE_VIDEO    = 0,
//    JUMP_PLAY_TYPE_AUDIO    = 1,
//};
typedef enum __FILE_PARSER_COMMAND
{
    CEDAR_PSR_CMD_START=0x00,           /* 启动parser                           */
    CEDAR_PSR_CMD_STOP,                 /* 停止parser                           */
    CEDAR_PSR_CMD_PAUS,                 /* 换音轨的时候会用到,仅psr_video有效                      */
    CEDAR_PSR_CMD_CONT,                 /* 换音轨的时候会用到,仅psr_video有效                         */
    CEDAR_PSR_CMD_FF,                   /* 设置快进模式                         */
    CEDAR_PSR_CMD_RR,                   /* 设置快退模式                         */
    CEDAR_PSR_CMD_CLEAR_FFRR,           /* 清除快进快退模式                     */
    CEDAR_PSR_CMD_GET_STATUS,           /* 获取file parser当前状态, aux = 1必须返回稳定值, aux =0 立即返回              */
    CEDAR_PSR_CMD_JUMP,                 /* 设置跳播模式, aux = des_tm           */
    CEDAR_PSR_CMD_CLEAR_JUMP,           /* 清除跳播状态                     */
    CEDAR_PSR_CMD_NOTIFY_ABORT,         /* 通知psr模块准备, ret = EPDK_OK/EPDK_FAIL */

    CEDAR_PSR_CMD_PREPARE_FF,               //准备FF,rr等, play->pause
    CEDAR_PSR_CMD_PREPARE_RR,               // play->pause
    CEDAR_PSR_CMD_PREPARE_CLEAR_FFRR,       // ffrr->pause

    CEDAR_PSR_CMD_OPEN = 0x20,          //分配资源
    CEDAR_PSR_CMD_CLOSE,                //释放资源
    CEDAR_PSR_CMD_OPEN_FILE,     /* preview也使用.   输入媒体文件, aux = CEDAR_MEDIA_FILE_FMT_MP3等                         */
    CEDAR_PSR_CMD_SET_CEDARINI,         //把用户的定制信息传给psr模块.可以接受指针,并使用.因为该块内存由cedar主控分配,运行期间绝不会释放. pbuffer = CedarINI*
    CEDAR_PSR_CMD_SET_AVS_HDL,          /* 设置AVS驱动句柄                      */
    CEDAR_PSR_CMD_CHKSPT,               /* 检查是否支持媒体文件                 */
    CEDAR_PSR_CMD_CHKSPT_FFRR,          /* 检查是否能够支持快进/快退            */
    CEDAR_PSR_CMD_CHKSPT_JUMPTYPE,      /* 检查是否能够支持跳播以及跳播类型,ret= enum JUMP_PLAY_TYPE,    */
    CEDAR_PSR_CMD_GET_ABSSKTOUT,        /* 获取输出音频流插槽                   */
    CEDAR_PSR_CMD_GET_VBSSKTOUT,        /* 获取输出视频流插槽                   */
    CEDAR_PSR_CMD_GET_LBSSKTOUT,        /* 获取输出字幕流插槽                   */
    CEDAR_PSR_CMD_GET_CURSERVICE_ONESEGFLAG, //这是向tspsr查询它所正在播放的service的调制方式.其他psr可以不实现.

    
    CEDAR_PSR_CMD_GET_ABSTYPE = 0x40,          /* 获取媒体文件音频流类型(当前正在播放的音频流)*/
    CEDAR_PSR_CMD_GET_VBSTYPE,          /* 获取媒体文件视频流类型               */
    CEDAR_PSR_CMD_GET_LBSTYPE,          /* 获取媒体文件字幕流类型,只返回内置字幕的类型. 本意是调用GetLbsFormat()获取当前播放的字幕格式.现在字幕管理统一给subtitlemanage.所以psr可以不管当前播放的字幕格式的.现在已经废弃不用了,转为使用CEDAR_PSR_CMD_GET_LBSTYPE2接口               */
    
    CEDAR_PSR_CMD_GET_ABSCNT,           /* 需过滤, 获取媒体文件的音频流总数             */
    CEDAR_PSR_CMD_GET_AUDSTREAM_PROFILE,/* 需过滤, 获取所有音频流的说明信息: pbuffer = __psr_audstream_profile_t, app malloc it. return = EPDK_OK or EPDK_FAIL*/
    CEDAR_PSR_CMD_GET_CUR_ABSSUFFIX,    /* 需过滤, 获取媒体文件音频流的下标号, ret = 下标号*/
    CEDAR_PSR_CMD_GET_ABSTYPE2,         /* 需过滤, 获取媒体文件音频流类型，选定下标号,aux = 下标号, ret = __cedar_audio_fmt_t*/
    CEDAR_PSR_CMD_SET_ABSSUFFIX,        /* 需过滤, 设置新的audio stream的数组下标号, aux = 下标号,ret = EPDK_OK/EPDK_FAIL     */
    CEDAR_PSR_CMD_AUD_RESYNC,           /* audio重同步，即告知PSR模块用新得到的audio chunk的PTS设置audio时间轴, 以AV同步.ret = EPDK_OK/EPDK_FAIL*/
    CEDAR_PSR_CMD_NOTIFY_CHANGE_AUDIO_STREAM,   /* 要求PSR模块更换音轨，PSR的动作是:向ADEC模块传送一次ABSDECINF，通知PSR子模块重设AUD_PTS，开始传输AUDIO数据*/
    
    CEDAR_PSR_CMD_GET_SUBTITLE_PROFILE, /* 可能要过滤, 暂时不做.只负责返回内嵌字幕的信息 */
    CEDAR_PSR_CMD_SELECT_SUBTITLE,      /*可能要过滤, 暂时不做. 选择一个字幕,将只处理内置字幕*/
    CEDAR_PSR_CMD_GET_SUBTITLE,         /*可能要过滤, 暂时不做. 返回正在播放的subtitle的数组下标号,if not find, return -1, 将只针对内置字幕   */
    CEDAR_PSR_CMD_GET_LBSTYPE2,         /* 可能要过滤, 获取媒体文件的字幕文件类型，选定下标号,aux = 下标号, ret = LYRIC_ITXT等,只针对内置字幕 */
    CEDAR_PSR_CMD_ENABLE_EXTERN_SUBTITLE,   //已经没有意义了,可以废弃.激活外挂字幕检测, AUX = 1:检测外挂字幕; aux =0:屏蔽外挂字幕
    CEDAR_PSR_CMD_SKIP_SUBTITLE,          //是否skip内置字幕. aux=1:skip; aux=0:传输.
    CEDAR_PSR_CMD_NOTIFY_CHANGE_SUBTITLE_STREAM,   //通知psr换字幕.psr会和ldec_itxt做一次握手.然后传输数据.
    
    CEDAR_PSR_CMD_DISCARD_DATA,         /* 是否传送取到的数据，aux = enum BITSTREAM_TYPE, pbuffer = 1(丢弃), 0(传送)
                                            在目前的体系下，如果禁止audio的传送，理论上会导致AV不同步，但一般调用这个IO接口时，是在起始播放前，
                                            ,起始播放之前就禁止，意味着永远不会再打开了.所以psr永远拿不到audio数据，永远do_sync_flag不会被置为1，所以也没有问题.
                                            换音轨也会调用,会重设音频时间.*/
    CEDAR_PSR_CMD_QUERY_DATAEND,           /* 查询PSR模块，数据是否已取完, ret, 1:取完，0:没取完*/
    CEDAR_PSR_CMD_QUERY_FORBID_JUMP_IN_CHGAUD,      /*查询PSR子模块是否禁止在换音轨的过程中加入跳播后处理, ret = EPDK_OK表示禁止，EPDK_FAIL表示不禁止 */
    

    CEDAR_PSR_CMD_GET_TOTAL_TIME=0x60,  /* 获取媒体文件播放总时间               */
    CEDAR_PSR_CMD_GET_TAG,              /* 获取媒体播放标签信息, pbuffer = __cedar_tag_inf_t*, 调用者分配内存 */
    CEDAR_PSR_CMD_SET_TAG,              /* 设置媒体播放标签信息                 */
    CEDAR_PSR_CMD_SET_FRSPEED,          /* 设置快进、快退速度                   */
    CEDAR_PSR_CMD_GET_FRSPEED,          /* 获取快进、快退速度                   */
    CEDAR_PSR_CMD_SET_FRPIC_TIME,       /* 检查是否能够支持快进/快退            */
    CEDAR_PSR_CMD_GET_VIDBPS,           /* 获取视频流的码率                     */
    CEDAR_PSR_CMD_GET_VIDFRMRATE,       /* 获取视频帧率                         */
    CEDAR_PSR_CMD_GET_VIDFRMSIZE,       /* 获取视频图像大小                     */

    //为缩略图新增的接口.
    CEDAR_PSR_CMD_GET_FILE_SIZE=0x80,    //pbuffer = *__s64, 
    CEDAR_PSR_CMD_GET_MEDIAINFO,   //pbuffer = __media_file_info_internal_t*, caller 分配内存
    CEDAR_PSR_CMD_SET_PROC_MODE,    //aux = enum MEDIA_STATUS, 实际上就是调用SetProcMode(), aux = MEDIA_STATUS_PLAY等, 当aux =MEDIA_STATUS_JUMP时, pbuffer = __jump_cfg_t*, 用于跳播时的设置
    CEDAR_PSR_CMD_GET_VBSINF,       //pbuffer = __vdec_mpeg4_vbs_inf_t**, 
    CEDAR_PSR_CMD_CLOSE_MEDIA_FILE, //
    CEDAR_PSR_CMD_GET_NEXT_CHUNK_INFO,  //GetNextChunkInfo(), pbuffer = __u32*, (chunk_type)
    CEDAR_PSR_CMD_GET_CHUNK_DATA,       //GetChunkData(), 返回值和GetChunkData()是一样的
    CEDAR_PSR_CMD_SKIP_BITSTREAM,       //SkipChunkData()
    CEDAR_PSR_CMD_SET_VDRV, //tspsr会在getnextchunkinfo()中直接向vdrv要数据
    CEDAR_PSR_CMD_SET_PREVIEW_MODE, //告诉psr模块当前为取预览图的模式. 为应付ts_psr而这样做. aux = 1:预览图模式; aux=0:正常播放模式

    //支持3D播放
    CEDAR_PSR_CMD_GET_VIDEO_DISPLAY_MODE_INFO = 0x100,
    CEDAR_PSR_CMD_SET_VIDEO_DISPLAY_MODE_INFO,

    
    CEDAR_PSR_CMD_SET_FOP = 0x5000,     // preview也使用.   设置用户自定义文件操作集合
    CEDAR_PSR_CMD_

} __file_parser_command_t;

typedef enum __FILE_PARSER_INTERNAL_COMMAND
{
    CEDAR_PSR_INTERNAL_CMD_STOP_PSR_DEV = 0x01,                 /* 关闭psr_dev */
    CEDAR_PSR_INTERNAL_CMD_CHKSPT_JUMPTYPE = 0x27,      /* 检查是否能够支持跳播以及跳播类型,ret= enum JUMP_PLAY_TYPE,    */
    CEDAR_PSR_INTERNAL_CMD_GET_CURSERVICE_ONESEGFLAG = 0x2B,   //这是向tspsr查询它所正在播放的service的调制方式.其他psr可以不实现.
    CEDAR_PSR_INTERNAL_CMD_GET_CUR_ABSSUFFIX = 0x2D,    /* 获取媒体文件音频流的下标号, ret = 下标号*/
    CEDAR_PSR_INTERNAL_CMD_SET_INITIAL_ABSSUFFIX = 0x2E,    //在正式解析前,即调用GetNextChunkInfo()前设置初始音频流的下标号
    CEDAR_PSR_INTERNAL_CMD_SET_ABSSUFFIX = 0x2F,        /* 设置新的audio stream的数组下标号, aux = 下标号,ret = EPDK_OK/EPDK_FAIL     */
    CEDAR_PSR_INTERNAL_CMD_AUD_RESYNC = 0x30,           /* audio重同步，即告知PSR模块用新得到的audio chunk的PTS设置audio时间轴, 以AV同步.ret = EPDK_OK/EPDK_FAIL*/
    CEDAR_PSR_INTERNAL_CMD_DISCARD_DATA = 0x31,         /* 是否传送取到的数据，aux = enum BITSTREAM_TYPE, pbuffer = 1(丢弃), 0(传送), ret = EPDK_OK设置成功*/
    CEDAR_PSR_INTERNAL_CMD_QUERY_FORBID_JUMP_IN_CHGAUD, /* 参考CEDAR_PSR_CMD_QUERY_FORBID_JUMP_IN_CHGAUD的说明, EPDK_OK代表禁止换音轨后跳播 */
    /* 从mixed chunk中获取音视频数据, pbuffer = __mix_buf_msg_t*, ret = __file_parser_return_val_t 
    ret的返回值只有以下4个，其余的返回值均作为fatal error
    FILE_PARSER_READ_END(1):所有数据均读到buffer中了.
    FILE_PARSER_ERR_VIDEO_BUF_FULL:video buffer已用完，需要新的video buffer
    FILE_PARSER_ERR_AUDIO_BUF_FULL:audio buffer已用完，需要新的audio buffer
    -1: fatal error*/
    CEDAR_PSR_INTERNAL_CMD_GET_MIX_CHUNK_DATA,          
    CEDAR_PSR_INTERNAL_CMD_SKIP_REMANENT_CHUNK_DATA, //和SkipChunkData()不同之处在于，调用这个接口可以把读了部分内容的chunk skip掉.SkipChunkData()最初定义的时候默认skip一个完整的chunk, ret = EPDK_OK/EPDK_FAIL.
    CEDAR_PSR_INTERNAL_CMD_NOTIFY_ABORT,        /*  通知psr模块准备, ret = EPDK_OK/EPDK_FAIL*/

    CEDAR_PSR_INTERNAL_CMD_SELECT_SUBTITLE = 0x50,  /* 选择subtitle的index */
    CEDAR_PSR_INTERNAL_CMD_GET_CUR_SUBTITLE,        /* 得到subtitle的index */
    //CEDAR_PSR_INTERNAL_CMD_CHKSPT_STREAMCODEC,           /* 查看文件中的音频视频流的解码支持情况, pbuffer == __file_codec_support_t,
                                                            //只要子模块实现了这个IO接口，一律返回EPDK_OK，否则EPDK_FAIL*/
    
    CEDAR_PSR_INTERNAL_CMD_

} __file_parser_internal_command_t;

typedef struct
{
    //psr_high -> psr_low的成员变量
    __u8*   pVidbuf;       //输入的video buf
    __u32   nVidBufSize;   //bufSize
    
    __u8*   pAudbuf;       //输入的audio buf
    __u32   nAudBufSize;   //bufSize

    //psr_low -> psr_high的成员变量
    __u32   nVidMsgValid;//LOW_PSR表示的video msg是否有效的标志
    __u32   nVidDataLen;//装填的video数据的长度
    __u32   nVidMsgType;
    void*   pVidMsg;    //已分配好空间，直接使用

    __u32   nAudMsgValid;
    __u32   nAudDataLen;//装填的audio数据的长度
    __u32   nAudMsgType;
    void*   pAudMsg;    //已分配好空间，直接使用
} __mix_buf_msg_t;

typedef struct __AUDIO_FILE_BS_INF
{
    __u32       ulSampleRate;       // 采样率 sample rate
    __u32       ulBitRate;          // 比特率 bit rate
    __u32       ulChannels;         // 声道数 channel
    __u32       ulDuration;         // 播放时间 duration
    __u32       ulBitsSample;       // 采样点位数 sample 8/16/24/32

} __audio_file_bs_inf_t;

typedef struct __AUDIO_FILE_ID3_INF
{
    __s32       ulAudio_name_sz;    //音频格式说明
    char*       ulAudio_name;       //mp3 /RealAudio Cook.sipo. / aac-lc....

    __s32       ulGenre_sz;         //流派
    char*       ulGenre;            //pop soft...

    __s32       ultitle_sz;         //歌曲名
    char*       ultitle;

    __s32       ulauthor_sz;        //演唱者
    char*       ulauthor;

} __audio_file_id3_inf_t;


//typedef struct __JUMP_CONFIG
//{
//    __s32       ms;             //the jumped time point in ms
//    __s16       flag;           //
//    __s16       dir;            //search direction, >0 search forward, <=0 search backward
//    __s16       mode;           //0--goto next key frame, 1--goto last key frame，
                                //这里的last是相对于查找方向而言的，在查找方向上
                                //的反方向的一个key frame,如果是search forward，
                                //就是回退的一个key frame，如果是search backward，就是向前进的一个key frame
//
//} __jump_cfg_t;


//#define MAX_VIDEO_STREAM_IN_FILE    1
//#define MAX_AUDIO_STREAM_IN_FILE    16
//typedef struct {
//    __s32    VideoNum;
//    __s32   VCodecSupport[MAX_VIDEO_STREAM_IN_FILE];//1, support, 0:not support
//    __s32    AudioNum;
//    __s32   ACodecSupport[MAX_AUDIO_STREAM_IN_FILE];
//}__file_codec_support_t;

typedef struct{
    __u8        nAudStrmNum;    //audio number.<=MAX_AUDIO_STREAM_NUM.
    __audio_bs_info_t        AudStrmList[CEDARLIB_MAX_AUDIO_STREAM_NUM];
} __psr_audstream_profile_t;

typedef struct{
    __u8        nSubtitleStrmNum;    //lyric number.<=MAX_SUBTITLE_STREAM_NUM.
    __subtitle_stream_info_t        SubtitleStreamList[MAX_SUBTITLE_STREAM_NUM];
} __psr_subtitle_profile_t;


typedef struct SUFFIX_MAP
{
    __s32   stream_num;         //指数组aud_stream_suffix_map_array的有效个数   
    __s32   stream_suffix_map_array[CEDARLIB_MAX_AUDIO_STREAM_NUM]; //音频流下标号的映射，数组下标就是对上层模块的音频流下标，数组值就是库的音频流下标。映射到文件解析库
}__suffix_map_t;
typedef struct tag_PsrVideoMediaFileInfoInternal
{
    __media_file_info_internal_t    MediaInfo;
    __suffix_map_t  audio_stream_suffix_map;    //过滤音轨用
}PsrVideoMediaFileInfoInternal;


typedef enum __FILE_PARSER_RETURN_VAL
{
    FILE_PARSER_RETURN_OK          = FILE_DEPACK_ERR_NONE              ,                                                                           

    FILE_PARSER_READ_END           = FILE_DEPACK_ERR_READ_END          ,     //read file or idx table end.以读不到数据作为标识,也可以表示一个chunk的数据都读完了。
    FILE_PARSER_BUF_LACK           = FILE_DEPACK_ERR_BUF_LACK          ,     //BUFFER is not enough to contain data.                                              
    FILE_PARSER_GET_NONE           = FILE_DEPACK_ERR_GET_NONE          ,     //read file success for (>0) bytes, but get nothing.                                 
    FILE_PARSER_ERROR_IGNORE       = FILE_DEPACK_ERR_ERROR_IGNORE      ,     //some exception happens, but can continue process.                                  
    FILE_PARSER_PAUSE_GET_DATA     = FILE_DEPACK_ERR_PAUSE_GET_DATA    ,     //pause get data from file, when in FF,RR mode,已经不用了。                          
    FILE_PARSER_ERR_VIDEO_BUF_FULL = FILE_DEPACK_ERR_VIDEO_BUF_FULL,     //表示PSR_***需要更多的video buffer去装填数据，目前仅PSR_TS使用                      
    FILE_PARSER_ERR_AUDIO_BUF_FULL = FILE_DEPACK_ERR_AUDIO_BUF_FULL,     //表示PSR_***需要更多的audio buffer去装填数据，目前仅PSR_TS使用                      

    FILE_PARSER_PARA_ERR           = FILE_DEPACK_ERR_PARA_ERR          ,       //some parameter error                                                               
    FILE_PARSER_OPEN_FILE_FAIL     = FILE_DEPACK_ERR_OPEN_FILE_FAIL    ,       //open media file failed                                                             
    FILE_PARSER_READ_FILE_FAIL     = FILE_DEPACK_ERR_READ_FILE_FAIL    ,       //read media file data failed                                                        
    FILE_PARSER_FILE_FMT_ERR       = FILE_DEPACK_ERR_FILE_FMT_ERR      ,       //media file format error                                                            
    FILE_PARSER_NO_AV              = FILE_DEPACK_ERR_NO_AV             ,       //neither audio nor video supported in media file                                    
    FILE_PARSER_END_OF_MOVI        = FILE_DEPACK_ERR_END_OF_MOVI       ,       //movi data end                                                                      
    FILE_PARSER_REQMEM_FAIL        = FILE_DEPACK_ERR_REQMEM_FAIL       ,       //request memory failed                                                              
    FILE_PARSER_EXCEPTION          = FILE_DEPACK_ERR_EXCEPTION         ,       //some impossible case happens.                                                      
    FILE_PARSER_                   = FILE_DEPACK_ERR_                  ,                                                                                 
} __file_parser_return_val_t;
#endif /*_PSR_H_*/

