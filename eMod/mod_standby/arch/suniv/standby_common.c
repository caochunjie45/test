/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                             	Standby system
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : standby_common.c
* By      : Sunny
* Version : v1.0
* Date    : 2012-2-22
* Descript: common operations for standby.
* Update  : date                auther      ver     notes
*           2012-2-22 13:15:34	Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#include "standby_i.h"


/*
*********************************************************************************************************
*                           standby_memcpy
*
*Description: memory copy function for standby.
*
*Arguments  :
*
*Return     :
*
*Notes      :
*
*********************************************************************************************************
*/
void standby_memcpy(void *dest, void *src, int n)
{
    char    *tmp_src = (char *)src;
    char    *tmp_dst = (char *)dest;

    if(!dest || !src){
        /* parameter is invalid */
        return;
    }

    for( ; n > 0; n--){
        *tmp_dst ++ = *tmp_src ++;
    }

    return;
}


/*
*********************************************************************************************************
*                           mdelay
*
*Description: mdelay function
*
*Arguments  :
*
*Return     :
*
*Notes      :
*
*********************************************************************************************************
*/
void standby_mdelay(int ms)
{
    standby_delay(ms * cpu_ms_loopcnt);
}

/*
*********************************************************************************************************
*                                       STANDBY INTERRUPT INITIALISE
*
* Description: standby interrupt initialise.
*
* Arguments  : none.
*
* Returns    : 0/-1;
*********************************************************************************************************
*/
__u64 standby_uldiv(__u64 dividend, __u32 divisior)
{
    __u64   tmpDiv = (__u64)divisior;
    __u64   tmpQuot = 0;
    __s32   shift = 0;

    if(!divisior)
    {
        /* divide 0 error abort */
        return 0;
    }

    while(!(tmpDiv & ((__u64)1<<63)))
    {
        tmpDiv <<= 1;
        shift ++;
    }

    do
    {
        if(dividend >= tmpDiv)
        {
            dividend -= tmpDiv;
            tmpQuot = (tmpQuot << 1) | 1;
        }
        else
        {
            tmpQuot = (tmpQuot << 1) | 0;
        }
        tmpDiv >>= 1;
        shift --;
    } while(shift >= 0);

    return tmpQuot;
}

/*
*********************************************************************************************************
*                                       STANDBY INTERRUPT INITIALISE
*
* Description: standby interrupt initialise.
*
* Arguments  : none.
*
* Returns    : 0/-1;
*********************************************************************************************************
*/
__u32 raw_lib_udiv(__u32 dividend, __u32 divisior)
{
    __u32   tmpDiv = (__u32)divisior;
    __u32   tmpQuot = 0;
    __s32   shift = 0;

    if(!divisior)
    {
        /* divide 0 error abort */
        return 0;
    }

    while(!(tmpDiv & ((__u32)1<<31)))
    {
        tmpDiv <<= 1;
        shift ++;
    }

    do
    {
        if(dividend >= tmpDiv)
        {
            dividend -= tmpDiv;
            tmpQuot = (tmpQuot << 1) | 1;
        }
        else
        {
            tmpQuot = (tmpQuot << 1) | 0;
        }
        tmpDiv >>= 1;
        shift --;
    } while(shift >= 0);

    return tmpQuot;
}

