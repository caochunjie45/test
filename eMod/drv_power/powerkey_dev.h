/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                           power key board driver
*
*                                    (c) Copyright 2010-2012, Sunny China
*                                             All Rights Reserved
*
* File    : powerkey_dev.h
* By      : Sunny
* Version : v1.0
* Date    : 2011-1-7 8:58:35
* Descript: powerkey input device extern interfaces.
* Update  : date                auther      ver     notes
*           2011-1-7 8:58:39    Sunny       1.0     powerkey driver interfaces.
*********************************************************************************************************
*/

#ifndef     __POWERKEY_DEV_H__
#define     __POWERKEY_DEV_H__

extern __s32   PowerKeyDevInit(void);
extern void    PowerKeyDevExit(void);
extern __s32   PowerKeyDevSendEvent(__u32 KeyCode, __bool IsDown);

#endif      /* __POWERKEY_DEV_H__ */
