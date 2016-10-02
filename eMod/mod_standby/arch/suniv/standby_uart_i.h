/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                             	Standby system
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : standby_uart_i.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-2-22
* Descript: uart internal header file for standby.
* Update  : date                auther      ver     notes
*           2012-2-22 15:24:27	Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __STANDBY_UART_I_H__
#define __STANDBY_UART_I_H__

#define STANDBY_UART0_REGS_BASE 	(0xf1c00000 + 0x28000)    //uart0 base
#define STANDBY_UART1_REGS_BASE 	(0xf1c00000 + 0x28400)    //uart1 base
#define STANDBY_UART2_REGS_BASE 	(0xf1c00000 + 0x28800)    //uart2 base
#define STANDBY_UART3_REGS_BASE 	(0xf1c00000 + 0x28C00)    //uart3 base
#define STANDBY_UART4_REGS_BASE 	(0xf1c00000 + 0x29000)    //uart4 base
#define STANDBY_UART5_REGS_BASE 	(0xf1c00000 + 0x29400)    //uart5 base
#define STANDBY_UART6_REGS_BASE 	(0xf1c00000 + 0x29800)    //uart6 base
#define STANDBY_UART7_REGS_BASE 	(0xf1c00000 + 0x29C00)    //uart7 base

typedef struct __standby_uart_t
{
    volatile unsigned int rbr;       // RBR, THR, DLL 
    volatile unsigned int dlh;       // DLH, IER      
    volatile unsigned int iir;       // IIR, FCR      
    volatile unsigned int lcr;       // LCR           
    volatile unsigned int mcr;       // MCR           
    volatile unsigned int lsr;       // LSR           
    volatile unsigned int msr;       // MSR           
    volatile unsigned int sch;       // SCH           
    volatile unsigned int dump[23];  // reserved bytes
    volatile unsigned int usr;       // USR           
    volatile unsigned int tfl;       // TFL           
    volatile unsigned int rfl;       // RFL           
} standby_uart_t;

#define thr rbr
#define dll rbr
#define ier dlh
#define fcr iir

#define FCR_FIFO_EN     0x01		/* Fifo enable */
#define FCR_RXSR        0x02		/* Receiver soft reset */
#define FCR_TXSR        0x04		/* Transmitter soft reset */

#define MCR_DTR         0x01
#define MCR_RTS         0x02
#define MCR_DMA_EN      0x04
#define MCR_TX_DFR      0x08

#define LCR_WLS_MSK	    0x03		/* character length select mask */
#define LCR_WLS_5	    0x00		/* 5 bit character length */
#define LCR_WLS_6	    0x01		/* 6 bit character length */
#define LCR_WLS_7	    0x02		/* 7 bit character length */
#define LCR_WLS_8	    0x03		/* 8 bit character length */
#define LCR_STB		    0x04		/* Number of stop Bits, off = 1, on = 1.5 or 2) */
#define LCR_PEN		    0x08		/* Parity eneble */
#define LCR_EPS		    0x10		/* Even Parity Select */
#define LCR_STKP	    0x20		/* Stick Parity */
#define LCR_SBRK	    0x40		/* Set Break */
#define LCR_BKSE	    0x80		/* Bank select enable */

#define LSR_DR		    0x01		/* Data ready */
#define LSR_OE		    0x02		/* Overrun */
#define LSR_PE		    0x04		/* Parity error */
#define LSR_FE		    0x08		/* Framing error */
#define LSR_BI		    0x10		/* Break */
#define LSR_THRE	    0x20		/* Xmit holding register empty */
#define LSR_TEMT	    0x40		/* Xmitter empty */
#define LSR_ERR		    0x80		/* Error */

#define RXFIFO_FULL     0x10      	/* Rx Fifo check full  */
#define RXFIFO_EMPTY    0x08      	/* Rx Fifo check empty */
#define TXFIFO_EMPTY    0x04      	/* Tx Fifo check empty */
#define TXFIFO_FULL     0x02      	/* Tx Fifo check full  */
#define UART_BUSY       0x01      	/* uart control busy   */

#define LCRVAL          0x03								/* 8 data, 1 stop, no parity */
#define MCRVAL          (MCR_DTR | MCR_RTS)			        /* RTS/DTR */
#define FCRVAL          (FCR_FIFO_EN | FCR_RXSR | FCR_TXSR)	/* Clear & enable FIFOs */

/* useful defaults for LCR */
#define LCR_8N1			0x03

#endif	//__STANDBY_UART_I_H__
