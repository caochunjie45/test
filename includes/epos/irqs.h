/*
*********************************************************************************************************
*											        ePOS
*						           the Easy Portable/Player Operation System
*									           IRQS sub-system
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File   : IRQS.h
* Version: V1.0
* By     : steven.zgj
*********************************************************************************************************
*/

#ifndef _IRQS_H_
#define _IRQS_H_

#include "typedef.h"
#include "cfgs.h"

#ifdef EPDK_USED_BY_KRNL
#define __swi(x)
#endif
//**********************************************************************************************************
//* define level 0( system level)
//************************************************
// ��������
/* ϵͳ���úŷ��� */
typedef struct
{
	 void *SWIHandler_SIOS          ;
	 void *SWIHandler_KRNL          ;
	 void *SWIHandler_MEMS          ;
	 void *SWIHandler_FSYS          ;
	 void *SWIHandler_EXEC          ;
	 void *SWIHandler_MODS          ;
	 void *SWIHandler_RESM          ;
	 void *SWIHandler_INT           ;
	 void *SWIHandler_DMA           ;
	 void *SWIHandler_TIME          ;
	 void *SWIHandler_IPIPE			;

	 void *SWIHandler_PWRS          ;
	 void *SWIHandler_ERRS          ;

	 void *SWIHandler_SVC           ;
	 void *SWIHandler_DEV           ;
	 void *SWIHandler_KSRV          ;
	 void *SWIHandler_PINS          ;
	 void *SWIHandler_CLK           ;

	 void *SWIHandler_MEM           ;
	 void *SWIHandler_HID           ;

	 void *SWIHandler_PWM           ;
	 void *SWIHandler_CHS           ;
	 void *SWIHandler_MSTUB         ;
	 void *SWIHandler_INPUT         ;
     void *SWIHandler_CONFIG        ;

} SWIHandler_SWIT_t;
#define SYSCALL_SWINO(x) (SWINO(0, SWIHandler_SWIT_t, x) << 8)

#define SWINO_SIOS 	SYSCALL_SWINO(SWIHandler_SIOS)  /* ��׼IOģ��ϵͳ           */
#define SWINO_KRNL 	SYSCALL_SWINO(SWIHandler_KRNL)  /* �ں�ϵͳ                 */
#define SWINO_MEMS 	SYSCALL_SWINO(SWIHandler_MEMS)  /* �ڴ����ϵͳ             */
#define SWINO_FSYS 	SYSCALL_SWINO(SWIHandler_FSYS)  /* �ļ�ϵͳ                 */
#define SWINO_EXEC 	SYSCALL_SWINO(SWIHandler_EXEC)  /* ���̹���ϵͳ             */
#define SWINO_MODS 	SYSCALL_SWINO(SWIHandler_MODS)  /* ģ�����ϵͳ             */
#define SWINO_RESM 	SYSCALL_SWINO(SWIHandler_RESM)  /* ��Դ����ϵͳ             */
#define SWINO_INT  	SYSCALL_SWINO(SWIHandler_INT)   /* �жϿ���ϵͳ             */
#define SWINO_DMA  	SYSCALL_SWINO(SWIHandler_DMA )  /* DMA                      */
#define SWINO_TIME 	SYSCALL_SWINO(SWIHandler_TIME)  /* ʱ�����ϵͳ             */
#define SWINO_IPIPE SYSCALL_SWINO(SWIHandler_IPIPE) /*                          */

#define SWINO_PWRS 	SYSCALL_SWINO(SWIHandler_PWRS)  /* ���Ĺ���ϵͳ             */
#define SWINO_ERRS 	SYSCALL_SWINO(SWIHandler_ERRS)  /* ���󱨸�ʹ���ϵͳ       */

#define SWINO_SVC   SYSCALL_SWINO(SWIHandler_SVC)   /*                          */
#define SWINO_DEV   SYSCALL_SWINO(SWIHandler_DEV)   /* �豸����ϵͳ             */
#define SWINO_KSRV  SYSCALL_SWINO(SWIHandler_KSRV)  /* �ں˷���                 */
#define SWINO_PINS  SYSCALL_SWINO(SWIHandler_PINS)  /* pin����                  */
#define SWINO_CLK   SYSCALL_SWINO(SWIHandler_CLK)   /* ʱ�ӷ���                 */
#define SWINO_MEM   SYSCALL_SWINO(SWIHandler_MEM)   /* �ڴ��豸����             */
#define SWINO_HID   SYSCALL_SWINO(SWIHandler_HID)   /* hid��ϵͳ                */

#define SWINO_PWM   SYSCALL_SWINO(SWIHandler_PWM)   /* pwm��ϵͳ                */
#define SWINO_CHS   SYSCALL_SWINO(SWIHandler_CHS)   /* charset ��ϵͳ           */
#define SWINO_MSTUB SYSCALL_SWINO(SWIHandler_MSTUB) /* module stub ��ϵͳ       */
#define SWINO_INPUT SYSCALL_SWINO(SWIHandler_INPUT) /* ������ϵͳ               */
#define SWINO_CFG   SYSCALL_SWINO(SWIHandler_CONFIG)/* ������ϵͳ               */

extern __pSWI_t  const *SWIVectorTable[];

void esIRQS_EI(void);
void esIRQS_DI(void);
void esIRQS_irqsr_ei(void);
void esIRQS_irqsr_di(void);

#endif  /* _IRQS_H_ */

