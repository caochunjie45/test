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
#ifndef  _DRV_ATBM_H_
#define  _DRV_ATBM_H_


typedef enum  __DRV_ATBM_CMD_SET
{
    DRV_ATBM_CMD_NONE              = 0,
    DRV_ATBM_CMD_INIT                 ,
    DRV_ATBM_CMD_EXIT                 ,

    DRV_ATBM_CMD_PASSON		,          
    DRV_ATBM_CMD_PASSOFF		,          

    DRV_ATBM_CMD_LOCKCHECK		,          

}__drv_atbm_cmd_set_t;



#endif   /*_DRV_FM_H_*/
