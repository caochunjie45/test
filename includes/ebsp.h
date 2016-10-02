/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                             Master BSP Modules
*
*                                    (c) Copyright 2011-2014, Sunny China
*                                             All Rights Reserved
*
* File    : ebsp.h
* By      : Sunny
* Version : v1.0
* Date    : 2011-11-18
* Descript: Master include file for bsp modules.
* Update  : date                auther      ver     notes
*           2011-11-18 9:25:33  Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __EBSP_H__
#define __EBSP_H__

#include "typedef.h"
#include "cfgs.h"

#if(EPDK_ARCH == EPDK_ARCH_SUNIII)
#include "arch/suniii/csp.h"
#include "arch/suniii/bsp.h"
#endif

#if(EPDK_ARCH == EPDK_ARCH_SUNIV)
#include "arch/suniv/csp.h"
#include "arch/suniv/bsp.h"
#endif

#if(EPDK_ARCH == EPDK_ARCH_SUNV)
#include "arch/sunv/sunv.h"
#include "arch/sunv/csp.h"
#include "arch/sunv/bsp.h"
#endif

#endif  //__EBSP_H__
