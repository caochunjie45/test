/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_herb sub-system
*                          (module name, e.g.mpeg4 decoder plug-in) module
*
*          (c) Copyright 2009-2010, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : avi_depack_lib.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2010-8-12
* Description:
    [cedarlib]
********************************************************************************
*/
#ifndef _FILE_DEPACK_LIB_H_
#define _FILE_DEPACK_LIB_H_

//返回值定义
#ifndef EPDK_OK
#define EPDK_OK (0) /* 代表成功                                                     */
#endif
#ifndef EPDK_FAIL
#define EPDK_FAIL (-1)  /* 代表失败                                                     */
#endif
#ifndef EPDK_TRUE
#define EPDK_TRUE (1)   /* 真                                                           */
#endif
#ifndef EPDK_FALSE
#define EPDK_FALSE (0)  /* 假                                                           */
#endif

//define when should file parser delay some time when ff/rr
#define FFRR_DELAY_PIC_FRM_CNT      (10)
#define FFRR_DELAY_AUD_FRM_CNT      (50)
/*******************************************************************************
    跳播类型: 视频文件的跳播方式和音频文件的跳播方式完全不同。因此分为两种分别
    处理。
*******************************************************************************/
enum JUMP_PLAY_TYPE{
    NOT_SUPPORT_JUMP_PLAY   = -1,
    JUMP_PLAY_TYPE_VIDEO    = 0,
    JUMP_PLAY_TYPE_AUDIO    = 1,
};

//定义文件解析库的返回值
typedef enum __FILE_DEPACK_RESULT
{
    FILE_DEPACK_ERR_NONE       = 0,        //no error

    FILE_DEPACK_ERR_READ_END        = 1,        //read file or idx table end.以读不到数据作为标识,也可以表示一个chunk的数据都读完了。
    FILE_DEPACK_ERR_BUF_LACK        = 2,        //BUFFER is not enough to contain data.
    FILE_DEPACK_ERR_GET_NONE        = 3,        //read file success for (>0) bytes, but get nothing.
    FILE_DEPACK_ERR_ERROR_IGNORE    = 4,        //some exception happens, but can continue process.
    FILE_DEPACK_ERR_PAUSE_GET_DATA  = 5,        //pause get data from file, when in FF,RR mode,已经不用了。
    FILE_DEPACK_ERR_VIDEO_BUF_FULL = 6,     //表示PSR_***需要更多的video buffer去装填数据，目前仅PSR_TS使用
    FILE_DEPACK_ERR_AUDIO_BUF_FULL = 7,     //表示PSR_***需要更多的audio buffer去装填数据，目前仅PSR_TS使用

    FILE_DEPACK_ERR_PARA_ERR        = -1,       //some parameter error
    FILE_DEPACK_ERR_OPEN_FILE_FAIL  = -2,       //open media file failed
    FILE_DEPACK_ERR_READ_FILE_FAIL  = -3,       //read media file data failed
    FILE_DEPACK_ERR_FILE_FMT_ERR    = -4,       //media file format error
    FILE_DEPACK_ERR_NO_AV           = -5,       //neither audio nor video supported in media file
    FILE_DEPACK_ERR_END_OF_MOVI     = -6,       //movi data end
    FILE_DEPACK_ERR_REQMEM_FAIL     = -7,       //request memory failed
    FILE_DEPACK_ERR_EXCEPTION       = -8,       //some impossible case happens.
    FILE_DEPACK_ERR_

} __file_depack_result_t;

//定义FileDepackIoctrl()的命令集,各具体的depack lib可以在此基础上扩展
typedef enum __FILE_DEPACK_COMMAND
{
    FILE_DEPACK_CMD_CHKSPT_JUMPTYPE = 0,     /* 检查是否能够支持跳播以及跳播类型,ret= enum JUMP_PLAY_TYPE,    */
    FILE_DEPACK_CMD_GET_VIDEO_FORMAT,        /* ret = VIDEO_XVID等   */
    FILE_DEPACK_CMD_DISCARD_DATA,            /* 禁止传送a/v数据，aux = BITSTREAM_TYPE_AUDIO等, buffer = 1:禁止；buffer = 0: 传送 */
    FILE_DEPACK_CMD_AUD_RESYNC,              /* 下次拿到audio 数据时，要set avsync模块 */
    FILE_DEPACK_CMD_GET_CUR_ABSSUFFIX,       /* 得到当前使用的音频流数组的下标号，如果没有，也返回0 */
    FILE_DEPACK_CMD_SET_ABSSUFFIX,           /* 设置新的音轨的下标号, 在播放以后设置音轨号，即GetNextChunkInfo()第一次被调用以后 */
    FILE_DEPACK_CMD_CHECK_NEW_ABS_UPDATED,   /* 检查新换的音轨是否被接受了*/
    FILE_DEPACK_CMD_GET_FILE_SIZE,          /* 得到文件大小，pbuffer = *s64, ret = FILE_PARSER_RETURN_OK. 这里的文件指的是psr所看到的文件。例如，对于ODF加密文件，depack_lib返回的是其内部封装的文件大小，不是ODF文件本身的大小*/
    FILE_DEPACK_CMD_NOTIFY_ABORT,           /*  通知psr模块准备, ret = EPDK_OK/EPDK_FAIL*/
    FILE_DEPACK_CMD_SET_CHUNK_BUF_SIZE,     /* 设置存储数据的chunk的大小, aux = size*/
    FILE_DEPACK_CMD_SET_INITIAL_ABSSUFFIX,     /* 与FILE_DEPACK_CMD_SET_ABSSUFFIX不同之处在于,在GetNextChunkInfo()调用之前，设置音轨号，以前默认是0 */
    

    //FILE_DEPACK_CMD_PROCESS_STUFDATA = 0x10,        /* 废弃, 处理stuf data，算出持续时间，设置audio时间轴 */
    FILE_DEPACK_CMD_,
} __file_depack_command_t;

typedef enum {
    CEDARLIB_AVS_VIDEO = 0, //AVS_VIDEO
    CEDARLIB_AVS_AUDIO = 1, //AVS_AUDIO
    CEDARLIB_AVS_AUDVID = 2,
}__cedarlib_clock_type_t;
typedef enum
{                                   // define parameter type for get and set time
    CEDARLIB_AVS_TIME_TOTAL=0,           // total time, base + offset
    CEDARLIB_AVS_TIME_BASE,              // base time, video time start value
    CEDARLIB_AVS_TIME_OFFSET,            // time offset, play time offset
    CEDARLIB_AVS_TIME_
} __cedarlib_time_type_t;   //__avs_time_type_t

typedef enum
{                                   
    CEDARLIB_AVS_ENABLE=1,          //AVS_ENABLE
    CEDARLIB_AVS_DISABLE = 0,       //AVS_DISABLE
} __cedarlib_avs_enable_t;

#define CEDARLIB_TAG_INF_SIZE (256)
typedef struct __CEDARLIB_TAG_INF
{
    unsigned char    tag_magic[4];            /* 媒体文件标签的标志符'A'/'V','T','A','G' */

    int   file_offset;                /* 媒体文件的断点在文件内的偏移         */
    int   time_offset;                /* 媒体文件的断点的时间偏移             */
    unsigned char    decinf[CEDARLIB_TAG_INF_SIZE]; /* 解码器的现场恢复信息                 */
}__cedarlib_tag_inf_t; //用于接口GetMediaTag()等的ptag.等同于__cedar_tag_inf_t
typedef struct __JUMP_CONFIG
{
    int       ms;             //the jumped time point in ms
    short       flag;           //
    short       dir;            //search direction, >0 search forward, <=0 search backward
    short       mode;           //0--goto next key frame, 1--goto last key frame，
                                //这里的last是相对于查找方向而言的，在查找方向上
                                //的反方向的一个key frame,如果是search forward，
                                //就是回退的一个key frame，如果是search backward，就是向前进的一个key frame

} __jump_cfg_t;
/*******************************************************************************
Function name: __audio_stream_info_t
Description:
    该数据结构仅仅用于函数__modify_aud_strhf()及其用到的函数.
    现在只用到比特率。以后根据情况再增加其他变量。
Members: 
    如果是mp3 cbr:
    nBlockAlign: == strh->dwSampleSize

    如果是mp3 vbr:
    nBlockAlign: 本音频流的最大的mp3_frame的字节数(单位:字节).一个avi chunk可以
                包含多个mp3_frame.
*******************************************************************************/
typedef struct
{
    //define audio codec type
    //__u32       codec_type;         // audio codec type
    //__u32       codec_type_sub;     // audio codec sub type, the highest bit mark endian type
                                    // bit0-bit15, sub type of the audio codec
                                    // bit16,      endian flag: 0, little ending; 1, big ending;
                                    // other bits, reserved
    //define audio bitstream format
    //__u16       channels;           // channel count
    //__u16       bits_per_sample;    // bits per sample
    //__u32       sample_per_frame;       //每个audio frame含有的sample数,不管是cbr还是vbr，每个audio frame含有的sample数都是固定的
    //__u32       sample_per_second;  // sample count per second，就是采样率
    unsigned int       avg_bit_rate;       // average bit rate，比特率
} __audio_stream_info_t;

typedef struct _FILE_DEPACK_ FILE_DEPACK;
typedef struct _FILE_DEPACK_ //
{
    //析构函数
    //void   (*deleteFILE_DEPACK)(FILE_DEPACK *pFileDepack);
    //定义了19个成员函数
    int   (*OpenMediaFile)(FILE_DEPACK *pFileDepack, int nFmtType, char *pFileName);    //ret = __file_depack_result_t
    int   (*CloseMediaFile)(FILE_DEPACK *pFileDepack);
    int   (*GetAbsFormat)(FILE_DEPACK *pFileDepack);
    int   (*GetVbsFormat)(FILE_DEPACK *pFileDepack);
    int   (*GetLbsFormat)(FILE_DEPACK *pFileDepack);
    int   (*GetAbsInfForADec)(FILE_DEPACK *pFileDepack, void *pAbsInf, unsigned int *pAbsInfLen);//用法同GetVbsInfForVDec
    int   (*GetVbsInfForVDec)(FILE_DEPACK *pFileDepack, void *pVbsInf, unsigned int *pVbsInfLen);//现在pVbsInf = __vdec_mpeg4_vbs_inf_t**, 指针的指针, pVbsInfLen是实际长度，一般没有用到
    int   (*GetLbsInfForLDec)(FILE_DEPACK *pFileDepack, void *pLbsInf, unsigned int *pLbsInfLen);//pLbsInf = __psr_2_ldec_msg_t*,
    int   (*SetProcMode)(FILE_DEPACK *pFileDepack, unsigned int  nMode, unsigned int uParam);
    int   (*GetNextChunkInfo)(FILE_DEPACK *pFileDepack, unsigned int *pChunkType);
    int   (*GetChunkData)(FILE_DEPACK *pFileDepack, unsigned char *pBuf, unsigned int nBufSize, unsigned int *pDataSize,unsigned char *pFrmInf, unsigned int *pFrmInfType);//需要设置一些数据结构，__psr_2_vdec_msg_t，可以看做是解析库自己定义的数据结构
    int   (*SkipChunkData)(FILE_DEPACK *pFileDepack);
    int   (*CheckIfSupportFFRR)(FILE_DEPACK *pFileDepack);
    int   (*SetFFRRSpeed)(FILE_DEPACK *pFileDepack, unsigned int nSpeed);
    int   (*SetFFRRPicShowTime)(FILE_DEPACK *pFileDepack, unsigned int nTime);
    int   (*GetMediaTag)(FILE_DEPACK *pFileDepack, void *ptag);
    int   (*SetMediaTag)(FILE_DEPACK *pFileDepack, void *ptag);
    int   (*GetMediaInfo)(FILE_DEPACK *pFileDepack, __media_file_info_internal_t *pMediaInfo);
    int   (*FileDepackIoctrl)(FILE_DEPACK *pFileDepack,int cmd,int aux, void *pbuffer);  // __file_depack_command_t

    // 回调函数,nMode = CEDARLIB_AVS_VIDEO等, nTimeType = CEDARLIB_AVS_TIME_TOTAL等, 
    // psr_lib会设置它所发现的video pts和audio pts. SetAudVidTime(pts(ms),CEDARLIB_AVS_VIDEO, CEDARLIB_AVS_TIME_BASE)
//    int   (*CbSetAudVidTime)(int nTime,int nMode, int nTimeType);   
//    int   (*CbGetAudVidTime)(int nMode, int nTimeType);
//    int   (*CbSetVideoClkSpeed)(unsigned int nSpeed);    //nSpeed的含义参考SetVideoClkSpeed(1);
//    int   (*CbSwitchAvSync)(int mode);    //设置avsync模块的do_sync_flag位，表示是否要sunc; mode:AVS_DISABLE等, 函数原型SwitchAvSync()
//    int   (*CbGetAudioDataInfo)(int codec_type, unsigned char *pbuf, unsigned int length, __audio_stream_info_t *paudstrminfo);
//    int   (*CbBuildAbsPacketHdr)(__audio_codec_format_t *pAbsInf, int uPacketLen, unsigned char *pBuf, int *pHdrLen);
//    int   (*CbUpdateAbsPacketHdr)(__audio_codec_format_t *pAbsInf, int uPacketLen, unsigned char *pBuf, int uHdrLen);
} FILE_DEPACK;

extern FILE_DEPACK* newFILE_DEPACKChild(void);  //创建实例, __cedarlib_file_fmt_t, CEDARLIB_FILE_FMT_AVI, (__cedar_media_file_fmt_t, CEDAR_MEDIA_FILE_FMT_AVI)等
extern void deleteFILE_DEPACKChild(FILE_DEPACK* thiz);

//需要外部实现的函数
extern int SysCb_SetAudVidTime(int nTime, int nMode, int nTimeType);
extern int SysCb_GetAudVidTime(int nMode, int nTimeType);
extern int SysCb_SetVideoClkSpeed(unsigned int nSpeed);  //nSpeed的含义参考SetVideoClkSpeed(1);
extern int SysCb_SwitchAvSync(int mode);        //mode: AVS_DISABLE, //设置avsync模块的do_sync_flag位，表示是否要sunc; mode:AVS_DISABLE等, 函数原型SwitchAvSync()
extern int SysCb_SetAVEndFlag(__s32 nType);    //__cedarlib_clock_type_t
extern int SysCb_GetAudioDataInfo(int codec_type, unsigned char *pbuf, unsigned int length, __audio_stream_info_t *paudstrminfo);
extern int Cedarlib_BuildAbsPacketHdr(__audio_codec_format_t *pAbsInf, int uPacketLen, unsigned char *pBuf, int *pHdrLen);
extern int Cedarlib_UpdateAbsPacketHdr(__audio_codec_format_t *pAbsInf, int uPacketLen, unsigned char *pBuf, int uHdrLen);

#endif  /* _FILE_DEPACK_LIB_H_ */

