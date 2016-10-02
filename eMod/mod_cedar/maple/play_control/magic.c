/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                           mod_cedarcore sub-system
*
*                                (c) Copyright 2009-20010, Clif.Chen China
*                                               All Rights Reserved
*
* File   : magic.c
* Version: V1.0
* By     : clif.c
* update : 
*********************************************************************************************************
*/
//#include "play_ctrl.h"
#include "epdk.h"

#pragma arm section rodata="MAGIC"
const __mods_mgsec_t modinfo =
{
    {'e','P','D','K','.','m','o','d'},		//.magic
    0x01000000,                				//.version
#ifdef DEBUG
    0x5a,                                   //.type
#else
    EMOD_TYPE_USER,                         //.type
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
