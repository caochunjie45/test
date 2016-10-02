/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                             Interrput Controller
*
*                                    (c) Copyright 2011-2014, Sunny China
*                                             All Rights Reserved
*
* File    : csp_int.h
* By      : Sunny
* Version : v1.0
* Date    : 2011-11-11
* Descript: interrupt controller interface header file.
* Update  : date                auther      ver     notes
*           2011-11-11 15:45:45 Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef	__CSP_INT_H__
#define	__CSP_INT_H__


//------------------------------------------------------------------------------
// interrupt source
//------------------------------------------------------------------------------
#define INTC_IRQNO_FIQ              0
#define INTC_IRQNO_UART0            1
#define INTC_IRQNO_UART1            2
#define INTC_IRQNO_UART2            3
#define INTC_IRQNO_UART3            4
#define INTC_IRQNO_IR0              5
#define INTC_IRQNO_IR1              6
#define INTC_IRQNO_TWI0             7
#define INTC_IRQNO_TWI1             8
#define INTC_IRQNO_TWI2             9
#define INTC_IRQNO_SPI0             10
#define INTC_IRQNO_SPI1             11
#define INTC_IRQNO_SPI2             12
#define INTC_IRQNO_SPDIF            13
#define INTC_IRQNO_AC97             14
#define INTC_IRQNO_TS               15
#define INTC_IRQNO_IIS              16
#define INTC_IRQNO_UART4            17
#define INTC_IRQNO_UART5            18
#define INTC_IRQNO_UART6            19
#define INTC_IRQNO_UART7            20
#define INTC_IRQNO_KEYPAD           21
#define INTC_IRQNO_TIMER0           22
#define INTC_IRQNO_TIMER1           23

// IRQ 24
#define INTC_IRQNO_TIMER2           24
#define INTC_IRQNO_ALARM            24
#define INTC_IRQNO_WATCHDOG         24

#define INTC_IRQNO_TIMER3           25
#define INTC_IRQNO_CAN              26
#define INTC_IRQNO_DMA              27
#define INTC_IRQNO_PIO              28
#define INTC_IRQNO_TP               29
#define INTC_IRQNO_ADDA             30
#define INTC_IRQNO_LRADC            31
#define INTC_IRQNO_SDMMC0           32
#define INTC_IRQNO_SDMMC1           33
#define INTC_IRQNO_SDMMC2           34
#define INTC_IRQNO_SDMMC3           35
#define INTC_IRQNO_MEMSTICK         36
#define INTC_IRQNO_NAND             37
#define INTC_IRQNO_USB0             38
#define INTC_IRQNO_USB1             39
#define INTC_IRQNO_USB2             40
#define INTC_IRQNO_SCR              41
#define INTC_IRQNO_CSI0             42
#define INTC_IRQNO_CSI1             43
#define INTC_IRQNO_LCD0             44
#define INTC_IRQNO_LCD1             45
#define INTC_IRQNO_MP               46

// IRQ 47
#define INTC_IRQNO_DE_FE0           47
#define INTC_IRQNO_DE_BE0           47

// IRQ 48
#define INTC_IRQNO_DE_FE1           48
#define INTC_IRQNO_DE_BE1           48
#define INTC_IRQNO_PMU              49
#define INTC_IRQNO_SPI3             50
#define INTC_IRQNO_TZASC            51
#define INTC_IRQNO_PATA             52
#define INTC_IRQNO_VE               53
#define INTC_IRQNO_SS               54
#define INTC_IRQNO_EMAC             55
#define INTC_IRQNO_SATA             56
#define INTC_IRQNO_GPS              57
#define INTC_IRQNO_HDMI             58
#define INTC_IRQNO_TVE01            59
#define INTC_IRQNO_ACE              60
#define INTC_IRQNO_TVD              61
#define INTC_IRQNO_PS20             62
#define INTC_IRQNO_PS21             63
#define INTC_IRQNO_USB3             64
#define INTC_IRQNO_USB4             65
#define INTC_IRQNO_PLE              66
#define INTC_IRQNO_TIMER4           67
#define INTC_IRQNO_TIMER5           68
#define INTC_IRQNO_GPU_GP           69
#define INTC_IRQNO_GPU_GPMMU        70
#define INTC_IRQNO_GPU_PP0          71
#define INTC_IRQNO_GPU_PPMMU0       72
#define INTC_IRQNO_GPU_PMU          73
#define INTC_IRQNO_GPU_RSV0         74
#define INTC_IRQNO_GPU_RSV1         75
#define INTC_IRQNO_GPU_RSV2         76
#define INTC_IRQNO_GPU_RSV3         77
#define INTC_IRQNO_GPU_RSV4         78
#define INTC_IRQNO_GPU_RSV5         79
#define INTC_IRQNO_GPU_RSV6         80


//------------------------------------------------------------------------------
//the max interrupt source number
//------------------------------------------------------------------------------
#define IRQ_SOUCE_MAX               96


//------------------------------------------------------------------------------
//the enum for fiq trigger mode
//------------------------------------------------------------------------------
typedef enum __FIQ_TRIGGER_MODE
{
	FIQ_TRIGGER_MODE_LOW_LEVEL      = 0x00,
	FIQ_TRIGGER_MODE_HIGH_LEVEL     = 0x01,
	FIQ_TRIGGER_MODE_NEGATIVE_EDGE  = 0x02,
	FIQ_TRIGGER_MODE_POSITIVE_EDGE  = 0x03,
	FIQ_TRIGGER_MODE_MAX            = 0x04,
} __fiq_trigger_mode_e; 


/*
*********************************************************************************************************
*                                           INTC INIT
*
* Description: Initialize interrupt controller.
*
* Arguments  : NONE
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 CSP_INTC_Init(void);

/*
*********************************************************************************************************
*                                           INTC EXIT
*
* Description: Exit interrupt controller.
*
* Arguments  : NONE
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 CSP_INTC_Exit(void);

/*
*********************************************************************************************************
*                                           GET INT SOURCE MAX
*
* Description: Get the max interrupt source number.
*
* Arguments  : NONE
*
* Returns    : The max int source number.
*
* Note       :
*********************************************************************************************************
*/
__s32 CSP_INTC_GetIntSourceMax(void);

/*
*********************************************************************************************************
*                               GET IRQ PENDING STATUS
*
* Description: Get interrupt pending staus.
*
* Arguments  : IRQNo    : the irq number to get pending status;
*              bPending : the address to store irq pending status.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 CSP_INTC_GetIRQPending(__u32 IRQNo, __u32 *bPending);

/*
*********************************************************************************************************
*                                           SET FIQ TRIGGER MODE
*
* Description: Set fiq trigger mode.
*
* Arguments  : TriggerMode  : the trigger mode of fiq to set.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 CSP_INTC_SetFIQTriggerMode(__u32 TriggerMode);

/*
*********************************************************************************************************
*                                           ENABLE INT
*
* Description: Enable interrupt.
*
* Arguments  : IRQNo  : the interrupt source number to enable.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 CSP_INTC_EnableINT(__u32 IRQNo);

/*
*********************************************************************************************************
*                                           DISABLE INT
*
* Description: Disable interrupt.
*
* Arguments  : IRQNo  : the interrupt source number to disable.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 CSP_INTC_DisableINT(__u32 IRQNo);

/*
*********************************************************************************************************
*                                         GET INTC MASK BITS
*
* Description: Get interrupt controller mask bits.
*
* Arguments  : pMaskBits  : the buffer to store mask bits;
*              Len        : the buffer byte length.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 CSP_INTC_GetMaskBits(__u32 *pMaskBits, __u32 Len);

/*
*********************************************************************************************************
*                                         SET INTC MASK BITS
*
* Description: Set interrupt controller mask bits.
*
* Arguments  : pMaskBits  : the buffer to set intterrput mask bits;
*              Len        : the buffer byte length.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 CSP_INTC_SetMaskBits(__u32 *pMaskBits, __u32 Len);

/*
*********************************************************************************************************
*                              SET VECTOR TABLE BASE ADDRESS
*
* Description: Set interrupt vector table base address.
*
* Arguments  : Addr : the address to set intterrput vector table base address.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 CSP_INTC_SetVectorTableAddr(__u32 Addr);

/*
*********************************************************************************************************
*                                   GET VECTOR ADDRESS
*
* Description: Get interrupt vector address.
*
* Arguments  : NONE.
*
* Returns    : vector table base address.
*
* Note       :
*********************************************************************************************************
*/
__u32 CSP_INTC_GetINTVectorAddr(void);

#endif	//__CSP_INT_H__
