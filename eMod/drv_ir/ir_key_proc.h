/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                                  DRV IR KEY
*
*                                    (c) Copyright 2011-2014, Jackie China
*                                             All Rights Reserved
*
* File    : ir_key_proc.h
* By      : Jackie
* Version : v1.0
* Date    : 2012-01-09
* Descript: ir key process header file.
* Update  : date                auther      ver     notes
*           2012-01-09 15:07    Jackie      1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __IR_KEY_PROC_H__
#define __IR_KEY_PROC_H__

#define IR_KEY_CODE_TABLE_SIZE      256
#define IR_REPEAT_CODE 0

extern __u32 ir_key_map[];
extern __s32 IR_KEY_Init(void);
extern __s32 IR_KEY_Exit(void);
extern __s32  IR_key_get_code(void );
#endif
