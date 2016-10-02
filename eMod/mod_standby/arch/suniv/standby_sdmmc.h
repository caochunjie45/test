/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                               standby module
*
*                                   (c) Copyright 2008-2009, kevin China
*                                            All Rights Reserved
*
* File    : standby_sdmmc.h
* By      : A-GAN
* Version : V1.0
* Date    : 2013-5-3 17:33
*********************************************************************************************************
*/

#ifndef  __STANDBY_SDMMC_H__
#define   __STANDBY_SDMMC_H__ 

#include "ePDK.h"
#include "standby_cfg.h"
#include "standby_reg.h"

extern void  standby_sdmmc_init(void);
extern void  standby_sdmmc_exit(void);
extern __s32 standby_query_sdmmc_event(void);

#endif

