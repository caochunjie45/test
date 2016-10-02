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
* File   : play_ctrl_s.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-12-23
* Description:
********************************************************************************
*/
#ifndef _PLAY_CTRL_S_H_
#define _PLAY_CTRL_S_H_

//需要的外部头文件
#include "maple_common.h"
#include "mod_cedar_i.h"
#include "ToolKit.h"
#include "audio_plybk.h"
#include "video_plybk.h"
//#include "dmx_data.h"
#include "maple_media_type.h"
//#include "subtitle_plybk.h"
#include "cedar_malloc.h"
#include "maple_ini.h"

//给别人使用的头文件,对外API
#include "player_interface.h"

//内部共用头文件
#include "play_ctrl.h"

//==============================================================================
//                              MEMORY MANAGEMENT MODULE
//==============================================================================
#undef  MALLOC
#undef  malloc
#undef  FREE
#undef  free

#define CEDAR_HEAPINIT()                CEDAR_InitHeap()
#define CEDAR_HEAPEXIT()                CEDAR_ExitHeap()
#define CEDAR_PHEAPINIT()               CEDAR_InitPhyHeap()
#define CEDAR_PHEAPEXIT()               CEDAR_ExitPhyHeap()

#define MALLOC(size)                    CEDAR_malloc(size)
#define malloc(size)                    CEDAR_malloc(size)
#define FREE(pbuf)                      CEDAR_free(pbuf)
#define free(pbuf)                      CEDAR_free(pbuf)
#define PHY_MALLOC(size, align)         CEDAR_phymalloc(size, align)
#define PHY_FREE(pbuf)                  CEDAR_phyfree(pbuf)

#ifndef MEMSET
#define MEMSET                          MEM_SET
#endif

#ifndef MEMCPY
#define MEMCPY                          MEM_CPY
#endif


#endif  /* _PLAY_CTRL_S_H_ */

