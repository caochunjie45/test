/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009,Kevin.z China
*                                              All Rights Reserved
*
* File   : muxer.h
* Version: V1.0
* By     : kevin
* Date   : 2009-3-22 11:09
*********************************************************************************************************
*/
#ifndef _MUXER_H_
#define _MUXER_H_


typedef enum __FILE_MUXER_COMMAND
{
    GINKGO_MUXER_CMD_START=0x00,        /* 启动muxer                            */
    GINKGO_MUXER_CMD_STOP,              /* 停止muxer                            */
    GINKGO_MUXER_CMD_GET_STATUS,        /* 获取当前状态                         */

    GINKGO_MUXER_CMD_CREATE_FILE = 0x20,/* 创建媒体文件及头信息                 */
    GINKGO_MUXER_CMD_CHKSPT,            /* 检查是否支持当前媒体文件             */
    GINKGO_MUXER_CMD_GET_ABSSKT,        /* 获取输入音频流通道                   */
    GINKGO_MUXER_CMD_GET_VBSSKT,        /* 获取输入视频流通道                   */
    GINKGO_MUXER_CMD_SET_MSGQ,          /* 设置消息通道                         */
    GINKGO_MUXER_CMD_SET_ABSHDR,        // 设置abshdr信息, aux = validlen, pbuffer = pabshdr

    GINKGO_MUXER_CMD_OPEN = 0x40,         //分配资源                              
    GINKGO_MUXER_CMD_CLOSE,               //释放资源                             
    GINKGO_MUXER_CMD_SET_FMTTYPE,         //设置 fmttype                          

    GINKGO_MUXER_CMD_SET_FOP = 0x50,     // preview也使用.   设置用户自定义文件操作集合

    GINKGO_MUXER_CMD_

} __file_muxer_command_t;


//typedef enum __FILE_MUXER_RETURN_VAL
//{
//    FILE_PARSER_RETURN_OK           = 0,    //no error
//
//    FILE_PARSER_PARA_ERR            = -1,   //some parameter error
//    FILE_PARSER_CREATE_FILE_FAIL    = -2,   //create media file failed
//    FILE_PARSER_WRITE_FILE_FAIL     = -3,   //write media file data failed
//    FILE_PARSER_NO_AV               = -5,   //neither audio nor video bitstream need mux
//    FILE_PARSER_REQMEM_FAIL         = -7,   //request memory failed
//
//    FILE_PARSER_
//
//} __file_parser_return_val_t;

typedef struct CODED_FRAME_INFO
{
    __s32       node_id;  //表示编码库内部的coded frame数组的元素下标号.
    __u8        *pData0;
    __s32       uSize0;
    __u8        *pData1;
    __s32       uSize1;
    __s64       pts;
    __s8        pts_valid;
    //__bool      isMultipleFrame;
    //__u8        stc_id;
    //__s8        frm_dec_mode;
    //__u32       stream_len;         // G2 need this field.
    __s32       uLen0;       //buffer中的有效数据的长度,audio使用，video使用uSize,因为一定会全部用完。
    __s32       uLen1;       //buffer中的有效数据的长度
    __u8		*privateData;
	__s32		privateDataLen;
} __coded_frame_info_t;

#endif /*_MUXER_H_*/
