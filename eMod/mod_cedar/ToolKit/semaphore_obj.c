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
* File   : semaphore_obj.c
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-4-30
* Description:
********************************************************************************
*/
#include "ToolKit_s.h"
#include "semaphore_obj.h"

//SYSCALL_KRNL(__u16,             esKRNL_SemAccept  )(__krnl_event_t *pevent);
//SYSCALL_KRNL(__krnl_event_t*,   esKRNL_SemCreate  )(__u16 cnt);
//SYSCALL_KRNL(__krnl_event_t*,   esKRNL_SemDel     )(__krnl_event_t *pevent, __u8 opt, __u8 *err);
//SYSCALL_KRNL(void,              esKRNL_SemPend    )(__krnl_event_t *pevent, __u16 timeout, __u8 *err);
//SYSCALL_KRNL(__u8,              esKRNL_SemPost    )(__krnl_event_t *pevent);
//SYSCALL_KRNL(__u8,              esKRNL_SemQuery   )(__krnl_event_t *pevent, OS_SEM_DATA *p_sem_data);
//SYSCALL_KRNL(void,              esKRNL_SemSet     )(__krnl_event_t *pevent, __u16 cnt, __u8 *err);

__u16 Impl_SemObj_Accept(SemObj *thiz)
{
    return esKRNL_SemAccept(thiz->pevent);
}
void Impl_SemObj_Pend(SemObj *thiz, __u16 timeout, __u8 *err)   //timeout的单位是 10ms
{
    esKRNL_SemPend(thiz->pevent, timeout, err);
}
__u8 Impl_SemObj_Post(SemObj *thiz)
{
    return esKRNL_SemPost(thiz->pevent);
}
__u8 Impl_SemObj_Query(SemObj *thiz, OS_SEM_DATA *p_sem_data)
{
    return esKRNL_SemQuery(thiz->pevent, p_sem_data);
}

SemObj* newSemObj(__s32 cnt)
{
    SemObj *pSem = (SemObj*)malloc(sizeof(SemObj));
    if(NULL == pSem)
    {
        WARNING("semobj malloc fail\n");
        return NULL;
    }
    memset(pSem, 0, sizeof(SemObj));
    pSem->pevent = esKRNL_SemCreate(cnt);
    if(NULL == pSem->pevent)
    {
        WARNING("SemCreate() fail\n");
        goto _err1;
    }
    pSem->SemAccept = Impl_SemObj_Accept;
    pSem->SemPend   = Impl_SemObj_Pend;
    pSem->SemPost   = Impl_SemObj_Post;
    pSem->SemQuery  = Impl_SemObj_Query;
    return pSem;
    
_err1:
    free(pSem);
    return NULL;
}
void deleteSemObj(SemObj *thiz, __u8 opt, __u8 *err)
{
    esKRNL_SemDel(thiz->pevent, opt, err);
    free(thiz);
}

