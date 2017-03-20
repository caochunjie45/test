/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                               mod_mmp sub-system
*                                               cmmb receive module
*
*                                    (c) Copyright 2008-2009, clif.c China
*                                               All Rights Reserved
*
* File   : demodDrv.c
* Version: V1.0
* By     : clif.c
* Date   : 2009-7-15 18:43
*********************************************************************************************************
*/
#include "demodDrv.h"
#include "os_adapter.h"
#include "DiB9090_app.h"

#define REG_PIOE_CFG0   (*((__u32*)0xf1c20890))
#define REG_PIOE_CFG1   (*((__u32*)0xf1c20894))
#define REG_PIOE_CFG2   (*((__u32*)0xf1c20898))
#define REG_PIOE_DATA   (*((__u32*)0xf1c208a0))

static demod_dev_t* g_Demod = NULL;
//static __hdle hDemodRstPin = NULL;
static __hdle hdl_1 = NULL;
static __hdle hdl_2 = NULL;

static __s32 ioInit(void)
{
#if (TS_INTERFACE_TYPE == 1)    //* use ssi mode.
    __u32 tmp;
    tmp = REG_PIOE_CFG0;
    tmp &= ~(0x000fffff);
    tmp |= 0x00033333;
    REG_PIOE_CFG0 = tmp;
#else
    __u32 tmp;
    tmp = REG_PIOE_CFG1;
    tmp &= 0x0000ffff;
    tmp |= 0x22220000;
    REG_PIOE_CFG1 = tmp;
    REG_PIOE_CFG2 = 0x22222222;
#endif

    return 0;
}

static __s32 ioDeinit(void)
{
#if (TS_INTERFACE_TYPE == 1)    //* use ssi mode.
    __u32 tmp;
    tmp = REG_PIOE_CFG0;
    tmp &= 0xfff00000;
    tmp |= 0x00011111;
    REG_PIOE_CFG0 = tmp;
#else
    REG_PIOE_CFG0 = 0x11111111;
    REG_PIOE_CFG1 = 0x11111111;
    REG_PIOE_DATA = 0x00000000;
#endif

    return 0;
}


static void demodReset(__u8 rst)
{
	
}


/*static*/ __s32 demoPowerOn(void)
{
    __user_pin_set_t  gpio_set[1];

    demod_9090_printf("entry demod power on\n");
	
    gpio_set->port = 8;
    gpio_set->port_num = 2;
    gpio_set->mul_sel = 1;
    gpio_set->pull = 1;
    gpio_set->drv_level = 1;
    gpio_set->data = 0;
    hdl_1 = esPINS_PinGrpReq(gpio_set, 1);

    gpio_set->port = 8;
    gpio_set->port_num = 1;
    gpio_set->mul_sel = 1;
    gpio_set->pull = 1;
    gpio_set->drv_level = 1;
    gpio_set->data = 0;

    hdl_2 = esPINS_PinGrpReq(gpio_set, 1);
	
    esKRNL_TimeDly(1);
    esPINS_WritePinData(hdl_1, 0x01, 0);

    esKRNL_TimeDly(2);
    esPINS_WritePinData(hdl_2, 0x01, 0);
    esKRNL_TimeDly(10);
	
	
    return 0;
}

/*static*/  __s32 demoPowerOff(void)
{
#if 0
    esPINS_WritePinData(hdl_1, 0, 0);
    esPINS_WritePinData(hdl_2, 0, 0);

    //esPINS_DevPinsRel(hdl_1, 1);
    //esPINS_DevPinsRel(hdl_2, 1);
    esPINS_PinGrpRel(hdl_1, 1);
    esPINS_PinGrpRel(hdl_2, 1);
#endif 

    return 0;
}

//* Initial hardware setting for tsc, such as dma, clock and io setting.
static __s32 InitSystemCfg(void)
{
    ioInit();
    iic_init();
    /*hDemodRstPin = esPINS_PioPinsReq(PIN_PHY_GROUP_B, 3);
    if(hDemodRstPin == NULL)
        return -1;*/
        
    return 0;
}

//* Release hardware setting for tsc.
static __s32 DeinitSystemCfg(void)
{
    iic_exit();
    ioDeinit();
    return 0;
}

//* lock main task.
static void demod_lock_task(void* p);


/*
*********************************************************************************************************
*                       DVB RECEIVE DRIVER MODULE INIT
*
*Description: dvb receive driver module init.
*
*Arguments  : None.
*
*Return     : Always return successful.
*
*/
__s32 MInit(void)
{
    InitSystemCfg();
    return EPDK_OK;
}


/*
*********************************************************************************************************
*                       DVB RECEIVE DRIVER MODULE EXIT
*
*Description: dvb receive driver module exit.
*
*Arguments  : None.
*
*Return     : Return successful if the driver has been closed.
*
*/
__s32 MExit(void)
{
    DeinitSystemCfg();
    return EPDK_OK;
}


/*
*********************************************************************************************************
*                       DVB RECEIVE DRIVER MODULE OPEN
*
*Description: cmmb receive driver module open.
*
*Arguments  : mid   the module id of dvb receive driver;
*             mode  the type of open.
*
*Return     : the handle of the video decode driver.
*                   != 0    open video decode driver module successful;
*                   == 0    open video decode driver module fialed.
*
*/
__mp *MOpen(__u32 mid, __u32 mode)
{
    WARNING("open demodDrv\n");
    g_Demod = (demod_dev_t*)MALLOC(sizeof(demod_dev_t));
    if(g_Demod == NULL)
    {
        return NULL;
    }
    memset(g_Demod, 0, sizeof(demod_dev_t));
    g_Demod->mid = mid;
    
    //* 1. 上电
    
    demoPowerOn();
    g_Demod->status = WORKING;
    
    
    //* 2. initial demod;
    if(dib_init() != 0)
    {
        WARNING("dibcom initial fail.\n");
        goto demod_open_fail;
    }
    
    //* return
    return (__mp*)g_Demod;
    
demod_open_fail:
    //* 4. 卸电
    demoPowerOff();
    
    //* 5. free
    FREE(g_Demod);
    g_Demod = NULL;
    return NULL;
}


/*
*********************************************************************************************************
*                       DVB RECEIVE DRIVER MODULE CLOSE
*
*Description: dvb receive driver module close.
*
*Arguments  : mp    the handle of the dvb driver;
*
*Return     : Always return successful.
*
*/
__s32 MClose(__mp *mp)
{
    demod_dev_t* pDemod = (demod_dev_t*)mp;
    
    if(g_Demod == NULL)
    {
        return -1;
    }
	
    if(g_Demod->status == WORKING)
    {
        esMODS_MIoctrl((__mp*)g_Demod, DEMOD_SLEEP, 0, NULL);
    }
    
    //* 1. 卸电
//	dib_uninit();
	demoPowerOff();
    
    //* 2. free
    FREE(g_Demod);
    g_Demod = NULL;
    return EPDK_OK;
}


/*
*********************************************************************************************************
*                       DVB RECEIVE DRIVER MODULE READ
*
*Description: cmmb receive driver read, do nothing.
*
*Arguments  :
*
*Return     :
*
*/
__u32 MRead(void *pdata, __u32 pktNumRequire, __u32 chan, __mp *mp)
{
    return 0;
}


/*
*********************************************************************************************************
*                       DVB RECEIVE DRIVER MODULE WRITE
*
*Description: cmmb receive driver write, do nothing.
*
*Arguments  :
*
*Return     :
*
*/
__u32 MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp)
{
    return 0;
}


/*
*********************************************************************************************************
*                       DVB RECEIVE DRIVER MODULE IO CONTROL
*
*Description: cmmb receive driver module IO control, process command from other module.
*
*Arguments  : mp        video decode driver handle;
*             cmd       the command need send to video decode driver;
*             aux       user define;
*             pbuffer   user define.
*
*Return     :
*
*/           

__s32 MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer)
{
    __s32           result;
    __s32           tmpret;
    demod_ss_t*     ss;
    
    if(g_Demod == NULL)
        return -1;
    
    switch(cmd)
    {
        case DEMOD_SLEEP:
            //* stop the lock task
            if(g_Demod->lockTaskPrio)
            {
                esKRNL_TimeDlyResume(g_Demod->lockTaskPrio);
                while(esKRNL_TDelReq(g_Demod->lockTaskPrio) != OS_TASK_NOT_EXIST)
                {
                    WARNING("waiting for lock task to exit.\n");
                    esKRNL_TimeDlyResume(g_Demod->lockTaskPrio);
                    esKRNL_TimeDly(1);
                }
                g_Demod->lockTaskPrio = 0;
                g_Demod->lockCb = NULL;
                g_Demod->lockCbParam = NULL;
            }
            
            //* set the tuner to sleep mode.
            g_Demod->status = SLEEP;
            result = 0;
            break;
            
        case DEMOD_WAKEUP:
            //* wake up tuner.
            g_Demod->status = WORKING;
            result = 0;
            break;
            
        case DEMOD_LOCK:
            
            if(g_Demod->status != WORKING)
            {
                WARNING("demod is not working.\n");
                result = -1;
            }
            else
            {
                if(g_Demod->lockTaskPrio == 0)
                {
                    //* create lock task
                    g_Demod->lockTaskPrio = esKRNL_TCreate(demod_lock_task, (void*)g_Demod, 0x400, KRNL_priolevel4);
                    if(g_Demod->lockTaskPrio == 0)
                    {
                        WARNING("fail to create demod lock task");
                        result = -1;
                    }
                    else
                        result = 0;
                }
                else
                    result = -1;
            }
                
            break;
            
        case DEMOD_UNLOCK:
            if(g_Demod->status != WORKING)
            {
                WARNING("demod is not working.\n");
                result = -1;
            }
            else
            {
                //* stop the lock task
                if(g_Demod->lockTaskPrio != 0)
                {
                    esKRNL_TimeDlyResume(g_Demod->lockTaskPrio);
                    while(esKRNL_TDelReq(g_Demod->lockTaskPrio) != OS_TASK_NOT_EXIST)
                    {
                        WARNING("waiting for lock task to exit.\n");
                        esKRNL_TimeDlyResume(g_Demod->lockTaskPrio);
                        esKRNL_TimeDly(1);
                    }
                    g_Demod->lockTaskPrio = 0;
                    g_Demod->lockCb = NULL;
                    g_Demod->lockCbParam = NULL;
                }
                
                result = 0;
            }
            
            break;
            
        case DEMOD_GET_STATUS:
            return g_Demod->status;
            
        case DEMOD_GET_SIGNAL_STATUS:
            
            if(g_Demod->status != WORKING)
            {
                WARNING("demod is not working.\n");
                result = EPDK_FAIL;
            }
		 
            if((g_Demod->status == WORKING) && (dib_lockstatus() >= 1))
            {
                ss = (demod_ss_t*)pbuffer;
                if(ss == NULL)
                    result = EPDK_FAIL;
                else
                {
//                    ss->quality = dib_signal_quality();
//                    ss->strength = dib_signal_strength();
			  dib_signal_strenth_quality(&(ss->quality), &(ss->strength));
                    result = EPDK_OK;
                }
            }
            else
                result = EPDK_FAIL;
            
            break;
        case DEMOD_GET_LOCK_STATUS:
        {
            if(g_Demod->status != WORKING)
            {
                WARNING("demod is not working.\n");
                result = 0;
                break;
            }

            result = dib_lockstatus();

	      /*Bridge@Mar26--2012*/
	     /*if unlock, re-tune*/
	     WARNING("lock[%d], freq[%d], bw[%d]\n", result, g_Demod->frequency, g_Demod->bandwidth);
		  
	     if(0 == result)
	     {
		 	if((g_Demod->frequency > 90000) && (g_Demod->frequency < 810000)) /*93.143Mhz~803.143 all freq*/
		 	{
			 	dib_tune(g_Demod->frequency, g_Demod->bandwidth/100 /* bandwidth_in_100KHz */);		 	
		 	}
	     }
		 
            break;
        }
        case DEMOD_TUNE:
			
            if(g_Demod->status != WORKING)
            {
            	   WARNING("demod is not working.\n");
                return EPDK_FAIL;
            }
            else
            {
                tune_param_t *tp = (tune_param_t*)pbuffer;

		     WARNING(" *****demodFreq[%d], bw[%d], freq[%d],bw[%d]\n",tp->frequency,  tp->bandwidth, g_Demod->frequency, g_Demod->bandwidth);
                
                if((g_Demod->frequency == tp->frequency) && (g_Demod->bandwidth == tp->bandwidth))
                {
		      WARNING("*******tune then lock status  return *********\n");
//                    if(dib_lockstatus() >= 1)    /*Bridge@Apr14--2012*/
//                        return EPDK_OK;
                }
                
                g_Demod->frequency = tp->frequency;
                g_Demod->bandwidth = tp->bandwidth;
                tmpret = dib_tune(tp->frequency, tp->bandwidth/100 /* bandwidth_in_100KHz */);
                if(EPDK_OK == tmpret)
                {
                    return EPDK_OK;
//                    esKRNL_TimeDly(25);
//                    if(dib_lockstatus() >= 1)
//                        return EPDK_OK;
//                    else
//                    {
//                        WARNING("why???????????????????????????\n");
//                        return EPDK_FAIL;
//                    }
                }
                else
                {
                	WARNING("demod dib_tune fail!\n");
                    return EPDK_FAIL;
                }
            }
            
        case DEMOD_SET_LOCK_CB:
            g_Demod->lockCbParam = (void*)aux;
            g_Demod->lockCb      = (__pCBK_t)pbuffer;
            result = 0;
            break;
            
        case DEMOD_GET_CUR_TUNE_PARAM:
            {
                tune_param_t *tp = (tune_param_t*)pbuffer;
                tp->frequency = g_Demod->frequency;
                tp->bandwidth = g_Demod->bandwidth;
                result = 0;
                break;
            }
            
        case DEMOD_GET_CA_SN:
            {
                memset(pbuffer, 0, aux);
                result = 0;
                break;
            }
            
        default :
            result = -1;
            break;
    }
    
    return result;
}


static void demod_lock_task(void* p)
{
    demod_dev_t* pDemod;
    
    pDemod = (demod_dev_t*)p;
    
    while(1)
    {
        //* check if there is a exit request.
        if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
        {
            WARNING("lock task receive exit request");
            break;
        }

		__msg("%s lock Judge_Month********\n\n", __func__);
		
        if(dib_lockstatus() == 0)
        {
            //* lock fail
            if(pDemod->lockCbParam)
            {
                pDemod->lockCbParam->lockOk = 0;
                pDemod->lockCbParam->signalStatus.strength = 0;
                pDemod->lockCbParam->signalStatus.quality  = 0;
            }
            
            if(pDemod->lockCb)
                esKRNL_CallBack(pDemod->lockCb, (void*)pDemod->lockCbParam);
        }
        else
        {
            //* lock fail
            if(pDemod->lockCbParam)
            {
                pDemod->lockCbParam->lockOk = 1;
                
//                pDemod->lockCbParam->signalStatus.quality = dib_signal_quality();
//                pDemod->lockCbParam->signalStatus.strength = dib_signal_strength();
		 dib_signal_strenth_quality(&(pDemod->lockCbParam->signalStatus.quality), &(pDemod->lockCbParam->signalStatus.strength));
            }
            
            if(pDemod->lockCb)
                esKRNL_CallBack(pDemod->lockCb, (void*)pDemod->lockCbParam);
        }
        
        esKRNL_TimeDly(100);
    }
    
    WARNING("lock task exit.\n");
    esKRNL_TDel(EXEC_prioself);
    
}
