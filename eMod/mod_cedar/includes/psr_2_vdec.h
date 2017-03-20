/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, kevin China
*                                              All Rights Reserved
*
* File   : psr_2_vdec.h
* Version: V1.0
* By     : kevin.z
* Date   : 2008-6-3 16:19
    [cedarlib]
*********************************************************************************************************
*/
#ifndef _PSR_2_VDEC_H_
#define _PSR_2_VDEC_H_

//#include "mod_cedar_i.h"

typedef struct __VDEC_RMG2_INIT_INFO
{
    int        bUMV;
    int        bAP;
    int        bAIC;
    int        bDeblocking;
    int        bSliceStructured;
    int        bRPS;
    int        bISD;
    int        bAIV;
    int        bMQ;
    int        iRvSubIdVersion;
    unsigned short        uNum_RPR_Sizes;
    unsigned short        FormatPlus;
    unsigned int        RmCodecID;

}__vdec_rmg2_init_info;


#define STC_ID_VALID_CTRL           (1<<0)      
#define PTS_VALID_CTRL              (1<<1)      //���δ��͵�������ݵ���ʼPTS
#define FRM_BORDER_CTRL             (1<<2)      //���δ��͵����ݿ��Ƿ��Ǿ�ȷ��һ��video frame
#define FIRST_PART_CTRL             (1<<3)      //���δ��͵����ݿ����ʼ���λ
#define LAST_PART_CTRL              (1<<4)      //���δ��͵����ݿ�Ľ������λ
#define SLICE_STRUCTURE_CTRL        (1<<5)
#define MULTIPLE_FRAME_CTRL         (1<<6)
#define FRM_DEC_MODE_VALID_CTRL     (1<<7)
#define FRM_RATE_CTLR               (1<<8)
#define BROKEN_SLICE_FLAG_CTRL      (1<<9)
//bit10~bit12, ldecռ��. [psr_2_ldec.h]
//bit13, 1<<13�Ѿ���ts_psrʹ����, �ο�tspsr��updata_data() 1<<13, ��vdecͨ����.
//bit14, dmx_data.hռ��,���I-frame��h264����ʹ��.
//ע��psr_2_vdec.h, psr_2_adec.h, psr_2_ldec.h, dmx_data.h���ж���ctrlbits,����Ҫ��ͻ!

typedef struct VDecDataCtrlBlk
{
    unsigned short       CtrlBits;
        //bit 0:    0 -- STC_ID section not valid,
        //          1 -- STC_ID section valid;
        //bit 1:    0 -- PTS section not valid,
        //          1 -- PTS section valid;
        //bit 2:    0 -- frame border not known,
        //          1 -- frame border known;
        //bit 3:    0 -- this data block is not the first part of one frame,
        //          1 -- this data block is the first part of one frame,
        //          bit 3 is valid only when bit 2 is set;
        //bit 4:    0 -- this data block is not the last part of one frame,
        //          1 -- this data block is the last part of one frame,
        //          bit 4 is valid only when bit 2 is set;
        //bit 5:    0 -- the frame data is not in slice structure mode,
        //          1 -- the frame data is in slice structure mode;
        //bit 6:    0 -- single frame data,
        //          1 -- multiple frame data;
        //bit 7:    0 -- frame decode mode is invalid,
        //          1 -- frame decode mode is valid;
        //bit 8:    0 -- frame rate is invalid,
        //          1 -- frame rate is valid;
        //bit 9:    0 -- broken slice flag invalid,
        //          1 -- broken slice flag valid;
        //others:   reserved

    unsigned char        uStcId;
        //0-STC 0, 1-STC 1

    unsigned char        uFrmDecMode;
        //frame decode mode, normal play mode forward/backward mode

    unsigned char        curDataNum;
        //current data number, for distinguish the sequential 2 packet bitstream for mpeg2

    long long       uPTS;
        //Corresponding PTS value for a data block, [63,0] of actual PTS, based on micro-second

    unsigned int       uFrmRate;
        // frame rate, the frame rate maybe known till get bitstream to send to decoder

} __vdec_datctlblk_t;


typedef struct __VDEC_MPEG4_VBS_INF //���е���Ƶ�����ʽ����������ݽṹ���ݣ���������mpeg4
{
    //video codec format information
    __video_codec_format_t  vCodecInf;

    //rmg2 vbs init information
    __vdec_rmg2_init_info   vRmg2Inf;

} __vdec_mpeg4_vbs_inf_t;


typedef union __PSR_2_VDEC_MSG
{
    //put every type which need be transfer as message here
    __vdec_datctlblk_t      vbsctlinf;      //video bitstream block information
    __vdec_mpeg4_vbs_inf_t  mpeg4vbsinf;    //mpeg4 video bitstream decode information,ʵ����û���õ���������vbs_inf�ķ����ǰ�tmpAviPsr->vFormat(__video_codec_format_t)�ĵ�ַ����video_frm->msg,Ȼ��ͰѸ�socket����vdec��.
    unsigned int                   status;         //plug-in current status, play/ff/rr...

} __psr_2_vdec_msg_t;


typedef enum __PSR_2_VDEC_MSG_TYPE
{
    /* NONE MESSAGE */
    PSR2VDEC_MSG_TYPE_NONE=0,           //no message

    /* Message from video file parser to video decoder*/
    PSR2VDEC_MSG_TYPE_RESERVE=CEDAR_PSR2VDEC_MSG_BASE,

    PSR2VDEC_MSG_TYPE_VBSINF,           //mpeg4 video bitstream decode information,             msg��Ӧ__video_codec_format_t*, ֻ��4���ֽ�
    PSR2VDEC_MSG_TYPE_VBSBLKINF,        //the information of current data block, border and pts,msg��Ӧ���ݽṹ__vdec_datctlblk_t of __psr_2_vdec_msg_t

    PSR2VDEC_MSG_TYPE_DATAEND,          //video bitstream data end,                             msg��Ӧunsigned int of __psr_2_vdec_msg_t

    PSR2VDEC_MSG_TYPE_QUERYBUF,         //query free buffer for store video bitstream data
    PSR2VDEC_MSG_TYPE_FREEBUF,          //the free buffer information

    /* Message from vide decoder to video file parser */
    PSR2VDEC_MSG_TYPE_DECIFRM,          //video decoder decode an I frame
    PSR2VDEC_MSG_TYPE_DECEND,           //video decoder decode all video bitstream finish

    PSR2VDEC_MSG_TYPE_FATAL_ERR,        //fatal error
    PSR2VDEC_MSG_TYPE_SYNCSTAT,         //sync status change between parser and decoder

    PSR2VDEC_MSG_TYPE_

} __psr_2_vdec_msg_type_t;


#endif /*_PSR_2_VDEC_H_*/
