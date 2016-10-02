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
    int   uDatatype ;     // encode type, ref to __cedar_subtitle_encode_t,描述字幕项内容的编码类型, 如果字幕项是文本,一般就是文本的字符编码类型;如果是图像,一般就是LYRIC_SUB_ENCODE_BITMAP
    int   SubtitleType;   //__cedar_lyric_fmt_t
    char    LbsChgFlg;    //indicate lyric stream has changed.
    unsigned char    privateData[LDEC_PRIVATE_DATA_LEN];  //PES_private_data;
} __lbs_datctlblk_t;

typedef union __PSR_2_LDEC_MSG
{
    //put every type which need be transfer as message here
    __subtitle_codec_format_t      lbs_info;   //for PSR2LDEC_MSG_TYPE_LBSINF. 原来是__lbs_info_msg, 实际上作为frm->msg的数据结构,当msgtype = PSR2LDEC_MSG_TYPE_LBSINF时,传的是__subtitle_codec_format_t*
    __lbs_datctlblk_t   lbs_dat_info;// for post data chunk, PSR2LDEC_MSG_TYPE_LBSCHUNK_INF.indicate chunk's info(e.g PTS).
} __psr_2_ldec_msg_t;


typedef enum __PSR_2_LDEC_MSG_TYPE
{
    PSR2LDEC_MSG_TYPE_NONE=0,           //no message

    PSR2LDEC_MSG_TYPE_LBSINF=CEDAR_PSR2LDEC_MSG_BASE,   //PSR->ldec,告知ldec相关信息，idxsub用; ldec->psr,告知psr本字幕文件的字幕流信息,smi用.因为smi的psr并不解析文件。
                                        //lyric bitstream frame head
    //lyric bitstream data end，
    //必须在读不到数据的情况下。只要读到数据，就不能发送该命令。即使是最后一笔数据。
    PSR2LDEC_MSG_TYPE_DATAEND,
    PSR2LDEC_MSG_TYPE_FATAL_ERR,        //fatal error

    PSR2LDEC_MSG_TYPE_SUB_OFFSET,       //subtitle bitstream offset,很早以前为IDXSUB准备的,实际上idxsub的实现没有采取这种办法，所以没有用了。
    PSR2LDEC_MSG_TYPE_ACCESS_FILE,      //access subtitle file type, 很早以前为IDXSUB准备的,实际上idxsub的实现没有采取这种办法，所以没有用了。

    PSR2LDEC_MSG_TYPE_LBSCHUNK_INF,     //lbs data chunk's info
    PSR2LDEC_MSG_TYPE_SYNCSTAT,
    PSR2LDEC_MSG_TYPE_
} __psr_2_ldec_msg_type_t;


#endif /*_PSR_2_LDEC_H_*/
