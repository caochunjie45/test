/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                               mod_mmp sub-system
*                                             avi file parser module
*
*                                    (c) Copyright 2008-2009, kevin.z China
*                                              All Rights Reserved
*
* File   : maple_cfg.h
* Version: V1.0
* By     : eric_wang
* Date   : 2012-06-21
*********************************************************************************************************
*/
#ifndef _MAPLE_CFG_H_
#define _MAPLE_CFG_H_

#include "maple_common.h"
#include "media_type.h"
#include "maple_media_type.h"
#include "maple_ini.h"

#include "cedar_malloc.h"
#include "cedar_fs.h"
#include "ToolKit.h"

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

//==============================================================================
//                              FILE SYSTEM ADAPT MODULE
//==============================================================================
#undef FOPEN
#undef fopen
#undef FCLOSE
#undef fclose
#undef FREAD
#undef fread
#undef FSEEK
#undef fseek
#undef FTELL
#undef ftell
#undef FWRITE
#undef fwrite


#define FOPEN(name, mode)               CEDAR_fopen((name),(mode))
#define fopen(name, mode)               CEDAR_fopen((name),(mode))
#define FCLOSE(fptr)                    CEDAR_fclose((fptr))
#define fclose(fptr)                    CEDAR_fclose((fptr))
#define FREAD(buf,unit,size,fptr)       CEDAR_fread((buf),(unit),(size),(fptr))
#define fread(buf,unit,size,fptr)       CEDAR_fread((buf),(unit),(size),(fptr))
#define FWRITE(buf,unit,size,fptr)      CEDAR_fwrite((buf),(unit),(size),(fptr))
#define fwrite(buf,unit,size,fptr)      CEDAR_fwrite((buf),(unit),(size),(fptr))
#define FSEEK(fptr,offset,origin)       CEDAR_fseek((fptr),(offset),(origin))
#define fseek(fptr,offset,origin)       CEDAR_fseek((fptr),(offset),(origin))
#define FTELL(fptr)                     CEDAR_ftell((fptr))
#define ftell(fptr)                     CEDAR_ftell((fptr))

#endif  /* _MAPLE_CFG_H_ */

