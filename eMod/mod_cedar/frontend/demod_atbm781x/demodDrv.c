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
#include "ATBM781X_app.h"

#define REG_PIOE_CFG0   (*((__u32*)0xf1c20890))
#define REG_PIOE_CFG1   (*((__u32*)0xf1c20894))
#define REG_PIOE_CFG2   (*((__u32*)0xf1c20898))
#define REG_PIOE_DATA   (*((__u32*)0xf1c208a0))

#define read_reg(reg)			(*(volatile unsigned long *)(reg))


static demod_dev_t* g_Demod = NULL;

static __u32 demod_init_ok = -1;

static __s32 demod_power_onoff(__bool b_en)
{
	__s32 ret;
	ES_FILE *h_power = eLIBs_fopen("b:\\HID\\POWER", "w");

	//WARNING("demod_power_onoff b_en = %d\n",b_en);
	if( b_en )
		ret = eLIBs_fioctrl(h_power, DRV_POWER_CMD_PWREPT_SET, POWER_EPT_3, (void *)((POWER_EPT_MOS_P) | (POWER_EPT_GATE_ON))); 
	else
    		ret = eLIBs_fioctrl(h_power, DRV_POWER_CMD_PWREPT_SET, POWER_EPT_3, (void *)((POWER_EPT_MOS_N) | (POWER_EPT_GATE_OFF)));
	
	if(ret)
	{
		WARNING("~~~~fail\n");
	}
	else
	{
		WARNING("~~~~ok\n");
		eLIBs_fclose(h_power);
   	}
	
	return 0;
}


//* lock main task.
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

		//WARNING("%s lock judge********\n\n", __func__);
		
        if(ATBM781X_lockstatus() == 0)
        {
		    WARNING("%s lock fail********\n\n", __func__);
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
    		WARNING("%s lock ok********\n\n", __func__);
            //* lock fail
            if(pDemod->lockCbParam)
            {
                pDemod->lockCbParam->lockOk = 1;
                
		        ATBM781X_signal_strength_quality(&(pDemod->lockCbParam->signalStatus.quality), &(pDemod->lockCbParam->signalStatus.strength), pDemod->t_or_t2);
            }
            
            if(pDemod->lockCb)
                esKRNL_CallBack(pDemod->lockCb, (void*)pDemod->lockCbParam);
        }
        
        esKRNL_TimeDly(100);
    }
    
    WARNING("lock task exit.\n");
    esKRNL_TDel(EXEC_prioself);
    
}

static __s32 ioInit(void)
{
    return 0;
}

static __s32 ioDeinit(void)
{
    return 0;
}

static void demodReset(__u8 rst)
{
	return;
}

__s32 demoPowerOn(void)
{
	__hdle hdl_1 = NULL;
	__hdle hdl_2 = NULL;
	__user_pin_set_t  gpio_set[1];

	//poweron
	demod_power_onoff(1);
	esKRNL_TimeDly(1);

	//tuner reset
	gpio_set->Port = 3;
	gpio_set->PortNum = 14;
	gpio_set->MultiSel = 1;
	gpio_set->Pull = 1;
	gpio_set->DrvLevel = 1;
	gpio_set->Data = 0;
	hdl_1 = esPINS_PinGrpReq(gpio_set, 1);
	
	//demod reset
	gpio_set->Port = 3;
	gpio_set->PortNum = 15;
	gpio_set->MultiSel = 1;
	gpio_set->Pull = 1;
	gpio_set->DrvLevel = 1;
	gpio_set->Data = 0;
	hdl_2 = esPINS_PinGrpReq(gpio_set, 1);

	esKRNL_TimeDly(1);
	esPINS_WritePinData(hdl_1, 0x00, 0);
	esKRNL_TimeDly(1);
	esPINS_WritePinData(hdl_2, 0x00, 0);
	
	esKRNL_TimeDly(4);

	esPINS_WritePinData(hdl_1, 0x01, 0);
	esKRNL_TimeDly(1);
	esPINS_WritePinData(hdl_2, 0x01, 0);

	esPINS_PinGrpRel(hdl_1, 1);
	esPINS_PinGrpRel(hdl_2, 1);
	
    return 0;
}

__s32 demoPowerOff(void)
{
	demod_power_onoff(0);

    return 0;
}

//* Initial hardware setting for tsc, such as dma, clock and io setting.
static __s32 InitSystemCfg(void)
{
    ioInit();
    iic_init();
        
    return 0;
}

//* Release hardware setting for tsc.
static __s32 DeinitSystemCfg(void)
{
    iic_exit();
    ioDeinit();
	
    return 0;
}


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
    
	WARNING("&&&& ATBM781X MOpen\n");
    
    //* 2. initial demod;
    if(ATBM781X_init() != 0)
    {
        WARNING("dibcom initial fail.\n");
        goto demod_open_fail;
    }
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
    //demod_dev_t* pDemod = (demod_dev_t*)mp;
    
    if(g_Demod == NULL)
    {
        return -1;
    }
	
    if(g_Demod->status == WORKING)
    {
        esMODS_MIoctrl((__mp*)g_Demod, DEMOD_SLEEP, 0, NULL);
    }

	ATBM781X_deinit();

	WARNING("&&&& ATBM781X MClose\n");
    //* 1. 卸电
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
        {
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
        }
        
        case DEMOD_WAKEUP:
        {
            //* wake up tuner.
            g_Demod->status = WORKING;
            result = 0;
            break;
        }
        
        case DEMOD_LOCK:
        {    
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
        }
        
        case DEMOD_UNLOCK:
        {
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
        }
        
        case DEMOD_GET_STATUS:
            return g_Demod->status;
            
        case DEMOD_GET_SIGNAL_STATUS:
        {    
            if(g_Demod->status != WORKING)
            {
                WARNING("demod is not working.\n");
                result = EPDK_FAIL;
            }
            
		    if((g_Demod->status == WORKING) && (ATBM781X_czp_lockstatus(g_Demod->t_or_t2) == 1))
            {
                ss = (demod_ss_t*)pbuffer;
                if(ss == NULL)
                    result = EPDK_FAIL;
                else
                {
			        ATBM781X_signal_strength_quality(&(ss->quality), &(ss->strength), g_Demod->t_or_t2);
                    result = EPDK_OK;
                }
            }
            else
                result = EPDK_FAIL;
            
            break;
        }
        
        case DEMOD_GET_LOCK_STATUS:
        {
            if(g_Demod->status != WORKING)
            {
                WARNING("demod is not working.\n");
                result = 0;
                break;
            }

            result = ATBM781X_lockstatus();
		  
    	    if(0 == result)
    	    {
    		    if((g_Demod->frequency > 90000) && (g_Demod->frequency < 810000)) /*93.143Mhz~803.143 all freq*/
    		 	{
    		 		WARNING("g_Demod->t_or_t2 = %d\n", g_Demod->t_or_t2);
    			 	ATBM781X_tune(g_Demod->frequency, g_Demod->bandwidth/100 /* bandwidth_in_100KHz */);		 	
    		 	}
    	    }
    		 
            break;
        }
        
        case DEMOD_TUNE:
		{	
            if(g_Demod->status != WORKING)
            {
            	   WARNING("demod is not working.\n");
                return EPDK_FAIL;
            }
            else
            {
                tune_param_t *tp = (tune_param_t*)pbuffer;
                
                if((g_Demod->frequency == tp->frequency) && (g_Demod->bandwidth == tp->bandwidth))
                {
			        WARNING("*****Tlock return\n");
                }

		     	eLIBs_printf(" *****demodFreq[%d], bw[%d], freq[%d],bw[%d]\n",tp->frequency,  tp->bandwidth, g_Demod->frequency, g_Demod->bandwidth);

		        if(g_Demod->bandwidth == tp->bandwidth)
			        tmpret = ATBM781X_tune(tp->frequency, 0);
		        else
			        tmpret = ATBM781X_tune(tp->frequency, tp->bandwidth);
	
        		g_Demod->frequency = tp->frequency;
        		g_Demod->bandwidth = tp->bandwidth;

        		__msg("ATBM781X_tune :tmpret = %d\n", tmpret);
        		
        		if(EPDK_OK == tmpret)
        		{
        			eDbug("####Tlock\n");
        			tmpret = ATBM781X_T2_2_TS(&(g_Demod->pDvbT2Params), &(g_Demod->pDvbTParams));
        			g_Demod->t_or_t2 = tmpret; //在此区别是T2还是T的信号
        			eDbug("g_Demod->t_or_t2 = %d,tmpret is %d \n", g_Demod->t_or_t2,tmpret);
        			if(tmpret > 0)
        				return EPDK_OK;
        			else
        				return EPDK_FAIL;
        		}
                else
                {
        			eDbug("####Tunlock\n");
                    return EPDK_FAIL;
                }
            }
         }

		case DEMOD_TUNE_DVB_T_SEARCH:
		{
            if(g_Demod->status != WORKING)
            {
            	   WARNING("demod is not working.\n");
                return EPDK_FAIL;
            }
            else
            {
                tune_param_t *tp = (tune_param_t*)pbuffer;
                
                if((g_Demod->frequency == tp->frequency) && (g_Demod->bandwidth == tp->bandwidth))
                {
					WARNING("*****Tlock return\n");
                }
				if(g_Demod->bandwidth == tp->bandwidth)
					tmpret = ATBM781X_dvb_t_tune(tp->frequency, 0);
				else
					tmpret = ATBM781X_dvb_t_tune(tp->frequency, tp->bandwidth);
			
				g_Demod->frequency = tp->frequency;
				g_Demod->bandwidth = tp->bandwidth;

				__msg("ATBM781X_tune :tmpret = %d\n", tmpret);
				
				return tmpret;
            }
        }

		case DEMOD_TUNE_PLAY:
		{
	 		if(g_Demod->status != WORKING)
            {
            	WARNING("demod is not working.\n");
                return EPDK_FAIL;
            }
            else
            {
                tune_param_t *tp = (tune_param_t*)pbuffer;

		        WARNING(" *****demodFreq[%d], bw[%d], freq[%d],bw[%d]\n",tp->frequency,  tp->bandwidth, g_Demod->frequency, g_Demod->bandwidth);
	            if((g_Demod->frequency == tp->frequency) && (g_Demod->bandwidth == tp->bandwidth) 
					&& (g_Demod->plpindex != tp->plpindex) && (g_Demod->plppid != tp->plppid))
                {
	                 g_Demod->frequency = tp->frequency;
	                 g_Demod->bandwidth = tp->bandwidth;
					 g_Demod->plpindex  = tp->plpindex;
					 g_Demod->plppid	= tp->plppid;
					 g_Demod->t_or_t2	= tp->t_t2_flag;
					 WARNING("g_Demod->t_or_t2 = %d\n", g_Demod->t_or_t2);
					 tmpret = ATBM781X_tune_0_play(tp->frequency, tp->bandwidth, tp->plpindex, tp->plppid);
                     demod_init_ok = tmpret;
                 }
			 
				else if((g_Demod->frequency == tp->frequency) && (g_Demod->bandwidth == tp->bandwidth) 
						&& (g_Demod->plpindex == tp->plpindex) && (g_Demod->plppid == tp->plppid))
				{
                    if(demod_init_ok == 0)
	                    tmpret = ATBM781X_tune_no_change_play(tp->frequency, tp->bandwidth, tp->plpindex, tp->plppid);
                    else
                        tmpret = ATBM781X_tune_play(tp->frequency, tp->bandwidth, tp->plpindex, tp->plppid);
                       
                }
				else
				{
                    WARNING("########################DEMOD_TUNE_PLAY###############################\n");
	                g_Demod->frequency = tp->frequency;
	                g_Demod->bandwidth = tp->bandwidth;
					g_Demod->plpindex  = tp->plpindex;
				    g_Demod->plppid	= tp->plppid;
					g_Demod->t_or_t2	= tp->t_t2_flag;
					WARNING("g_Demod->t_or_t2 = %d\n", g_Demod->t_or_t2);
	                tmpret = ATBM781X_tune_play(tp->frequency, tp->bandwidth, tp->plpindex, tp->plppid);
                    demod_init_ok = tmpret;

                }
				return tmpret;

	
            }				
			break;
		}

        case DEMOD_TUNE_T2_SEARCH:
		{	
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
                }
                WARNING("##########DEMOD_TUNE_T2_SEARCH#############################################\n");

                g_Demod->frequency = tp->frequency;
                g_Demod->bandwidth = tp->bandwidth;
				tmpret = check_channel_lock(&(g_Demod->pDvbT2Params), tp);
				return tmpret;
            }
        }
        
		case DEMOD_GET_TUNE_PLPPID:
		{
			{
				tune_param_t *tp = (tune_param_t*)pbuffer;
					tp->plppid = g_Demod->pDvbT2Params.u8DataPlpIdArray[tp->plpindex];
				result = 0;
				break;
			}
		}
		
        case DEMOD_SET_LOCK_CB:
        {
            g_Demod->lockCbParam = (void*)aux;
            g_Demod->lockCb      = (__pCBK_t)pbuffer;
            result = 0;
            break;
        }
        
        case DEMOD_GET_CUR_TUNE_PARAM:
        {
            {
                tune_param_t *tp = (tune_param_t*)pbuffer;
                tp->frequency = g_Demod->frequency;
                tp->bandwidth = g_Demod->bandwidth;
				tp->t_t2_flag = g_Demod->t_or_t2;

				if(g_Demod->t_or_t2 == 3)
				{
					tp->plpnumber = g_Demod->pDvbT2Params.u8DataPlpNumber;
					tp->plpindex  = 0;
				}

                result = 0;
                break;
            }
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
