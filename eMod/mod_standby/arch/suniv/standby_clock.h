/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                             	Standby system
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : standby_clock.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-2-22
* Descript: clock operations for standby.
* Update  : date                auther      ver     notes
*           2012-2-22 14:07:58	Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __STANDBY_CLOCK_H__
#define __STANDBY_CLOCK_H__

#include "standby_cfg.h"
#include "standby_clock_i.h"

struct sun4i_clk_div_t 
{
    __u32   cpu_div:4;      //division of cpu clock, divide core_pll 
    __u32   axi_div:4;      //division of axi clock, divide cpu clock
    __u32   ahb_div:4;      //division of ahb clock, divide axi clock
    __u32   apb_div:4;      //division of apb clock, divide ahb clock
    __u32   ahb_src:2;
    __u32   reserved:14;
};

__s32 gate_clock_init(void);
__s32 gate_clock_exit(void);



__s32 standby_clk_init(void);
__s32 standby_clk_exit(void);
__s32 standby_clk_core2losc(void);
__s32 standby_clk_core2hosc(void);
__s32 standby_clk_core2pll(void);
__s32 standby_clk_plldisable(void);
__s32 standby_clk_pllenable(void);
__s32 standby_clk_hoscdisable(void);
__s32 standby_clk_hoscenable(void);
__s32 standby_clk_ldodisable(void);
__s32 standby_clk_ldoenable(void);
__s32 standby_clk_setdiv(struct sun4i_clk_div_t  *clk_div);
__s32 standby_clk_getdiv(struct sun4i_clk_div_t  *clk_div);
void  standby_clk_dramgating(int onoff);
__s32 standby_clk_apb2losc(void);
__s32 standby_clk_apb2hosc(void);

extern __u32   cpu_ms_loopcnt;

#endif  //__STANDBY_CLOCK_H__

