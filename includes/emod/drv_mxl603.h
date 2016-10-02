/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                               hello world sample
*
*                                (c) Copyright 2006-2007, Steven.ZGJ China
*                                            All    Rights Reserved
*
* File    : drv_fm.h
* By      : jerry
* Version : V1.00
* Date    : 2009-5
*********************************************************************************************************
*/
#ifndef  _DRV_MXL603_H_
#define  _DRV_MXL603_H_


typedef enum  __DRV_MXL603_CMD_SET
{
    DRV_MXL603_CMD_NONE              = 0,
    DRV_MXL603_CMD_INIT                 ,
    DRV_MXL603_CMD_EXIT                 ,

    DRV_MXL603_CMD_LOCKFRE         ,
    DRV_MXL603_CMD_UNLOCKFRE     ,
    DRV_MXL603_CMD_RXPOWER      ,


}__drv_mxl603_cmd_set_t;



#endif   /*_DRV_FM_H_*/
