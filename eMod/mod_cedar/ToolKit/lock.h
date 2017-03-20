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
    �򵥵Ļ������������ź����ķ�װ
********************************************************************************
*/
#ifndef _LOCK_H_
#define _LOCK_H_
#define KRNL_LOCKVAL        (0) //��ʾ��ʼ��ʱ�Ƿ���ס
#define KRNL_UNLOCKVAL      (1)

typedef struct tag_CedarLock CedarLock;
typedef void       (*CedarLock_Unlock)         (CedarLock *thiz);
typedef void       (*CedarLock_Lock)           (CedarLock *thiz);    //����lock��
typedef __s32      (*CedarLock_Accept)         (CedarLock *thiz);    //lock�������ͷ���FAIL
typedef struct tag_CedarLock
{
    pthread_mutex_t     SimpleLock;       //��,�������semaphore���ڹ���ʱ���봴��
    CedarLock_Unlock    unlock;
    CedarLock_Lock      lock;  //����lock��
    CedarLock_Accept    accept; //lock�������ͷ���EPDK_FAIL
}CedarLock;
extern __s32 CedarLock_Init(CedarLock *thiz);
extern __s32 CedarLock_Exit(CedarLock *thiz);
extern CedarLock* newCedarLock(void);
extern void deleteCedarLock(CedarLock *thiz);
#endif  /* _LOCK_H_ */

