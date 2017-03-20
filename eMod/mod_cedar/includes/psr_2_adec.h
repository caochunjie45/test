/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, kevin China
*                                              All Rights Reserved
*
* File   : psr_2_adec.h
* Version: V1.0
* By     : kevin.z
* Date   : 2008-6-3 16:19
    [cedarlib]
*********************************************************************************************************
*/
#ifndef _PSR_2_ADEC_H_
#define _PSR_2_ADEC_H_

//#include "mod_cedar_i.h"
//#include "abs_header.h"


typedef struct __ADEC_FREEBUF_INF
{
    void        *pBuf;
    unsigned int       nBufSize;

}__adec_freebuf_inf_t;


//#define STC_ID_VALID_CTRL           (1<<0)
#define AUD_PTS_VALID_CTRL              (1<<1)
#define AUD_FRM_BORDER_CTRL             (1<<2)  //��ʾ�ϸ��ȷ��֡�߽�,һ�����ݿ����һ֡,��0��ʾ�����ϸ��ȷ��֡�߽硣���ڵ�adec��Ҫ�����ϸ��һ��audio frame������adec�����ĸ�λ����ֵ
#define AUD_FIRST_PART_CTRL             (1<<3)  //����Ҫ���͵����ݿ�(chunk)�Ŀ�ʼ,������ϸ�ȷ��֡�߽磬����һ��֡����ʼ���������λ��0����ʾҪadec���������ݿ�Ŀ�ʼ�ͽ�β��Ҳ��û������PTS�ˡ�
#define AUD_LAST_PART_CTRL              (1<<4)  //����Ҫ���͵����ݿ�(chunk)�Ľ�β��������ϸ�ȷ��֡�߽磬���Ǹ�֡�Ľ�β�������������һ֡�����֮֡���֡�Ľ�β��Ҳ���ܲ��ǽ�β�������λ��0����ʾҪadec���������ݿ�Ŀ�ʼ�ͽ�β
//#define SLICE_STRUCTURE_CTRL        (1<<5)
//#define MULTIPLE_FRAME_CTRL         (1<<6)
//#define FRM_DEC_MODE_VALID_CTRL     (1<<7)
//#define FRM_RATE_CTLR               (1<<8)
//#define BROKEN_SLICE_FLAG_CTRL      (1<<9)
//bit13, 1<<13�Ѿ���ʹ����, �ο�tspsr��updata_data() 1<<13����ʾһ����֡�Ŀ�ʼ

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
typedef struct ADecDataCtrlBlk
{
    unsigned short       CtrlBits;   //�ο�����
    //__u8        uFrmDecMode;
        //frame decode mode, normal play mode forward/backward mode
    long long       uPts;
        //Corresponding PTS value for a data block, [63,0] of actual PTS, based on micro-second,us, 
} __adec_datctlblk_t;

typedef struct __PSR_2_ADEC_MSG //union
{
    //put every type which need be transfer as message here
    __audio_codec_format_t  absfmtinf;  // audio bitstream format information
    int                   frmnum;     // audio bitstream frame number��ʵ��ʹ��ʱ������Ϊadec��Ҫ��ȡ���������ļ��е�λ�ã���λ:byte
    int                   datafrmsize;// data frame size, for sync data, û��ʹ��
    unsigned int                   stufdatsize;// stuffing data size before audio bitstream, ��Щavi�ļ��Ŀ�ʼ������Ƶ��stuff����.adec->psr������ֻ��avi_psr���õ�.
    __adec_datctlblk_t      absctlinf;      //video bitstream block information
} __psr_2_adec_msg_t;


typedef enum __PSR_2_ADEC_MSG_TYPE
{
    PSR2ADEC_MSG_TYPE_NONE=0,           // no message

    PSR2ADEC_MSG_TYPE_ABSDECINF=CEDAR_PSR2ADEC_MSG_BASE,
                                        // audio bitstream frame head
    PSR2ADEC_MSG_TYPE_ABSBLKINF,        //��Ӧfrmnum��absctlinf, the information of current data block, border and pts and so on��PSR2ADEC_MSG_TYPE_ABSBLKINF��PSR2ADEC_MSG_TYPE_FRMNUM����ͬһ�����ݽṹ
    PSR2ADEC_MSG_TYPE_TAGINF,           // tag information
    PSR2ADEC_MSG_TYPE_FRMNUM,           // ��Ӧfrmnum, audio frame number, for parser and deocder sync��һ��adec���ص�socket->msgtype����PSR2ADEC_MSG_TYPE_FRMNUM����ʾmsgЯ����������Ҫ�����������ļ��е�ƫ����
    PSR2ADEC_MSG_TYPE_QUERYABSBUF,      // query free buffer to fill audio bitstream
    PSR2ADEC_MSG_TYPE_DATAFRMSIZE,      // data frame size, for sync data between parser and decoder
    PSR2ADEC_MSG_TYPE_STUFDATA_SIZE,    // stuffing data size, dec->psr
    PSR2ADEC_MSG_TYPE_CLEAR_STUFDATA,   // parser clear stuffing data already

    PSR2ADEC_MSG_TYPE_DATAEND,          // audio bitstream end
    PSR2ADEC_MSG_TYPE_FATAL_ERR,        // fatal error
    PSR2ADEC_MSG_TYPE_SYNCSTAT,         //sync status change between parser and decoder,for jump play
    PSR2ADEC_MSG_TYPE_ADECTASK_STOP,    //ADEC֪ͨPSR:adec task�˳��ˣ�adecģ�鴦��ֹͣ״̬

    PSR2ADEC_MSG_TYPE_

} __psr_2_adec_msg_type_t;


#endif /* _PSR_2_ADEC_H_ */
