/*
*********************************************************************************************************
*                                                    eMOD
*                                            the Easy Module System
*                                               drv_rtc sub-system
*
*                                   (c) Copyright 2006-20010, kevin.z China
*                                              All Rights Reserved
*
* File   : drv_rtc.h
* Version: v1.0
* By     : kevin.z.m
* Date   : 2010-6-20 15:08
*********************************************************************************************************
*/
#ifndef _DRV_RTC_H_
#define _DRV_RTC_H_


typedef enum __RTC_INT_TYPE
{
    RTC_INT_TYPE_ALARM,     /* alarm interrupt  */
    RTC_INT_TYPE_TIMER,     /* timer interrupt  */
    RTC_INT_TYPE_CNTER,     /* conter interrupt */
    RTC_INT_TYPE_

} __rtc_int_type_e;

typedef enum _AlarmMode{
    ALARM_MODE_TIMER,
    ALARM_MODE_CALENDAR,
}AlarmMode_t;

//After @day day , @hour hour and @second second, 
//the alarm will make irq pending
typedef struct _AlarmTmrModeDuration{
    __u8 day;
    __u8 hour;
    __u8 minute;
    __u8 second;
}AlarmTmrModeDuration_t;

typedef struct _AlarmCalendarModeSetting{
    __u8     hour;
    __u8     minute;
    __u8     second;
    __bool alarmInMonday;//if not 0, alarm will make int pend at hour:minute:second in Monday 
    __bool alarmInTusday;//if not 0, alarm will make int pend at hour:minute:second in Tuesday
    __bool alarmInWesday;//if not 0, alarm will make int pend at hour:minute:second in Wednesday 
    __bool alarmInTursday;//if not 0, alarm will make int pend at hour:minute:second in  Tuesday 
    __bool alarmInFriday;//if not 0, alarm will make int pend at hour:minute:second in Friday if 
    __bool alarmInSaturday;//if not 0, alarm will make int pend at hour:minute:second in Saturday
    __bool alarmInSunday;//if not 0, alarm will make int pend at hour:minute:second in Sunday 
}AlarmCalendarModeSetting_t;

typedef enum __RTC_DEVICE_CMD_SET
{
    RTC_CMD_GET_TIME,       /* get current time, aux = 0, pbuf = (__time_t *)pInTime */
    RTC_CMD_SET_TIME,       /* set current time, aux = 0, pbuf = (__time_t *)pOutTime*/

    RTC_CMD_GET_DATE,       /* get current date, pbuf = (__date_t *)pInDate  */
    RTC_CMD_SET_DATE,       /* set current date, pbuf = (__date_t *)pOutDate */

    RTC_CMD_GET_WEEK_NO,    //get current week no, return value is weekNO, 0 is monday, 7 is sunday
    RTC_CMD_SET_WEEK_NO,    //set current week no, aux = (BSP_RTC_WeekNo_t)weekno

    RTC_CMD_REQUEST_ALARM,  /* request alarm, aux = AlarmMode_t, pBuffer = 0*/
    RTC_CMD_RELEASE_ALARM,  /* release alarm, aux = hAlarm, pBuffer = 0 */

/* start alarm, aux = handle, 
    if alarm mode is timer, aux = hAlarm, pBuffer = AlarmTmrModeDuration_t
    if alarm mode is calendar, aux = hAlarm, pBuffer = AlarmCalendarModeSetting_t*/
    RTC_CMD_START_ALARM,    
    
    RTC_CMD_STOP_ALARM,     /* stop alarm, aux = hAlarm,pBuffer = NULL*/
    RTC_CMD_QUERY_ALARM,    /* query when time the alarm will make irq, aux = hAlarm, pBuffer = NULL*/

 /* is interrupt of has come, aux = hAlarm, pBuffer= __rtc_int_type_e, only support  now*/
    RTC_CMD_QUERY_INT,     
    RTC_CMD_SETCLOCKOUT,    /* set rtc clock out*/

    RTC_CMD_

} __rtc_device_cmd_set_e;


#endif


