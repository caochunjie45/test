/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                                  DRV IR KEY
*
*                                    (c) Copyright 2011-2014, Jackie China
*                                             All Rights Reserved
*
* File    : ir_protocol.h
* By      : Jackie
* Version : v1.0
* Date    : 2012-01-09
* Descript: ir protocol header file(support nec protocol currently).
* Update  : date                auther      ver     notes
*           2012-01-09 15:07    Jackie      1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __IR_PROTOCOL_H__
#define __IR_PROTOCOL_H__

#define IR_NOT_CHECK_USER_CODE  (0x00) //not check user code
#define IR_ERROR_CODE  0xffffffff
#define IR_REPEAT_CODE           0x0

/* IR remote control setting for NEC protocol */
/* ����ϵͳѡ���IRʱ������ */
/* 24MHz ��512 ��Ƶϵ��,�õ� sample_fre = 46.875Khz, sample_clk = 1/sample_fre = 21.33us
* CCMUģ��ѡ��̶�24Mhz��
*  1.CCMU-IR clock Setup Register, ��Ƶϵ����Χ[1-64],��8Ϊ3Mhz
*  2.IRģ���Ƶϵ����Χ{64,128,256,512}.ѡ��64��8x64 = 512��
*  
*/

#define NEC_IR_FORMAT_LEADING          (380)  /*sample_clk = 21.33us, 8.5ms   < sample_clk*408 ~= 8.53ms  < 9ms(NEC leading word) */
#define NEC_IR_NORMAL_OR_REPEAT_CODE   (160)  /*sample_clk = 21.33us, 2.25ms  < sample_clk*160 ~=3.4ms   < 4.5ms(NEC repeat word) */
#define NEC_IR_ONE_OR_ZERO_BIT         (53)  /*sample_clk = 21.33us, 0.565ms < sample_clk*53 ~=1.1275ms < 1.69ms(NEC high bit duration) */
#define NEC_IR_DMIN                    (19)  /*sample_clk = 21.33us, sample_clk*19 =  405us  < 560us */
#define NEC_IR_DMAX                    (90)  /*sample_clk = 21.33us, sample_clk*84 =  1792us > 1690us = 2250us - 560us */
#define NEC_IR_PMAX                    (30)  /*sample_clk = 21.33us, sample_clk*30 =  640us  >  560us */
                                      

extern __s32 IR_PROTOCOL_NecCodeValid(__u32 code, __u32 user_code);
extern __u32 IR_PROTOCOL_NecDecode(const __u8 *buf, const __u32 size);

#endif
