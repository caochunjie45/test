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
* File   : video_plybk_cfg.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-2-23
* Description:
********************************************************************************
*/
#ifndef _SUBTITLE_PLYBK_CFG_H_
#define _SUBTITLE_PLYBK_CFG_H_

//需要的外部头文件

#include "mod_cedar_i.h"
#include "cedar.h"
#include "cedar_malloc.h"
#include "cedar_fs.h"
//==============================================================================
//                              MEMORY MANAGEMENT MODULE
//==============================================================================
#undef  MALLOC
#undef  malloc
#undef  FREE
#undef  free
#undef  PHY_MALLOC
#undef  PHY_FREE
#undef  CEDAR_HEAPINIT
#undef  CEDAR_HEAPEXIT
#undef  CEDAR_PHEAPINIT
#undef  CEDAR_PHEAPEXIT

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

//==============================================================================
//                              FILE SYSTEM ADAPT MODULE
//==============================================================================
#undef FOPEN
#undef fopen
#undef FCLOSE
#undef fclose
#undef FREAD
#undef fread
#undef FWRITE
#undef fwrite
#undef FSEEK
#undef fseek
#undef FTELL
#undef ftell
#undef FIOCTRL
#undef fioctrl

#define FOPEN(name, mode)               eLIBs_fopen((name),(mode))
#define fopen(name, mode)               eLIBs_fopen((name),(mode))
#define FCLOSE(fptr)                    eLIBs_fclose((fptr))
#define fclose(fptr)                    eLIBs_fclose((fptr))
#define FREAD(buf,unit,size,fptr)       eLIBs_fread((buf),(unit),(size),(fptr))
#define fread(buf,unit,size,fptr)       eLIBs_fread((buf),(unit),(size),(fptr))
#define FSEEK(fptr,offset,origin)       eLIBs_fllseek((fptr),(offset),(origin))
#define fseek(fptr,offset,origin)       eLIBs_fllseek((fptr),(offset),(origin))
#define FTELL(fptr)                     eLIBs_flltell((fptr))
#define ftell(fptr)                     eLIBs_flltell((fptr))
#define FIOCTRL(fp, cmd, aud, pbuf)     eLIBs_fioctrl(fp, cmd, aud, pbuf)
#define fioctrl(fp, cmd, aud, pbuf)     eLIBs_fioctrl(fp, cmd, aud, pbuf)
#define FWRITE(buf,unit,size,fptr)      eLIBs_fwrite((buf),(unit),(size),(fptr))
#define fwrite(buf,unit,size,fptr)      eLIBs_fwrite((buf),(unit),(size),(fptr))


#endif  /* _SUBTITLE_PLYBK_CFG_H_ */

