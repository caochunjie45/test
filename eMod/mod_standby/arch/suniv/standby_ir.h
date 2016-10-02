/*
*********************************************************************************************************
*                                                    ePOS
*                                   the Easy Portable/Player Operation System
*                                        architecture related source file
*
*                                      (c) Copyright 2009-2010, jerry China
*                                            All Rights Reserved
*
* File    : standby_ir.h
* By      : jerry
* Version : V1.00
*********************************************************************************************************
*/

#ifndef  _STANDBY_IR_H_
#define  _STANDBY_IR_H_

#include "epdk.h"
#include "standby_cfg.h"
#include "standby_reg.h"



/* normal dma 0 configuration register  */
#define NDMA_OFFSET   (0x20)

#define DMAC_REG_o_CFG(n)              (0x100 + n*NDMA_OFFSET + 0x00)
#define DMAC_REG_o_SRC_ADDR(n)         (0x100 + n*NDMA_OFFSET + 0x04)
#define DMAC_REG_o_DST_ADDR(n)         (0x100 + n*NDMA_OFFSET + 0x08)
#define DMAC_REG_o_BYTE_CNT(n)         (0x100 + n*NDMA_OFFSET + 0x0C)

#define DMAC_REG_N0_CFG             ( 0xf0000000 +DMA_REG_BASE + DMAC_REG_o_CFG(0)       )
#define DMAC_REG_N0_SRC_ADDR        ( 0xf0000000 +DMA_REG_BASE + DMAC_REG_o_SRC_ADDR(0)  )
#define DMAC_REG_N0_DST_ADDR        ( 0xf0000000 +DMA_REG_BASE + DMAC_REG_o_DST_ADDR(0)  )
#define DMAC_REG_N0_BYTE_CNT        ( 0xf0000000 +DMA_REG_BASE + DMAC_REG_o_BYTE_CNT(0)  )

#define  IR_REG_BASE  STANDBY_IR_REG_BASE
/* ir module register for remote controller */
#define  IR_REG_CTRL     (IR_REG_BASE+0x00)  /*IR Control Register---used for ir */
#define  IR_REG_RXCTL    (IR_REG_BASE+0x10)  /*IR Rx Configure Register */
#define  IR_REG_RXFIFO   (IR_REG_BASE+0x20) /*IR Rx FIFO---used for ir */
#define  IR_REG_RXINT    (IR_REG_BASE+0x2C) /*IR Rx Interrupt Control Register---used for ir */
#define  IR_REG_RXSTA    (IR_REG_BASE+0x30) /*IR Rx Status Register---used for ir */
#define  IR_REG_CIR      (IR_REG_BASE+0x34) /*Consumer IR Configure Register---used for ir*/

#define IR_RXINT_ALL_MASK     (0x3f)
#define IR_RXINT_RPEI_EN   (0x1<<1) /* receive packet end interrupt */
#define IR_RXINT_RAI_EN     (0x1<<4) /* rxfifo available interrupt */
#define IR_RXINT_DRQ_EN   (0x01<<5)
#define IR_RXSTA_ALL_MASK   (0x13)
#define IR_RXSTA_RAC_MASK  (0x7f<<6) /* rxFIFO Available Counter.0:no data,16:16 byte in rxFIFO */


/* NEC remote controller format decode variable */
#define NEC_IR_FORMAT_LEADING          401  /*sample_clk = 21.33us, 8.5ms   < sample_clk*408 ~= 8.53ms  < 9ms(NEC leading word) */
#define NEC_IR_NORMAL_OR_REPEAT_CODE   160  /*sample_clk = 21.33us, 2.25ms  < sample_clk*160 ~=3.4ms   < 4.5ms(NEC repeat word) */
#define NEC_IR_ONE_OR_ZERO_BIT         53   /*sample_clk = 21.33us, 0.565ms < sample_clk*53 ~=1.1275ms < 1.69ms(NEC high bit duration) */
#define NEC_IR_DMIN                    19   /*sample_clk = 21.33us, sample_clk*19 =  405us  < 560us */
#define NEC_IR_DMAX                    84   /*sample_clk = 21.33us, sample_clk*84 =  1792us > 1690us = 2250us - 560us */
#define NEC_IR_PMAX                    30   /*sample_clk = 21.33us, sample_clk*30 =  640us  >  560us */

#define IR_ERROR_CODE  0xffffffff
#define IR_REPEAT_CODE 0




typedef  struct  __IRW_RESERVE_STATUS
{
    __u32  ir_int;        //保存IR的中断控制器，退出的时候恢复
    __u32  ir_dma;        //保留IR用到的DMA控制器，退出的时候恢复

}
__irs_reserve_status_t;

extern __s32 standby_ir_init(void);
extern __s32 standby_ir_exit(void);
extern __s32 standby_ir_detect(void);
extern __s32 standby_ir_verify(void);

#endif  /*_STANDBY_IR_H_*/
