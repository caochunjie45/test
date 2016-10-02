/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              ir keyboard driver
*
*                                (c) Copyright 2006-2007, Steven.ZGJ China
*                                            All    Rights Reserved
*
* File    : drv_irkey_i.h
* By      : jerry
* Version : V1.00
*********************************************************************************************************
*/
#ifndef _DRV_POWER_I_H_
#define _DRV_POWER_I_H_

#include "epdk.h"
#include "199_def.h"

typedef struct  __CURRENT_LEVEL_T
{
    __u32       level1;
    __u32       level2;
    __u32       level3;
    __u32       level4;
    __u32       level5;
    __u32       level6;
    __u32       level7;
    __u32       level8;
    __u32       level9;
    __u32       level10;
    __u32       level11;
    __u32       level12;
    __u32       level13;
    __u32       level14;
    __u32       level15;
    __u32       level16;
    __u32       level17;
    __u32       level18;
    __u32       level19;
    __u32       level20;
} __current_level_t;

typedef struct  __SYS_P1CURRENT_CFG_T
{
    __current_level_t       videocurrent;
    __current_level_t       picturecurrent;
    __current_level_t       ebookcurrent;
    __u32       standbycurrent;
    __u32       musiccurrent;
}
__sys_p1current_cfg_t;


typedef struct STRUCT_POWER_DEV
{
    __u8            mid;
    __s32           hPass;
    ES_FILE        *iic_file;
    ES_FILE        *key_file;
}__powerdev_t;

typedef struct STRUCT_POWER_DRV
{
    __u8            mid;
    __u8            used;
    __hdle          hReg;
    __powerdev_t    power_dev;
}__power_drv_t;

extern __dev_devop_t    power_20_dev_ops;
extern __dev_devop_t    power_19_dev_ops;
extern __dev_devop_t    power_18_dev_ops;
extern __dev_devop_t    nopower_dev_ops;

extern __s32 DEV_Power_18_Ioctrl(__hdle hPower, __u32 cmd, __s32 aux, void *pbuffer);
extern __s32 DEV_Power_19_Ioctrl(__hdle hPower, __u32 cmd, __s32 aux, void *pbuffer);
extern __s32 DEV_Power_20_Ioctrl(__hdle hPower, __u32 cmd, __s32 aux, void *pbuffer);

extern void P1_OpLock(void);
extern void P1_OpUnlock(void);

__s32 DRV_Power_MInit(void);
__s32 DRV_Power_MExit(void);
__mp* DRV_Power_MOpen(__u32 mid, __u32 mode);
__s32 DRV_Power_MClose(__mp *mp);
__u32 DRV_Power_MRead(void *pdata, __u32 size, __u32 n, __mp *mp);
__u32 DRV_Power_MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp);
__s32 DRV_Power_MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer);

#endif /*_DRV_POWER_I_H_*/


