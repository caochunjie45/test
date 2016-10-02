/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                             	Standby system
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : standby_tmr.c
* By      : Sunny
* Version : v1.0
* Date    : 2012-2-22
* Descript: timer operations for standby.
* Update  : date                auther      ver     notes
*           2012-2-22 13:34:52	Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#include "standby_i.h"


static __standby_tmr_reg_t *TmrReg;
static __u32 TmrIntCtl;
static __u32 Tmr0Ctl, Tmr0IntVal, Tmr0CntVal;

/*
*********************************************************************************************************
*                                     TIMER INIT
*
* Description: initialise timer for standby.
*
* Arguments  : none
*
* Returns    : EPDK_OK if succeeded, other if failed;
*********************************************************************************************************
*/
__s32 standby_tmr_init(void)
{
    //set timer register base
    TmrReg = (__standby_tmr_reg_t *)STANDBY_TMR_REG_BASE;

    //backup timer registers
    TmrIntCtl   = TmrReg->IntCtl;
    Tmr0Ctl     = TmrReg->Tmr0Ctl;
    Tmr0IntVal  = TmrReg->Tmr0IntVal;
    Tmr0CntVal  = TmrReg->Tmr0CntVal;

    /* config timer interrrupt */
    //config timer interrrupt
    TmrReg->IntCtl     = 0;
    TmrReg->IntSta     = 1;

    /* config timer1 for process udelay */
    TmrReg->Tmr1Ctl  = 0;
    standby_delay(1000);
    TmrReg->Tmr1Ctl  = (1<<7)|(5<<4);
    standby_delay(1000);

    return EPDK_OK;
}


/*
*********************************************************************************************************
*                                     TIMER EXIT
*
* Description: exit timer for standby.
*
* Arguments  : none
*
* Returns    : EPDK_TRUE/EPDK_FALSE;
*********************************************************************************************************
*/
__s32 standby_tmr_exit(void)
{
	__s32 i;
    //restore timer0 parameters
    TmrReg->Tmr0IntVal  = Tmr0IntVal;
    TmrReg->Tmr0CntVal  = Tmr0CntVal;

#if 1	
	TmrReg->Tmr0Ctl = 0;
	TmrReg->Tmr0Ctl = Tmr0Ctl &(~0x1);
	while(TmrReg->Tmr0Ctl & 0x2)
	{
		for(i = 0; i<=500; i++);			
	}	
	TmrReg->Tmr0Ctl |= (Tmr0Ctl&0x1);


#else
	TmrReg->Tmr0Ctl     = Tmr0Ctl;
#endif
    TmrReg->IntCtl      = TmrIntCtl;

    return 0;
}


/*
*********************************************************************************************************
*                           standby_tmr_set
*
*Description: set timer for wakeup system.
*
*Arguments  : second    time value for wakeup system.
*
*Return     : result, 0 - successed, -1 - failed;
*
*Notes      :
*
*********************************************************************************************************
*/
__s32 standby_tmr_set(__u32 second)
{
    //config timer interrrupt
    TmrReg->IntSta     = 1;
    TmrReg->IntCtl     = 1;

    //config timer0 for standby
    TmrReg->Tmr0Ctl    = 0;
    TmrReg->Tmr0IntVal = second << 10;
    TmrReg->Tmr0Ctl    = (1<<7) | (5<<4);
    TmrReg->Tmr0Ctl   |= (1<<2);  //src = 24M
    TmrReg->Tmr0Ctl   |= (1<<1);
    TmrReg->Tmr0Ctl   |= (1<<0);

    return 0;
}

/*
*********************************************************************************************************
*                                     TIMER QUERY
*
* Description: standby query timer.
*
* Arguments  : type     type of timer for query.
*
* Returns    : EPDK_TRUE/EPDK_FALSE;
*********************************************************************************************************
*/
__s32 standby_tmr_query(__standby_tmr_type_e type)
{
    switch(type)
    {
        case STANDBY_TMR_TICK:
        {
            if(TmrReg->IntSta & (1<<0))
            {
                TmrReg->IntSta = 1;
                return EPDK_TRUE;
            }
            break;
        }

        case STANDBY_TMR_ALARM:
        {
        	//not support now
            break;
        }

        default:
        {
            break;
        }
    }
    return EPDK_FALSE;
}


/*
*********************************************************************************************************
*                           standby_tmr_mdlay
*
*Description: delay ms
*
*Arguments  : ms    time for delay;
*
*Return     : none
*
*Notes      :
*
*********************************************************************************************************
*/
void standby_tmr_mdlay(int ms)
{
    if(ms < 30)
	{
        ms = 30;
    }
	
    TmrReg->Tmr1IntVal = ms;
    TmrReg->Tmr1Ctl   |= (1<<1);
    TmrReg->Tmr1Ctl   |= (1<<0);
    standby_delay(3000);
    while(TmrReg->Tmr1CntVal);

    return;
}
