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
* File   : pthread_mutex_h.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-1-26
* Description:
    给模块内部共用的头文件
********************************************************************************
*/
#ifndef _PTHREAD_MUTEX_H_H_
#define _PTHREAD_MUTEX_H_H_
#include "pthread_mutex.h"
extern void pthread_mutex_lock(pthread_mutex_t lock);
extern void pthread_mutex_unlock(pthread_mutex_t lock);
extern int pthread_mutex_accept(pthread_mutex_t lock);
extern void pthread_mutex_init(pthread_mutex_t *lock,void *ptr);
extern void pthread_mutex_exit(pthread_mutex_t lock);
#endif  /* _PTHREAD_MUTEX_H_H_ */

