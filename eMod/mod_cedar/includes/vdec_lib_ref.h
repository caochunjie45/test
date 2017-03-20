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
    ����vdec_lib�����ϵͳ��Ӳ����ص�һЩ����,������EPDKϵͳ�µ�����.
    [cedarlib]
********************************************************************************
*/
#ifndef _VDEC_LIB_REF_H_
#define _VDEC_LIB_REF_H_

#if 0       //�����Ƿ�EPDK�޹أ�������ɺ�����epdk.h
#ifndef EPDK_OK
#define EPDK_OK (0)
#endif
typedef unsigned int        __u32;
typedef unsigned short      __u16;
typedef unsigned char       __u8;
typedef signed int          __s32;
typedef signed short        __s16;
typedef signed char         __s8;
#include "cfgs.h"        /* ϵͳ����     */
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
#include "epdk.h"  // sunii.h, ccmu.h ����ebios.h�а����ˣ�������ƽ̨��оƬ��ص����ö�������
#endif
#endif  /* _VDEC_LIB_REF_H_ */

