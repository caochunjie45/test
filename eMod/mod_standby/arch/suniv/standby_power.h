/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                             	Standby system
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : standby_power.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-2-22
* Descript: power operations for standby.
* Update  : date                auther      ver     notes
*           2012-2-22 14:48:41	Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __STANDBY_POWER_H__
#define __STANDBY_POWER_H__

#include "standby_cfg.h"

typedef enum __power_vol_type_e
{
    STANDBY_POWER_VOL_DCDC1,
    STANDBY_POWER_VOL_DCDC2,
    STANDBY_POWER_VOL_DCDC3,
    STANDBY_POWER_VOL_LDO1,
    STANDBY_POWER_VOL_LDO2,
    STANDBY_POWER_VOL_LDO3,
    STANDBY_POWER_VOL_LDO4,
} power_vol_type_e;

#define AXP_ADDR        (0x34)
#define AXP_IICBUS      (0)
#define AXP20_LDO1      (0x00)
#define AXP20_LDO2      (0x28)
#define AXP20_LDO3      (0x29)
#define AXP20_LDO4      (0x28)
#define AXP20_BUCK2     (0x23)
#define AXP20_BUCK3     (0x27)

#define AXP20_IRQEN1    (0x40)
#define AXP20_IRQEN2    (0x41)
#define AXP20_IRQEN3    (0x42)
#define AXP20_IRQEN4    (0x43)
#define AXP20_IRQEN5    (0x44)

#define AXP20_IRQ1      (0x48)
#define AXP20_IRQ2      (0x49)
#define AXP20_IRQ3      (0x4A)
#define AXP20_IRQ4      (0x4B)
#define AXP20_IRQ5      (0x4C)

#define AXP20LDO1       1300

struct axp_info 
{
    power_vol_type_e 	id;
    int     			min_uV;
    int     			max_uV;
    int     			step_uV;
    int     			vol_reg;
    int     			vol_shift;
    int     			vol_nbits;
};


#define AXP_WAKEUP_KEY          (1<<0)
#define AXP_WAKEUP_LOWBATT      (1<<1)
#define AXP_WAKEUP_USB          (0<<2)
#define AXP_WAKEUP_AC           (0<<3)

#define AXP_WAKEUP	(AXP_WAKEUP_KEY | AXP_WAKEUP_LOWBATT)


typedef enum __STANDBY_POWER_TYPE
{
    STANDBY_POWER_KEY_SHORT     = 1,
    STANDBY_POWER_KEY_LONG      = 2,
    STANDBY_POWER_LOW_POWER     = 3,

} __standby_power_type_e;

extern __s32 standby_power_init(void);
extern __s32 standby_power_exit(void);
extern void  standby_set_voltage(power_vol_type_e type, __s32 voltage);
extern __u32 standby_get_voltage(power_vol_type_e type);


#endif  /* __STANDBY_POWER_H__ */


