/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                                 DRV IR KEY
*
*                                    (c) Copyright 2011-2014, Jackie China
*                                             All Rights Reserved
*
* File    : drv_ir_key_i.h
* By      : Jackie
* Version : v1.0
* Date    : 2012-01-09
* Descript: ir key driver internal header file.
* Update  : date                auther      ver     notes
*           2012-01-09 15:07    Jackie      1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __DRV_IR_KEY_I_H__
#define __DRV_IR_KEY_I_H__

#include "epdk.h"

typedef struct IR_KEY_DEV
{
    __u32 used;
}__ir_key_dev_t;

typedef struct IR_KEY_DRV
{
    __u32            mid;
    __u32            used;
    __u32            status;
    __input_dev_t    *input_dev;   // ir key device handle for register input subsystem     
    __ir_key_dev_t   ir_key_dev;    // ir key device
    
}__ir_key_drv_t;

typedef struct IR_KEY_MSG
{
    __u32 key_value;  //key value.
    __u32 flag;       //1:keydown; 0:keyup.		
}__ir_key_msg_t;

extern __s32 DRV_IR_KEY_MInit(void);
extern __s32 DRV_IR_KEY_MExit(void);
extern __mp* DRV_IR_KEY_MOpen(__u32 mid, __u32 mode);
extern __s32 DRV_IR_KEY_MClose(__mp *mp);
extern __u32 DRV_IR_KEY_MRead(void *pdata, __u32 size, __u32 n, __mp *mp);
extern __u32 DRV_IR_KEY_MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp);
extern __s32 DRV_IR_KEY_MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer);

#endif
