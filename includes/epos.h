/*
*********************************************************************************************************
*											        ePOS
*						           the Easy Portable/Player Operation System
*									           epos headfile
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : epos.h
* By      : Steven.ZGJ
* Version : V0.1
*
* note�����е�ģ���ڲ������ȫ���Եĳ���������Ҫ���"ģ����_xxx", ����"MEMS_xxx", "EXEC_xxx"
************************************************************************************************************************
*/

#ifndef EPOS_H
#define EPOS_H

#include "typedef.h"

#ifdef 	EPOS_GLOBALS
#define EPOS_EXT
#else
#define EPOS_EXT  extern
#endif

/* ePOS VERSION NUMBER */
#define 	EPOSVer	0x01000000				//1.00.0000

/*
**************************
* INCLUDE HEADER FILES
**************************
*/
//level0
#include    "typedef.h"
#include    "cfgs.h"
#include    "dbgs.h"
//level1
#include    "elibs\\libc.h"
#include    "ebsp.h"

//level2
#include    "epos\\epos_para.h"
#include    "epos\\irqs.h"
#include    "epos\\mems.h"
#include    "epos\\krnl.h"
#include    "epos\\ksrv.h"
#include    "epos\\eexf.h"
#include    "epos\\exec.h"
#include    "epos\\mods.h"
#include    "epos\\resm.h"
#include    "epos\\errs.h"


/*
*****************
*   �ռ������ص�ȫ�ֶ���
*****************
*/
#define EPOS_proc_segsize	(32 * 1024 * 1024)		// process �ڴ��е�ַ�δ�С
#define EPOS_umod_segsize	(32 * 1024 * 1024)		// user module �ڴ��е�ַ�δ�С
#define EPOS_smod_segsize	(1024 * 1024)			// sys module �ڴ��е�ַ�δ�С

#define EPOS_pid_min        1                       // ��Сpid
#define EPOS_pid_max        63                      // ��������64������(pid = 0 ~ 63)ռ��2G�Ŀռ�(32M*64)
#define EPOS_umid_min       64                      //
#define EPOS_umid_max       95                      // ��������32���û�ģ��(mid = 64 ~ 95)ռ��1G�Ŀռ�(32M*32))
#define EPOS_smid_min       96                      // ��������128��ϵͳģ��(mid = 96 ~ 223)
#define EPOS_smid_max       0xfc                    // ��������127��ϵͳģ��(mid = 96 ~ 223)

#define EPOS_rsvid          0xfd                    // ϵͳ����id
#define EPOS_sysid          0xfe                    // ϵͳ�ں�id
#define EPOS_curid          0xff                    // ��ǰid

#define EPOS_id_mumber      255
#define EPOS_umod_startaddr 0x80000000
#define EPOS_smod_startaddr 0xe0000000



extern __epos_para_t start_para;
extern __u8          DebugEnable;

EPOS_EXT __krnl_xcb_t   *esXCBTbl[EPOS_id_mumber];      // pcb/mcb��ַ��, ��¼��ǰ�򿪵�pcb/mcb
#define  esPCBTbl   	((__exec_pcb_t **)esXCBTbl)
#define  esMCBTbl   	((__mods_mcb_t **)esXCBTbl)
#define  esIDCur    	(((__krnl_tcb_t *)OSTCBCur)->xcb->id)

EPOS_EXT __krnl_event_t *esEposSrvQ;

#ifdef EPDK_USED_BY_KRNL
void epossrv(void *p_arg);
#endif

#endif						//EPOS_H

