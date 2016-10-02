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
#include "FC8300_api.h"



demod_dev_t* g_Demod = NULL;

#define abs(x) (x) >= 0 ? (x):-(x)


//* lock main task.
static void demod_lock_task(void* p)
{
    demod_dev_t* pDemod;
    
    pDemod = (demod_dev_t*)p;
    

    
    WARNING("lock task exit.\n");
    esKRNL_TDel(EXEC_prioself);
    
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
    
    //* 1. ио╣Г
    
    g_Demod->status = WORKING;
    
	WARNING("&&&& FC8300B MOpen\n");
    
    //* 2. initial demod;
    if(mtv_init(4) != 0)
    {
        WARNING("dibcom initial fail.\n");
        goto demod_open_fail;
    }
    return (__mp*)g_Demod;
    
demod_open_fail:
    
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

	mtv_deinit();

	WARNING("&&&& FC8300B MClose\n");
    
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
                
            break;
        }
        
        case DEMOD_UNLOCK:
        {
            
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
            
		    if(g_Demod->status == WORKING)
            {
				u8 lock;
				double CN;
				double BER_A;
				double PER_A;
				double BER_B;
				double PER_B;
				double BER_C; 
				double PER_C;
				s32 i32RSSI;
				
				ss = (demod_ss_t*)pbuffer;
                if(ss == NULL)
                    result = EPDK_FAIL;
                else
                {
					mtv_signal_measure(&lock,&CN,&BER_A,&PER_A,&BER_B,&PER_B,&BER_C,&PER_C,&i32RSSI);

					
					ss->quality = (u8)CN;
					ss->strength = abs(i32RSSI);

					{
						__u32 quality,strength;

						quality = (__u32)CN;
						strength = (__u32)i32RSSI;

						if(quality > 28)
							quality = 28;
						if(quality < 14)
							quality = 14;

						if(strength > -49)
							strength = -49;
						if(strength < -89)
							strength = -89;
							
						ss->quality = 98 - (28 - quality)*7;
						ss->strength = 100 - (-49 - strength)*2.5;
						
					}
					result = EPDK_OK;
                }
            }
            else
                result = EPDK_FAIL;
            
            break;
        }
        
        case DEMOD_GET_LOCK_STATUS:
        {
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

        		g_Demod->frequency = tp->frequency;
        		g_Demod->bandwidth = tp->bandwidth;

				tmpret =mtv_set_channel(tp->frequency,22);
				
				if(EPDK_OK == tmpret)
				{
        			eDbug("####set channel ok \n");

					tmpret = mtv_lock_check();

					if(EPDK_OK == tmpret)
					{
	        			eDbug("####channel locked \n");
						return EPDK_OK;
					}
					else
					{
	        			eDbug("####channel unlocked \n");
						return EPDK_FAIL;
					}
				}
        		
            }
            break;
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

				{
                    WARNING("########################DEMOD_TUNE_PLAY###############################\n");
	                g_Demod->frequency = tp->frequency;
	                g_Demod->bandwidth = tp->bandwidth;

					tmpret =mtv_set_channel(tp->frequency,22);
					
					if(EPDK_OK == tmpret)
					{
						eDbug("change channel ok \n");
						return EPDK_OK;
					}
					else
					{
						eDbug("change channel failed \n");
						return EPDK_FAIL;
					}

                }
            }				
			break;
		}

        		
        case DEMOD_SET_LOCK_CB:
        {
            break;
        }
        
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
            break;
        }
            
        default :
            result = -1;
            break;
    }
    
    return result;
}
