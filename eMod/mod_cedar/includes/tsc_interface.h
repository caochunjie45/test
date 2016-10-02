/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, Kevin.z China
*                                              All Rights Reserved
*
* File   : tsc.h
* Version: V1.0
* By     : Kevin.Z
*********************************************************************************************************
*/
#ifndef _TSC_INTERFACE
#define _TSC_INTERFACE

//#include "maple_common.h"

#define TSC_DRIVER_FILE_PATH  CEDAR_PLUGINS_DIRECTORY"tsc.drv"

#define MAX_CHAN_NUM                32

#define PID_ALL                     8192
#define PID_PAT                     0

#define VIDEO_CHAN_BASE             (0)
#define MAX_VIDEO_CHAN              (1)

#define AUDIO_CHAN_BASE             (VIDEO_CHAN_BASE + MAX_VIDEO_CHAN)
#define MAX_AUDIO_CHAN              (1) //1, 2

#define SUBTITLE_CHAN_BASE          (AUDIO_CHAN_BASE + MAX_AUDIO_CHAN)
#define MAX_SUBTITLE_CHAN           (1) //1, 2

#define SECTION_CHAN_BASE           (SUBTITLE_CHAN_BASE + MAX_SUBTITLE_CHAN)
#define MAX_SECTION_CHAN            (12)

#define TS_DATA_CHAN_BASE           (SECTION_CHAN_BASE + MAX_SECTION_CHAN)
#define MAX_TS_DATA_CHAN            (15)    //15, 13

#define VIDEO_NOTIFY_PKT_NUM_HW     (1024)//(512)//sw//(1024)//hw
#define VIDEO_NOTIFY_PKT_NUM_SW     (512)//(512)//sw//(1024)//hw
#define AUDIO_NOTIFY_PKT_NUM        (128)   //(64)
#define SUBTITLE_NOTIFY_PKT_NUM     (16)    //(64), 有超时机制,不用担心
#define SECTION_NOTIFY_PKT_NUM      (4)      //(64)
#define TS_DATA_NOTIFY_PKT_NUM      (32)    //(64)


typedef enum PID_FILTER_TYPE
{
    PID_FILTER_TYPE_VIDEO,          //* notify data arrived message every 32K bytes.(174 packets).
    PID_FILTER_TYPE_AUDIO,          //* notify data arrived message every 2K bytes. (12 packets).
    PID_FILTER_TYPE_SUBTITLE,       //* notify data arrived message every 1K bytes. (6 packets).
    PID_FILTER_TYPE_SECTION,        //* notify data arrived message every 1 packets received.
    PID_FILTER_TYPE_TS_DATA,        //* notify data arrived message every 2 packets received.
    
    PID_FILTER_TYPE_NUM
}pid_filter_type_t; //目前的实际作用是根据类型决定用第几个filter.filter的buffer是事先分配好的.video, audio, subtitle的buffersize各不同.


typedef struct PID_FILTER_REGISTER
{
    pid_filter_type_t   type;           //* filter type, channel of different filter type has different buffer size.
    __u32               pid;            //* one channel has one pid.
    __pCBK_t            callback;       //* callback function, when packets is enougth, the driver will call this function. dataArriveCallBack()
    void*               callbackparam;  //* parameter passed to the callback function.一般是__krnl_event_t*     dataSem;
}filter_register_t;


typedef struct TSF_DATA
{
	__u8* pData;                        //* start address of ts packets.第一段数据,因为是ringbuffer,所以可能会回头,这样就分为两段数据.
	__u32 pktNum;                       //* packet number of the first part of ts packets.
	__u8* pRingData;                    //* start address of the second part of ts packets, 第二段数据.
	                                    //* pid channel use a loop buffer, so there may be two part of ts packets when buffer loop back.
	__u32 ringPktNum;                   //* packet number of the second part of ts packets.
	
}tsf_data_t;


typedef enum TSC_CMD
{
    //* open a pid channel to receive ts packets.
    TSC_CMD_OPEN_CHAN                 ,     //* aux = 0, pbuffer = a pointer to a variable of struct type filter_register_t;
                                            //* return value: EPDK_FAIL if fail, the channel id if success.
                                            
    //* close a pid channel to stop receiving ts packets.
    TSC_CMD_CLOSE_CHAN                ,     //* aux = the channel id returned by TSC_CMD_OPEN_CHAN io command; pbuffer = NULL;
    
    //* request ts packets from a pid channel.
    TSC_CMD_REQUEST_CHAN_DATA		  ,     //* aux = channel id, pbuffer = a pointer to a variable of struct type tsf_data_t;
    
    //* flush ts packets of a pid channel.
    TSC_CMD_FLUSH_CHAN_DATA			  ,     //* aux = channel id, pbuffer = bytes to flush.
    
    //* get the number of received packet of a pid channel.
    TSC_CMD_GET_CHAN_STATUS           ,     //* aux = channel id, pbuffer = NULL;
                                            //* return value = number of bytes.
    
    //* open pcr detection of a pid channel, the pcr value will passed through the callback function you set.
    TSC_CMD_OPEN_PCR_DETECT           ,     //* aux = pcr_pid, pbuffer = callback function(__pCBK_t), 
                                            //* the call back function receive a parameter of type 'void*', you can use it as an __u32 variable, it means the (pcr_value>>1);
                                            
    //* close pcr detection.
    TSC_CMD_CLOSE_PCR_DETECT          ,     //* aux = 0, pbuffer = NULL;

    TSC_CMD_SET_FECONFIG,                   //设置前端配置的情况, pbuffer = CFEConfig*
    
    TSC_CMD_OPEN,                           // tsc打开, IO_Close暂不需要, TSC_CMD_SET_FECONFIG必须在IO_Open之前调用.
    
    TSC_CMD_SET_DEMODDRV,					//由MAPLE主控传递下来给DEMUX再传递TSC的DEMOD驱动的句柄，TSC拿到后就可以对DEMOD驱动进行操作
    
    TSC_CMD_NUM
}tsc_cmd_t;


#endif /*_TSC_INTERFACE*/
