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
* File   : semaphore_obj.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-4-30
* Description:
    对EPDK系统的信号量操作的封装
********************************************************************************
*/
#ifndef _SEMAPHORE_OBJ_H_
#define _SEMAPHORE_OBJ_H_

typedef struct tag_SemObj SemObj;
typedef __u16 (*SemObj_Accept)(SemObj *thiz);
typedef void  (*SemObj_Pend)(SemObj *thiz, __u16 timeout, __u8 *err);   //OS_NO_ERR, OS_TIMEOUT
typedef __u8  (*SemObj_Post)(SemObj *thiz);
typedef __u8  (*SemObj_Query)(SemObj *thiz, OS_SEM_DATA *p_sem_data);
typedef struct tag_SemObj
{
    __krnl_event_t  *pevent;
    
    SemObj_Accept   SemAccept;
    SemObj_Pend     SemPend;
    SemObj_Post     SemPost;
    SemObj_Query    SemQuery;
}SemObj;
extern SemObj* newSemObj(__s32 cnt);
extern void deleteSemObj(SemObj *thiz, __u8 opt, __u8 *err);

#endif  /* _SEMAPHORE_OBJ_H_ */

