/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                             	Standby system
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : standby_dram.c
* By      : Sunny
* Version : v1.0
* Date    : 2012-2-22
* Descript: dram operations for standby.
* Update  : date                auther      ver     notes
*           2012-2-22 13:42:52	Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#include "standby_i.h"

#define 		DRAM_BASE_ADDR      (0xc2000000)
#define 		DRAM_TRANING_SIZE   (16)
static __u32 	DramTraningAreaBack[DRAM_TRANING_SIZE];


extern void  dram_power_save_process(void);
extern __u32 dram_power_up_process(void);

__s32 standby_dram_enable_selfrefresh(void)
{
	
	//backup dram traning area
    standby_memcpy((char *)DramTraningAreaBack, 
    			   (char *)DRAM_BASE_ADDR,
    			   sizeof(DramTraningAreaBack));
	
	//dram enter selfrefresh
	dram_power_save_process();
	return 0;
}

__s32 standby_dram_disable_selfrefresh(void)
{
	//dram exit selfrefresh
	dram_power_up_process();
	
	//restore dram traning area
    standby_memcpy((char *)DRAM_BASE_ADDR, 
    			   (char *)DramTraningAreaBack, 
    			   sizeof(DramTraningAreaBack));
	return 0;
}
