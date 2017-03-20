/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, kevin China
*                                              All Rights Reserved
*
* File   : psr_2_ldec.h
* Version: V1.0
* By     : kevin.z
* Date   : 2008-6-3 16:19
    [cedarlib]
*********************************************************************************************************
*/
#ifndef _PSR_2_LDEC_H_
#define _PSR_2_LDEC_H_

//#include "mod_cedar_i.h"

typedef enum __ACCESS_SUBFILE_DATA
{
    LDEC_ACCESS_FILE_SUB = 0,
    LDEC_ACCESS_FILE_IDX = 1,

    LDEC_ACCESS_FILE_

} __access_sub_file_data_t;

//typedef struct __LBS_INFO_MSG
//{
//    int   subtitle_src_type;  //enum __SUBTITLE_SOURCE, ext or embeded.
//}__lbs_info_msg;



//#define LDEC_PTS_VALID_CTRL             (1<<0)
//#define LDEC_DURATION_VALID_CTRL        (1<<1)
//#define LDEC_DATATYPE_VALID_CTRL        (1<<2)
//#define LDEC_SUBTYPE_VALID_CTRL         (1<<3)
//#define LDEC_FIRST_PART_CTRL            (1<<4)
//#define LDEC_LAST_PART_CTRL             (1<<5)

#define LDEC_PTS_VALID_CTRL             (1<<1)
#define LDEC_FIRST_PART_CTRL            (1<<3)
#define LDEC_LAST_PART_CTRL             (1<<4)
#define LDEC_DURATION_VALID_CTRL        (1<<10)
#define LDEC_DATATYPE_VALID_CTRL        (1<<11)
#define LDEC_SUBTYPE_VALID_CTRL         (1<<12)

#define LDEC_PRIVATE_DATA_LEN 16


typedef struct LbsDataCtrlBlk
{
    unsigned short   CtrlBits;
    long long   uPTS;           //this data chunk's PTS. ms
    long long   uPTS2;           //next data chunk's PTS. ms
    long long   uDuration;      //unit:ms
    int   uDatatype ;     // encode type, ref to __cedar_subtitle_encode_t,������Ļ�����ݵı�������, �����Ļ�����ı�,һ������ı����ַ���������;�����ͼ��,һ�����LYRIC_SUB_ENCODE_BITMAP
    int   SubtitleType;   //__cedar_lyric_fmt_t
    char    LbsChgFlg;    //indicate lyric stream has changed.
    unsigned char    privateData[LDEC_PRIVATE_DATA_LEN];  //PES_private_data;
} __lbs_datctlblk_t;

typedef union __PSR_2_LDEC_MSG
{
    //put every type which need be transfer as message here
    __subtitle_codec_format_t      lbs_info;   //for PSR2LDEC_MSG_TYPE_LBSINF. ԭ����__lbs_info_msg, ʵ������Ϊfrm->msg�����ݽṹ,��msgtype = PSR2LDEC_MSG_TYPE_LBSINFʱ,������__subtitle_codec_format_t*
    __lbs_datctlblk_t   lbs_dat_info;// for post data chunk, PSR2LDEC_MSG_TYPE_LBSCHUNK_INF.indicate chunk's info(e.g PTS).
} __psr_2_ldec_msg_t;


typedef enum __PSR_2_LDEC_MSG_TYPE
{
    PSR2LDEC_MSG_TYPE_NONE=0,           //no message

    PSR2LDEC_MSG_TYPE_LBSINF=CEDAR_PSR2LDEC_MSG_BASE,   //PSR->ldec,��֪ldec�����Ϣ��idxsub��; ldec->psr,��֪psr����Ļ�ļ�����Ļ����Ϣ,smi��.��Ϊsmi��psr���������ļ���
                                        //lyric bitstream frame head
    //lyric bitstream data end��
    //�����ڶ��������ݵ�����¡�ֻҪ�������ݣ��Ͳ��ܷ��͸������ʹ�����һ�����ݡ�
    PSR2LDEC_MSG_TYPE_DATAEND,
    PSR2LDEC_MSG_TYPE_FATAL_ERR,        //fatal error

    PSR2LDEC_MSG_TYPE_SUB_OFFSET,       //subtitle bitstream offset,������ǰΪIDXSUB׼����,ʵ����idxsub��ʵ��û�в�ȡ���ְ취������û�����ˡ�
    PSR2LDEC_MSG_TYPE_ACCESS_FILE,      //access subtitle file type, ������ǰΪIDXSUB׼����,ʵ����idxsub��ʵ��û�в�ȡ���ְ취������û�����ˡ�

    PSR2LDEC_MSG_TYPE_LBSCHUNK_INF,     //lbs data chunk's info
    PSR2LDEC_MSG_TYPE_SYNCSTAT,
    PSR2LDEC_MSG_TYPE_
} __psr_2_ldec_msg_type_t;


#endif /*_PSR_2_LDEC_H_*/
