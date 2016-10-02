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
* File   : pthread_mutex.c
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-1-26
* Description:
********************************************************************************
*/
#include "ToolKit_s.h"
#include "pthread_mutex_h.h"
void pthread_mutex_lock(pthread_mutex_t lock)
{
	__u8		err;

    while(1)
    {
        esKRNL_SemPend(lock,0,&err);
        if(OS_NO_ERR != err)
        {
            WARNING("pthread sempend fail[%x], again\n", err);
        }
        else
        {
            break;
        }
    }
}

void pthread_mutex_unlock(pthread_mutex_t lock)
{
    esKRNL_SemPost(lock);
}

int pthread_mutex_accept(pthread_mutex_t lock)
{
    return (int)esKRNL_SemAccept(lock);
}

void pthread_mutex_init(pthread_mutex_t *lock,void *ptr)
{
    __u32  cnt      = (__u32)ptr;
    __u16  user_cnt = (cnt & 0xffff);
    
	*lock = esKRNL_SemCreate((__u16)user_cnt);

	return ;
}

void pthread_mutex_exit(pthread_mutex_t lock)
{
    __u8  err;

    esKRNL_SemDel(lock,OS_DEL_ALWAYS,&err);
}

