/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                            Rtc Bsp Module
*
*                                    (c) Copyright 2011-2015, cele.ma China
*                                             All Rights Reserved
*
* File    : bsp_rtc.h
* By      : cele.ma
* Version : v1.0
* Date    : 2011-12-1 16:57:13
* Update  : date                auther      ver     notes
*           2011-12-1 16:57:15	 cele.ma    v1.0    	
*********************************************************************************************************
*/

#ifndef __BSP_RTC_H__
#define __BSP_RTC_H__


//------------------------------------------------------------------------------
//          Alarm module
//------------------------------------------------------------------------------
typedef struct __BSP_ALARM_PARA
{       
    __u32 day;
    __u32 hour;
    __u32 minute;
    __u32 second;
} __bsp_alarm_para_t;

__s32  BSP_Alarm_Init(void);
__s32  BSP_Alarm_Exit(void);
__s32  BSP_Alarm_SetTime(__bsp_alarm_para_t  pPara);
__s32  BSP_Alarm_GetTime(__bsp_alarm_para_t *pPara);
__s32  BSP_Alarm_EnableIrq(void);
__s32  BSP_Alarm_DisableIrq(void);
__s32  BSP_Alarm_Enable(void);
__s32  BSP_Alarm_Disable(void);
__s32  BSP_Alarm_GetIrqNo(__u32 *pIrqNo);
__bool BSP_Alarm_QueryPending(void);
__s32  BSP_Alarm_ClearPending(void);

//------------------------------------------------------------------------------
//            RTC module
//------------------------------------------------------------------------------
typedef enum __BSP_RTC_WEEK_NO
{
    BSP_RTC_MONNDAY = 0,
    BSP_RTC_TUESDAY,
    BSP_RTC_WEDNESDAY,
    BSP_RTC_THURSDAY,
    BSP_RTC_FRIDAY,
    BSP_RTC_SATURDAY,
    BSP_RTC_SUNDAY
} __bsp_rtc_week_no_t;

__s32 BSP_RTC_Init(void);
__s32 BSP_RTC_Exit(void);
__s32 BSP_RTC_SetTime(__u32  Hour, __u32  Minute, __u32  Second);
__s32 BSP_RTC_GetTime(__u32 *Hour, __u32 *Minute, __u32 *Second);
__s32 BSP_RTC_SetDate(__u32 Year, __u32 Month, __u32 Day);
__s32 BSP_RTC_GetDate(__u32 *Year, __u32 *Month, __u32 *Day);
__s32 BSP_RTC_SetWeekNo(__bsp_rtc_week_no_t WeekNo);
__s32 BSP_RTC_GetWeekNo(__bsp_rtc_week_no_t *WeekNo);

#endif  //__BSP_RTC_H__

