/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                    (c) Copyright 2008-2009, Kevin.Z China
*                                              All Rights Reserved
*
* File   : drv_adec.h
* Version: V1.0
* By     : Kevin.Z
* Date   : 2008-9-26 11:49
*********************************************************************************************************
*/
#ifndef _DRV_ADEC_H_
#define _DRV_ADEC_H_
typedef enum __AUDIO_DECODE_DRV_COMMAND
{                                           // command list for audio decode driver
    DRV_ADEC_CMD_START=0x00,                // start audio decode driver
    DRV_ADEC_CMD_STOP,                      // stop audio decode driver
    DRV_ADEC_CMD_PAUSE,                     // pause audio decode driver
    DRV_ADEC_CMD_CONTI,                     // continue audio decode driver
    DRV_ADEC_CMD_FF,                        // fast forward command
    DRV_ADEC_CMD_RR,                        // fast reverse command
    DRV_ADEC_CMD_CLEAR_FFRR,                // clear fast forward/reverse
    DRV_ADEC_CMD_GET_STATUS,                // get audio deocde driver status, aux = 1表示一定要返回一个稳定状态
    DRV_ADEC_CMD_JUMP,                      // jump, aux = (__u32)destinate time, pbuffer = 1代表要求精确跳播
    DRV_ADEC_CMD_CLEAR_JUMP,                // clear jump

    DRV_ADEC_CMD_PREPARE_FF,                //play->pause, 所谓准备,就是清vbs buffer,清的方法是设置adeclib新的状态，然后调一次decode()接口.
    DRV_ADEC_CMD_PREPARE_RR,                //play->pause
    DRV_ADEC_CMD_PREPARE_CLEAR_FFRR,        //ffrr->pause

    DRV_ADEC_CMD_OPEN,                      // 分配资源
    DRV_ADEC_CMD_CLOSE,                     // 释放资源
    DRV_ADEC_CMD_SET_CEDARINI,              //设置cedarini
    
    DRV_ADEC_CMD_SET_DATAINPUT=0x20,        // set data input
    DRV_ADEC_CMD_QUERY_ABSBUFFER,           // query free buffer
    DRV_ADEC_CMD_DATA_END,                  // data end
    DRV_ADEC_CMD_QUERY_ABSBUFFER_USAGE,     //查询abs buffer的使用率，即当前装载的数据和总buffer的百分比，ret = 0~100, 100代表100%

    DRV_ADEC_CMD_GET_BITRATE=0x30,          // get audio bit rate
    DRV_ADEC_CMD_GET_BITSPERSAMPLE,         // get bits count per sample
    DRV_ADEC_CMD_GET_FS,                    // get aduio sample rate
    DRV_ADEC_CMD_GET_CHN,                   // get channel count
    DRV_ADEC_CMD_GET_PCMBUFSIZE,            // get pcm buffer size for request memory
    DRV_ADEC_CMD_GET_DATA_FRM_SIZE,         // get data frame size, for sync data
    DRV_ADEC_CMD_SET_WITH_VIDEO,            // set play with video flag
    DRV_ADEC_CMD_GET_RDRTYPE,               // get audio render type

    DRV_ADEC_CMD_SET_CB_WAITABS=0x40,       // set call back function, wait audio bitstream data
    DRV_ADEC_CMD_SET_CB_GETPCMBUF,          // set call back function, get pcm buffer
    DRV_ADEC_CMD_SET_CB_RELEASEPCMDATA,     // set call back function, release pcm data
    DRV_ADEC_CMD_SET_CB_SENDMSG,            // set call back function, send message frame
    DRV_ADEC_CMD_SET_CB_SETTIME,            // set call back function, set audio play time
    DRV_ADEC_CMD_SET_CB_FREEABSBUF,         // set call back function, free audio bitstream buffer
    DRV_ADEC_CMD_SET_ABSDECINF,             // set audio bitstream information for decode
    DRV_ADEC_CMD_SET_TAGINF,                // set tag information for decode
    DRV_ADEC_CMD_SET_CLEAR_STUFDATA,        // set message that stuffing data has been cleared

    DRV_ADEC_CMD_SET_EQ=0x50,               // set eq
    DRV_ADEC_CMD_GET_EQ,                    // set eq
    DRV_ADEC_CMD_SET_VPS,                   // set variable play speed, 目前已经不再使用它, 变速另有一套机制, adrv不管变速了.
    DRV_ADEC_CMD_GET_VPS,                   // get variable play speed, 目前已经不再使用它

    //adrv所处理的ab播放是FSEEK方式的ab播放，jump方式不需要adrv处理
    DRV_ADEC_CMD_SETPOINT_A,                // set A point for A-B play, aux = ms，实际上adrv不需要上面给出a点的PTS。
    DRV_ADEC_CMD_SETPOINT_B,                // set B point for A-B play, 老版本是enableABplay,新版本只是记录上层程序要求的B点的PTS。
    DRV_ADEC_CMD_CLEAR_AB,                  // clear A-B point
    DRV_ADEC_CMD_ENABLE_AB,                 // enable A-B play
    DRV_ADEC_CMD_SET_AB_LOOPCNT,            // set A-B play loop count
    DRV_ADEC_CMD_GET_AB_LOOPCNT,            // get A-B play loop count
    //DRV_ADEC_CMD_SET_AB_MODE,               // aux = ADRV_AB_MODE_FSEEK
    
    DRV_ADEC_CMD_SET_SPECTRUM,              // set if need get spectrum
    DRV_ADEC_CMD_GET_SPECTRUM,              // get spectrum
    DRV_ADEC_CMD_SET_FFRRSPEED,             // set fast forward or backward speed
    DRV_ADEC_CMD_GET_TAG,                   // get audio tag

    DRV_ADEC_CMD_

} __audio_decode_drv_command_t;


#endif /* _DRV_VDEC_H_ */

