/**
 * CopyRight: all rights reserved by rockchips fuzhou inc. 
 * 10.30.2010
 */



//#include <linux/module.h>
//#include <linux/device.h>
//#include <linux/kernel.h>
//#include <mach/hardware.h>
//#include <mach/iomux.h>
//#include <mach/gpio.h>
//#include <mach/irqs.h>

//#include <TSTV.h>
//#include <stdio.h>
#include "ATBM781X_app.h"

/********************************************************************************************/
//
//
//    DEBUG Options
//
//
/********************************************************************************************/
#if 0
#define DBG(x...)	WARNING(x...)
#else
#define DBG(x...)
#endif

#define UPDATE_CNT 					2
#define DELAY_CNT 					6
#define DELAYSTREN_CNT 				3
#define ATBM781X_I2C_ADDR 			0x40
#define MAXLINERA_I2C_ADDR			0x60
#define abs(x) (x) >= 0 ? (x):-(x)

static UINT8 ATBM781X_T2_UPDATE = 0;

/********************************************************************************************/
//
//
//    Source Code
//
//
/********************************************************************************************/

__s32 ATBM781X_init(void)
{
	int	ret = 0;

	/*	
	if(ATBM781X_STATE)
	{
		ATBMStandbyWakeUp();
	}
	else
	*/		
	{
		ATBM781xPowerOnInit();
		ATBM781xSetDVBTxMode(8000);
		//ATBMSetDVBT2BaseAndLiteProfile();

		//TUNER MODULE	
		ATBM781xHoldDSP();
		ATBM781xTunerI2CViaDemodOn();
		esKRNL_TimeDly(DELAY_CNT);

		//tuner
		MXL603RegInit();

		esKRNL_TimeDly(DELAY_CNT);
		ATBM781xTunerI2CViaDemodOff();
		ATBM781xStartDSP();

		//ATBM781X_STATE = 1;
	}

	return ret;
}


void ATBM781X_deinit(void)
{
	//tuner
	//ATBM781xHoldDSP();
	//ATBMI2CByPassOn();
	ATBM781xTunerI2CViaDemodOn();
	esKRNL_TimeDly(DELAYSTREN_CNT);

	MxLWare603_API_CfgDevPowerMode(0x60, MXL603_PWR_MODE_STANDBY);

	esKRNL_TimeDly(DELAYSTREN_CNT);
	ATBM781xTunerI2CViaDemodOff();
	//ATBMI2CByPassOff();
	//ATBM781xStartDSP();

	//demod
	//ATBMStandby();
	//ATBM781xSusPend();
	ATBM781xSusPend();
	return;
}

__s32 ATBM781X_dvb_t_tune(unsigned int frequency_KHz, unsigned int bandwidth_KHz)
{
	__s32	ret = -1;
	__u8	result = 0;

	ATBM781xHoldDSP();
	ATBM781xTunerI2CViaDemodOn();
	esKRNL_TimeDly(DELAY_CNT);

	MXL603FreLock(frequency_KHz*1000);

	esKRNL_TimeDly(DELAY_CNT);
	ATBM781xTunerI2CViaDemodOff();
	ATBM781xStartDSP();

	if(bandwidth_KHz) //带框改变的时候才去设置下带宽   如果是同一带宽，传进来的是0
		ATBM781xSetDVBTxMode(bandwidth_KHz);
	
	//demod
	ATBM781xSetAutoPLPT2();
	ATBM781xSetDVBT2DisablePLPDecode();

	//esKRNL_TimeDly(35);
	ATBM781xDVBTxSigalParamsLockCheck(&result);
	__msg("ATBM781x: signal check: result = %d\n", result);
	if(result)
	{
		__msg("tune LOCK ---->>>\n");
		//ret = 0;
		ATBM781xSetDefaultHierarchyHpT();
		ATBM781xDVBTxChannelLockCheck(&result);

		esKRNL_TimeDly(3);

		if(ret)
			return EPDK_OK;
		else
			return EPDK_FAIL;
		
	}
	else
	{	
		__msg("tune UNLOCK ---->>>\n");
		ret = -1;
	}

	return ret;
}

__s32 ATBM781X_tune(unsigned int frequency_KHz, unsigned int bandwidth_KHz)
{
	__s32	ret = -1;
	__u8	result = 0;

	ATBM781xHoldDSP();
	ATBM781xTunerI2CViaDemodOn();
	esKRNL_TimeDly(DELAY_CNT);

	MXL603FreLock(frequency_KHz*1000);

	esKRNL_TimeDly(DELAY_CNT);
	
	result = ATBM781xTunerI2CViaDemodOff();
	
	result = ATBM781xStartDSP();

	if(bandwidth_KHz) //带框改变的时候才去设置下带宽   如果是同一带宽，传进来的是0
    {
		result = ATBM781xSetDVBTxMode(bandwidth_KHz);
    }
	//demod
	result = ATBM781xSetAutoPLPT2();
	
	result = ATBM781xSetDVBT2DisablePLPDecode();

	ATBM781xDVBTxSigalParamsLockCheck(&result);
	eDbug("ATBM781x: signal check: result = %d\n", result);
	if(result)
	{
		eDbug("tune LOCK ---->>>\n");
		ret = 0;
	}
	else
	{	
		eDbug("tune UNLOCK ---->>>\n");
		ret = -1;
	}

	return ret;
}


__s32 ATBM781X_tune_0_play(unsigned int frequency_KHz, unsigned int bandwidth_KHz, __u8 plpindex, __u8 plpid)
{

	__s32	ret = -1;
	__u8	result = 0;
	__u8	tv_mode = 0;

	
//demod
	ATBM781xSetAutoPLPT2();
	ATBM781xSetDVBT2DisablePLPDecode();


	ATBM781xDVBTxSigalParamsLockCheck(&result);
    __msg("ATBM781x: signal check: result = %d\n", result);
	if(result)
	{
		__msg("SIGNAL LOCK ret = 0---->>>\n");
		ATBM781xDVBTxModeDetected(&tv_mode);
		ret = 0;
	}
	else
	{	
		__msg("SIGNAL UNLOCK ret = -1 ---->>>\n");
		ret = -1;
		return ret;
	}

	if(tv_mode == 2)
	{
		ATBM781xSetDefaultHierarchyHpT();
		if(ret == 0)
		{
			ATBM781xDVBTxChannelLockCheck(&result);
			if(result)
				ret = 0;
			else
				ret = -1;
		}
	}
	else if(tv_mode == 3)
	{
		ATBM781xSetDVBT2EnablePLPDecode();
		if(ret == 0)
		{
			switch(plpindex)
			{
				case 0:
					ATBM781xSetAutoPLPT2();
					ATBM781xDVBTxChannelLockCheckForManual(&result);
					__msg("ATBM781xSetAutoPLPT2: result = %d\n", result);
					break;
				default:
					ATBM781xSetPLPForMultiPLPDVBT2(plpid);
					ATBM781xDVBTxChannelLockCheckForManual(&result);
					__msg("ATBM781xSetPLPForMultiPLPDVBT2:plpid: %d  result = %d\n", plpid, result);
					break;
			}
			if(result)
				ret = 0;
			else
				ret = -1;
		}
	}
	else
	{
		ret = -1;
	}
	
	return ret;
}

__s32 ATBM781X_tune_no_change_play(unsigned int frequency_KHz, unsigned int bandwidth_KHz, __u8 plpindex, __u8 plpid)
{
	__s32	ret = -1;
	__u8	result = 0;

	ATBM781xTSLockFlag(&result);

	if(result)
			ret = 0;
		else
			ret = -1;

	return ret ;
}

__s32 ATBM781X_tune_play(unsigned int frequency_KHz, unsigned int bandwidth_KHz, __u8 plpindex, __u8 plpid)
{

	__s32	ret = -1;
	__u8	result = 0;
	__u8	tv_mode = 0;




	__msg("ATBM781X_tune_play: bandwidth_KHz = %d---\n", bandwidth_KHz);
	if(bandwidth_KHz)
	    ATBM781xSetDVBTxMode(bandwidth_KHz);

	ATBM781xHoldDSP();
	//ATBMI2CByPassOn();
	ATBM781xTunerI2CViaDemodOn();
	esKRNL_TimeDly(DELAY_CNT);
	
	MXL603FreLock(frequency_KHz*1000);
	
	esKRNL_TimeDly(60);
	//ATBMI2CByPassOff();
	ATBM781xTunerI2CViaDemodOff();
	ATBM781xStartDSP();

//demod
	ATBM781xSetAutoPLPT2();
	ATBM781xSetDVBT2DisablePLPDecode();

	ATBM781xDVBTxSigalParamsLockCheck(&result);
__msg("ATBM781x: signal check: result = %d\n", result);
	if(result)
	{
		__msg("SIGNAL LOCK ret = 0---->>>\n");
		ATBM781xDVBTxModeDetected(&tv_mode);
		ret = 0;
	}
	else
	{	
		__msg("SIGNAL UNLOCK ret = -1 ---->>>\n");
		ret = -1;
		return ret;
	}

	if(tv_mode == 2)
	{
		ATBM781xSetDefaultHierarchyHpT();
		if(ret == 0)
		{
			ATBM781xDVBTxChannelLockCheck(&result);
			if(result)
				ret = 0;
			else
				ret = -1;
		}
	}
	else
	{
		ATBM781xSetDVBT2EnablePLPDecode();
		if(ret == 0)
		{
			switch(plpindex)
			{
				case 0:
					ATBM781xSetAutoPLPT2();
					ATBM781xDVBTxChannelLockCheck(&result);

					break;
				default:
					ATBM781xSetPLPForMultiPLPDVBT2(plpid);
					ATBM781xDVBTxChannelLockCheck(&result);
					break;
			}
			if(result)
				ret = 0;
			else
				ret = -1;
		}
	}
	return ret;
}


//CAI++ FOR 搜到台的话需要保存3个变量
__s32 ATBM781X_T2_2_TS(ATBM781x_DVBT2_PARAMS *pDvbT2Params,ATBM781x_DVBT_PARAMS* pDvbTParams)
{
	__s32	ret = -1, tv_count = 0, i = 0, j = 0;
	__u8	tv_mode = 0,  result = 0;

	ATBM781xDVBTxModeDetected(&tv_mode);
	if(ATBM781x_DVBT2_MODE == tv_mode)
	{
		ATBM781xGetSignalParamsT2(pDvbT2Params);
		
		ATBM781xSetDVBT2EnablePLPDecode();
		__msg("ATBM781x_DVBT2_MODE ---->>>\n");
		ret = 3;

	}
	else if(ATBM781x_DVBT_MODE == tv_mode)
	{
		ATBM781xGetSignalParamsT(pDvbTParams);

		ATBM781xSetDefaultHierarchyHpT();
		__msg("ATBM781x_DVBT_MODE ---->>>\n");	
		ret = 2;
	}
	else
	{
		ret = -1;
	}
	return ret;
}

__s32 ATBM781X_T2_TUNER()
{
	__s32	ret = -1, tv_count = 0;
	__u8	tv_mode = 0, i = 0, result = 0;
	ATBM781x_DVBT2_PARAMS pDvbT2Params;

	ATBM781xDVBTxModeDetected(&tv_mode);
	if(ATBM781x_DVBT2_MODE == tv_mode)
	{
		ATBM781xGetSignalParamsT2(&pDvbT2Params);
		
		ATBM781xSetDVBT2EnablePLPDecode();

		ret = pDvbT2Params.u8DataPlpNumber;
	}
	else
	{
		ret = -1;
	}
	return ret;
}

__s32 check_channel_lock(ATBM781x_DVBT2_PARAMS *pDvbT2Params, tune_param_t *tp)
{
	__u8 result = 0;
	WARNING("tp->plpindex = %d\n", tp->plpindex);
	switch(tp->plpindex)
	{
		case 0:
			WARNING("come here case 0");
			ATBM781xSetAutoPLPT2();
			ATBM781xDVBTxChannelLockCheck(&result);
		break;
		default:
			ATBM781xSetPLPForMultiPLPDVBT2(pDvbT2Params->u8DataPlpIdArray[tp->plpindex]);
			ATBM781xDVBTxChannelLockCheck(&result);
		break;
	}
	esKRNL_TimeDly(3);
	WARNING("check_channel_lock: result = %d\n", result);
	
	if(result)
		return EPDK_OK;
	else
		return EPDK_FAIL;
}

__s32 ATBM781X_lockstatus(void)
{
	__u8 ret = 0;

	ATBM781xDVBTxSigalParamsLockCheck(&ret);
	if(ret)
	{
		WARNING("##Dlock\n");
		return 1;		
	}
	else
	{
		WARNING("##Dunlock\n");
		return 0;
	}

}

__s32 ATBM781X_czp_lockstatus(__s32	t_t2_flag)
{
	__u8 ret = 0;

	ATBM781xDVBTxSigalParamsLockCheck(&ret);

	if(ret)
	{
		if(t_t2_flag == 3)
		{
			if(ATBM781X_T2_UPDATE >= UPDATE_CNT)
			{
				//WARNING("##update\n");
				ATBM781xDVBT2ThreadProcess();
				ATBM781X_T2_UPDATE = 0;
			}
			else
			{
				ATBM781X_T2_UPDATE++;
			}
		}
		WARNING("###ATBM781X_czp_lockstatus######lock\n");
		return 1;		
	}
	else
	{
		WARNING("###ATBM781X_czp_lockstatus######unlock\n");
		return 0;
	}

}




void ATBM781X_signal_strength_quality(UINT8* signal_quality, UINT8* signal_strength, __s32	t_t2_flag)
{
	int	ret = 0;
	short rxpower = 0,strength = 0;

	//ATBM781xHoldDSP();
	ATBM781xTunerI2CViaDemodOn();
	esKRNL_TimeDly(DELAYSTREN_CNT);

	ret = MxLWare603_API_ReqTunerRxPower(0x60,&rxpower);

	esKRNL_TimeDly(DELAYSTREN_CNT);
	ATBM781xTunerI2CViaDemodOff();
	//ATBM781xStartDSP();

	//ts check
	//ATBMFrameErrorRatio();


	if(1 == ret)//MXL_FAILED
	{
		*signal_strength = 0;
	}
	else
	{
		strength = abs(rxpower);
		strength = strength/100;
		
		if(strength >= 130)
			strength = 20;
		else if(strength >= 70)
			strength = 160-strength;//90-30
		else
			strength = 92;
	}

	*signal_strength = strength;

	if(2 == t_t2_flag)
		ATBM781xGetDVBTSigalQuality(signal_quality);
	else
		ATBM781xGetDVBT2SignalQuality(signal_quality);

	WARNING("#####ret = %d,signal_quality = %d,signal_strength = %d, t_t2_flag = %d\n",ret,*signal_quality,*signal_strength, t_t2_flag);
	
	return;	
}


__s32 ATBM781X_i2c_addr(void)
{
	return ATBM781X_I2C_ADDR;
}


__s32 ATBM781X_i2c_speed(void)
{
	return 100;
}

/********************************************************************************************/
//
//
//    OVER
//
//
/********************************************************************************************/

