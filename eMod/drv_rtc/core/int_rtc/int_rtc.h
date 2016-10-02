/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                               RTC Driver Module
*
*                                    (c) Copyright 2011-2014, Sunny China
*                                             All Rights Reserved
*
* File    : nop_rtc.h
* By      : Sunny
* Version : v1.0
* Date    : 2011-5-19
* Descript: nop rtc dirver.
* Update  : date                auther      ver     notes
*           2011-5-19 20:39:29  Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __NOP_RTC_H__
#define __NOP_RTC_H__

#include "epdk.h"

//rtc function
extern __s32 Int_RtcInit(void);
extern __s32 int_RtcExit(void);
extern __s32 Int_SetDate(__u32 year, __u32 month, __u32 day);
extern __s32 Int_GetDate(__u32 *year, __u32 *month, __u32 *day);
extern __s32 Int_SetTime(__u32 hour, __u32 minute, __u32 second);
extern __s32 Int_GetTime(__u32 *hour, __u32 *minute, __u32 *second);
extern __s32 Int_AlarmInit(void);
extern __s32 Int_AlarmExit(void);
extern __s32 Int_AlarmEnable(void);
extern __s32 Int_AlarmDisable(void);
extern __s32 Int_AlarmInstIsr(void);
extern __s32 Int_AlarmUninstIsr(void);
extern __u32 Int_AlarmQueryINT(void);
extern __s32 Int_AlarmEnableINT(__u32 mode, __u32 attr);
extern __s32 Int_AlarmDisableINT(__u32 mode, __u32 attr);
extern __s32 Int_AlarmSetDuration(__u32 duration);
extern __u32 Int_AlarmQuery(void);

#endif //__NOP_RTC_H__

