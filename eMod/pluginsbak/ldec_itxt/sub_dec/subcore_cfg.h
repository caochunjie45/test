/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_mmp sub-system
*                          ldec sub decoder plug-in module
*
*          (c) Copyright 2009-2010, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : subcore_cfg.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2009-7-3
********************************************************************************
*/
#ifndef _SUBCORE_CFG_H_
#define _SUBCORE_CFG_H_
//1. PCƽ̨
#ifdef PC_PLATFORM
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <memory.h>


#include "sub_decoder.h"
#include "vdec_type.h"

//2. EPDK
#else

#include "ldec_itxt_s.h"
//#include "sub_decoder.h"
//#include "vdec_type.h"
//#include "mod_cedar_i.h"
//#include "ldec_lib_cfg.h"

#undef TRUE
#undef FALSE

#define TRUE    1
#define FALSE   0
#endif
#endif  /* _SUBCORE_CFG_H_ */

