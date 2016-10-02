/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                          RTC DRV Module
*
*                                    (c) Copyright 2011-2015, cele.ma China
*                                             All Rights Reserved
*
* File    : int_rtc.c
* By      : cele.ma
* Version : v1.0
* Date    : 2011-12-1 20:29:43
* Update  : date                auther      ver     notes
*           2011-12-1 20:29:44	 cele.ma    v1.0    	
*********************************************************************************************************
*/

#include "int_rtc.h"

#define INT_DAY_SECONDS   (24*60*60)
#define INT_HOUR_SECONDS  (60*60)
/*
*********************************************************************************************************
*                              __ISR_Alarm
*
* Description: internal alarm ISR.
*
* Arguments  : pArg    : 
*              
*              
*               
* Returns    : EPDK_OK if succeeded, EPDK_FAIL if failed.
*
* Note       :
*********************************************************************************************************
*/
static __s32 __ISR_Alarm(void *pArg)
{
    if(Int_AlarmQueryINT() & 1)
    {
        Int_AlarmDisable();
        //wait 1 32k cycle for clear alarm pending
        while(Int_AlarmQueryINT()&1);
        esKSRV_SendMsg(KMSG_USR_SYSTEM_ALARM_COMMING, KMSG_PRIO_HIGH);
        return EPDK_OK;
    }

    return EPDK_FAIL;
}

/*
*********************************************************************************************************
*                               Int_RtcInit
*
* Description: init rtc.
*
* Arguments  : 
*              
*              
*               
* Returns    : EPDK_OK if succeeded, EPDK_FAIL if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 Int_RtcInit(void)
{
    //init rtc....... 
    BSP_RTC_Init();
    
    return EPDK_OK;
}
/*
*********************************************************************************************************
*                                  Int_RtcExit
*
* Description: exit rtc.
*
* Arguments  : 
*              
*              
*               
* Returns    : EPDK_OK if succeeded, EPDK_FAIL if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 Int_RtcExit(void)
{
    BSP_RTC_Exit();
    
    return EPDK_OK;
}

/*
*********************************************************************************************************
*                              Int_AlarmInit
*
* Description: init alarm.
*
* Arguments  : 
*              
*              
*               
* Returns    : EPDK_OK if succeeded, EPDK_FAIL if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 Int_AlarmInit(void)
{
    BSP_Alarm_Init();
    
    return EPDK_OK;
}

/*
*********************************************************************************************************
*                          Int_AlarmExit
*
* Description: exit alarm.
*
* Arguments  : pBlk    : the virtual address of create space;
*              
*              
*               
* Returns    : EPDK_OK if succeeded, EPDK_FAIL if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 Int_AlarmExit(void)
{
    BSP_Alarm_Exit();
    
    return EPDK_OK;
}

/*
*********************************************************************************************************
*                                Int_SetDate
*
* Description: set  rtc date
*
* Arguments  : Year     :
*              mounth   :
*              day      :
*               
* Returns    : EPDK_OK if succeeded, EPDK_FAIL if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 Int_SetDate(__u32 year, __u32 month, __u32 day)
{
    return BSP_RTC_SetDate(year, month, day);
}

/*
*********************************************************************************************************
*                                Int_GetDate
*
* Description:get rtc date
*
* Arguments  : *year    :
*              *mounth  :
*              *day     :
*               
* Returns    : EPDK_OK if succeeded, EPDK_FAIL if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 Int_GetDate(__u32 *year, __u32 *month, __u32 *day)
{
    return BSP_RTC_GetDate(year, month, day);
}

/*
*********************************************************************************************************
*                                 Int_SetTime
*
* Description: set rtc time
*
* Arguments  :  hour    :
*               minute  :
*               second  :
*               
* Returns    : EPDK_OK if succeeded, EPDK_FAIL if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 Int_SetTime(__u32 hour, __u32 minute, __u32 second)
{
    return BSP_RTC_SetTime(hour, minute, second);
}

/*
*********************************************************************************************************
*                                 Int_GetTime
*
* Description:  get rtc time
*
* Arguments  : *hour    :
*              *minute  :
*              *second  :
*               
* Returns    : EPDK_OK if succeeded, EPDK_FAIL if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 Int_GetTime(__u32 *hour, __u32 *minute, __u32 *second)
{
    return BSP_RTC_GetTime(hour, minute, second);
}

/*
*********************************************************************************************************
*                               Int_AlarmEnable
*
* Description: enable alarm
*
* Arguments  : 
*              
*                             
* Returns    : EPDK_OK if succeeded, EPDK_FAIL if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 Int_AlarmEnable(void)
{
    return BSP_Alarm_Enable();
}

/*
*********************************************************************************************************
*                                 Int_AlarmDisable
*
* Description: disable alarm 
*
* Arguments  : 
*               
*               
* Returns    : EPDK_OK if succeeded, EPDK_FAIL if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 Int_AlarmDisable(void)
{
    return BSP_Alarm_Disable();
}

/*
*********************************************************************************************************
*                                  Int_AlarmInstIsr
*
* Description: install alarm ISR
*
* Arguments  : 
*              
*               
* Returns    : EPDK_OK if succeeded, EPDK_FAIL if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 Int_AlarmInstIsr(void)
{
    esINT_InsISR(Int_AlarmQueryINT() >> 24, (__pISR_t)__ISR_Alarm, (void *)0);
    return EPDK_OK;
}

/*
*********************************************************************************************************
*                                  Int_AlarmUninstIsr
*
* Description: uninstall alarm ISR.
*
* Arguments  : 
*              
*               
* Returns    : EPDK_OK if succeeded, EPDK_FAIL if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 Int_AlarmUninstIsr(void)
{
    esINT_UniISR(Int_AlarmQueryINT() >> 24, (__pISR_t)__ISR_Alarm);
    return EPDK_OK;
}

/*
*********************************************************************************************************
*                                   Int_AlarmQueryINT
*
* Description: query alarm intNo.
*
* Arguments  : 
*              
*               
* Returns    : IntNo if succeeded, 0 if failed.
*
* Note       :
*********************************************************************************************************
*/
__u32 Int_AlarmQueryINT(void)
{
    __u32 pIrqNo = 0;
    if (EPDK_OK != BSP_Alarm_GetIrqNo(&pIrqNo))
    {
        __wrn("get alarm irqno failed!\n");
        return 0;
    }
    if (EPDK_TRUE == BSP_Alarm_QueryPending())
    {
        if (EPDK_OK == BSP_Alarm_ClearPending())
        {
            return ((pIrqNo<<24) | 1);
        }     
    }
   
    return ((pIrqNo<<24) | 0);
}

/*
*********************************************************************************************************
*                                   Int_AlarmEnableINT
*
* Description: enable alarm int
*
* Arguments  : mode     :
*              atrr     :
*              
*               
* Returns    : EPDK_OK if succeeded, EPDK_FAIL if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 Int_AlarmEnableINT(__u32 mode, __u32 attr)
{
	if (EPDK_OK == BSP_Alarm_EnableIrq())
	{
		esINT_EnableINT(Int_AlarmQueryINT() >> 24);
		return EPDK_OK;
	}
	return EPDK_FAIL;
}

/*
*********************************************************************************************************
*                                   Int_AlarmDisableINT
*
* Description: disable alarm int.
*
* Arguments  : mode     :
*              attr     :
*              
*               
* Returns    : EPDK_OK if succeeded, EPDK_FAIL if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 Int_AlarmDisableINT(__u32 mode, __u32 attr)
{
	if(EPDK_OK == BSP_Alarm_DisableIrq())
	{
		esINT_DisableINT(Int_AlarmQueryINT() >> 24);
		return EPDK_OK;
	}
	return EPDK_FAIL;
}

/*
*********************************************************************************************************
*                                   Int_AlarmSetDuration
*
* Description: set alarm duration.
*
* Arguments  : duration     : 
*              
*               
* Returns    : EPDK_OK if succeeded, EPDK_FAIL if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 Int_AlarmSetDuration(__u32 duration)
{
    __bsp_alarm_para_t pPara;
    __u32                 mod = 0;
    
    pPara.day    = duration/INT_DAY_SECONDS;
    mod          = duration%INT_DAY_SECONDS;
    pPara.hour   = mod/INT_HOUR_SECONDS;
    mod          = mod%INT_HOUR_SECONDS;
    pPara.minute = mod/60;
    mod          = mod%60;
    pPara.second = mod;
    
    if (EPDK_OK == BSP_Alarm_SetTime(pPara))
    {
        return EPDK_OK;
    }
    
    return EPDK_FAIL;
}

/*
*********************************************************************************************************
*                                    Int_AlarmQuery
*
* Description: query alarm
*
* Arguments  : 
*              
*               
* Returns    : duration if succeeded, 0 if failed.
*
* Note       :
*********************************************************************************************************
*/
__u32 Int_AlarmQuery(void)
{
    __u32 ret;
    __bsp_alarm_para_t pPara;
    
    if (EPDK_OK == BSP_Alarm_GetTime(&pPara))
    {
        ret = pPara.day*INT_DAY_SECONDS + pPara.hour*INT_HOUR_SECONDS + pPara.minute*60 + pPara.second;
        return ret;
    }
    
    return 0;
}

