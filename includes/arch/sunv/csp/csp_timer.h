/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                               CSP TIMER Module
*
*                                    (c) Copyright 2011-2015, cele.ma China
*                                             All Rights Reserved
*
* File    : csp_timer.h
* By      : cele.ma
* Version : v1.0
* Date    : 2011-11-21 14:22:25
* Update  : date                auther      ver     notes
*           2011-11-21 14:22:27	cele.ma     v1.0    	
*********************************************************************************************************
*/

#ifndef __CSP_TIMER_H__
#define __CSP_TIMER_H__

typedef enum __CSP_TMR_PRECISION
{
    CSP_TMR_PRECISION_MICRO_SECOND,
    CSP_TMR_PRECISION_MILLI_SECOND,
    CSP_TMR_PRECISION_SECOND       
} __csp_tmr_precision_e;

typedef struct __CSP_TMR_PARA
{
    __csp_tmr_precision_e Precision;
    __u32                 MaxPeriod;
} __csp_tmr_para_t;

typedef enum __CSP_TMR_MODE
{
    CSP_TMR_MODE_CONTINUOUS,
    CSP_TMR_MODE_ONE_SHOOT,
} __csp_tmr_mode_e;


//------------------------------------------------------------------------------
//  the source clock of LOSC
//------------------------------------------------------------------------------
typedef enum __CSP_TMR_LOSC_CLK_SRC
{
    CSP_TMR_LOSC_CLK_SRC_INTERNAL_32K = 0,
    CSP_TMR_LOSC_CLK_SRC_EXTENAL_32768,
} __csp_tmrc_clk_src_e;

//------------------------------------------------------------------------------
//  the source clock of timer 
//------------------------------------------------------------------------------
typedef enum __CSP_TMR_CLK_SRC
{
    CSP_TMR_CLK_SRC_LOSC,
    CSP_TMR_CLK_SRC_HOSC,
    CSP_TMR_CLK_SRC_PLL6_6
} __csp_tmr_clk_src_e;

__s32  CSP_TMR_Init(__csp_tmrc_clk_src_e LoscSrc);
__s32  CSP_TMR_Exit(void);

__hdle CSP_TMR_RequestTmr(__csp_tmr_para_t *pPara);
__s32  CSP_TMR_ReleaseTmr(__hdle hTmr);

__s32  CSP_TMR_GetTmrIrqNo(__hdle hTmr, __u32 *Irqno);
__s32  CSP_TMR_EnableTmrIrq(__hdle hTmr);
__s32  CSP_TMR_DisableTmrIrq(__hdle hTmr);
__bool CSP_TMR_QueryTmrPending(__hdle hTmr);
__s32  CSP_TMR_ClearTmrPending(__hdle hTmr);

__s32  CSP_TMR_StartTmr(__hdle hTmr, __u32 nPeriod, __csp_tmr_mode_e TmrMode);
__s32  CSP_TMR_StopTmr(__hdle hTmr);

//------------------------------------------------------------------------------
//  watch-dog module
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//  parameters for watch-dog request
//------------------------------------------------------------------------------
typedef struct __CSP_WATCHDOG_PARA
{
    __csp_tmr_clk_src_e SrcClk;
    __bool              ResetValid;
    __u32               Time;           //base on 1s
} __csp_watchdog_para_t;

__s32 CSP_TMR_SetWatchDogPara(__csp_watchdog_para_t *pPara);
__s32 CSP_TMR_GetWatchDogPara(__csp_watchdog_para_t *pPara);
__s32 CSP_TMR_EnableWatchDog(void);
__s32 CSP_TMR_DisableWatchDog(void);
__s32 CSP_TMR_ClearWatchDog(void);

//------------------------------------------------------------------------------
//          avs module
//------------------------------------------------------------------------------
__hdle CSP_TMR_RequestAvs(void);
__s32  CSP_TMR_ReleaseAvs(__hdle hAvs);
__s32  CSP_TMR_EnableAvs(__hdle hAvs);
__s32  CSP_TMR_DisableAvs(__hdle hAvs);
__s32  CSP_TMR_SetAvsDivisor(__hdle hAvs, __u32  Divisor);
__s32  CSP_TMR_GetAvsDivisor(__hdle hAvs, __u32 *pDivisor);
__s32  CSP_TMR_SetAvsCount(__hdle hAvs, __u32  Count);
__s32  CSP_TMR_GetAvsCount(__hdle hAvs, __u32 *pCount);
__s32  CSP_TMR_PauseAvs(__hdle hAvs);
__s32  CSP_TMR_ContinueAvs(__hdle hAvs);


#endif //__CSP_TIMER_H__

