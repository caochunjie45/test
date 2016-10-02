/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_cedar sub-system
*                                 pthread mutex module
*
*          (c) Copyright 2010-2012, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : pthread_mutex.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-1-26
* Description:
    对外API头文件,
    更详细的内容参考GUI  [PAL_Def.h]
********************************************************************************
*/
#ifndef _PTHREAD_MUTEX_H_
#define _PTHREAD_MUTEX_H_
/*标准线程互斥接口定义*/       
#define  pthread_mutex_t                __krnl_event_t*
#define  pthread_self()		            esKRNL_GetTIDCur()


#endif  /* _PTHREAD_MUTEX_H_ */

