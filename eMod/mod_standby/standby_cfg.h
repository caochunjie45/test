/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                               Standby Module
*
*                                    (c) Copyright 2006-2010, kevin.z China
*                                             All Rights Reserved
*
* File    : standby_cfg.h
* By      : kevin.z
* Version : v1.0
* Date    : 2010-7-8 13:14
* Descript: config for standby.
* Update  : date                auther      ver     notes
*********************************************************************************************************
*/
#ifndef _STANDBY_CFG_H_
#define _STANDBY_CFG_H_

#include "mod_standby_i.h"

//config wakeup source for standby
#define ALLOW_IR_WAKEUP_STANDBY         (1)     // if allow ir wakeup standby
#define ALLOW_USB_WAKEUP_STANDBY        (0)     // if allow usb plug-in/out wakeup standby
#define ALLOW_SD_WAKEUP_STANDBY         (0)     // if allow sd  wakeup standby
#define ALLOW_TPADCKEY_WAKEUP_STANDBY   (1)     // if allow tpadc key wakeup standby
#define ALLOW_POWERKEY_WAKEUP_STANDBY   (0)     // if allow power key wakeup standby
#define ALLOW_NMI_WAKEUP_STANDBY        (0)     // if allow nmi wakeup standby
#define ALLOW_ALARM_WAKEUP_STANDBY      (0)     // if allow alarm wakeup standby
#define ALLOW_USREVENT_WAKEUP_STANDBY   (0)     // if allow user event wakeup standby

#define __stanby_here__      standby_puts(__FILE__);standby_put_uint(__LINE__);standby_puts("\n");

#endif  //_STANDBY_CFG_H_

