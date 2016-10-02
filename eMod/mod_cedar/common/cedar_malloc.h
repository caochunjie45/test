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
* File   : cedar_malloc.h
* Version: V1.0
* By     : kevin
* Date   : 2008-11-20 16:08
    cedar体系的malloc的API头文件
*********************************************************************************************************
*/
#ifndef _CEDAR_MALLOC_H_
#define _CEDAR_MALLOC_H_

//#include "mod_cedar_i.h"

//typedef struct __CEDAR_BUF_NODE
//{
//    __u32       addr;       //the address of the buffer malloc from system heap
//    __u32       size;       //the size of the memory block
//
//    struct __CEDAR_BUF_NODE  *prev;     //prev pointer, point to front node
//    struct __CEDAR_BUF_NODE  *next;     //next pointer, point to next node
//
//} __cedar_buf_node_t;


extern __s32 CEDAR_InitHeap(void);
extern __s32 CEDAR_ExitHeap(void);
extern void *CEDAR_malloc(__s32 size);
extern __s32 CEDAR_free(void *pbuf);

extern __s32 CEDAR_InitPhyHeap(void);
extern __s32 CEDAR_ExitPhyHeap(void);
extern void *CEDAR_phymalloc(__s32 size, __s32 align);
extern __s32 CEDAR_phyfree(void *pbuf);


#endif  //_CEDAR_MALLOC_H_

