/*
**********************************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              eMOD Sub-System
*
*                                   (c) Copyright 2007-2009, Steven.ZGJ.China
*                                             All Rights Reserved
*
* Moudle  : memory system
* File    : sys_mem.h
*
* By      : Steven
* Version : v1.0
* Date    : 2008-10-20 14:57:02
**********************************************************************************************************************
*/

#ifndef _SYS_MEM_H_
#define _SYS_MEM_H_

#include "typedef.h"
#include "cfgs.h"


#ifdef EPDK_USED_BY_KRNL
#define __swi(x)
#endif

/* ��device�Ļص���������     */
typedef enum __DRAM_ACCESS_CMD
{
    DRAM_ACCESS_DISABLE_REQ=0,      /* dram�����ֹ���� */
    DRAM_ACCESS_DISABLE_CANCEL,     /* dramȡ����ֹ���� */
    DRAM_ACCESS_

}__dram_access_cmd_t;


typedef enum __DRAM_WORK_MODE
{
    DRAM_WORK_MODE_LCD,             /* ��ǰΪLCD����ģʽ��     ��Ҫ����DRAM��������ϵͳƽ���������� */
    DRAM_WORK_MODE_TV,              /* ��ǰΪTV����ģʽ��      ��Ҫ����DRAM��������ϵͳ˲ʱ�������� */
    DRAM_WORK_MODE_TV_480P,         /* ��ǰΪTV  480P����ģʽ����Ҫ����DRAM��������ϵͳ˲ʱ�������� */
    DRAM_WORK_MODE_TV_576P,         /* ��ǰΪTV  720P����ģʽ����Ҫ����DRAM��������ϵͳ˲ʱ�������� */
    DRAM_WORK_MODE_TV_720P,         /* ��ǰΪTV  720P����ģʽ����Ҫ����DRAM��������ϵͳ˲ʱ�������� */
    DRAM_WORK_MODE_TV_1080P,        /* ��ǰΪTV 1080P����ģʽ����Ҫ����DRAM��������ϵͳ˲ʱ�������� */

    DRAM_WORK_MODE_

}__dram_work_mode_t;


typedef enum __DRAM_USER_MODE
{
    DRAM_USER_MODE_NONE = 0,
    DRAM_USER_MODE_VIDEO,
    DRAM_USER_MODE_MUSIC,
    DRAM_USER_MODE_PICTURE,
    DRAM_USER_MODE_ENCODER,
    DRAM_USER_MODE_

}__dram_user_mode_t;


typedef enum __SRAM_REQ_MODE
{
    SRAM_REQ_MODE_WAIT  = 0,    //request sram block in wait mode
    SRAM_REQ_MODE_NWAIT = 1,    //request sram block in none-wait mode
    SRAM_REQ_MODE_

} __sram_req_mode_e;

typedef __csp_sram_zone_e   __sram_block_e;
typedef __csp_sram_module_e __sram_block_map_e;

typedef struct
{
    __pSWI_t esMEM_DramWakeup           ;
    __pSWI_t esMEM_DramSuspend          ;
    __pSWI_t esMEM_DramState            ;
    __pSWI_t esMEM_RegDramAccess        ;
    __pSWI_t esMEM_UnRegDramAccess      ;
    __pSWI_t esMEM_SwitchDramWorkMode   ;
    __pSWI_t esMEM_RequestDramUsrMode   ;
    __pSWI_t esMEM_ReleaseDramUsrMode   ;
    __pSWI_t esMEM_SramReqBlk           ;
    __pSWI_t esMEM_SramRelBlk           ;
    __pSWI_t esMEM_SramSwitchBlk        ;

}SWIHandler_MEM_t;

#ifndef SIM_PC_WIN
#define SYSCALL_MEM(x,y) __swi(SWINO(SWINO_MEM, SWIHandler_MEM_t, y))x y
#else
#define SYSCALL_MEM(x,y) x y
#endif

SYSCALL_MEM(__s32,  esMEM_DramWakeup        )(void);
SYSCALL_MEM(__s32,  esMEM_DramSuspend       )(void);
SYSCALL_MEM(__s32,  esMEM_DramState         )(void);
SYSCALL_MEM(__hdle, esMEM_RegDramAccess     )(__u8 dev_type, __pCB_ClkCtl_t dram_access);
SYSCALL_MEM(__s32,  esMEM_UnRegDramAccess   )(__hdle dram_usr);
SYSCALL_MEM(__s32,  esMEM_SwitchDramWorkMode)(__s32 mode);
SYSCALL_MEM(__s32,  esMEM_RequestDramUsrMode)(__s32 mode);
SYSCALL_MEM(__s32,  esMEM_ReleaseDramUsrMode)(__s32 mode);
SYSCALL_MEM(__hdle, esMEM_SramReqBlk        )(__sram_block_e uBlock, __sram_req_mode_e uMode);
SYSCALL_MEM(__s32,  esMEM_SramRelBlk        )(__hdle hSram);
SYSCALL_MEM(__s32,  esMEM_SramSwitchBlk     )(__hdle hSram, __sram_block_map_e uMap);



extern SWIHandler_MEM_t SWIHandler_MEM;


//************************************************
// ��������
__s32 MEM_Init(const __dram_para_t * para);
__s32 MEM_Exit(void);
__s32 MEM_DramReset(void);
__s32 MEM_EnableDramSelfFresh(void);
__s32 MEM_DisableDramSelfFresh(__u32 nDrmClk);
__s32 MEM_DisableDramAcess(void);
__s32 MEM_EnableDramAcess(void);


//**********************************************************************************************************

#endif  /* _SYS_MEM_H_ */

