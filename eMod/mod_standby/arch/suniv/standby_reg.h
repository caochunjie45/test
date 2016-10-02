/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                               standby module
*
*                                   (c) Copyright 2008-2011, kevin China
*                                            All Rights Reserved
*
* File    : standby_reg.h
* By      : kevin
* Version : V1.0
* Date    : 2011-1-18 11:01
*********************************************************************************************************
*/
#ifndef __STANDBY_REG_H__
#define __STANDBY_REG_H__

#include "mod_standby_i.h"

#define STANDBY_RREG(reg)           (*(volatile __u32 *)(reg))
#define STANDBY_WREG(reg, val)      (*(volatile __u32 *)(reg) = val)

#define STANDBY_CCU_REG_BASE    (0xf1c20000)
#define STANDBY_TWI0_REG_BASE   (0xf1c2ac00)
#define STANDBY_TWI1_REG_BASE   (0xf1c2b000)
#define STANDBY_TWI2_REG_BASE   (0xf1c2b400)
#define STANDBY_TMR_REG_BASE    (0xf1c20c00)
#define STANDBY_KEY_REG_BASE    (0xf1c22800)
#define STANDBY_IR_REG_BASE     (0xf1c21800)
#define STANDBY_INT_REG_BASE    (0xf1c20400)
#define STANDBY_USB0_REG_BASE   (0xf1c13000)
#define STANDBY_USB1_REG_BASE   (0xf1c14000)
#define STANDBY_USB2_REG_BASE   (0xf1c1c000)
#define STANDBY_DMA_REG_BASE    (0xf1c02000)
#define STANDBY_DRAM_REG_BASE   (0xf1C01000)

//dram
#define	SW_VA_DRAM_IO_BASE		STANDBY_DRAM_REG_BASE
#define	SW_VA_CCM_IO_BASE		STANDBY_CCU_REG_BASE
#define	SW_VA_TIMERC_IO_BASE	STANDBY_TMR_REG_BASE

#endif  //__STANDBY_REG_H__

