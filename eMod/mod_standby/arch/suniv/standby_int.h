/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                             	Standby system
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : standby_int.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-2-22
* Descript: interrupt operations for standby.
* Update  : date                auther      ver     notes
*           2012-2-22 13:15:34	Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __STANDBY_INT_H__
#define __STANDBY_INT_H__

#include "standby_cfg.h"


//define interrupt source
enum interrupt_source_e
{
    INT_SOURCE_EXTNMI   = 0,
    INT_SOURCE_IR0      = 5,
    //INT_SOURCE_IR1      = 6,
    INT_SOURCE_KEYPAD   = 21,
    INT_SOURCE_TIMER0   = 22,
    INT_SOURCE_TIMER1   = 23,
    INT_SOURCE_ALARM    = 24,
    INT_SOURCE_PIO      = 28,
    INT_SOURCE_TOUCHPNL = 29,
    INT_SOURCE_LRADC    = 31,
    INT_SOURCE_SDMMC0   = 32,
    INT_SOURCE_SDMMC1   = 33,
    INT_SOURCE_SDMMC2   = 34,
   
    INT_SOURCE_USB0     = 38,
    INT_SOURCE_USB1     = 39,
    INT_SOURCE_USB2     = 40,
};


//define register for interrupt controller
struct standby_int_reg_t
{
    volatile __u32   Vector;
    volatile __u32   BaseAddr;
    volatile __u32   reserved0;
    volatile __u32   NmiCtrl;

    volatile __u32   IrqPend[3];
    volatile __u32   reserved1;

    volatile __u32   FiqPend[3];
    volatile __u32   reserved2;

    volatile __u32   TypeSel[3];
    volatile __u32   reserved3;

    volatile __u32   IrqEn[3];
    volatile __u32   reserved4;

    volatile __u32   IrqMask[3];
    volatile __u32   reserved5;

    volatile __u32   IrqResp[3];
    volatile __u32   reserved6;

    volatile __u32   IrqForce[3];
    volatile __u32   reserved7;

    volatile __u32   IrqPrio[6];
};

extern __s32 standby_int_init(void);
extern __s32 standby_int_exit(void);
extern __s32 standby_enable_int(enum interrupt_source_e src);
extern __s32 standby_disable_int(enum interrupt_source_e src);
extern __s32 standby_query_int(enum interrupt_source_e src);


#endif  //__STANDBY_INT_H__

