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
* File   : tsDemux_s.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2012-4-1
* Description:
********************************************************************************
*/
#ifndef _TSDEMUX_S_H_
#define _TSDEMUX_S_H_
//引用的外部头文件
#include "mod_cedar_i.h"
#include "tsc_interface.h"

#define DEBUG_DUMPDATA (0)

//对外API
#include "tsDemux_interface.h"

//内部共用头文件
#include "tsDemux.h"

#if (DEBUG_DUMPDATA)
#include "dumpData.h"
extern CDumpData       *pCDumpData;
#define DUMPBUF_SIZE (2*1024*1024)
#endif


#endif  /* _TSDEMUX_S_H_ */

