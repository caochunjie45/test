/*
*********************************************************************************************************
*													ePDK
*								   the Easy	Portable/Player	Development	Kits
*											   exf sub-system
*
*								(c)	Copyright 2006-2007, Steven.ZGJ	China
*											All	Rights Reserved
*
* File	 : eexf.h
* Version: V1.0
* By	 : steven.zgj
*********************************************************************************************************
*/

#ifndef	EEXF_H
#define	EEXF_H

#include "typedef.h"
#include "cfgs.h"

#include "krnl.h"
#include "mems.h"

#define		EEXF_TYPE_NORMAL	 0

#define		EEXF_MAGIC0		'e'
#define		EEXF_MAGIC1		'P'
#define		EEXF_MAGIC2		'D'
#define		EEXF_MAGIC3		'K'
#define		EEXF_MAGIC4		'.'
#define		EEXF_MAGIC5		'e'
#define		EEXF_MAGIC6		'x'
#define		EEXF_MAGIC7		'f'

typedef	struct __EXEC_MGSEC
{
	char		    magic[8];			// "ePDK.exf"
	__u32			version;
	__u8			type;				// 0: reserved
	
	__u32			heapaddr;			//heap��32M�ռ��ڵ�λ�ã�ͨ�������1M(31*1024*1024)
										//����ַ����0����ô��ʾ���̲���Ҫ�ֲ��ѣ�������ϵͳ��
	__u32			heapsize;			//heap�ĳ�ʼ��С
	
	__s32           (*main)(void * p_arg);  //pcb���һ��������׵�ַ
	__u32			mtskstksize;		/* ��һ�������stack	size                        */
	__u8			mtskprio;			/* ��һ����������ȼ�                               */

} __exec_mgsec_t;


#endif	//define EEXF_H
