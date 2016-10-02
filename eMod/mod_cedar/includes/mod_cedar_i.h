/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, kevin China
*                                              All Rights Reserved
*
* File   : mod_cedar_i.h
* Version: V1.0
* By     : steven.zgj
* Modify : kevin.z      2008-5-30 15:06
*   作为cedar插件群的共用头文件(_s.h)
*********************************************************************************************************
*/
#ifndef _MOD_CEDAR_I_H_
#define _MOD_CEDAR_I_H_


#include "epdk.h"

#include "media_type.h"
#include "psr_2_vdec.h"
#include "abs_header.h"

#include "psr.h"        //parser socket
#include "adec.h"       //audio decoder socket
#include "vdec.h"       //video decoder socket
#include "ardr.h"       //audio renderer socket
#include "ldec.h"       //lyric decoder socket
#include "drv_adec.h"   //audio decode driver socket
#include "drv_vdec.h"   //video decode driver socket
#include "drv_avs.h"    //audio video sync driver socket
#include "vdec_2_vdrv.h"


#include "cedar_task_prio.h"
#include "cedar.h"

#ifdef __cplusplus
#define DECLS_BEGIN extern "C" {
#define DECLS_END   }
#else
#define DECLS_BEGIN
#define DECLS_END
#endif/*__cplusplus*/

//#define SUPPORT_DTS         (0)
//#define SUPPORT_AC3         (0)

//打印设置
#define ERIC_WARNING    __wrn
//#define ERIC_WARNING(...)    __wrn(__VA_ARGS__)
//#define ERIC_WARNING(...)

//#define ERIC_MSG        __wrn
//#define ERIC_MSG(...)           __wrn(__VA_ARGS__)
#define ERIC_MSG(...)

//90度旋转测试
//#define ROTATE_90_TEST
//显示模式userdef测试
//#define TEST_USERDEF_VID_SHOW_MODE

#define SYS_TIMEDELAY_UNIT (10) //系统的delay的时间单位是10ms
#define AUDIO_JUMP_POINT_THRESH  (1200) //指跳播到的时间点的最小值，单位ms
//#define FFRR2PLAY_DELAY_TIME     (1500)  //在ffrr->play的状态转换时，需要aply delay的时间，用于缓冲数据
//define 64 bit signe and unsigne data type
#ifndef DATA_TYPE_X___s64
#define DATA_TYPE_X___s64
typedef signed long long int        __s64;
#endif

#ifndef DATA_TYPE_X___u64
#define DATA_TYPE_X___u64
typedef unsigned long long int      __u64;
#endif

//define max picture size can be support for every video decoder
#define PIC_CAPTURE_DIRECTION   1  //1表示从上到下抓取，-1表示从下到上抓取.

//define the format get data from system call
typedef struct __VIDEO_ENCRYPT_ITEM
{
    char    format[16]; //"mpeg2","mpeg4","rmvb","h264","vc1""wmv1","wmv2","vp6","avs"
    __s32   support;
    __u32   s_width;
    __u32   s_height;
    __u32   reserved;

} __video_encrypt_item;


//define audio tag information size
//#define AUD_TAG_INF_SIZE                CEDAR_TAG_INF_SIZE
#define CEDAR_FRPIC_HOLDTIME_DEFAULT    (30)    /* 快进、快退状态下，图片显示停留时间,(30ms)            */
#define CEDAR_FRSPEED_DEFAULT           (64)    /* 快进快退速度，默认值为64倍速，应用程序可调           */

//define audio render type
typedef enum __AUDIO_RDR_TYPE
{
    CEDAR_ADEC_ARDR_TYPE_SW=0,  // audio render is processed by software
    CEDAR_ADEC_ARDR_TYPE_HW     // audio render is processed by hardware

} __audio_rdr_type_t;


//define the type of data which taken with socket
#define CEDAR_DATATYPE_NONE         (0<<0)      /* none data                            */
#define CEDAR_DATATYPE_START        (1<<0)      /* start data of file                   */
#define CEDAR_DATATYPE_MID          (1<<1)      /* middle data of file                  */
#define CEDAR_DATATYPE_END          (1<<2)      /* end data of file                     */

//define the type of data which taken with socket
#define GINKGO_DATATYPE_NONE        (0<<0)      /* none data        */
#define GINKGO_DATATYPE_START       (1<<0)      /* start data       */
#define GINKGO_DATATYPE_MID         (1<<1)      /* middle data      */
#define GINKGO_DATATYPE_END         (1<<2)      /* end data         */


//define the type base number of message  which taken with socket
//#define CEDAR_MSGTYPE_NONE          (0<<16)     /* none message                                                     */
//#define CEDAR_PSR2ADEC_MSG_BASE     (1<<16)     /* message base number between parser and audio deocder             */
//#define CEDAR_PSR2VDEC_MSG_BASE     (2<<16)     /* message base number between parser and video deocder             */
//#define CEDAR_PSR2LDEC_MSG_BASE     (3<<16)     /* message base number between parser and lyric deocder             */
//#define CEDAR_ADEC2ARDR_MSG_BASE    (4<<16)     /* message base number between audio decoder and render             */
//#define CEDAR_ARDR2APLY_MSG_BASE    (5<<16)     /* message base number between render and audio playback            */
//#define CEDAR_VDEC2VPLY_MSG_BASE    (6<<16)     /* message base number between video decoder and video playback     */
//#define CEDAR_LDEC2LRDR_MSG_BASE    (7<<16)     /* message base number between lyric decoder and lyric render       */
//#define CEDAR_LRDR2LPLY_MSG_BASE    (8<<16)     /* message base number between lyric render and lyric playback      */
//#define CEDAR_ADEC2ADRV_MSG_BASE    (9<<16)     /* message base number between audio decoder plug-in and driver     */
//#define CEDAR_VDEC2VDRV_MSG_BASE    (10<<16)    /* message base number between video decoder plug-in and driver     */
//#define CEDAR_ARDR2ADRV_MSG_BASE    (11<<16)    /* message base number between audio render plug-in and driver      */


#define CEDAR_PSR_ABS_QSIZE         (1)         //audio bitstream socket queue size
#define CEDAR_PSR_ABS_BUFSIZE       (0)         //audio bitstream socket buffer size

#define CEDAR_PSR_VBS_QSIZE         (1)         //video bitstream socket queue size
#define CEDAR_PSR_VBS_BUFSIZE       (0)         //video bitstream socket buffer size

#define CEDAR_PSR_LBS_QSIZE         (1)         //lyric bitstream socket queue size
#define CEDAR_PSR_LBS_BUFSIZE       (0)         //lyric bitstream socket buffer size

#define CEDAR_DEC_VIDEO_PIC_QSIZE   (1)         //picture frame socket queue size
#define CEDAR_DEC_VIDEO_PIC_BUFSIZE (0)         //picture frame socket buffer size

//==============================================================================
//                      FILE SYSTEM SYSTEM-CALL
//==============================================================================
typedef struct __ESFILE                 FILE;
#undef FIOCTRL
#undef fioctrl
#undef FWRITE
#undef fwrite
#define FOPEN(name, mode)               eLIBs_fopen((name),(mode))
#define fopen(name, mode)               eLIBs_fopen((name),(mode))
#define FCLOSE(fptr)                    eLIBs_fclose((fptr))
#define fclose(fptr)                    eLIBs_fclose((fptr))
#define FREAD(buf,unit,size,fptr)       eLIBs_fread((buf),(unit),(size),(fptr))
#define fread(buf,unit,size,fptr)       eLIBs_fread((buf),(unit),(size),(fptr))
#define FSEEK(fptr,offset,origin)       eLIBs_fllseek((fptr),(offset),(origin))
#define fseek(fptr,offset,origin)       eLIBs_fllseek((fptr),(offset),(origin))
#define FTELL(fptr)                     eLIBs_flltell((fptr))
#define ftell(fptr)                     eLIBs_flltell((fptr))
#define FIOCTRL(fp, cmd, aud, pbuf)     eLIBs_fioctrl(fp, cmd, aud, pbuf)
#define fioctrl(fp, cmd, aud, pbuf)     eLIBs_fioctrl(fp, cmd, aud, pbuf)
#define FWRITE(buf,unit,size,fptr)      eLIBs_fwrite((buf),(unit),(size),(fptr))
#define fwrite(buf,unit,size,fptr)      eLIBs_fwrite((buf),(unit),(size),(fptr))

#define GETVOLFSPACE(partname)          eLIBs_GetVolFSpace((partname))

//==============================================================================
//                      PRINT INFORMATION SYSTEM-CALL
//==============================================================================
#define MSG                 __msg
#define MESSAGE             __msg
#define WRN                 __wrn
#define WARNING             __wrn
#define ERR                 __err
#define ERROR               __err
#define LOG                 __log
#define INF                 __inf
#define INFORMATION         __inf
#define DUMP_MEM(x, y, z)   eLIBs_dumpmem(x, y, z)
#define printf              __wrn

//==============================================================================
//                      MEMORY OPERATION SYSTEM-CALL
//==============================================================================
#define MALLOC(x)                       esMEMS_Malloc(0, x)
#define malloc(x)                       esMEMS_Malloc(0, x)
#define FREE(x)                         esMEMS_Mfree(0, x)
#define free(x)                         esMEMS_Mfree(0, x)
#define realloc(f, x)                   esMEMS_Realloc(0, f, x)
#define REALLOC(f, x)                   esMEMS_Realloc(0, f, x)

#define palloc(x, y)                    esMEMS_Palloc(x, y)
#define pfree(x, y)                     esMEMS_Pfree(x, y)
#define BALLOC(x)                       esMEMS_Balloc(x)
#define BFREE(x, y)                     esMEMS_Bfree(x, y)

//#define memset(buf, val, size)          eLIBs_memset(buf, val, size)
//#define memcpy(dst, src, size)          eLIBs_memcpy(dst, src, size)
//#define memcmp(src1, src2, size)        eLIBs_memcmp(src1, src2, size)

//#define MEM_SET         memset
//#define MEM_CPY         memcpy
//#define MEMCMP          memcmp 

extern void  memset(void * pmem, __u8 data_val, __size_t size);
extern void *memcpy(void * pdest, const void * psrc, __size_t size);

#undef MEM_SET
#undef MEM_CPY
#define MEM_SET(buf, val, size)         memset(buf, val, size)
#define MEM_CPY(dst, src, size)         memcpy(dst, src, size)
//#define MEMSET                          MEM_SET
//#define MEMCPY                          MEM_CPY


#define TOUPPER(n)                      eLIBs_toupper(n)
#define toupper(n)                      eLIBs_toupper(n)
#define STR_NCMP(dst, src, size)        eLIBs_strncmp(dst, src, size)
#define strncmp(dst, src, size)         eLIBs_strncmp(dst, src, size)
#define STR_CMP(s1, s2)                 eLIBs_strcmp(s1, s2)
#define strcmp(s1, s2)                  eLIBs_strcmp(s1, s2)
#define STR_LEN(s)                      eLIBs_strlen(s)
#define strlen(s)                       eLIBs_strlen(s)
#define STR_NCPY(s1, s2, n)             eLIBs_strncpy(s1, s2, n)
#define strncpy(s1, s2, n)              eLIBs_strncpy(s1, s2, n)
#define STR_CPY(s1, s2)                 eLIBs_strcpy(s1, s2)
#define strcpy(s1, s2)                  eLIBs_strcpy(s1, s2)
#define STR_CHR(s, ch)                  eLIBs_strchr(s,ch)
#define strchr(s, ch)                   eLIBs_strchr(s,ch)
#define STR_STR(s1, s2)                 eLIBs_strstr(s1, s2)
#define strstr(s1, s2)                  eLIBs_strstr(s1, s2)
#define STR_CAT(s1, s2)                 eLIBs_strcat(s1, s2)
#define strcat(s1, s2)                  eLIBs_strcat(s1, s2)

#define stricmp(s1, s2)                 eLIBs_stricmp(s1, s2)
#define strnicmp(dst, srt, size)        eLIBs_strnicmp(dst, srt, size)
#define strrchr(s, ch)                  eLIBs_strchrlast(s,ch)
#define sprintf                         eLIBs_sprintf

//==============================================================================
//                      TASK SCHEDULE SYSTEM-CALL
//==============================================================================
#define DISABLE_OS_TASK_SWITCH()        esKRNL_SchedLock()
#define ENABLE_OS_TASK_SWITCH()         esKRNL_SchedUnlock()
#define VirAddr2PhyAddr(x)              esMEMS_VA2PA((void *)(x))
#define DisableInterrupt()              ENTER_CRITICAL(cpu_sr)
#define EnableInterrupt()               EXIT_CRITICAL(cpu_sr)

//#define DisableSwitch                   DISABLE_OS_TASK_SWITCH
//#define EnableSwitch                    ENABLE_OS_TASK_SWITCH


//==============================================================================
//                      PRINTF & ASSERT FOR DEBUG
//==============================================================================
#define SIMUASSERT(f)               \
    if(!(f))                        \
    {                               \
        WARNING("param wrong\n");   \
        return -1;                  \
    }

#define return_if_fail(p) if(!(p)) \
	{WARNING("Warning: "#p" failed.\n" \
		); return;}
#define return_val_if_fail(p, ret) if(!(p)) \
	{WARNING("Warning: "#p" failed.\n" \
	); return (ret);}

typedef struct __WAVE_HEADER1
{
    __u32       uRiffFcc;       // four character code, "RIFF"
    __u32       uFileLen;       // file total length, don't care it

    __u32       uWaveFcc;       // four character code, "WAVE"

    __u32       uFmtFcc;        // four character code, "fmt "
    __u32       uFmtDataLen;    // Length of the fmt data (=16)
    __u16       uWavEncodeTag;  // WAVE File Encoding Tag
    __u16       uChannels;      // Channels: 1 = mono, 2 = stereo
    __u32       uSampleRate;    // Samples per second: e.g., 44100
    __u32       uBytesPerSec;   // sample rate * block align
    __u16       uBlockAlign;    // channels * bits/sample / 8
    __u16       uBitsPerSample; // 8 or 16

    __u32       uDataFcc;       // four character code "data"
    __u32       uSampDataSize;  // Sample data size(n)

} __wave_header_t1;
//==============================================================================
//      定义存储用户定制信息的数据结构，定制信息从配置文件读取.
//==============================================================================
#define PERMISSION_TABLE_SIZE   (32)
/*******************************************************************************
Function name: tag_CedarINI
Members: 
    授权表的结构不给有效个数,因为规定最后一项是CEDAR_ABS_TYPE_NONE或CEDAR_VBS_TYPE_NONE.
    但不能超过PERMISSION_TABLE_SIZE,所以最大有效个数为PERMISSION_TABLE_SIZE-1.
*******************************************************************************/
typedef struct tag_CedarVideoPermissionIndex
{
    __cedar_video_fmt_t         nType;              /* __cedar_video_fmt_t */
    __s32                       nPermissionFlag;    //1:授权，0:禁止
}CedarVideoPermissionIndex;
typedef struct tag_CedarAudioPermissionIndex
{
    __cedar_audio_fmt_t         nType;              /* __cedar_audio_fmt_t */
    __s32                       nPermissionFlag;    //1:授权，0:禁止
}CedarAudioPermissionIndex;

/*******************************************************************************
Cedar.ini文件的配置项说明:
Members: 
    AUDIO_RUN_ONLY_BUF_SIZE: audio驱动的bufsize.单位kbytes, 128代表128k
    AUDIO_WITH_VIDEO_BUF_SIZE:同上
    AACSBR:是否打开aac解码驱动的sbr功能, 0关闭;1打开
*******************************************************************************/
typedef struct tag_CedarINI
{
	CedarVideoPermissionIndex   VideoPermissionTable[PERMISSION_TABLE_SIZE]; //和VideoPermissionTable[]有关系,对照关系
    CedarAudioPermissionIndex   AudioPermissionTable[PERMISSION_TABLE_SIZE]; //和AudioPermissionTable[]有关系,对照关系
    __s32   nDelayBeforePlay;   // 在play前delay的时间,单位ms.因为19因为带宽问题可能需要缓冲数据，然后播才能确保声音不卡顿.
    __s32   nAUDIO_RUN_ONLY_BUF_SIZE;   //配置audiodrv的buffersize的参数
    __s32   nAUDIO_WITH_VIDEO_BUF_SIZE;    
    __s32   nAACSBREnable;  //0:disable, 1:enable
    //__s32   n1080PSupport;  //0:不支持1080p的视频文件, 1:支持
    __s32   max_support_height;   //0:全支持;  //720表示最大支持到720P,1080表示最大支持到1080P
    __s32   aux_wav_buf_size;  //通过CEDAR_CMD_PLAY_AUX_WAV_BUFFER播放的附加声音数据的buffer大小,ardr open 时就会按此值分配好内存.
}CedarINI;

typedef struct tag_CFEConfig
{
    //* configuration for tuner/demodulator mode selection.
    //* configuration for interface between TS controller and Demodulator.
    __s32    nTS_INTERFACE_TYPE;                         //* 0: using ts spi;            1: using ts ssi.
    __s32    TS_INTERFACE_PACKET_SYNC_METHOD;           //* 0: by PSync signal;         1: by 0x47 sync byte; 2:by both PSync signal and sync byte.
    __s32    TS_INTERFACE_SSI_DATA_ORDER;               //* 0: MSB first;               1: LSB first.
    __s32    TS_INTERFACE_CLOCK_SIGNAL_POLARITY;        //* 0: rising edge capture;     1: faling edge capture.
    __s32    TS_INTERFACE_ERROR_SIGNAL_POLARITY;        //* 0: high level active;       1: low level active.
    __s32    TS_INTERFACE_DATA_VALID_SIGNAL_POLARITY;   //* 0: high level active;       1: low level active.
    __s32    TS_INTERFACE_PSYNC_SIGNAL_POLARITY;        //* 0: high level active;       1: low level active.
    __s32    TS_INTERFACE_PACKET_SIZE;                  //* 0: 188;             1: 192;             2: 204.
}CFEConfig;  //数字电视前端配置,给demod驱动和tsc驱动使用.

#endif  /* _MOD_MMP_I_H_ */


