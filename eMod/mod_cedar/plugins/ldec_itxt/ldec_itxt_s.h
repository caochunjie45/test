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
* File   : ldec_itxt_s.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-10-8
* Description:
********************************************************************************
*/
#ifndef _LDEC_ITXT_S_H_
#define _LDEC_ITXT_S_H_

//需要的外部头文件
#include "ldec_lib_cfg.h"
#include "mod_cedar_i.h"
#include "psr_2_ldec.h"
#include "ldec_2_lrdr.h"
#include "ToolKit.h"

#ifndef EPDK_DEBUG_LEVEL
#define EPDK_DEBUG_LEVEL            EPDK_DEBUG_LEVEL_LOG_ALL
//#define EPDK_DEBUG_LEVEL            EPDK_DEBUG_LEVEL_LOG_WARNINGS
#endif

//给别人使用的头文件
#include "ldec.h"

//内部共用头文件
#include "ldec_lbs.h"

#include "itxt_decode.h"
#include "itextdec.h"
#include "issadec.h"
#include "sub_decoder.h"
#include "pgs_decoder_interface.h"

#if PLAY_DVBT_DVBSUBTITLE
#include "ldec_idvb_i.h"
#endif
#include "CedarIsubtitleDec.h"
#include "CedarItxtDec.h"
#include "CedarIpgsDec.h"

#if PLAY_ISDBT_CLOSECAPTION
#include "isdb_decode.h"
#include "CedarIisdbDec.h"
#endif

#include "ldec_itxt.h"

#endif  /* _LDEC_ITXT_S_H_ */

