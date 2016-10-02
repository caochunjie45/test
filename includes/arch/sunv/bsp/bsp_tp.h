/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                                BSP TOUCHPANEL
*
*                                    (c) Copyright 2011-2014, Jackie China
*                                             All Rights Reserved
*
* File    : bsp_tp.h
* By      : Jackie
* Version : v1.0
* Date    : 2011-12-26
* Descript: bsp touchpanel header file.
* Update  : date                auther      ver     notes
*           2011-12-26 15:07    Jackie      1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __BSP_TP_H__
#define __BSP_TP_H__


#define BSP_TP_IRQ_ENABLE_TEMP          (0x1U<<18)
#define BSP_TP_IRQ_ENABLE_OVERRUN       (0x1U<<17)
#define BSP_TP_IRQ_ENABLE_DATA          (0x1U<<16)
#define BSP_TP_IRQ_ENABLE_UP            (0x1U<<1)
#define BSP_TP_IRQ_ENABLE_DOWN          (0x1U<<0)
#define BSP_TP_IRQ_ENABLE_ALL           (BSP_TP_IRQ_ENABLE_TEMP|BSP_TP_IRQ_ENABLE_OVERRUN|BSP_TP_IRQ_ENABLE_DATA \
                                        |BSP_TP_IRQ_ENABLE_UP|BSP_TP_IRQ_ENABLE_DOWN)

#define BSP_TP_IRQ_PENDING_TEMP         (0x1U<<18)
#define BSP_TP_IRQ_PENDING_OVERRUN      (0x1U<<17)
#define BSP_TP_IRQ_PENDING_DATA         (0x1U<<16)
#define BSP_TP_IRQ_PENDING_UP           (0x1U<<1)
#define BSP_TP_IRQ_PENDING_DOWN         (0x1U<<0)
#define BSP_TP_IRQ_PENDING_MASK         (BSP_TP_IRQ_PENDING_TEMP|BSP_TP_IRQ_PENDING_OVERRUN|BSP_TP_IRQ_PENDING_DATA \
                                        |BSP_TP_IRQ_PENDING_UP|BSP_TP_IRQ_PENDING_DOWN)

//tp work mode select,three work modes 
typedef enum
{    
    BSP_TP_MODE_SINGLE_POINT    ,       // x,y
    BSP_TP_MODE_DUAL_POINT      ,       // x,y,dx,dy
    BSP_TP_MODE_PRESSURE        ,       // x,y,x2,y2,z1,z2  
}__tp_work_mode_e;

//sample frequency
typedef enum
{
    BSP_TP_SAMP_FREQ0 = 0   ,             // 
    BSP_TP_SAMP_FREQ1       ,             // 
    BSP_TP_SAMP_FREQ2       ,             // 
    BSP_TP_SAMP_FREQ3       ,             // 
    BSP_TP_SAMP_FREQ4       ,             // 
    BSP_TP_SAMP_FREQ5       ,             // 
    BSP_TP_SAMP_FREQ6       ,             // 
    BSP_TP_SAMP_FREQ7       ,             // 
    BSP_TP_SAMP_FREQ8       ,             // 
    BSP_TP_SAMP_FREQ9       ,             // 
    BSP_TP_SAMP_FREQ10      ,             // 
    BSP_TP_SAMP_FREQ11      ,             // 
    BSP_TP_SAMP_FREQ12      ,             // 
    BSP_TP_SAMP_FREQ13      ,             //
    BSP_TP_SAMP_FREQ14      ,             // 
    BSP_TP_SAMP_FREQ15      ,             //                                     
}__tp_sample_freq_e;

//adjust sensitivity for pen down detection,total 16 level
typedef enum
{
	BSP_TP_SENSTIVITY_L0 = 0,
    BSP_TP_SENSTIVITY_L1	,
    BSP_TP_SENSTIVITY_L2    ,
    BSP_TP_SENSTIVITY_L3    ,
    BSP_TP_SENSTIVITY_L4    ,
    BSP_TP_SENSTIVITY_L5    ,
    BSP_TP_SENSTIVITY_L6    ,
    BSP_TP_SENSTIVITY_L7    ,
    BSP_TP_SENSTIVITY_L8    ,
    BSP_TP_SENSTIVITY_L9    ,
    BSP_TP_SENSTIVITY_L10   , // 0xA     
    BSP_TP_SENSTIVITY_L11   , // 0xB   
    BSP_TP_SENSTIVITY_L12   , // 0xC 
    BSP_TP_SENSTIVITY_L13   , // 0xD 
    BSP_TP_SENSTIVITY_L14   , // 0xE 
    BSP_TP_SENSTIVITY_L15   , // 0xF          
}__tp_senstivity_e;

/*
*********************************************************************************************************
*                           BSP TP INIT
*
* Description: BSP TP init.
*
* Parameters : sample_freq : sample_frequency.     
               fifo_depth  : fifo depth.          
               sensitivity : sensitivity for pen down detection.
*
* Returns    : EPDK_OK if success, EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/

__s32 BSP_TP_Init(__u32 sample_freq, __u32 fifo_depth, __u32 sensitivity);

/*
*********************************************************************************************************
*                           BSP TP EXIT
*
* Description: BSP TP exit.
*
* Parameters : NONE.               
*
* Returns    : EPDK_OK if success, EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/

__s32 BSP_TP_Exit(void);

/*
*********************************************************************************************************
*                           BSP TP SET WORKMODE
*
* Description: BSP TP set work mode.
*
* Parameters : work_mode : tp work mode.               
*
* Returns    : EPDK_OK if success, EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/

__s32 BSP_TP_SetWorkMode(__u32 work_mode);

/*
*********************************************************************************************************
*                           BSP TP SET SAMPLE FREQUENCY
*
* Description: BSP TP set sample frequency.
*
* Parameters : sample_freq : sample frequency.               
*
* Returns    : EPDK_OK if success, EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/

__s32 BSP_TP_SetSampleFreq(__u32 sample_freq);

/*
*********************************************************************************************************
*                           BSP TP GET DATA
*
* Description: BSP TP get data.
*
* Parameters : NONE.    
*
* Returns    : Data read.
*
* Note       : 
*********************************************************************************************************
*/

__s32 BSP_TP_GetData(void);

/*
*********************************************************************************************************
*                           BSP TP CLEAR FIFO
*
* Description: BSP TP clear fifo.
*
* Parameters : NONE.               
*
* Returns    : EPDK_OK if success, EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/

__s32 BSP_TP_ClearFifo(void);

/*
*********************************************************************************************************
*                           BSP TP ENABLE INTTERRUPT
*
* Description: BSP TP enable interrupt.
*
* Parameters : irq_type : interrupt type.               
*
* Returns    : EPDK_OK if success, EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/

__s32 BSP_TP_EnableInt(__u32 irq_type);

/*
*********************************************************************************************************
*                           BSP TP DISABLE INTTERRUPT
*
* Description: BSP TP disable interrupt.
*
* Parameters : irq_type : interrupt type.               
*
* Returns    : EPDK_OK if success, EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/

__s32 BSP_TP_DisableInt(__u32 irq_type);

/*
*********************************************************************************************************
*                           BSP TP QUERY INTERRUPT STATUS
*
* Description: BSP TP query interrupt status.
*
* Parameters : NONE.               
*
* Returns    : If any intterrupt is pending, the corresponding bit is set to 1.
*
* Note       : 
*********************************************************************************************************
*/

__s32 BSP_TP_QueryInt(void);

/*
*********************************************************************************************************
*                           BSP TP CLEAR INTERRUPT PENDING
*
* Description: BSP TP clear interrupt pending.
*
* Parameters : irq_type : interrupt type.                 
*
* Returns    : EPDK_OK if success, EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/

__s32 BSP_TP_ClearIntPending(__u32 irq_type);

#endif
