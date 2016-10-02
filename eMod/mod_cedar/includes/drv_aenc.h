/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                    (c) Copyright 2008-2009, Kevin.Z China
*                                              All Rights Reserved
*
* File   : drv_aenc.h
* Version: V1.0
* By     : kevin.z
* Date   : 2009-3-22 11:23
*********************************************************************************************************
*/
#ifndef _DRV_AENC_H_
#define _DRV_AENC_H_

typedef enum __AUDIO_ENCODE_DRV_COMMAND
{                                           // command list for audio encode driver
    DRV_AENC_CMD_START=0x00,                // start audio encode driver
    DRV_AENC_CMD_STOP,                      // stop audio encode driver

    DRV_AENC_CMD_SET_PCMINPUT=0x20,         // set pcm data input
    DRV_AENC_CMD_QUERY_PCMBUF,              // query free pcm buffer
    DRV_AENC_CMD_GET_STATUS,                // get audio encoder driver status
    DRV_AENC_CMD_SET_ABSENCINF,             // set audio bitstream information for encode
    DRV_AENC_CMD_GETTIME,                   // get current encode time
    DRV_AENC_CMD_GET_ABSHDR,                // 得到头信息, pbuffer = pabshdr; aux = bufsize, ret = validlen

    DRV_AENC_CMD_SET_CB_WAITPCM=0x40,       // set call back function, wait audio pcm sample data
    DRV_AENC_CMD_SET_CB_GETABSBUF,          // set call back function, get audio bitstream buffer
    DRV_AENC_CMD_SET_CB_RELABSDATA,         // set call back function, release audio bitstream data
    DRV_AENC_CMD_SET_CB_SENDMSG,            // set call back function, send message frame
    DRV_AENC_CMD_SET_CB_FREEPCMBUF,         // set call back function, free audio pcm buffer

    DRV_AENC_CMD_

} __audio_encode_drv_command_t;


#endif /* _DRV_AENC_H_ */

