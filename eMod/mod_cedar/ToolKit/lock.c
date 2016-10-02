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
* File   : lock.c
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-3-6
* Description:
********************************************************************************
*/
#include "ToolKit_s.h"
#include "lock.h"

void Impl_CedarLock_Unlock(CedarLock *thiz)
{
    pthread_mutex_unlock(thiz->SimpleLock);
}
void Impl_CedarLock_Lock(CedarLock *thiz)    //必须lock到
{
    pthread_mutex_lock(thiz->SimpleLock);
}
__s32 Impl_CedarLock_Accept(CedarLock *thiz)        //lock不到，就返回FAIL
{
    __s32   ret = pthread_mutex_accept(thiz->SimpleLock);
    if(!ret)
	{
	    return EPDK_FAIL;
	}
    else
    {
        return EPDK_OK;
    }
}
__s32 CedarLock_Init(CedarLock *thiz)
{
    memset(thiz, 0, sizeof(CedarLock));
    pthread_mutex_init(&thiz->SimpleLock,(void *)KRNL_UNLOCKVAL);
    thiz->unlock = Impl_CedarLock_Unlock;
    thiz->lock = Impl_CedarLock_Lock;
    thiz->accept = Impl_CedarLock_Accept;
    if(NULL == thiz->SimpleLock)
    {
        WARNING("cedarlock init fail, create locker fail.\n");
        return EPDK_FAIL;
    }
    return EPDK_OK;
}
__s32 CedarLock_Exit(CedarLock *thiz)
{
    if(thiz->SimpleLock)
    {
        pthread_mutex_exit(thiz->SimpleLock);
        thiz->SimpleLock = NULL;
    }
    return EPDK_OK;
}

CedarLock* newCedarLock()
{
    __s32   ret;
    CedarLock    *pCedarLock = (CedarLock*)malloc(sizeof(CedarLock));
    if(NULL == pCedarLock)
    {
        WARNING("pCedarLock = NULL, malloc fail\n");
        return NULL;
    }
    //先初始化基类
    ret = CedarLock_Init(pCedarLock);
    if(EPDK_OK != ret)
    {
        WARNING("CedarLock Init fail, check code!\n");
        goto __err0;
    }
    //重载虚函数
    //初始化子类变量
    return (CedarLock*)pCedarLock;
    
__err0:
    free(pCedarLock);
    return NULL;
}
void deleteCedarLock(CedarLock *thiz)
{
    __s32   ret;
    CedarLock *pCedarLock = (CedarLock*)thiz;
    ret = CedarLock_Exit(pCedarLock);
    if(EPDK_OK!=ret)
    {
        WARNING("CedarLock exit fail\n");
    }
    free(pCedarLock);
}

