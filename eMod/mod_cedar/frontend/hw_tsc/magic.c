/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                               module magic segment
*
*                                     (c) Copyright 2008-2009, Kevin China
*                                               All Rights Reserved
*
* File    : magic.h
* By      : kevin.z
* Version : V1.00
* Date    : 2008-7-31 15:13
*********************************************************************************************************
*/
//#include "maple_common.h"
#include "epdk.h"

#pragma arm section rodata="MAGIC"
const __mods_mgsec_t modinfo =
{
    {'e','P','D','K','.','m','o','d'},      //.magic
    0x01000000,                             //.version
    EMOD_TYPE_DRV_TSC,  //TSC_DRV_INSTALL_TYPE,                   //.type
    0xF0000,                                //.heapaddr
    0x400,                                  //.heapsize
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
