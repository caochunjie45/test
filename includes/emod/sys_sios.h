/*
*********************************************************************************************************
*											        ePOS
*						           the Easy Portable/Player Operation System
*									           siosim sub-system
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File   : sys_sios.h
* Version: V1.0
* By     : steven.zgj
*********************************************************************************************************
*/

#ifndef _SYS_SIOS_H_
#define _SYS_SIOS_H_

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
	__pSWI_t esSIOS_getchar   ;
    __pSWI_t esSIOS_gets      ;
    __pSWI_t esSIOS_putchar   ;
    __pSWI_t esSIOS_putstr    ;
    __pSWI_t esSIOS_setbaud   ;
    __pSWI_t esSIOS_SetOutput ;
}SWIHandler_SIOS_t;

extern SWIHandler_SIOS_t SWIHandler_SIOS;
#ifndef SIM_PC_WIN
#define SYSCALL_SIOS(x,y) __swi(SWINO(SWINO_SIOS, SWIHandler_SIOS_t, y))x y
#else
#define SYSCALL_SIOS(x,y) x y
#endif

SYSCALL_SIOS(__u8, esSIOS_getchar  )(void);
SYSCALL_SIOS(void, esSIOS_gets     )(char * str);
SYSCALL_SIOS(void, esSIOS_putchar  )(__u8 ch);
SYSCALL_SIOS(void, esSIOS_putstr   )(const char * str);
SYSCALL_SIOS(void, esSIOS_setbaud  )(unsigned int baud);
SYSCALL_SIOS(int,  esSIOS_SetOutput)(const char *filename, const char *mode);

//**********************************************************************************************************

//**********************************************************************************************************
//* define level 1( system level)
//************************************************
// ��������

//************************************************
// �������Ͷ���

//************************************************
// ��������

extern SWIHandler_SIOS_t SWIHandler_SIOS;

//************************************************
// ��������
__s32 SIOS_Init(void);
__s32 SIOS_Exit(void);
__s32 SIOS_ClockChange(void);
__s32 SIOS_ClockDone(void);
__s32 SIOS_RegHw(void);
__s32 SIOS_UnregHw(void);

//**********************************************************************************************************

#endif  /* _SYS_SIOS_H_ */

