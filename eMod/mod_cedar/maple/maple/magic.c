/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                               module magic segment
*
*                                  (c) Copyright 2011-2014, Calder.Chen China
*                                               All Rights Reserved
*
* File    : magic.h
* By      : CC
* Version : V1.00
* Date    : 2011-01-01 8:30
*********************************************************************************************************
*/
//#include "maple_common.h"
#include "epdk.h"

#pragma arm section rodata="MAGIC"
const __mods_mgsec_t modinfo =
{
    {'e','P','D','K','.','m','o','d'},      //* magic
    0x01000000,                             //* version
#ifdef  DEBUG
    0x4a,                     	            //* type
#else
    EMOD_TYPE_USER,                         //* type
#endif
    0xF0000,                                //* heapaddr
    0x400,                                  //* heapsize
    {                                       //* mif
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
