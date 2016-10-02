/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           hello world sample
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : magic.c
* By      : Steven.ZGJ
* Version : V1.00
*********************************************************************************************************
*/
#include "drv_mcu_i.h"

#pragma arm section rodata="MAGIC"
const __mods_mgsec_t modinfo =
{
    {'e','P','D','K','.','m','o','d'},		//.magic
    0x01000000,                				//.version
    EMOD_TYPE_DRV_MCU,                       //.type
    0xF0000,                                //.heapaddr
    0x400,                      			//.heapsize
	{                                       //.mif
	    &DRV_MCU_MInit,
	    &DRV_MCU_MExit,
	    &DRV_MCU_MOpen,
	    &DRV_MCU_MClose,
	    &DRV_MCU_MRead,
    	&DRV_MCU_MWrite,
	    &DRV_MCU_MIoctrl
	}
};
#pragma arm section
