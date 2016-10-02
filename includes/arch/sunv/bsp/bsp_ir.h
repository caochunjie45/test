/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                                   BSP IR
*
*                                    (c) Copyright 2011-2014, Jackie China
*                                             All Rights Reserved
*
* File    : bsp_ir.h
* By      : Jackie
* Version : v1.0
* Date    : 2012-01-09
* Descript: bsp ir header file.
* Update  : date                auther      ver     notes
*           2012-01-09 15:07    Jackie      1.0     Create this file.
*********************************************************************************************************
*/
#ifndef __BSP_IR_H__
#define __BSP_IR_H__

typedef enum
{
    BSP_IR_SELECT_IR0,
    BSP_IR_SELECT_IR1
}__select_ir_e;

typedef enum
{
	IR_MODE_MIR_0576  = 0x00,  // 0.576Mbit/s-MIR mode,not used! 
	IR_MODE_MIR_1152  = 0x01,  // 1.152Mbit/s-MIR mode,not used!
	IR_MODE_FIR       = 0x02,  // 4.0Mbit/s-FIR mode,not used!
	IR_MODE_CIR		  = 0x03,
}__ir_mode_e;

typedef enum
{
    BSP_IR_CIR_SAMPCLK_DIV_64,
    BSP_IR_CIR_SAMPCLK_DIV_128,
    BSP_IR_CIR_SAMPCLK_DIV_256,
    BSP_IR_CIR_SAMPCLK_DIV_512
}__cir_sample_clk_div_e;

typedef enum
{
	IR_RX_FIFO_OVERRUN_INT		= 0x01,
	IR_RX_PACKET_END_INT		= 0x02,
	IR_RX_ILLEGAL_SYMBOL_INT	= 0x04,
	IR_RX_CRC_ERR_INT			= 0x08,
	IR_RX_FIFO_AVAILABLE_INT	= 0x10,
	IR_RX_ALL_INT				= 0x1f,	
}__ir_rx_int_type_e;

/*
*********************************************************************************************************
*                           BSP IR INIT
*
* Description: BSP IR init.
*
* Parameters : ir_num : 
                           BSP_IR_SELECT_IR0 : select IR0.  
                           BSP_IR_SELECT_IR1 : select IR1.             
*
* Returns    : EPDK_OK if succeed, others if fail.
*
* Note       : 
*********************************************************************************************************
*/
__hdle BSP_IR_Init(__u32 ir_num);

/*
*********************************************************************************************************
*                           BSP IR EXIT
*
* Description: BSP IR exit.
*
* Parameters : NONE.               
*
* Returns    : NONE.
*
* Note       : 
*********************************************************************************************************
*/
void BSP_IR_Exit(__hdle  hIR);

/*
*********************************************************************************************************
*                           BSP IR SET MODE
*
* Description: BSP IR set mode.
*
* Parameters : mode : IR_MODE_MIR_0576, IR_MODE_MIR_1152, IR_MODE_FIR, IR_MODE_CIR.               
*
* Returns    : NONE.
*
* Note       : 
*********************************************************************************************************
*/
void BSP_IR_SetMode(__hdle  hIR, __u32 mode);


/*
*********************************************************************************************************
*                           BSP IR ENABLE RX
*
* Description: BSP IR enable rx.
*
* Parameters : enable :
                        0 : disable.     
                        1 : enable.          
*
* Returns    : NONE.
*
* Note       : 
*********************************************************************************************************
*/
void BSP_IR_EnableRx(__hdle  hIR, __u32 enable);

/*
*********************************************************************************************************
*                           BSP IR RX INVERT PULSE POLARITY
*
* Description: BSP IR rx invert pulse polarity.
*
* Parameters : invert :
                        0 : not invert.     
                        1 : invert.          
*
* Returns    : NONE.
*
* Note       : 
*********************************************************************************************************
*/
void BSP_IR_RxInvertPulsePolarity(__hdle  hIR, __u32 invert);

/*
*********************************************************************************************************
*                           BSP IR RX ENABLE INTERRUPT
*
* Description: BSP IR tx enable interrupt.
*
* Parameters : int_type : interrupt type.               
*
* Returns    : NONE.
*
* Note       : 
*********************************************************************************************************
*/
void BSP_IR_RxEnableInt(__hdle  hIR, __u32 int_type, __u32 enable);

/*
*********************************************************************************************************
*                           BSP IR RX QUERY INTERRUPT
*
* Description: BSP IR rx query interrupt.
*
* Parameters : NONE.               
*
* Returns    : Interrupt status.
*
* Note       : 
*********************************************************************************************************
*/
__u32 BSP_IR_RxQueryInt(__hdle  hIR);

/*
*********************************************************************************************************
*                           BSP IR RX CLEAR INTERRUPT PENDING
*
* Description: BSP IR rx clear interrupt pending.
*
* Parameters : pending_bit : the bit to be cleared.               
*
* Returns    : NONE.
*
* Note       : 
*********************************************************************************************************
*/
void BSP_IR_RxClearIntPending(__hdle  hIR, __u32 pending_bit);

/*
*********************************************************************************************************
*                           BSP IR RX SET FIFO TRIGGER LEVEL
*
* Description: BSP IR rx set fifo trigger level.
*
* Parameters : trigger_level : should between 0 and 15.               
*
* Returns    : NONE.
*
* Note       : 
*********************************************************************************************************
*/
void BSP_IR_RxSetFifoTrigLevel(__hdle  hIR, __u32 trigger_level);

/*
*********************************************************************************************************
*                           BSP IR CIR CONFIG
*
* Description: BSP IR cir config.
*
* Parameters : idle_threshold  : should between 0 and 2^8-1.               
*              noise_threshold : should between 0 and 2^6-1.
               sample_clk_div  : 
                                 BSP_IR_CIR_SAMPCLK_DIV_64  : cir sample clock is ir_clk/64.
                                 BSP_IR_CIR_SAMPCLK_DIV_128 : cir sample clock is ir_clk/128.
                                 BSP_IR_CIR_SAMPCLK_DIV_256 : cir sample clock is ir_clk/256.
                                 BSP_IR_CIR_SAMPCLK_DIV_512 : cir sample clock is ir_clk/512.
* Returns    : NONE.
*
* Note       : 
*********************************************************************************************************
*/
void BSP_IR_CirConfig(__hdle  hIR, __u32 idle_threshold, __u32 noise_threshold, __u32 sample_clk_div);

/*
*********************************************************************************************************
*                           BSP IR RX GET FIFO COUNTER
*
* Description: BSP IR rx get fifo counter.
*
* Parameters : NONE.               
*
* Returns    : Rx fifo available counter.
*
* Note       : 
*********************************************************************************************************
*/
__u32 BSP_IR_RxGetFifoCnt(__hdle  hIR);

/*
*********************************************************************************************************
*                           BSP IR READ DATA
*
* Description: BSP IR read data.
*
* Parameters : NONE.               
*
* Returns    : Cir data.
*
* Note       : 
*********************************************************************************************************
*/
__u32 BSP_IR_ReadData(__hdle  hIR);

#endif
