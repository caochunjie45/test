/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                               module magic segment
*
*                                   (c) Copyright 2008-2009, Kevin China
*                                               All Rights Reserved
*
* File    : magic.c
* By      : Kevin
* Version : V1.00
* Date    : 2008-7-16 9:34
*********************************************************************************************************
*/
//#include "ldec_lrc_i.h"
#include "ePDK.h"
#pragma arm section rodata="MAGIC"
const __mods_mgsec_t modinfo =
{
    {'e','P','D','K','.','m','o','d'},      //.magic
    0x01000000,                             //.version
#ifdef DEBUG
    0x5e,                                   //.type
#else
    EMOD_TYPE_USER,                         //.type
#endif
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
