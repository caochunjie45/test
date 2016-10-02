/*
*********************************************************************************************************
*											        ePOS
*					           the Easy Portable/Player Operation System
*								      time management sub-system
*
*					        (c) Copyright 2006-2007, Steven.ZGJ China
*										All	Rights Reserved
*
* File	 :	rtc.c
* By	 :	Steven.ZGJ
* Versio :	v1.00
* Update : kevin.z.m   2009-6-10 11:21
********************************************************************************************************
*/

#include "rtc_core.h"
#include ".\\ext_rtc\\ext_rtc.h"
#include ".\\int_rtc\\int_rtc.h"

ES_FILE             **pRtcIICHdl;
static __alarm_t    sys_alarm = { 0, 0, 0, 0 };
__rtc_dev_t         RtcFuncTbl;


/*
********************************************************************************************************
*										  RTC INIT
*
* Descrition: init rtc module.
*
* Argumets	 : none
*
* Return	 : result.
*                  EPDK_OK,    init rtc module successed;
*                  EPDK_FAIL,  init rtc module failed;
********************************************************************************************************
*/
__s32 RTC_Init(void)
{
    int rtcExist = 0;
    
    if(esCFG_GetKeyValue("rtc_para", "rtc_exist", &rtcExist, 1))
    {
        __wrn("rtc exist config information error\n");
    }
    
    pRtcIICHdl = &RtcFuncTbl.hIIC;
    
    if(2 == rtcExist)
    {
        //use exteral rtc
        RtcFuncTbl.hIIC = eLIBs_fopen("b:\\BUS\\TWI1", "r+");
        if(!RtcFuncTbl.hIIC)
        {
            __wrn("rtc driver try to open bus\\twi1 failed!\n");
            return EPDK_FAIL;
        }
        
        RtcFuncTbl.SetDate = PCF8563_SetDate;
        RtcFuncTbl.GetDate = PCF8563_GetDate;
        RtcFuncTbl.SetTime = PCF8563_SetTime;
        RtcFuncTbl.GetTime = PCF8563_GetTime;

        RtcFuncTbl.AlarmEnable      = PCF8563_AlarmEnable;
        RtcFuncTbl.AlarmDisable     = PCF8563_AlarmDisable;
        RtcFuncTbl.AlarmInstIsr     = PCF8563_AlarmInstIsr;
        RtcFuncTbl.AlarmUninstIsr   = PCF8563_AlarmUninstIsr;
        RtcFuncTbl.AlarmQueryINT    = PCF8563_AlarmQueryINT;
        RtcFuncTbl.AlarmEnableINT   = PCF8563_AlarmEnableINT;
        RtcFuncTbl.AlarmDisableINT  = PCF8563_AlarmDisableINT;
        RtcFuncTbl.AlarmSetDuration = PCF8563_AlarmSetDuration;
        RtcFuncTbl.AlarmQuery       = PCF8563_AlarmQuery;
        RtcFuncTbl.QueryInt         = PCF8563_AlarmQueryINT;
    }
    else
    {
        //use internal rtc init rtc and alarm
        //init rtc......
        Int_RtcInit();
        //init alarm......
        Int_AlarmInit();
        
        RtcFuncTbl.hIIC = (ES_FILE *)1;
        RtcFuncTbl.SetDate = Int_SetDate;
        RtcFuncTbl.GetDate = Int_GetDate;
        RtcFuncTbl.SetTime = Int_SetTime;
        RtcFuncTbl.GetTime = Int_GetTime;
        
        RtcFuncTbl.AlarmEnable      = Int_AlarmEnable;
        RtcFuncTbl.AlarmDisable     = Int_AlarmDisable;
        RtcFuncTbl.AlarmInstIsr     = Int_AlarmInstIsr;
        RtcFuncTbl.AlarmUninstIsr   = Int_AlarmUninstIsr;
        RtcFuncTbl.AlarmQueryINT    = Int_AlarmQueryINT;
        RtcFuncTbl.AlarmEnableINT   = Int_AlarmEnableINT;
        RtcFuncTbl.AlarmDisableINT  = Int_AlarmDisableINT;
        RtcFuncTbl.AlarmSetDuration = Int_AlarmSetDuration;
        RtcFuncTbl.AlarmQuery       = Int_AlarmQuery;
        RtcFuncTbl.QueryInt         = Int_AlarmQueryINT;
    }

    return EPDK_OK;
}

/*
*********************************************************************************************************
*											  RTC EXIT
*
* Description: exit rtc module
*
* Arguments	 : none
*
* Returns	 : result.
*
*********************************************************************************************************
*/
__s32 RTC_Exit(void)
{
    return EPDK_OK;
}


/*
*********************************************************************************************************
*                                            GET RTC TIME
*
* Description: get rtc time;
*
* Arguments  : time     pointer to time for return current rtc time;
*
* Returns    : EPDK_OK/EPDK_FAIL
*
* Notes      :
*
*********************************************************************************************************
*/
__s32 RTC_GetTime(__time_t *time)
{
    __u32   tmpHour, tmpMin, tmpSec;
    __s32   result;

    if(!time || !RtcFuncTbl.hIIC)
    {
        return EPDK_FAIL;
    }

    result = RtcFuncTbl.GetTime(&tmpHour, &tmpMin, &tmpSec);
    if(result != EPDK_OK)
	{
	    return EPDK_FAIL;
	}

    time->hour   = tmpHour;
    time->minute = tmpMin;
    time->second = tmpSec;

    return EPDK_OK;
}
/*
*********************************************************************************************************
*                                            SET RTC TIME
*
* Description: set rtc time;
*
* Arguments  : time     pointer to time for return current rtc time;
*
* Returns    : EPDK_OK/EPDK_FAIL
*
* Notes      :
*
*********************************************************************************************************
*/
__s32 RTC_SetTime(__time_t *time)
{
    if(!time || !RtcFuncTbl.hIIC)
    {
        return EPDK_FAIL;
    }

    return RtcFuncTbl.SetTime(time->hour, time->minute, time->second);
}


/*
*********************************************************************************************************
*                                            GET RTC DATE
*
* Description: get rtc date;
*
* Arguments  : date     pointer to date for return;
*
* Returns    : EPDK_OK/EPDK_FAIL
*
*********************************************************************************************************
*/
__s32 RTC_GetDate(__date_t *date)
{
    __u32       tmpYear, tmpMonth, tmpDay;

    if(!date || !RtcFuncTbl.hIIC)
    {
        return EPDK_FAIL;
    }

    if(RtcFuncTbl.GetDate(&tmpYear, &tmpMonth, &tmpDay) != EPDK_OK)
	{
	    return EPDK_FAIL;
	}

	date->year  = tmpYear;
	date->month = tmpMonth;
	date->day   = tmpDay;

    return EPDK_OK;
}
/*
*********************************************************************************************************
*                                            SET RTC DATE
*
* Description: set rtc date
*
* Arguments  : date     pointer to date;
*
* Returns    : EPDK_OK/EPDK_FAIL
*
* Notes      :
*
*********************************************************************************************************
*/
__s32 RTC_SetDate(__date_t *date)
{
    if(!date || !RtcFuncTbl.hIIC)
    {
        return EPDK_FAIL;
    }

    return RtcFuncTbl.SetDate(date->year, date->month, date->day);
}


/*
*********************************************************************************************************
*                                            REQUEST ALARM
*
* Description: request alarm;
*
* Arguments  : mode     type of alarm need be requested;
*
* Returns    : result, handle of alarm;
*
* Notes      :
*
*********************************************************************************************************
*/
__hdle RTC_RequestAlarm(__u32 mode)
{
    if(sys_alarm.used)
    {
        //alarm has been requested by someone
        return 0;
    }

    sys_alarm.type = mode;
    sys_alarm.used = 1;

	return (__hdle)&sys_alarm;
}


/*
*********************************************************************************************************
*                                            RELEASE ALARM
*
* Description: release alarm;
*
* Arguments  : alarm    handle of alarm need be released;
*
* Returns    : result   EPDK_OK/EPDK_FAIL;
*
* Notes      :
*
*********************************************************************************************************
*/
__s32 RTC_ReleaseAlarm(__hdle alarm)
{
    __alarm_t   *tmpAlarm = (__alarm_t *)alarm;

    if(tmpAlarm != &sys_alarm)
    {
        //alarm handle error, release alarm failed
        return EPDK_FAIL;
    }

    sys_alarm.type = 0;
    sys_alarm.used = 0;

	return EPDK_OK;
}


/*
*********************************************************************************************************
*                                            START ALARM
*
* Description: start alarm;
*
* Arguments  : alarm    alarm handle;
*              time     duration of alarm comming from now;
*
* Returns    : result,  EPDK_OK/EPDK_FAIL.
*
* Notes      :
*********************************************************************************************************
*/
__s32 RTC_StartAlarm(__hdle alarm, __u32 time)
{
    __alarm_t   *tmpAlarm = (__alarm_t *)alarm;

    if(tmpAlarm != &sys_alarm)
    {
        //alarm handle error, start alarm failed
        return EPDK_FAIL;
    }

    //set alarm duration
    if (EPDK_FAIL == RtcFuncTbl.AlarmSetDuration(time))
    {
        __wrn("alarm set duration fail!\n");
        return EPDK_FAIL;
    }
    //enable alarm
    if (EPDK_FAIL == RtcFuncTbl.AlarmEnable())
    {
        __wrn("alarm enable fail!\n");
        return EPDK_FAIL;
    }
    //install alarm isr handle
    RtcFuncTbl.AlarmInstIsr();
    //enable alarm interrupt
    
    if (EPDK_FAIL == RtcFuncTbl.AlarmEnableINT(ALARM_INTERRUPT_NORMAL, 0))
    {
        __wrn("alarm enable int fail!\n");
        return EPDK_FAIL;
    }
    
	return EPDK_OK;
}


/*
*********************************************************************************************************
*                                            STOP ALARM
*
* Description: stop alarm;
*
* Arguments  : alarm    alarm handle;
*
* Returns    : result,  EPDK_OK/EPDK_FAIL;
*
* Notes      :
*
*********************************************************************************************************
*/
__s32 RTC_StopAlarm(__hdle alarm)
{
    __alarm_t   *tmpAlarm = (__alarm_t *)alarm;

    if(tmpAlarm != &sys_alarm)
    {
        //alarm handle error, stop alarm failed
        __wrn("alarm handle error! stop alarm failed!\n");
        return EPDK_FAIL;
    }

    //disable alarm interrupt
    if (EPDK_FAIL == RtcFuncTbl.AlarmDisableINT(ALARM_INTERRUPT_NORMAL, 0))
    {
        __wrn("disable alarm int fail!\n");
        return EPDK_FAIL;
    }
    //uninstall alarm isr handle
    RtcFuncTbl.AlarmUninstIsr();

    //disable alarm
    if (EPDK_FAIL == RtcFuncTbl.AlarmDisable())
    {
        __wrn("diable alarm fail!\n");
        return EPDK_FAIL;
    }

	return EPDK_OK;
}
/*
*********************************************************************************************************
*                                       QUERY ALARM
*
* Description: query alarm time.
*
* Arguments  : alarm    alarm handle;
*
* Returns    : the time of current alarm, based on second.
*
* Notes      :
*
*********************************************************************************************************
*/
__u32 RTC_QuerryAlarm(__hdle alarm)
{
    return RtcFuncTbl.AlarmQuery();
}


/*
*********************************************************************************************************
*                                       QUERY RTC INTERRUPT SOURCE
*
* Description: query rtc intterrupt source.
*
* Arguments  : type     source of interrupt source;
*
* Returns    : result.
*                   0,  rtc interrupt has not happended;
*                   1,  rtc interrupt has happended;
*
* Notes      : only check alarm interrupt now.
*
*********************************************************************************************************
*/
__s32 RTC_QuerryInt(__s32 type)
{
    //we only check alarm interrupt now
    return (RtcFuncTbl.QueryInt() & 1);
}

