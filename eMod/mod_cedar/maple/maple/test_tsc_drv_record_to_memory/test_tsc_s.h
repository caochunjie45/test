/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_cedar sub-system
*                          (module name, e.g.mpeg4 decoder plug-in) module
*
*          (c) Copyright 2010-2012, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : test_tsc_s.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2012-4-5
* Description:
********************************************************************************
*/
#ifndef _TEST_TSC_S_H_
#define _TEST_TSC_S_H_

//引用的外部头文件
#include "ePDK.h"
#include "mod_cedar_i.h"
#include "demod_interface.h"
#include "tsc_interface.h"

//==============================================================================
//                      PRINT INFORMATION SYSTEM-CALL
//==============================================================================
#define MSG                 __msg
#define MESSAGE             __msg
#define WRN                 __wrn
#define WARNING             __wrn
#define ERR                 __err
#define ERROR               __err
#define LOG                 __log
#define INF                 __inf
#define INFORMATION         __inf
#define DUMP_MEM(x, y, z)   eLIBs_dumpmem(x, y, z)
#define printf              eLIBs_printf

//==============================================================================
//                      MEMORY OPERATION SYSTEM-CALL
//==============================================================================
#define MALLOC(x)                       esMEMS_Malloc(0, x)
#define malloc(x)                       esMEMS_Malloc(0, x)
#define FREE(x)                         esMEMS_Mfree(0, x)
#define free(x)                         esMEMS_Mfree(0, x)
#define realloc(f, x)                   esMEMS_Realloc(0, f, x)
#define REALLOC(f, x)                   esMEMS_Realloc(0, f, x)

#define palloc(x, y)                    esMEMS_Palloc(x, y)
#define pfree(x, y)                     esMEMS_Pfree(x, y)
#define BALLOC(x)                       esMEMS_Balloc(x)
#define BFREE(x, y)                     esMEMS_Bfree(x, y)

//对外API
#include "test_tsc.h"

//内部共用头文件
#include "dumpDataV3.h"

#endif  /* _TEST_TSC_S_H_ */

