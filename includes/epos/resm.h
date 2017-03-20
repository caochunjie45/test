/*
*********************************************************************************************************
*											        ePOS
*						           the Easy Portable/Player Operation System
*									           EXEC sub-system
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File   : RESM.h
* Version: V1.0
* By     : steven.zgj
*********************************************************************************************************
*/

#ifndef _RESM_H_
#define _RESM_H_

#include "typedef.h"
#include "cfgs.h"


#ifdef EPDK_USED_BY_KRNL
#define __swi(x)
#endif
//**********************************************************************************************************
//* define level 0( system level)
//************************************************
// ��������

//************************************************
// �������Ͷ���


//************************************************
// ��������
/* GLOBAL VARIABLES */

//************************************************
// ��������
/* system call table */
typedef struct 
{
    __pSWI_t esRESM_ROpen                ;
    __pSWI_t esRESM_RClose               ;
    __pSWI_t esRESM_RRead                ;
    __pSWI_t esRESM_RSeek                ;
} SWIHandler_RESM_t;
#ifndef SIM_PC_WIN
#define SYSCALL_RESM(x,y) __swi(SWINO(SWINO_RESM, SWIHandler_RESM_t, y))x y
#else
#define SYSCALL_RESM(x,y) x y
#endif
SYSCALL_RESM(__resm_rsb_t*, esRESM_ROpen    )(const char *resfile, const char *pMode);
SYSCALL_RESM(__s32,         esRESM_RClose   )(__resm_rsb_t* res);
SYSCALL_RESM(__u32,         esRESM_RRead    )(void *pdata, __u32 size, __u32 n, __resm_rsb_t * res);
SYSCALL_RESM(__s32,         esRESM_RSeek    )(__resm_rsb_t *res, __s32 Offset, __s32 Whence);
//**********************************************************************************************************

//**********************************************************************************************************
//* define level 1( system level)
//************************************************
// ��������

//************************************************
// �������Ͷ���

//************************************************
// ��������

extern SWIHandler_RESM_t SWIHandler_RESM;
//************************************************
// ��������
__s32 esRESM_init (void);
__s32 esRESM_exit (void);


//**********************************************************************************************************

#endif  /* _EXEC_H_ */

