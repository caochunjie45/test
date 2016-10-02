
#include "Atbm781x.h"



/*#define ENABLE_BLIND_SCAN_PSI_SI_PARSING 1*/
volatile uint8 ATBM781xDvbSxBlindScanState = 0;

struct ATBM781xDvbSxChannelScanParams gATBM781xDvbSxScanParams;

const char *gStrDvbSCodeRate[]        = {"1/2", "2/3", "3/4", "5/6", "7/8", "unknown"};
const char *gStrDvbS2CodeRate[]       = {"unknown","1/4", "1/3","2/5", "1/2","3/5", "2/3", "3/4", "4/5", "5/6", "8/9", "9/10", "unknown" };
const char *gStrDvbSxConstellation[]  ={"QPSK", "8PSK", "16APSK", "32APSK", "unknown"};

/****************************************************************************
Function:    ATBM781xDvbsSetTunerFrequency
Parameters: u32CenterFrequencyk, u32symbolratek
Return:      0 is Success,Other is Failed
Description:      
This function set DVB-S/S2 tuner. 
****************************************************************************/
ATBM781x_STATUS ATBM781xDvbsSetTunerFrequency(uint32 u32CenterFrequencyk, uint32 u32symbolratek)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	/*Port Tuner interface*/
	/*	ATBM781xTunerI2CViaDemodOn();
	RDA5815Initial();
	RDA5815Set(u32CenterFrequencyk/1000, u32symbolratek);
	status |=ATBM781xSleep(10);
	RDA5815Initial();
	RDA5815Set(u32CenterFrequencyk/1000, u32symbolratek);
	ATBM781xTunerI2CViaDemodOff();
	status |=ATBM781xSetDvbSxSpectrumAuto(); 
	*/
	return status;
}

/****************************************************************************
Function:    ATBM781xDvbsTunerLockFlag
Parameters: pTunerLockFlag, 
Return:      0 is Success,Other is Failed
Description:      
The lock flag indicates DVB-S/S2 tuner lock status.
****************************************************************************/
ATBM781x_STATUS ATBM781xDvbsTunerLockFlag(uint8 *pTunerLockFlag)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	/*Port Tuner interface*/
	/* *pTunerLockFlag = 1 or *pTunerLockFlag = 0*/
	*pTunerLockFlag = 1;


	return status;
}


/****************************************************************************
Function:    ATBM781xDvbSxBlindScanInit
Parameters: none, 
Return:      0 is Success,Other is Failed
Description:      
This  function Initialize blind scan, the parameters can be adjusted depends
on scanning range and tuner used.
****************************************************************************/
ATBM781x_STATUS ATBM781xDvbSxBlindScanInit(void)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint32 u32Iter = 0;

	/*The following setting maybe changed be DTV application, such scan a small frequency range.*/
	gATBM781xDvbSxScanParams.u32ScanStartFrequencyKHz = 950*1000;  /*Blind scan start frequency, may be changed.*/
	gATBM781xDvbSxScanParams.u32ScanStopFrequencyKHz  = 2150*1000; /*Blind Scan stop frequency.*/
	gATBM781xDvbSxScanParams.u32MinSymbolRateK        = 2000;      /*optional: 1000K--->3000K minimal symbol rate will be received. */
	gATBM781xDvbSxScanParams.u32MaxSymbolRateK        = 45000;
	gATBM781xDvbSxScanParams.u32BlindScanStepKHz      = 10000;
	gATBM781xDvbSxScanParams.u32BlindScanFineStepKHz  = 2000;
	gATBM781xDvbSxScanParams.u8SingalMayBeChecked     = 1;
	gATBM781xDvbSxScanParams.u32MaxCarrierOffsetKHz   = 10000;
	gATBM781xDvbSxScanParams.i32ScanCheckedCFOKHz     = 0;
	/*--Scan setting parameters end-------------------------*/
	gATBM781xDvbSxScanParams.u32PreTunedAbsCFOKHz     = 0;
	gATBM781xDvbSxScanParams.u32PreTunedSymbolRateK   = 0;
	gATBM781xDvbSxScanParams.u32PreTunedCorseSymbolRateK = 0;
	gATBM781xDvbSxScanParams.u32PreTunedRaciPower     = 0;
	gATBM781xDvbSxScanParams.u32ValidChannelNumber    = 0;
	gATBM781xDvbSxScanParams.ui32ScanedValideChannelNum = 0;
	gATBM781xDvbSxScanParams.u32CurrentWorkFreKHz     = gATBM781xDvbSxScanParams.u32ScanStartFrequencyKHz;
	gATBM781xDvbSxScanParams.u32ScanPreCheckedFreqKHz = 0;

	gATBM781xDvbSxScanParams.u32TunerLPF              = 40;  /*Max DVB-S/S2 bandwidth setting 40MHz LPF.*/

	for(u32Iter = 0; u32Iter < MAX_SCANED_CHANNEL_NUMBER; u32Iter++)
	{
		gATBM781xDvbSxScanParams.ATBM781xDvbSxChannel[u32Iter].u32FrequencyKHz = 0;
		gATBM781xDvbSxScanParams.ATBM781xDvbSxChannel[u32Iter].u32SymbolRateK = 0;	  

		gATBM781xDvbSxScanParams.ATBM781xDvbSxChannel[u32Iter].stDvbSxSignalPrams.DVBS2CodeRate = ATBM781x_DVBS2_CODE_RATE_DUMMY;
		gATBM781xDvbSxScanParams.ATBM781xDvbSxChannel[u32Iter].stDvbSxSignalPrams.DVBSCodeRate = ATBM781x_DVBS_CODE_RATE_UNKNOWN;
		gATBM781xDvbSxScanParams.ATBM781xDvbSxChannel[u32Iter].stDvbSxSignalPrams.DVBSxModu = ATBM781x_DVBSx_MODU_UNKNOWN;
		gATBM781xDvbSxScanParams.ATBM781xDvbSxChannel[u32Iter].stDvbSxSignalPrams.DVBSxSymbolRateK = 0;
	}

	gATBM781xDvbSxScanParams.pFuncTunerSetFrequency = ATBM781xDvbsSetTunerFrequency;
	gATBM781xDvbSxScanParams.pFuncTunerLockStatus   = ATBM781xDvbsTunerLockFlag;

	ATBM781xDvbSxBlindScanState = 1;
	return status;
}

/****************************************************************************
Function:    ATBM781xDvbSxBlindScanVerifyChannel
Parameters: u32CenterFrequency, pu8Exist: 0 indicate no same channel exist in range.
Return:      0 is Success,Other is Failed
Description:      
This  function is an internal function call be blind scan functions
****************************************************************************/
ATBM781x_STATUS ATBM781xDvbSxBlindScanVerifyChannel(uint32 u32CenterFrequency, uint8 *pu8Exist)
{
	uint32 u32iter = 0;
	int i32IterCenterFreq = 0;
	int i32CenterFrequency = 0;
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;

	i32CenterFrequency = u32CenterFrequency;
	if(gATBM781xDvbSxScanParams.u32ValidChannelNumber == 0)
	{
		*pu8Exist = 0;
		status = ATBM781x_SUCCESSFUL;
		return status;
	}

	for(u32iter =0; u32iter<gATBM781xDvbSxScanParams.u32ValidChannelNumber; u32iter++)
	{
		i32IterCenterFreq = gATBM781xDvbSxScanParams.ATBM781xDvbSxChannel[u32iter].u32FrequencyKHz;
		if((i32CenterFrequency < i32IterCenterFreq + (int)(gATBM781xDvbSxScanParams.ATBM781xDvbSxChannel[u32iter].u32SymbolRateK/2))&&(i32CenterFrequency > i32IterCenterFreq - (int)(gATBM781xDvbSxScanParams.ATBM781xDvbSxChannel[u32iter].u32SymbolRateK/2))) /*carrier offset <1000KHz*/
		{
			*pu8Exist = 1; /*found one channel which have equal center frequency in channel list, should skip it.*/
			return status;
		}
	}
	/*Check for Channel list memory for blind scan*/
	if(gATBM781xDvbSxScanParams.u32ValidChannelNumber + 1 < MAX_SCANED_CHANNEL_NUMBER)
	{
		*pu8Exist = 0;
		status = ATBM781x_SUCCESSFUL;
	}else{
		*pu8Exist = 1;
		status = ATBM781x_FAILED;
	}
	return status;
}


/****************************************************************************
Function:    ATBM781xDvbSxBlindScanOneChannel
Parameters: u32CenterFrequency, pNewChannel:1 indicate new channel locked.
Return:      0 is Success,Other is Failed
Description:      
This  function is an internal function call be blind scan functions
****************************************************************************/
ATBM781x_STATUS ATBM781xDvbSxBlindScanOneChannel(uint32 CenterFrequency, uint8 *pNewChannel)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint8   u8LockFlag = 0;
	int     i32CFOKHz  = 0;
	uint32  u32AbsCFOKHz = 0;
	uint32  u32SymbolrateK = 0;	
	uint32  u32RaciPower = 0;
	uint8   ChannelExist  = 0;	
	uint32  u32MaxSymbolRate = gATBM781xDvbSxScanParams.u32MaxSymbolRateK;
	uint32  u32MinSymbolrate =  gATBM781xDvbSxScanParams.u32MinSymbolRateK;
	uint32  u32CenterFrequencyKHz = CenterFrequency;
	uint8   u8DemodLockedFlag = 0;
	uint8   u8TunerLockFlag = 0;
	int    i32LockedCFO = 10000; /*KHz, Init value will be set as signal checked*/
	uint32 u32AbsLockedCFO = 10000;
	uint8  u8CheckedTimes = 0;
	uint32 u32CheckedFreqOffsetKHz = 0, u32CoarseSymbolFakeDetectedK = 0;
	int  i32TmpCenterFreqKHz = 0;	
	*pNewChannel = 0;
	gATBM781xDvbSxScanParams.i32ScanCheckedCFOKHz = 0;
	gATBM781xDvbSxScanParams.u32PreTunedAbsCFOKHz = 0;
	gATBM781xDvbSxScanParams.u32PreTunedSymbolRateK = 0;	
	gATBM781xDvbSxScanParams.u32PreTunedCorseSymbolRateK = 0;
	gATBM781xDvbSxScanParams.u32PreTunedRaciPower = 0;
	gATBM781xDvbSxScanParams.u8SingalMayBeChecked = 1; /*default */
	/*Check whether current center frequency were in scanning frequency range.*/
	if((u32CenterFrequencyKHz < gATBM781xDvbSxScanParams.u32ScanStartFrequencyKHz)||(u32CenterFrequencyKHz > gATBM781xDvbSxScanParams.u32ScanStopFrequencyKHz))
	{
		status = ATBM781x_FAILED;
		return status;
	}

	status  =ATBM781xHoldDSP();
	status  |= ATBM781xTunerI2CViaDemodOn();
	status  |= gATBM781xDvbSxScanParams.pFuncTunerSetFrequency(u32CenterFrequencyKHz, u32MaxSymbolRate);
	status  |= ATBM781xSleep(20);
	status  |= gATBM781xDvbSxScanParams.pFuncTunerLockStatus(&u8LockFlag);
	status  |= ATBM781xTunerI2CViaDemodOff();
	status  |= ATBM781xSetDvbSxSpectrumAuto(); //Set IQ as last detected signal spectrum indicates,
	status  |= ATBM781xStartDSP();


	/*Check Tuner is locked*/
	if((status == ATBM781x_SUCCESSFUL)&&(u8LockFlag == 1))
	{
		status |= ATBM781xHoldDSP();
		status |= ATBM781xStartDSP(); /*Make sure DSP is reset.*/
		status |= ATBM781xSleep(100);
		status |= ATBM781xGetDVBSxCarrierOffset(&i32CFOKHz);
		status |=ATBM781xGetDVBSxSymbolRate(&u32SymbolrateK);
		status |=ATBM781xGetDVBS2RightCCIPower(&u32RaciPower);
		status |=ATBM781xDVBSxCoarseSymbolRate(&u32CoarseSymbolFakeDetectedK);
		u32AbsCFOKHz = i32CFOKHz > 0? i32CFOKHz: -i32CFOKHz;
		gATBM781xDvbSxScanParams.u32PreTunedAbsCFOKHz = u32AbsCFOKHz;
		gATBM781xDvbSxScanParams.u32PreTunedSymbolRateK = u32SymbolrateK;
		gATBM781xDvbSxScanParams.u32PreTunedRaciPower = u32RaciPower;
		gATBM781xDvbSxScanParams.u32PreTunedCorseSymbolRateK = u32CoarseSymbolFakeDetectedK;
		gATBM781xDvbSxScanParams.i32ScanCheckedCFOKHz = i32CFOKHz;

		/*StrDbgTmp.Format(L"Scan Tune:FreqS: %d SymbolRate: %d  CoarseSymbolRate: %d CoarseCFO:%d RaciPower: 0x%x\n", u32CenterFrequencyKHz, u32SymbolrateK,u32CoarseSymbolFakeDetectedK, i32CFOKHz, u32RaciPower);*/
		/*dvbs_logFile.WriteString(StrDbgTmp);*/

		/*make sure CFO is valid, If this is a valid channel with large CFO, CFO can be smaller in next scan.*/
		if((u32AbsCFOKHz < gATBM781xDvbSxScanParams.u32MaxCarrierOffsetKHz)&&(u32SymbolrateK <= (u32MaxSymbolRate + 1000))&&(u32SymbolrateK > u32MinSymbolrate)&&(status == ATBM781x_SUCCESSFUL))
		{        
			i32TmpCenterFreqKHz = u32CenterFrequencyKHz;
			if((u32SymbolrateK <5000)&&(u32AbsCFOKHz <3000))
			{
				if(u32AbsCFOKHz >= 2000)
				{
					if(i32CFOKHz > 0)
					{
						i32TmpCenterFreqKHz = i32TmpCenterFreqKHz  + i32CFOKHz - 1000;
					}
					else
					{
						i32TmpCenterFreqKHz = i32TmpCenterFreqKHz  + i32CFOKHz + 1000;
					}

				}else
				{
					i32TmpCenterFreqKHz = i32TmpCenterFreqKHz + i32CFOKHz/2; 
				} 
			}
			else
			{
				i32TmpCenterFreqKHz = i32TmpCenterFreqKHz + i32CFOKHz;
			}
			u32CenterFrequencyKHz = (uint32)(i32TmpCenterFreqKHz);
			while((u32AbsLockedCFO > 1000)&&(u8CheckedTimes < 1)) /*CFO > 1000K, will calibrate it if signal locked.*/
			{
				status = ATBM781xDvbSxBlindScanVerifyChannel(u32CenterFrequencyKHz, &ChannelExist);

				u32CheckedFreqOffsetKHz = (gATBM781xDvbSxScanParams.u32ScanPreCheckedFreqKHz > u32CenterFrequencyKHz)? (gATBM781xDvbSxScanParams.u32ScanPreCheckedFreqKHz - u32CenterFrequencyKHz):(u32CenterFrequencyKHz- gATBM781xDvbSxScanParams.u32ScanPreCheckedFreqKHz);
				if(u32CheckedFreqOffsetKHz < 1000) /*should not lock the same frequency*/
				{
					break; /*Have saved the same channel or invalid signal checked*/
				}
				if(i32CFOKHz < -5000)
				{
					/*gATBM781xDvbSxScanParams.u8SingalMayBeChecked*/
					break; /*Have saved the same channel or invalid signal checked*/
				}
				if((i32CFOKHz >= -5000)&&(i32CFOKHz <= -3000)&&(u32CheckedFreqOffsetKHz <6000))/*-5000 changed to -3000*/
				{
					break;
				}
				if((status == ATBM781x_SUCCESSFUL)&&(ChannelExist != 1)) 
				{
					status  =ATBM781xHoldDSP();
					status  |= ATBM781xTunerI2CViaDemodOn();
					status  |= gATBM781xDvbSxScanParams.pFuncTunerSetFrequency(u32CenterFrequencyKHz, u32SymbolrateK);
					status  |= ATBM781xSleep(20);
					status  |= gATBM781xDvbSxScanParams.pFuncTunerLockStatus(&u8TunerLockFlag);
					status  |= ATBM781xTunerI2CViaDemodOff();
					status  |= ATBM781xSetDvbSxSpectrumAuto(); 
					status  |= ATBM781xStartDSP();


					/*Set DVBS/S2 auto detection mode				*/
					status |=ATBM781xSetDvbSxAutoMode( );
					status |=ATBM781xSleep(100);
					status|= ATBM781xGetDVBSxCarrierOffset(&i32CFOKHz);
					status|= ATBM781xGetDVBSxSymbolRate(&u32SymbolrateK);
					if((i32CFOKHz > 5000)||(u32SymbolrateK < u32MinSymbolrate)||(u32SymbolrateK >(u32MaxSymbolRate + 1000)))
					{
						/*gATBM781xDvbSxScanParams.u8SingalMayBeChecked = 1;*/
						break; /*after jump, CFO is big, may not lock stable*/
					}
					status |=ATBM781xDVBSxChannelLockCheckBlindScan(&u8DemodLockedFlag);
					gATBM781xDvbSxScanParams.u32ScanPreCheckedFreqKHz = u32CenterFrequencyKHz; /*Record the checked frequency.*/
					if(u8DemodLockedFlag)
					{
						status|=ATBM781xSleep(200);
						status|= ATBM781xGetDVBSxLockedCFO(&i32LockedCFO);
						status|= ATBM781xGetDVBSxSymbolRate(&u32SymbolrateK);
						u32AbsLockedCFO = i32LockedCFO > 0? i32LockedCFO: -i32LockedCFO;

						/*Calibrate tuned frequency by locked CFO*/
						i32TmpCenterFreqKHz = u32CenterFrequencyKHz;
						i32TmpCenterFreqKHz = i32TmpCenterFreqKHz + i32LockedCFO;
						u32CenterFrequencyKHz =(uint32)i32TmpCenterFreqKHz;			 
						status |=ATBM781xDVBSxSignalParams(&gATBM781xDvbSxScanParams.ATBM781xDvbSxChannel[gATBM781xDvbSxScanParams.u32ValidChannelNumber].stDvbSxSignalPrams);   
						/*StrDbgTmp.Format(L"Result of TuneAgain:Freq = Tune+LockedCF0: %d Symbol rate: %d LockedCFO:%d u8DemodLockedFlag:%d\n", u32CenterFrequencyKHz, u32SymbolrateK, i32LockedCFO, u8DemodLockedFlag);*/
						/*}					*/
					}
					else
					{
						status|= ATBM781xGetDVBSxCarrierOffset(&i32CFOKHz);
						status|= ATBM781xGetDVBSxSymbolRate(&u32SymbolrateK);
						/*StrDbgTmp.Format(L"Result of TuneAgain:Freq: %d Symbol rate: %d CoarseCFO:%d u8DemodLockedFlag:%d \n", u32CenterFrequencyKHz, u32SymbolrateK, i32CFOKHz, u8DemodLockedFlag);*/
						/*dvbs_logFile.WriteString(StrDbgTmp);										*/
						break;
					}					
				}
				else
				{
					break; /*Have saved the same channel*/
				}
				/*#endif*/
				/*if((status == ATBM781x_SUCCESSFUL)&&(ChannelExist != 1)&&(u8DemodLockedFlag)&&(u32AbsLockedCFO <=1000))  //Save a new channel*/
				status = ATBM781xDvbSxBlindScanVerifyChannel(u32CenterFrequencyKHz, &ChannelExist);
				if((status == ATBM781x_SUCCESSFUL)&&(ChannelExist != 1)&&(u8DemodLockedFlag))  /*Save a new channel*/
				{
					gATBM781xDvbSxScanParams.ATBM781xDvbSxChannel[ gATBM781xDvbSxScanParams.u32ValidChannelNumber].u32FrequencyKHz = u32CenterFrequencyKHz;
					gATBM781xDvbSxScanParams.ATBM781xDvbSxChannel[ gATBM781xDvbSxScanParams.u32ValidChannelNumber].u32SymbolRateK = u32SymbolrateK;
					gATBM781xDvbSxScanParams.u32ValidChannelNumber  = gATBM781xDvbSxScanParams.u32ValidChannelNumber  + 1;
					*pNewChannel = 1;
					gATBM781xDvbSxScanParams.i32ScanCheckedCFOKHz = 0; /*Signal found, the gCheckedCFOKHz will not be used, as bw/2 is used.*/
					/*StrDbgTmp.Format(L"Scan Save channel:Freq: %d Symbol rate: %d u32ValidChannelNumber:%d\n", u32CenterFrequencyKHz, u32SymbolrateK, gATBM781xDvbSxScanParams.u32ValidChannelNumber);*/
					break;
				}
				if((ChannelExist == 1)&&(u8DemodLockedFlag))
				{
					gATBM781xDvbSxScanParams.i32ScanCheckedCFOKHz = 0; /*same Signal found, the gCheckedCFOKHz will not be used.*/
				}
				u8CheckedTimes = u8CheckedTimes + 1;
			} /*end of while*/

		}
		else  /*No any signal checked of CFO and symbol rate*/
		{			
			gATBM781xDvbSxScanParams.u8SingalMayBeChecked = 1;
		}
	} 
	else /*Tuner not locked*/
	{
		gATBM781xDvbSxScanParams.u8SingalMayBeChecked = 0;
	}
	return status;
}
/**/

unsigned char blDvbSxScanStop = 0;
/****************************************************************************
Function:    ATBM781xDvbSxBlindScanProcess
Parameters: none.
Return:      0 is Success,Other is Failed
Description:      
This  function is sample of blind scan process.
****************************************************************************/
ATBM781x_STATUS ATBM781xDvbSxBlindScanProcess( )
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;

	uint32 u32StartFrequency = gATBM781xDvbSxScanParams.u32ScanStartFrequencyKHz;
	uint32 u32StopFrequency = gATBM781xDvbSxScanParams.u32ScanStopFrequencyKHz;

	uint32 u32ScanCenterFrequencyKHz = u32StartFrequency;
	uint32 u32tmpSymbolrate = 0, u32NextSearchingfreq = 0;
	uint32 u32StepAdjustKHz = 0;
	/*uint32 u32TmpBlindScanFineStepKHz = 0;*/
	uint8 u8NewChannelFound = 0;
	uint32 ValidChannlIndex = 0;    
	int32 i32CfoAndSymbolRateK = 0;
	int32 i32BlindScanStepKHz = 0;
	do{
		u32tmpSymbolrate = 0; 
		/*u32NextSearchingfreq = 0;*/
		u32StepAdjustKHz = 0;
		/*u32TmpBlindScanFineStepKHz = 0;*/
		i32BlindScanStepKHz = gATBM781xDvbSxScanParams.u32BlindScanStepKHz;
		status |=ATBM781xDvbSxBlindScanOneChannel(u32ScanCenterFrequencyKHz, &u8NewChannelFound);

		/*Center frequency of next scan channel usually using current Frequency add step Frequency*/
		/*may be changed if valid channel found on current frequency*/
		/*u32ScanCenterFrequencyKHz = u32ScanCenterFrequencyKHz + gATBM781xDvbSxScanParams.u32BlindScanStepKHz;	*/

		if(gATBM781xDvbSxScanParams.u32PreTunedSymbolRateK >980) /*SymbolRate >0*/
		{
			if(gATBM781xDvbSxScanParams.i32ScanCheckedCFOKHz >= i32BlindScanStepKHz) /*IF CFO too large, will not be used. here is 10M*/
			{
				u32ScanCenterFrequencyKHz = u32ScanCenterFrequencyKHz + gATBM781xDvbSxScanParams.u32BlindScanStepKHz;
			}
			/*CFO in range of [2M---10M]*/
			if((gATBM781xDvbSxScanParams.i32ScanCheckedCFOKHz >= 2000)&&(gATBM781xDvbSxScanParams.i32ScanCheckedCFOKHz < i32BlindScanStepKHz))
			{
				u32ScanCenterFrequencyKHz = u32ScanCenterFrequencyKHz + gATBM781xDvbSxScanParams.i32ScanCheckedCFOKHz - 500;
			}
			/*CFO in range of [-2M---2M]*/
			if((gATBM781xDvbSxScanParams.i32ScanCheckedCFOKHz >= -2000)&&(gATBM781xDvbSxScanParams.i32ScanCheckedCFOKHz <2000))
			{
				if(gATBM781xDvbSxScanParams.u32PreTunedRaciPower == 0)
				{
					u32ScanCenterFrequencyKHz = u32ScanCenterFrequencyKHz + gATBM781xDvbSxScanParams.u32BlindScanStepKHz;
				}
				else
				{
					u32ScanCenterFrequencyKHz = u32ScanCenterFrequencyKHz + 2000;
				}
			}
			/*CFO in range of [<(-2M)]*/
			if(gATBM781xDvbSxScanParams.i32ScanCheckedCFOKHz < -2000)
			{
				if(gATBM781xDvbSxScanParams.u32PreTunedRaciPower == 0)
				{
					u32ScanCenterFrequencyKHz = u32ScanCenterFrequencyKHz + gATBM781xDvbSxScanParams.u32BlindScanStepKHz; 
				}
				else
				{
					/*Abs(SymbolRate*0.6 + CFO) + 2M*/
					i32CfoAndSymbolRateK = (gATBM781xDvbSxScanParams.u32PreTunedSymbolRateK*3)/5;
					i32CfoAndSymbolRateK = i32CfoAndSymbolRateK + gATBM781xDvbSxScanParams.i32ScanCheckedCFOKHz;
					u32StepAdjustKHz = i32CfoAndSymbolRateK > 0? i32CfoAndSymbolRateK: -i32CfoAndSymbolRateK;
					u32StepAdjustKHz = u32StepAdjustKHz + 1000; /*safe*/
					/*Step no more than u32BlindScanStepKHz:10MHz*/
					if(u32StepAdjustKHz >gATBM781xDvbSxScanParams.u32BlindScanStepKHz)
					{
						u32StepAdjustKHz = gATBM781xDvbSxScanParams.u32BlindScanStepKHz;
					}
					if(u32StepAdjustKHz < 2000)
					{
						u32StepAdjustKHz = 2000;
					}
					u32ScanCenterFrequencyKHz =u32ScanCenterFrequencyKHz + u32StepAdjustKHz;
				}				
			}
		}
		else  /*Symbol Rate == 0, will check coarse symbol rate */
		{
			if(gATBM781xDvbSxScanParams.u32PreTunedCorseSymbolRateK > 5000) /*Coarse symbol rate > 5M*/
			{
				if(gATBM781xDvbSxScanParams.u32PreTunedRaciPower > 0)
				{					

					/*	u32StepAdjustKHz = (gATBM781xDvbSxScanParams.u32PreTunedCorseSymbolRateK * 3)/5;*/

					/*if(u32StepAdjustKHz >3000)*/
					/*{*/
					u32StepAdjustKHz = 3000; /*Max step 30MHz*/
					/*}*/
					u32ScanCenterFrequencyKHz = u32ScanCenterFrequencyKHz + u32StepAdjustKHz; /*add half of coarse symbol rate range*/
				}
				else
				{
					u32StepAdjustKHz = (gATBM781xDvbSxScanParams.u32PreTunedCorseSymbolRateK * 3)/5;
					if(u32StepAdjustKHz >10000)
					{
						u32StepAdjustKHz = 10000; /*Max step 30MHz*/
					}
					if(u32StepAdjustKHz < 5000)
					{
						u32StepAdjustKHz = 5000; /*Max step 30MHz*/
					}
					u32ScanCenterFrequencyKHz = u32ScanCenterFrequencyKHz + u32StepAdjustKHz; /*add half of coarse symbol rate range*/
				}
			}
			else /*Coarse symbol < 5M*/
			{
				if(gATBM781xDvbSxScanParams.u32PreTunedRaciPower > 0)
				{
					if(gATBM781xDvbSxScanParams.i32ScanCheckedCFOKHz > 4000)
					{
						u32StepAdjustKHz = (uint32)(gATBM781xDvbSxScanParams.i32ScanCheckedCFOKHz/2);
						if(gATBM781xDvbSxScanParams.i32ScanCheckedCFOKHz > 6000)
						{
							u32StepAdjustKHz = 3000;
						}
						u32ScanCenterFrequencyKHz = u32ScanCenterFrequencyKHz + u32StepAdjustKHz;
					}
					else
					{
						u32ScanCenterFrequencyKHz = u32ScanCenterFrequencyKHz + gATBM781xDvbSxScanParams.u32BlindScanFineStepKHz; /*Minimal step*/
					}	
				}else
				{

					if(gATBM781xDvbSxScanParams.i32ScanCheckedCFOKHz > 4000)
					{
						u32StepAdjustKHz = (uint32)(gATBM781xDvbSxScanParams.i32ScanCheckedCFOKHz);
						if(gATBM781xDvbSxScanParams.i32ScanCheckedCFOKHz > 10000)
						{
							u32StepAdjustKHz = 10000;
						}
						u32ScanCenterFrequencyKHz = u32ScanCenterFrequencyKHz + u32StepAdjustKHz;
					}
					else
					{
						u32ScanCenterFrequencyKHz = u32ScanCenterFrequencyKHz + 5000; /*Minimal step*/
					}	
				}

			}			
		}   

		gATBM781xDvbSxScanParams.u32CurrentWorkFreKHz = u32ScanCenterFrequencyKHz;/*used for process */

		/*Process if new channel found */ 
		if((u8NewChannelFound == 1)&&(gATBM781xDvbSxScanParams.u32ValidChannelNumber > 0)&&(gATBM781xDvbSxScanParams.u32ValidChannelNumber <= MAX_SCANED_CHANNEL_NUMBER))
		{
			ValidChannlIndex = gATBM781xDvbSxScanParams.u32ValidChannelNumber - 1;
			/*memory index			*/
			u32tmpSymbolrate    =  gATBM781xDvbSxScanParams.ATBM781xDvbSxChannel[ValidChannlIndex].u32SymbolRateK;
			if(gATBM781xDvbSxScanParams.ATBM781xDvbSxChannel[ValidChannlIndex].stDvbSxSignalPrams.DVBSxType == ATBM781x_DVBS_MODE)
			{
				u32NextSearchingfreq = gATBM781xDvbSxScanParams.ATBM781xDvbSxChannel[ValidChannlIndex].u32FrequencyKHz + (u32tmpSymbolrate*27)/40 + 1000 ;	//roll off *1.35 for DVB-S
			}
			else
			{
				u32NextSearchingfreq = gATBM781xDvbSxScanParams.ATBM781xDvbSxChannel[ValidChannlIndex].u32FrequencyKHz + (u32tmpSymbolrate*3)/5 + 1000;	//roll off *1.2
			}
			/*save some searching time, and cut repetition times.*/
			if(u32NextSearchingfreq > u32ScanCenterFrequencyKHz)
			{
				u32ScanCenterFrequencyKHz = u32NextSearchingfreq; 
			}	
			else
			{
				u32ScanCenterFrequencyKHz = u32NextSearchingfreq + 1000; /*KHz*/
			}
			gATBM781xDvbSxScanParams.u32CurrentWorkFreKHz = u32ScanCenterFrequencyKHz;/*used for process */

		} /*End if found new channel*/

		/*Scan next channel */
		gATBM781xDvbSxScanParams.ui32ScanedValideChannelNum = gATBM781xDvbSxScanParams.u32ValidChannelNumber;

	}while((u32ScanCenterFrequencyKHz < u32StopFrequency)&&(ATBM781xDvbSxBlindScanState)&&!blDvbSxScanStop);


	gATBM781xDvbSxScanParams.u32CurrentWorkFreKHz = gATBM781xDvbSxScanParams.u32ScanStopFrequencyKHz;/*used for process */


	return status;
}


/*Sample of blind scan start*/
void ATBM781xDvbSxBlindScanStart(void)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	status |= ATBM781xDvbSxBlindScanInit();	
	status |= ATBM781xDvbSxBlindScanProcess();	
	if(status != ATBM781x_SUCCESSFUL)
	{
		/*ATBM781xDebugPrint("ATBM781xDvbSxBlindScanStart failed\n"); */
	}
}

/*Sample of blind scan stop*/
void ATBM781xDvbSxBlindScanStop(void)
{
	ATBM781xDvbSxBlindScanState = 0;
}
/*Sample of get blind scan result*/
void ATBM781xDvbSxBlindScanResult(struct ATBM781xDvbSxChannelScanParams *pDvbSxScanParams)
{
	uint32 u32Iter = 0;

	for(u32Iter =0; u32Iter< gATBM781xDvbSxScanParams.u32ValidChannelNumber; u32Iter++)
	{
           pDvbSxScanParams->ATBM781xDvbSxChannel[u32Iter].u32FrequencyKHz = gATBM781xDvbSxScanParams.ATBM781xDvbSxChannel[u32Iter].u32FrequencyKHz;
	    pDvbSxScanParams->ATBM781xDvbSxChannel[u32Iter].u32SymbolRateK  = gATBM781xDvbSxScanParams.ATBM781xDvbSxChannel[u32Iter].u32SymbolRateK;
	}
}

