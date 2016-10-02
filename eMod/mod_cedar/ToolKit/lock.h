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
* File   : lock.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-3-6
* Description:
    简单的互斥锁，基于信号量的封装
********************************************************************************
*/
#ifndef _LOCK_H_
#define _LOCK_H_
#define KRNL_LOCKVAL        (0) //表示初始化时是否锁住
#define KRNL_UNLOCKVAL      (1)

typedef struct tag_CedarLock CedarLock;
typedef void       (*CedarLock_Unlock)         (CedarLock *thiz);
typedef void       (*CedarLock_Lock)           (CedarLock *thiz);    //必须lock到
typedef __s32      (*CedarLock_Accept)         (CedarLock *thiz);    //lock不到，就返回FAIL
typedef struct tag_CedarLock
{
    pthread_mutex_t     SimpleLock;       //锁,这里就是semaphore，在构造时必须创建
    CedarLock_Unlock    unlock;
    CedarLock_Lock      lock;  //必须lock到
    CedarLock_Accept    accept; //lock不到，就返回EPDK_FAIL
}CedarLock;
extern __s32 CedarLock_Init(CedarLock *thiz);
extern __s32 CedarLock_Exit(CedarLock *thiz);
extern CedarLock* newCedarLock(void);
extern void deleteCedarLock(CedarLock *thiz);
#endif  /* _LOCK_H_ */

