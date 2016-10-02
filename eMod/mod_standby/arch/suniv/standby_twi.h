/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                             	Standby system
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : standby_twi.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-2-22
* Descript: twi operations for standby.
* Update  : date                auther      ver     notes
*           2012-2-22 13:15:34	Sunny       1.0     Create this file.
*********************************************************************************************************
*/
#ifndef __STANDBY_TWI_H__
#define __STANDBY_TWI_H__

#include "standby_cfg.h"

typedef struct tag_twic_reg
{
    volatile unsigned int reg_saddr;
    volatile unsigned int reg_xsaddr;
    volatile unsigned int reg_data;
    volatile unsigned int reg_ctl;
    volatile unsigned int reg_status;
    volatile unsigned int reg_clkr;
    volatile unsigned int reg_reset;
    volatile unsigned int reg_efr;
    volatile unsigned int reg_lctl;

}__twic_reg_t;

enum twi_op_type_e
{
    TWI_OP_RD,
    TWI_OP_WR,
};


extern __s32 standby_twi_init(int group);
extern __s32 standby_twi_exit(void);
extern __s32 twi_byte_rw(enum twi_op_type_e op, __u8 saddr, __u8 baddr, __u8 *data);

#endif  //__STANDBY_TWI_H__

