/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                               mod_mmp sub-system
*                                               cedar malloc module
*
*                                       (c) Copyright 2008-2009, kevin China
*                                               All Rights Reserved
*
* File   : red_malloc.h
* Version: V1.0
* By     : Bayden.Chen
* Date   : 
*********************************************************************************************************
*/
#ifndef _RED_MALLOC_H_
#define _RED_MALLOC_H_

extern __s32 RED_InitHeap(void);
extern __s32 RED_ExitHeap(void);
extern void *RED_malloc(__s32 size);
extern __s32 RED_free(void *pbuf);

extern __s32 RED_InitPhyHeap(void);
extern __s32 RED_ExitPhyHeap(void);
extern void *RED_phymalloc(__s32 size, __s32 align);
extern __s32 RED_phyfree(void *pbuf);

extern __s32 RED_DumpPhyHeap(void);

extern void *RED_phyrealloc(void *f, __s32 size, __s32 align);

extern void *RED_phycalloc(__s32 size, __s32 align);

#endif  //_RED_MALLOC_H_

