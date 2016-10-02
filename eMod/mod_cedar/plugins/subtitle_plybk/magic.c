/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                    (c) Copyright 2008-2009, Kevin.Z China
*                                              All Rights Reserved
*
* File    : magic.c
* By      : Kevin.Z
* Version : V1.00
*********************************************************************************************************
*/
//#include "mod_cedar_ii.h"
//#include "video_plybk_s.h"
#include "ePDK.h"

#pragma arm section rodata="MAGIC"
const __mods_mgsec_t modinfo =
{
    {'e','P','D','K','.','m','o','d'},		//.magic
    0x01000000,                				//.version
#ifdef DEBUG
    0x47,  //EMOD_TYPE_PLG_VPLY
#else
    EMOD_TYPE_USER, //0x46, //EMOD_TYPE_USER,EMOD_TYPE_PLG_VPLY //.type, 0x46
#endif
    0xF0000,                                //.heapaddr
    0x400,                      			//.heapsize
	{                                       //.mif
	    &MInit,
	    &MExit,
	    &MOpen,
	    &MClose,
	    &MRead,
    	&MWrite,
	    &MIoctrl
	}
};
#pragma arm section
