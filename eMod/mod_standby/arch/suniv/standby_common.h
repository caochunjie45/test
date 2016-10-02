/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                             	Standby system
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : standby_common.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-2-22
* Descript: common operations for standby.
* Update  : date                auther      ver     notes
*           2012-2-22 13:15:34	Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __STANDBY_COMMON_H__
#define __STANDBY_COMMON_H__

__u64 standby_uldiv(__u64 dividend, __u32 divisior);
void  standby_memcpy(void *dest, void *src, int n);
void  standby_mdelay(int ms);
void  standby_delay(int cycle);
__u32 raw_lib_udiv(__u32 dividend, __u32 divisior);

#define __stanby_here__      standby_puts(__FILE__);standby_put_uint(__LINE__);standby_puts("\n");

#endif  //__STANDBY_COMMON_H__

