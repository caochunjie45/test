
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
//#define LDEC_MAX_TXT_FRM_SIZE     (1024 * 64)   //һ֡�ĳ���,������subΪ��׼,�ı����Ͳ���malloc��ô���.
#define LDEC_MAX_TXT_COPY_SIZE    (1024 * 32)     //ר�����ں�psr�������ݵ�buffer,Ŀ����һ��װ��psrҪװ������,��Щpsr����mkv�Դ�����Ļ��Ҫ�����һ�δ���.
//#define LDEC_MAX_TXT_ANCI_SIZE    (1024 * 6)     //��ֹ����ʱ���ݻ�ͷ
//
//#define LDEC_MAX_TXT_FRM_NUM       (4)  //frame�ĸ���
//#define LDEC_MAX_TXT_DATA_NUM      (64) //lbs�ֿ�ĸ���

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
//ʵ��Ա����
typedef __s32 (*LdecItxt_Open)(LdecItxt *thiz);
typedef __s32 (*LdecItxt_Close)(LdecItxt *thiz);
typedef struct __LDEC_ITXT
{
    __u32               mid;          //lyric decoder
    __u8                socket_tsk_prio;   //priority of the main task
    __u8                dec_tsk_prio;      //priority of the main task
    __s32               IOOpenFlag;     //��ʾ�Ƿ������IO_Open.

    __s8                stop_flag;          //stop flag for main task
    //__s8                decFlag;            //����flag, ��ʾ�Ƿ����ڽ���,�𻥳����õ�.
    //__s8                useAnciBuf;     //�Ƿ�ʹ����ancibuf,�������ͷ.
    __hdle              pLSktin;            //socket for input lrc bitstream
    __hdle              pLSktout;           //socket for ouput lrc item, not use now.
    __mp                *hAvs;              //av-sync driver handle

    __s32               nOneSegFlag;        //��ǰ�˵���ģʽ��isdbʱ,ָʾ��service��oneseg����fullseg�����.��������ģʽ��dvb,atsc,������������.��0����.  

    __subtitle_codec_format_t   SubtitleCodecInfo;  //���ڽ���������Ļ��psrģ�鴫������Ļ��Ϣ,ʵ����,codec_type��data_encode_type�ǲ�����,ts_psr������Ҳδ����ȷ,ֻ��subtitle_bs_src����.codec_type���нӿ�CEDAR_LDEC_CMD_SET_LBSTYPEר������,���ø�ldec_itxt->subtitle_type.
    __s32               subtitle_type;      // srt, sub, etc.__cedar_lyric_fmt_t, ��Ļ�ļ�����
    __s32               frm_rate;           //video frame rate. be careful : ��ֵ�Ŵ���1000��!, for sub decode
    __s32               width;              //for sub decode
    __s32               height;             //for sub decode
    __s32               screenWidth;    //��ʾ���ڵĿ��
    __s32               screenHeight;
//    __s32               tvType;             //sub��Ļ�Ľ��룬��ҪtvSystemType, TV_SYSTEM_PAL
//    __u8                lastColorCode[4];
//    __u8                lastContrast[4];

    //__ldec_lbs_buf_t    ldecLbsBuf;     //������
    //__ldec_manager_t    ldecManager;    //������
    //__cedar_subtitle_item_t outputItem;
    //__u8                *anciDataBuf;   //������, ����ʱ����lbsbuf��ͷ�õ�.
    __u8                *copyDataBuf;   //ר�����ں�psr�������ݵ�buf,ldec_itxt����ֱ�Ӱ�lbsbuf��psr��.������Ϊ�����ʱ��,���Ҫ��Ϊһ�α��봫��������Ļ��Ŀ.��lbsbuf�л�ͷ������,����һ��ֻ��������buf,���ܴ���.Ϊ������������,�ɴ�ר����һ����buf����.

    __s32               nResetLdecBufFlag;    //���³�ʼ��ldecDrv������buffer�ı��λ.
    __krnl_event_t      *LbsInputSem;            //semaphore for wakeup lyric decode task, lbsbuf_input_sema, ���ǵȴ��õ�,���ǻ���
    __krnl_event_t      *FrameBufSem;            //semaphore for wakeup lyric decode task, frame_release_sema.
    CedarIsubtitleDec   *pCedarIsubtitleDecoder; //������Ļ������,һ����startʱ��malloc����.
    
    ReqSubtitleFrameMode    nRequestFrameMode;  //ReqSubtitleFrameMode_AppReq��.

    //ʵ��Ա����
    LdecItxt_Open   open;
    LdecItxt_Close  close;

} LdecItxt;

extern __s32 LdecItxt_Init(LdecItxt *thiz, __u32 mid);
extern __s32 LdecItxt_Exit(LdecItxt *thiz);
extern LdecItxt* newLdecItxt(__u32 mid);
extern void deleteLdecItxt(LdecItxt* thiz);

#endif

