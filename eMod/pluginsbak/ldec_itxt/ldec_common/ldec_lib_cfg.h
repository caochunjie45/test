/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                               mod_mmp sub-system
*                                              video decode module
*
*                                       (c) Copyright 2008-2009, kevin China
*                                               All Rights Reserved
*
* File   : ldec_lib_cfg.h
* Version: V1.0
* By     : kevin
* Date   : 2009-2-1 16:43
*********************************************************************************************************
*/

#ifndef _LDEC_LIB_CFG_H_
#define _LDEC_LIB_CFG_H_

//#include "cedar_malloc.h"
#include "mod_cedar_i.h"
#include "cedar_malloc.h"


//==============================================================================
//                              MEMORY MANAGEMENT MODULE
//==============================================================================
#define CEDAR_HEAPINIT()                CEDAR_InitHeap()
#define CEDAR_HEAPEXIT()                CEDAR_ExitHeap()
#define CEDAR_PHEAPINIT()               CEDAR_InitPhyHeap()
#define CEDAR_PHEAPEXIT()               CEDAR_ExitPhyHeap()

#define CEDAR_MALLOC(size)              CEDAR_malloc(size)
#define CEDAR_FREE(pbuf)                CEDAR_free(pbuf)
#define CEDAR_PHYMALLOC(size, align)    CEDAR_phymalloc(size, align)
#define CEDAR_PHYFREE(pbuf)             CEDAR_phyfree(pbuf)

//redefine malloc function to cedar heap
#undef MALLOC
#undef malloc
#undef FREE
#undef free

#define MALLOC                          CEDAR_MALLOC
#define malloc                          CEDAR_MALLOC
#define FREE                            CEDAR_FREE
#define free                            CEDAR_FREE
#define PHY_MALLOC                      CEDAR_PHYMALLOC
#define phy_malloc                      CEDAR_PHYMALLOC
#define PHY_FREE                        CEDAR_PHYFREE
#define phy_free                        CEDAR_PHYFREE


/*
================================================================================
                                CRITICAL PROTECT MODULE
================================================================================
*/
#define osal_switch_disable()           esKRNL_SchedLock()
#define osal_switch_enable()            esKRNL_SchedUnlock()

#define osal_interupt_disable()         ENTER_CRITICAL(cpu_sr)
#define osal_interupt_enable()          EXIT_CRITICAL(cpu_sr)


#endif  //_LDEC_LIB_CFG_H_

