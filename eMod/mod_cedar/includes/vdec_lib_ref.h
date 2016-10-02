/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_duckweed sub-system
*                          (module name, e.g.mpeg4 decoder plug-in) module
*
*          (c) Copyright 2010-2012, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : vdec_lib_ref.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2010-11-4
* Description:
    定义vdec_lib所需的系统、硬件相关的一些配置,这里是EPDK系统下的配置.
    [cedarlib]
********************************************************************************
*/
#ifndef _VDEC_LIB_REF_H_
#define _VDEC_LIB_REF_H_

#if 0       //测试是否EPDK无关，测试完成后还是用epdk.h
#ifndef EPDK_OK
#define EPDK_OK (0)
#endif
typedef unsigned int        __u32;
typedef unsigned short      __u16;
typedef unsigned char       __u8;
typedef signed int          __s32;
typedef signed short        __s16;
typedef signed char         __s8;
#include "cfgs.h"        /* 系统配置     */
#include "const.h"
#include "epdk_arch_cfgs.h"
#include "epdk_case_cfgs.h"
#include "sunii.h"
#include "ccmu.h"
#include "intc.h"
#include "macc.h"
#ifdef EPDK_OK
#undef EPDK_OK
#endif
#else 
#include "epdk.h"  // sunii.h, ccmu.h 都在ebios.h中包含了，所有与平台、芯片相关的配置都在这里
#endif
#endif  /* _VDEC_LIB_REF_H_ */

