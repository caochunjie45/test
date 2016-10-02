/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                               standby module
*
*                                   (c) Copyright 2008-2009, kevin China
*                                            All Rights Reserved
*
* File    : standby_sdmmc.c
* By      : A-GAN
* Version : V1.0
* Date    : 2013-5-3 17:33
*********************************************************************************************************
*/
#include "standby_sdmmc.h"

#if(ALLOW_SD_WAKEUP_STANDBY)


__u32 before_sta;
__u32 after_sta;

void  standby_sdmmc_init(void)
{
	volatile __u32 *base;
	__u32  data;	

	
	base =(volatile __u32 *) 0xF1C208E8;	
	data = *base;

	before_sta = (data &(1 << 0));

	return ;
}
void  standby_sdmmc_exit(void)
{
	return ;
}	
__s32 standby_query_sdmmc_event(void)
{

	__s32 ret;

	__u32 *base;
	__u32  data;

	base =(__u32 *) 0xF1C208E8;	
	data = *base;

	after_sta = (data &(1 << 0));

		
	if(after_sta != before_sta)
	{
		ret = EPDK_TRUE;
	}
	else
	{
		ret = EPDK_FALSE;
	}
		
	return ret;
}	

#endif

