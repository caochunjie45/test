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
//    ��������: ��Ƶ�ļ���������ʽ����Ƶ�ļ���������ʽ��ȫ��ͬ����˷�Ϊ���ֱַ�
//    ����
//*******************************************************************************/
//enum JUMP_PLAY_TYPE{
//    NOT_SUPPORT_JUMP_PLAY   = -1,
//    JUMP_PLAY_TYPE_VIDEO    = 0,
//    JUMP_PLAY_TYPE_AUDIO    = 1,
//};
typedef enum __FILE_PARSER_COMMAND
{
    CEDAR_PSR_CMD_START=0x00,           /* ����parser                           */
    CEDAR_PSR_CMD_STOP,                 /* ֹͣparser                           */
    CEDAR_PSR_CMD_PAUS,                 /* �������ʱ����õ�,��psr_video��Ч                      */
    CEDAR_PSR_CMD_CONT,                 /* �������ʱ����õ�,��psr_video��Ч                         */
    CEDAR_PSR_CMD_FF,                   /* ���ÿ��ģʽ                         */
    CEDAR_PSR_CMD_RR,                   /* ���ÿ���ģʽ                         */
    CEDAR_PSR_CMD_CLEAR_FFRR,           /* ����������ģʽ                     */
    CEDAR_PSR_CMD_GET_STATUS,           /* ��ȡfile parser��ǰ״̬, aux = 1���뷵���ȶ�ֵ, aux =0 ��������              */
    CEDAR_PSR_CMD_JUMP,                 /* ��������ģʽ, aux = des_tm           */
    CEDAR_PSR_CMD_CLEAR_JUMP,           /* �������״̬                     */
    CEDAR_PSR_CMD_NOTIFY_ABORT,         /* ֪ͨpsrģ��׼��, ret = EPDK_OK/EPDK_FAIL */

    CEDAR_PSR_CMD_PREPARE_FF,               //׼��FF,rr��, play->pause
    CEDAR_PSR_CMD_PREPARE_RR,               // play->pause
    CEDAR_PSR_CMD_PREPARE_CLEAR_FFRR,       // ffrr->pause

    CEDAR_PSR_CMD_OPEN = 0x20,          //������Դ
    CEDAR_PSR_CMD_CLOSE,                //�ͷ���Դ
    CEDAR_PSR_CMD_OPEN_FILE,     /* previewҲʹ��.   ����ý���ļ�, aux = CEDAR_MEDIA_FILE_FMT_MP3��                         */
    CEDAR_PSR_CMD_SET_CEDARINI,         //���û��Ķ�����Ϣ����psrģ��.���Խ���ָ��,��ʹ��.��Ϊ�ÿ��ڴ���cedar���ط���,�����ڼ�������ͷ�. pbuffer = CedarINI*
    CEDAR_PSR_CMD_SET_AVS_HDL,          /* ����AVS�������                      */
    CEDAR_PSR_CMD_CHKSPT,               /* ����Ƿ�֧��ý���ļ�                 */
    CEDAR_PSR_CMD_CHKSPT_FFRR,          /* ����Ƿ��ܹ�֧�ֿ��/����            */
    CEDAR_PSR_CMD_CHKSPT_JUMPTYPE,      /* ����Ƿ��ܹ�֧�������Լ���������,ret= enum JUMP_PLAY_TYPE,    */
    CEDAR_PSR_CMD_GET_ABSSKTOUT,        /* ��ȡ�����Ƶ�����                   */
    CEDAR_PSR_CMD_GET_VBSSKTOUT,        /* ��ȡ�����Ƶ�����                   */
    CEDAR_PSR_CMD_GET_LBSSKTOUT,        /* ��ȡ�����Ļ�����                   */
    CEDAR_PSR_CMD_GET_CURSERVICE_ONESEGFLAG, //������tspsr��ѯ�������ڲ��ŵ�service�ĵ��Ʒ�ʽ.����psr���Բ�ʵ��.

    
    CEDAR_PSR_CMD_GET_ABSTYPE = 0x40,          /* ��ȡý���ļ���Ƶ������(��ǰ���ڲ��ŵ���Ƶ��)*/
    CEDAR_PSR_CMD_GET_VBSTYPE,          /* ��ȡý���ļ���Ƶ������               */
    CEDAR_PSR_CMD_GET_LBSTYPE,          /* ��ȡý���ļ���Ļ������,ֻ����������Ļ������. �����ǵ���GetLbsFormat()��ȡ��ǰ���ŵ���Ļ��ʽ.������Ļ����ͳһ��subtitlemanage.����psr���Բ��ܵ�ǰ���ŵ���Ļ��ʽ��.�����Ѿ�����������,תΪʹ��CEDAR_PSR_CMD_GET_LBSTYPE2�ӿ�               */
    
    CEDAR_PSR_CMD_GET_ABSCNT,           /* �����, ��ȡý���ļ�����Ƶ������             */
    CEDAR_PSR_CMD_GET_AUDSTREAM_PROFILE,/* �����, ��ȡ������Ƶ����˵����Ϣ: pbuffer = __psr_audstream_profile_t, app malloc it. return = EPDK_OK or EPDK_FAIL*/
    CEDAR_PSR_CMD_GET_CUR_ABSSUFFIX,    /* �����, ��ȡý���ļ���Ƶ�����±��, ret = �±��*/
    CEDAR_PSR_CMD_GET_ABSTYPE2,         /* �����, ��ȡý���ļ���Ƶ�����ͣ�ѡ���±��,aux = �±��, ret = __cedar_audio_fmt_t*/
    CEDAR_PSR_CMD_SET_ABSSUFFIX,        /* �����, �����µ�audio stream�������±��, aux = �±��,ret = EPDK_OK/EPDK_FAIL     */
    CEDAR_PSR_CMD_AUD_RESYNC,           /* audio��ͬ��������֪PSRģ�����µõ���audio chunk��PTS����audioʱ����, ��AVͬ��.ret = EPDK_OK/EPDK_FAIL*/
    CEDAR_PSR_CMD_NOTIFY_CHANGE_AUDIO_STREAM,   /* Ҫ��PSRģ��������죬PSR�Ķ�����:��ADECģ�鴫��һ��ABSDECINF��֪ͨPSR��ģ������AUD_PTS����ʼ����AUDIO����*/
    
    CEDAR_PSR_CMD_GET_SUBTITLE_PROFILE, /* ����Ҫ����, ��ʱ����.ֻ���𷵻���Ƕ��Ļ����Ϣ */
    CEDAR_PSR_CMD_SELECT_SUBTITLE,      /*����Ҫ����, ��ʱ����. ѡ��һ����Ļ,��ֻ����������Ļ*/
    CEDAR_PSR_CMD_GET_SUBTITLE,         /*����Ҫ����, ��ʱ����. �������ڲ��ŵ�subtitle�������±��,if not find, return -1, ��ֻ���������Ļ   */
    CEDAR_PSR_CMD_GET_LBSTYPE2,         /* ����Ҫ����, ��ȡý���ļ�����Ļ�ļ����ͣ�ѡ���±��,aux = �±��, ret = LYRIC_ITXT��,ֻ���������Ļ */
    CEDAR_PSR_CMD_ENABLE_EXTERN_SUBTITLE,   //�Ѿ�û��������,���Է���.���������Ļ���, AUX = 1:��������Ļ; aux =0:���������Ļ
    CEDAR_PSR_CMD_SKIP_SUBTITLE,          //�Ƿ�skip������Ļ. aux=1:skip; aux=0:����.
    CEDAR_PSR_CMD_NOTIFY_CHANGE_SUBTITLE_STREAM,   //֪ͨpsr����Ļ.psr���ldec_itxt��һ������.Ȼ��������.
    
    CEDAR_PSR_CMD_DISCARD_DATA,         /* �Ƿ���ȡ�������ݣ�aux = enum BITSTREAM_TYPE, pbuffer = 1(����), 0(����)
                                            ��Ŀǰ����ϵ�£������ֹaudio�Ĵ��ͣ������ϻᵼ��AV��ͬ������һ��������IO�ӿ�ʱ��������ʼ����ǰ��
                                            ,��ʼ����֮ǰ�ͽ�ֹ����ζ����Զ�����ٴ���.����psr��Զ�ò���audio���ݣ���Զdo_sync_flag���ᱻ��Ϊ1������Ҳû������.
                                            ������Ҳ�����,��������Ƶʱ��.*/
    CEDAR_PSR_CMD_QUERY_DATAEND,           /* ��ѯPSRģ�飬�����Ƿ���ȡ��, ret, 1:ȡ�꣬0:ûȡ��*/
    CEDAR_PSR_CMD_QUERY_FORBID_JUMP_IN_CHGAUD,      /*��ѯPSR��ģ���Ƿ��ֹ�ڻ�����Ĺ����м�����������, ret = EPDK_OK��ʾ��ֹ��EPDK_FAIL��ʾ����ֹ */
    

    CEDAR_PSR_CMD_GET_TOTAL_TIME=0x60,  /* ��ȡý���ļ�������ʱ��               */
    CEDAR_PSR_CMD_GET_TAG,              /* ��ȡý�岥�ű�ǩ��Ϣ, pbuffer = __cedar_tag_inf_t*, �����߷����ڴ� */
    CEDAR_PSR_CMD_SET_TAG,              /* ����ý�岥�ű�ǩ��Ϣ                 */
    CEDAR_PSR_CMD_SET_FRSPEED,          /* ���ÿ���������ٶ�                   */
    CEDAR_PSR_CMD_GET_FRSPEED,          /* ��ȡ����������ٶ�                   */
    CEDAR_PSR_CMD_SET_FRPIC_TIME,       /* ����Ƿ��ܹ�֧�ֿ��/����            */
    CEDAR_PSR_CMD_GET_VIDBPS,           /* ��ȡ��Ƶ��������                     */
    CEDAR_PSR_CMD_GET_VIDFRMRATE,       /* ��ȡ��Ƶ֡��                         */
    CEDAR_PSR_CMD_GET_VIDFRMSIZE,       /* ��ȡ��Ƶͼ���С                     */

    //Ϊ����ͼ�����Ľӿ�.
    CEDAR_PSR_CMD_GET_FILE_SIZE=0x80,    //pbuffer = *__s64, 
    CEDAR_PSR_CMD_GET_MEDIAINFO,   //pbuffer = __media_file_info_internal_t*, caller �����ڴ�
    CEDAR_PSR_CMD_SET_PROC_MODE,    //aux = enum MEDIA_STATUS, ʵ���Ͼ��ǵ���SetProcMode(), aux = MEDIA_STATUS_PLAY��, ��aux =MEDIA_STATUS_JUMPʱ, pbuffer = __jump_cfg_t*, ��������ʱ������
    CEDAR_PSR_CMD_GET_VBSINF,       //pbuffer = __vdec_mpeg4_vbs_inf_t**, 
    CEDAR_PSR_CMD_CLOSE_MEDIA_FILE, //
    CEDAR_PSR_CMD_GET_NEXT_CHUNK_INFO,  //GetNextChunkInfo(), pbuffer = __u32*, (chunk_type)
    CEDAR_PSR_CMD_GET_CHUNK_DATA,       //GetChunkData(), ����ֵ��GetChunkData()��һ����
    CEDAR_PSR_CMD_SKIP_BITSTREAM,       //SkipChunkData()
    CEDAR_PSR_CMD_SET_VDRV, //tspsr����getnextchunkinfo()��ֱ����vdrvҪ����
    CEDAR_PSR_CMD_SET_PREVIEW_MODE, //����psrģ�鵱ǰΪȡԤ��ͼ��ģʽ. ΪӦ��ts_psr��������. aux = 1:Ԥ��ͼģʽ; aux=0:��������ģʽ

    //֧��3D����
    CEDAR_PSR_CMD_GET_VIDEO_DISPLAY_MODE_INFO = 0x100,
    CEDAR_PSR_CMD_SET_VIDEO_DISPLAY_MODE_INFO,

    
    CEDAR_PSR_CMD_SET_FOP = 0x5000,     // previewҲʹ��.   �����û��Զ����ļ���������
    CEDAR_PSR_CMD_

} __file_parser_command_t;

typedef enum __FILE_PARSER_INTERNAL_COMMAND
{
    CEDAR_PSR_INTERNAL_CMD_STOP_PSR_DEV = 0x01,                 /* �ر�psr_dev */
    CEDAR_PSR_INTERNAL_CMD_CHKSPT_JUMPTYPE = 0x27,      /* ����Ƿ��ܹ�֧�������Լ���������,ret= enum JUMP_PLAY_TYPE,    */
    CEDAR_PSR_INTERNAL_CMD_GET_CURSERVICE_ONESEGFLAG = 0x2B,   //������tspsr��ѯ�������ڲ��ŵ�service�ĵ��Ʒ�ʽ.����psr���Բ�ʵ��.
    CEDAR_PSR_INTERNAL_CMD_GET_CUR_ABSSUFFIX = 0x2D,    /* ��ȡý���ļ���Ƶ�����±��, ret = �±��*/
    CEDAR_PSR_INTERNAL_CMD_SET_INITIAL_ABSSUFFIX = 0x2E,    //����ʽ����ǰ,������GetNextChunkInfo()ǰ���ó�ʼ��Ƶ�����±��
    CEDAR_PSR_INTERNAL_CMD_SET_ABSSUFFIX = 0x2F,        /* �����µ�audio stream�������±��, aux = �±��,ret = EPDK_OK/EPDK_FAIL     */
    CEDAR_PSR_INTERNAL_CMD_AUD_RESYNC = 0x30,           /* audio��ͬ��������֪PSRģ�����µõ���audio chunk��PTS����audioʱ����, ��AVͬ��.ret = EPDK_OK/EPDK_FAIL*/
    CEDAR_PSR_INTERNAL_CMD_DISCARD_DATA = 0x31,         /* �Ƿ���ȡ�������ݣ�aux = enum BITSTREAM_TYPE, pbuffer = 1(����), 0(����), ret = EPDK_OK���óɹ�*/
    CEDAR_PSR_INTERNAL_CMD_QUERY_FORBID_JUMP_IN_CHGAUD, /* �ο�CEDAR_PSR_CMD_QUERY_FORBID_JUMP_IN_CHGAUD��˵��, EPDK_OK�����ֹ����������� */
    /* ��mixed chunk�л�ȡ����Ƶ����, pbuffer = __mix_buf_msg_t*, ret = __file_parser_return_val_t 
    ret�ķ���ֵֻ������4��������ķ���ֵ����Ϊfatal error
    FILE_PARSER_READ_END(1):�������ݾ�����buffer����.
    FILE_PARSER_ERR_VIDEO_BUF_FULL:video buffer�����꣬��Ҫ�µ�video buffer
    FILE_PARSER_ERR_AUDIO_BUF_FULL:audio buffer�����꣬��Ҫ�µ�audio buffer
    -1: fatal error*/
    CEDAR_PSR_INTERNAL_CMD_GET_MIX_CHUNK_DATA,          
    CEDAR_PSR_INTERNAL_CMD_SKIP_REMANENT_CHUNK_DATA, //��SkipChunkData()��֮ͬ�����ڣ���������ӿڿ��԰Ѷ��˲������ݵ�chunk skip��.SkipChunkData()��������ʱ��Ĭ��skipһ��������chunk, ret = EPDK_OK/EPDK_FAIL.
    CEDAR_PSR_INTERNAL_CMD_NOTIFY_ABORT,        /*  ֪ͨpsrģ��׼��, ret = EPDK_OK/EPDK_FAIL*/

    CEDAR_PSR_INTERNAL_CMD_SELECT_SUBTITLE = 0x50,  /* ѡ��subtitle��index */
    CEDAR_PSR_INTERNAL_CMD_GET_CUR_SUBTITLE,        /* �õ�subtitle��index */
    //CEDAR_PSR_INTERNAL_CMD_CHKSPT_STREAMCODEC,           /* �鿴�ļ��е���Ƶ��Ƶ���Ľ���֧�����, pbuffer == __file_codec_support_t,
                                                            //ֻҪ��ģ��ʵ�������IO�ӿڣ�һ�ɷ���EPDK_OK������EPDK_FAIL*/
    
    CEDAR_PSR_INTERNAL_CMD_

} __file_parser_internal_command_t;

typedef struct
{
    //psr_high -> psr_low�ĳ�Ա����
    __u8*   pVidbuf;       //�����video buf
    __u32   nVidBufSize;   //bufSize
    
    __u8*   pAudbuf;       //�����audio buf
    __u32   nAudBufSize;   //bufSize

    //psr_low -> psr_high�ĳ�Ա����
    __u32   nVidMsgValid;//LOW_PSR��ʾ��video msg�Ƿ���Ч�ı�־
    __u32   nVidDataLen;//װ���video���ݵĳ���
    __u32   nVidMsgType;
    void*   pVidMsg;    //�ѷ���ÿռ䣬ֱ��ʹ��

    __u32   nAudMsgValid;
    __u32   nAudDataLen;//װ���audio���ݵĳ���
    __u32   nAudMsgType;
    void*   pAudMsg;    //�ѷ���ÿռ䣬ֱ��ʹ��
} __mix_buf_msg_t;

typedef struct __AUDIO_FILE_BS_INF
{
    __u32       ulSampleRate;       // ������ sample rate
    __u32       ulBitRate;          // ������ bit rate
    __u32       ulChannels;         // ������ channel
    __u32       ulDuration;         // ����ʱ�� duration
    __u32       ulBitsSample;       // ������λ�� sample 8/16/24/32

} __audio_file_bs_inf_t;

typedef struct __AUDIO_FILE_ID3_INF
{
    __s32       ulAudio_name_sz;    //��Ƶ��ʽ˵��
    char*       ulAudio_name;       //mp3 /RealAudio Cook.sipo. / aac-lc....

    __s32       ulGenre_sz;         //����
    char*       ulGenre;            //pop soft...

    __s32       ultitle_sz;         //������
    char*       ultitle;

    __s32       ulauthor_sz;        //�ݳ���
    char*       ulauthor;

} __audio_file_id3_inf_t;


//typedef struct __JUMP_CONFIG
//{
//    __s32       ms;             //the jumped time point in ms
//    __s16       flag;           //
//    __s16       dir;            //search direction, >0 search forward, <=0 search backward
//    __s16       mode;           //0--goto next key frame, 1--goto last key frame��
                                //�����last������ڲ��ҷ�����Եģ��ڲ��ҷ�����
                                //�ķ������һ��key frame,�����search forward��
                                //���ǻ��˵�һ��key frame�������search backward��������ǰ����һ��key frame
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
    __s32   stream_num;         //ָ����aud_stream_suffix_map_array����Ч����   
    __s32   stream_suffix_map_array[CEDARLIB_MAX_AUDIO_STREAM_NUM]; //��Ƶ���±�ŵ�ӳ�䣬�����±���Ƕ��ϲ�ģ�����Ƶ���±꣬����ֵ���ǿ����Ƶ���±ꡣӳ�䵽�ļ�������
}__suffix_map_t;
typedef struct tag_PsrVideoMediaFileInfoInternal
{
    __media_file_info_internal_t    MediaInfo;
    __suffix_map_t  audio_stream_suffix_map;    //����������
}PsrVideoMediaFileInfoInternal;


typedef enum __FILE_PARSER_RETURN_VAL
{
    FILE_PARSER_RETURN_OK          = FILE_DEPACK_ERR_NONE              ,                                                                           

    FILE_PARSER_READ_END           = FILE_DEPACK_ERR_READ_END          ,     //read file or idx table end.�Զ�����������Ϊ��ʶ,Ҳ���Ա�ʾһ��chunk�����ݶ������ˡ�
    FILE_PARSER_BUF_LACK           = FILE_DEPACK_ERR_BUF_LACK          ,     //BUFFER is not enough to contain data.                                              
    FILE_PARSER_GET_NONE           = FILE_DEPACK_ERR_GET_NONE          ,     //read file success for (>0) bytes, but get nothing.                                 
    FILE_PARSER_ERROR_IGNORE       = FILE_DEPACK_ERR_ERROR_IGNORE      ,     //some exception happens, but can continue process.                                  
    FILE_PARSER_PAUSE_GET_DATA     = FILE_DEPACK_ERR_PAUSE_GET_DATA    ,     //pause get data from file, when in FF,RR mode,�Ѿ������ˡ�                          
    FILE_PARSER_ERR_VIDEO_BUF_FULL = FILE_DEPACK_ERR_VIDEO_BUF_FULL,     //��ʾPSR_***��Ҫ�����video bufferȥװ�����ݣ�Ŀǰ��PSR_TSʹ��                      
    FILE_PARSER_ERR_AUDIO_BUF_FULL = FILE_DEPACK_ERR_AUDIO_BUF_FULL,     //��ʾPSR_***��Ҫ�����audio bufferȥװ�����ݣ�Ŀǰ��PSR_TSʹ��                      

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

