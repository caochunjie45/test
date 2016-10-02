/***************************************************************************** 
Copyright 2012, AltoBeam Inc. All rights reserved.

File Name: Atbm781xDVBC.c

Version: 1.2.2
Released 2015-03-19

Description: Example programming interface for ATBM781x

Important struct:

gstATBM781xDvbConfig
All hardware dependent parameters should be customized according to system design          

Important functions:

ATBM781xPowerOnInit()
This function is the first function should be called. default DVB-T/T2 8MHz Bandwidth. 
ATBM781xSetDvbtBandwidth(unsigned char u8bandwidthMHz) function can be used to set Bandwidth for DVB-T/T2.

ATBM781xSetDVBCMode()
This function sets the demodulator working in DVB-C mode.
For example, it can be used in mode switching between DVB-T/T2 and DVB-C

ATBM781xDVBCChannelLockCheck(uint8 *LockStatus)
This function checks DVB-C lock status until timeout.
***********************************************************************************************************/
#include "Atbm781x.h"
//#include <stdio.h>

uint32	u32DVBCInternalOSCFreq;
uint8 pszMerlinDVBCInternalReg[] = 
{
	0x00, 0x13, 0x01,
	0x00, 0x14, 0x03,
	0x09, 0x53, 0x36, 
	0x09, 0x55, 0x37, 
	0x09, 0x5f, 0xaa, 
	0x09, 0x60, 0x5a,
	0x09, 0x61, 0xaa, 
	0x09, 0x62, 0x5a,
	0x09, 0x63, 0x16,
	0x0a, 0x72, 0x00,
	0x0a, 0xb9, 0x05, /*DVB-C BW*/
	0x0a, 0xba, 0x3b,
	0x0a, 0xbb, 0x6f,
	0x0a, 0xed, 0x01,
	0x0a, 0xfb, 0x03, 
	/*0x0d, 0xc5, 0x20,*/
	/*0x0f, 0x4a, 0x02,	*/
	0x15, 0x3a, 0x00, 
	0x15, 0x3f, 0x00, 
	0x15, 0x4c, 0x02,
	0x88, 0x10, 0x5e, 
	0x88, 0x11, 0x02,
	0x95, 0x20, 0x00, 
	0x95, 0x21, 0x20, //PER Smaller window
	/*0x0a, 0x83, 0x60  //ACI_CCI th*/
};

uint8 pszMerlin40DVBCInternalReg[] =
{
	0x00, 0x09, 0x00,  
	0x00, 0x0a, 0x00,  
	0x00, 0x19, 0x1, 
	0x09, 0x51, 0x22, 
	0x09, 0x52, 0x40, 
	0x09, 0x56, 0xff,
	0x09, 0x57, 0xfc, 
	0x0a, 0x5a, 0x0
};


uint8 pszMLiteDVBCInternalReg[] =
{
	0x00, 0x09, 0x00,
	0x00, 0x0a, 0x00,
	0x00, 0x13, 0x01,
	0x00, 0x14, 0x03,
	0x00, 0x19, 0x01,
	0x0a, 0x72, 0x00,
	0x0a, 0xb9, 0x05,
	0x0a, 0xed, 0x01,
	0x0a, 0xfb, 0x03,
	0x15, 0x00, 0x01,
	/*0x15, 0x20, 0x00,*/
	/*0x15, 0x21, 0x80,*/
	0x15, 0x3a, 0x00,
	0x15, 0x3f, 0x00,
	0x15, 0x4c, 0x02,
	0x88, 0x10, 0x5e,
	0x88, 0x11, 0x02,
	0x95, 0x20, 0x00,
	0x95, 0x21, 0x80,
	0x0a, 0x83, 0x50, //Add for CCI
	0x09, 0x52, 0x2a,
	0x09, 0x53, 0x2a,
	0x0a, 0x87, 0x40,
	0x0a, 0x5f, 0x38
};

/********************************************************************************
function    : ATBM781xSetDVBCMode
parameter   :stPara.stDvbcPara.ui8InputMode 
return      :Operate  Status:ATBM781x_SUCCESSFUL if correct and other is abnormal.
Description:Call this function to set demodulator working in DVB-C mode
*********************************************************************************/
ATBM781x_STATUS  ATBM781xSetDVBCMode( )
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint8 ui8ChipId = 0, ui8AnaReg = 0;
	gstATBM781xDvbConfig.ui8ConfigDVBType = ATBM781x_DVBC_MODE; /*DVB-T/T2 auto detection.*/
	if(!gATBM781xPowerOnInited)
	{
		/*printf("Should call PowerOnInit before this function\n");*/
		return ATBM781x_SUCCESSFUL;
	}
	ui8Status |= ATBM781xChipID(&ui8ChipId);
	ui8Status |= ATBM781xWrite(0x00 ,0x3  ,0x01);       /*software reset  */

	ui8Status |= ATBM781xSetSysClkClockDVBC();


	/*ui8Status = ATBM781xStandByWakeUp();*/
	/*ui8Status |= ATBM781xWrite(0x01, 0x0c, 0x01); // i2c select PLL clock*/
	ui8Status |= ATBM781xWrite(0x00, 0x04, 0x00); /*set 1 after all registers set done*/
	ui8Status |= ATBM781xWrite(0x01, 0x02, 0x01);	 /*I2C related, more solid	*/
	ui8Status |= ATBM781xWrite(0x10, 0xfb, 0x1); /*10H	PWM , PDM works and better than PWM*/

	ui8Status |= ATBM781xSetFreqAndRateRatio(gstATBM781xDvbConfig); /*may be changed*/
	ui8Status |= ATBM781xSetBandWidthPara(gstATBM781xDvbConfig);
	if((0xa0 == ui8ChipId)||(0xa2 == ui8ChipId))
	{
		ui8Status |= ATBM781xWriteRegArray(pszMerlinDVBCInternalReg,sizeof(pszMerlinDVBCInternalReg)/sizeof(uint8));
	}

	if (ui8ChipId == 0xa2)
	{
		ui8Status |= ATBM781xWriteRegArray(pszMerlin40DVBCInternalReg,sizeof(pszMerlin40DVBCInternalReg)/sizeof(uint8));
	}
	if (ui8ChipId == 0xa4)
	{
		ui8Status |= ATBM781xWriteRegArray(pszMLiteDVBCInternalReg,sizeof(pszMLiteDVBCInternalReg)/sizeof(uint8));
	}
	if (ui8ChipId == 0xa0)
	{
		ui8Status |= ATBM781xWrite(0x0a, 0x83, 0x60);
		ui8Status |= ATBM781xWrite(0x0f, 0x4a, 0x02);
	}
	/*DVB-C using IF mode*/
	ui8Status |= ATBM781xWrite(0x15, 0x11, 0x01); 
	ui8Status |= ATBM781xWrite(0x15, 0x12, 0x00); 
	ui8Status |= ATBM781xWrite(0x00, 0x16, 0x01); 

	/*Set TS interface registers*/
	ui8Status |= ATBM781xSetTSMode(gstATBM781xDvbConfig.stMPEGTSMode);
	ui8Status |= ATBM781xSetGpioPins();
	ui8Status  |= ATBM781xDVBCSet(gstATBM781xDvbConfig);
	if((ui8ChipId == 0xa2)||(ui8ChipId == 0xa4))
	{
		ui8Status |= ATBM781xWrite(0x15, 0x00, 0x01); /* merlin40*/
		ui8Status |= ATBM781xWrite(0x03, 0x03, 0x01);  /*	*/
		/*ATBM781xWrite(0x01,0x06, 0x00);  //en_ack_dly I2C fib ISSUE*/
	}

	ui8Status |= ATBM781xWrite(0x06, 0x41, 4);
	/*ui8Status |= ATBM781xWrite(0x00, 0x22, 1);*/

	if(0xa2 == ui8ChipId)	
	{
		ui8Status |= ATBM781xWrite(0x06,0x19, 0x20);  /**/
		ui8Status |= ATBM781xWrite(0x06,0x21, 0x7b);  /**/
		ui8Status |= ATBM781xWrite(0x06,0x21, 0x73);  /**/
		ui8Status |= ATBM781xSleep(10);
		ui8Status |= ATBM781xRead(0x06, 0x1A, &ui8AnaReg);
		ui8AnaReg = ui8AnaReg + 58; /*0x40 - 6*/
		ui8Status |= ATBM781xWrite(0x06, 0x19, ui8AnaReg);
	}
	ui8Status |= ATBM781xWrite(0x17, 0xd8, 0x0f);	
	ui8Status |= ATBM781xWrite(0x00, 0x09, 0x10);
	ui8Status |= ATBM781xWrite(0x00, 0x2a, 0x01); /*shut down LDPC*/
	ui8Status |= ATBM781xWrite(0x02, 0x0c, u8ATBM781xSoftwareVersion);  /*record SDK software version*/
	ui8Status |= ATBM781xWrite(0x00, 0x04, 1);/*Config Done	   */
	if(0xa2 == ui8ChipId)	
	{
		ui8Status |= ATBM781xSleep(10);
		ui8Status |= ATBM781xWrite(0x06,0x19, 0x20);  /**/
		ui8Status |= ATBM781xWrite(0x06,0x21, 0x7b);  /**/
		ui8Status |= ATBM781xWrite(0x06,0x21, 0x73);  /**/
		ui8Status |= ATBM781xSleep(10);
		ui8Status |= ATBM781xRead(0x06, 0x1A, &ui8AnaReg);
		ui8AnaReg = ui8AnaReg + 58; /*0x40 - 6*/
		ui8Status |= ATBM781xWrite(0x06, 0x19, ui8AnaReg);
	}
	ui8Status |= ATBM781xWrite(0x01, 0x0c, 0x01);	
	ui8Status |= ATBM781xWrite(0x00,0x05,1); /*reset DSP */
	ui8Status |= ATBM781xWrite(0x00,0x05,0); /*reset DSP*/
	return ui8Status;
}

/********************************************************************************
function    : ATBM781xDVBCSet
parameter   :stPara 
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description:only used to dvbc
*********************************************************************************/
ATBM781x_STATUS  ATBM781xDVBCSet( stATBM781xDvbConfig stPara)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint32 ui32OSCFreq = 0;
	ATBM781x64Data  i64Data,i64Data1;
	ui8Status |=ATBM781xDVBCWrite(0x04, 0x1);  /*DVBC Module Reset*/

	ui8Status |=ATBM781xDVBCWrite(0x100, 1); 
	ui8Status |=ATBM781xDVBCWrite(0x104,0x01);
	ui8Status |=ATBM781xDVBCWrite(0x304,0x05);

	/*dSymbolRateM = (stPara.stDvbcPara.iSymbolRate*1.0)/1000; */
	/*ui8Status |= ATBM781xWrite(0x15, 0x37, 0x10); */



	if((int)(stPara.ui32DemodClkKHz - 24000) == 0) /*24MHz crystal*/
	{
		ui32OSCFreq = stPara.ui32DemodClkKHz*2;	
	}
	if((int)(stPara.ui32DemodClkKHz - 30400) == 0) /*30.4MHz crystal*/
	{
		ui32OSCFreq = stPara.ui32DemodClkKHz;	
	}
	u32DVBCInternalOSCFreq= ui32OSCFreq;	
	/*Default DVBC_IF_INPUT*/
	/*if(stPara.stDvbcPara.ui8InputMode == DVBC_IF_INPUT)*/
	{
		/* 		IF_Freq = stPara.ui32TunerIF;	*/
		/*iItbCoarseFrequencyOffset = (int)(134217728*(ui32OSCFreq - IF_Freq)/ui32OSCFreq);*/
		i64Data1.ui32High = 0;
		i64Data1.ui32Low = ui32OSCFreq;
		if (ui32OSCFreq > stPara.DvbCTunerInput.u32IFFrequencyKHz)
		{
			ATBM781x64Mult(&i64Data,134217728,(ui32OSCFreq - stPara.DvbCTunerInput.u32IFFrequencyKHz));
		}
		else
		{
			ATBM781x64Mult(&i64Data,134217728,(stPara.DvbCTunerInput.u32IFFrequencyKHz - ui32OSCFreq));
		}
		ui8Status |=ATBM781xDVBCWrite(0x118,ATBM781x64Div(i64Data,i64Data1));
		/* 		ATBM781xDVBCWrite(0x118,iItbCoarseFrequencyOffset);*/
	}

	ui8Status |= ATBM781xDVBCSetCarrier(ui32OSCFreq,	stPara.DvbCTunerInput.u32SymbolRateK);	
	ui8Status |= ATBM781xDVBCSetSymbolRate(ui32OSCFreq, stPara.DvbCTunerInput.u32SymbolRateK);		 
	ui8Status |= ATBM781xDVBCSetQAM();	 

	ui8Status |=ATBM781xDVBCWrite(0x394,0xc7); 
	ui8Status |=ATBM781xDVBCWrite(0x39c,0x36); /*DVB-C phase noise*/
	ui8Status |=ATBM781xDVBCWrite(0x20c,0x10); 
	ui8Status |=ATBM781xDVBCWrite(0x204,0x400); /*DVB-C symbol rate offset related	*/
	ui8Status |=ATBM781xDVBCWrite(0x04, 0x00);  /*DVBC Module start processing*/
	return ui8Status;

}
/****************************************************************************
Function:	ATBM781xDVBCSetQAM
Parameters:   none                
Return: 	Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description:
Call this function to configure DVBC QAM related registers:			 
******************************************************************************/
ATBM781x_STATUS  ATBM781xDVBCSetQAM(void)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	ui8Status |= ATBM781xDVBCWrite(0x3cc,0x3f8); /*Auto QAM detect*/
	ui8Status |= ATBM781xDVBCWrite(0x364,0xe);
	return ui8Status;
}
/****************************************************************************
Function:	ATBM781xDVBCSetCarrier
Parameters:   ui32OSCFreq: OSC sampling frequency   
ui32SymbolRateK, symbol rate. in Ksps
Return: 	Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal
Description:
Call this function to configure DVBC registers according to OSC frequency  
and symbol rate.
******************************************************************************/
ATBM781x_STATUS    ATBM781xDVBCSetCarrier(uint32 ui32OSCFreq, uint32 ui32SymbolRateK)
{

	ATBM781x_STATUS   ui8Status = ATBM781x_SUCCESSFUL;
	uint32  ui32IfSrchStartFreq = 0;
	uint32	ui32IfSrchStepFreq = 0;
	uint32  ui32IfSearchStartFoOvrFsamp = 0; 
	uint32  ui32IfSearchStepFstpOvrFsamp = 0;  
	uint32	ui32TimSearchLockTimeout = 0;
	uint32	ui32PlCarrierFrequencyRange = 0;
	uint32	ui32CarrierLockAcquireTimeout = 0;
	ATBM781x64Data i64Data,i64Data1;

	/* Calculate parameters for coarse stepped carrier search*/
	/*2^27  =  134217728*/
	/*134217728*3 = 402653184*/

	/*  	uint32 ui32WinEndFreqMHz = 2^27*3*ui32SymbolRateK/20;*/
	/* 	uint32 ui32WinEndFreqMHz;*/

	ATBM781x64Mult(&i64Data,402653184,ui32SymbolRateK);
	ATBM781x64Mult(&i64Data1,20,ui32OSCFreq);
	ui32IfSearchStartFoOvrFsamp =  ATBM781x64Div(i64Data,i64Data1);

	/* 	ui32IfSearchStepFstpOvrFsamp = ui32SymbolRateK/ui32OSCFreq/20;*/
	ATBM781x64Mult(&i64Data,134217728,ui32SymbolRateK);
	ATBM781x64Mult(&i64Data1,ui32OSCFreq,20);

	ui32IfSearchStepFstpOvrFsamp =  ATBM781x64Div(i64Data,i64Data1);


	ui32IfSrchStartFreq = (ui32IfSearchStartFoOvrFsamp); 

	ui8Status |= ATBM781xDVBCWrite(0x3b4,ui32IfSrchStartFreq);

	ui32IfSrchStepFreq = (ui32IfSearchStepFstpOvrFsamp);
	ui8Status |= ATBM781xDVBCWrite(0x3b0,ui32IfSrchStepFreq);

	/* Program parameters for fine stepped carrier search*/
	/* Step size*/
	ui32TimSearchLockTimeout = 20;	/* 20 for 1% step;*/
	ui8Status |=ATBM781xDVBCWrite(0x388,ui32TimSearchLockTimeout);

	/* Search range*/
	ui32PlCarrierFrequencyRange = 39;/* 39 for +/-15% limits*/
	ui8Status |=ATBM781xDVBCWrite(0x38c,ui32PlCarrierFrequencyRange);


	/* Standard times and thresholds for all other QAM levels*/
	ui32CarrierLockAcquireTimeout = 4;   	
	ui8Status |=ATBM781xDVBCWrite(0x348, ui32CarrierLockAcquireTimeout);
	return ui8Status;

}

/****************************************************************************
Function:		ATBM781xDVBCSetSymbolRate
Parameters:   ui32OSCFreq: OSC sampling frequency, 30400, in KHz   
ui32SymbolRateK, symbol rate, in Kbps
Return: 		Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal
Description:
Call this function to configure all DVB-C  symbol rate related registers.			 
******************************************************************************/
ATBM781x_STATUS  ATBM781xDVBCSetSymbolRate(uint32 ui32OSCFreq, uint32 ui32SymbolRateK)
{
	ATBM781x_STATUS   ui8Status = ATBM781x_SUCCESSFUL;
	uint32   ui32Log2DecFactor = 0;		
	uint32	ui32DecCicOutputShift = 0; 
	uint32	ui32AfcSlowControlTC = 0;	

	uint32	ui32TrlSampleRate = 0;
	uint32	ui32TrlSampleRateRecip = 0;
	/* 	uint32		    ui32SmpsPerSymb;		*/
	/* 	uint32		    ui32CicDcGain;*/
	uint32          ui32Tmp = 0;
	ATBM781x64Data      i64Data,i64Data1;

	/* Calculate factor by which decimation filter must reduce rate*/

	/* 	ui32Log2DecFactor = (uint32)((ATBM781xLog10ConvertdB((ui32OSCFreq/ui32SymbolRateK/4))/ATBM781xLog10ConvertdB(2))); //Ðè²âÊÔ¸ÃÌõ*/
	ui32Log2DecFactor = ((ATBM781xLog10ConvertdB(ui32OSCFreq)-ATBM781xLog10ConvertdB(4))-ATBM781xLog10ConvertdB(ui32SymbolRateK))/ATBM781xLog10ConvertdB(2);

	/* Samples per symbol*/
	if(ui32Log2DecFactor > 3)
	{
		ui32Log2DecFactor = 3;
	}

	/* 	ui32SmpsPerSymb = 1000*ui32OSCFreq/ui32SymbolRateK;*/

	/* 	ui32TrlSampleRate = (uint32)((1<<17)/(ui32SmpsPerSymb/(1<<ui32Log2DecFactor)));*/
	/* 	ui32TrlSampleRate = (uint32)((1<<(17+ui32Log2DecFactor)*1000)/(ui32SmpsPerSymb));*/
	ui32Tmp = ATBM781xFixPow(2,ui32Log2DecFactor);
	ATBM781x64Mult(&i64Data,131072,ui32SymbolRateK*ui32Tmp);
	i64Data1.ui32High = 0;
	i64Data1.ui32Low = ui32OSCFreq;

	/*ui32TrlSampleRate = 131072*ATBM781xFixPow(2,ui32Log2DecFactor)*ui32SymbolRateK/ui32OSCFreq;*/
	ui32TrlSampleRate = ATBM781x64Div(i64Data,i64Data1);
	ui8Status = ATBM781xDVBCWrite(0x210, ui32TrlSampleRate);

	/* 	ui32TrlSampleRateRecip = (1<<(10-ui32Log2DecFactor))*ui32SmpsPerSymb/1000;*/
	ATBM781x64Mult(&i64Data,1024,ui32OSCFreq);
	i64Data1.ui32High = 0;
	i64Data1.ui32Low = ui32SymbolRateK*ui32Tmp;

	/* 	ui32TrlSampleRateRecip = 1024*ATBM781xFixPow(2,ui32Log2DecFactor)*ui32OSCFreq/ui32SymbolRateK;*/
	ui32TrlSampleRateRecip = ATBM781x64Div(i64Data,i64Data1);

	ui8Status |= ATBM781xDVBCWrite(0x214, ui32TrlSampleRateRecip);
	/* Calculate CIC gain*/
	/* 	if (ui32Log2DecFactor >0)*/
	/* 	{*/
	/* 		ui32CicDcGain  = ATBM781xFixPow(2*(1<<(ui32Log2DecFactor-1)),3);*/
	/* 		//Calculate CIC gain *=*/
	/* 		ui32CicDcGain *= ATBM781xFixPow(3*(1<<(ui32Log2DecFactor-1)),0);*/
	/* 	}*/
	/* */
	/* 	ui32DecCicOutputShift = (ui32Log2DecFactor > 0) ? ((uint32)((ATBM781xLog10ConvertdB(ui32CicDcGain)/ATBM781xLog10ConvertdB(2)))) : 1;*/
	ui32DecCicOutputShift = 1;

	if(ui32Log2DecFactor ==1)
	{
		ui32DecCicOutputShift = 4;
	}
	if(ui32Log2DecFactor == 2)
	{
		ui32DecCicOutputShift = 7;
	}
	if(ui32Log2DecFactor == 3)
	{
		ui32DecCicOutputShift = 10;
	}


	ui8Status |= ATBM781xDVBCWrite(0x128, ui32DecCicOutputShift);
	ui8Status |= ATBM781xDVBCWrite(0x124,ui32Log2DecFactor);

	/* Program AFC tracking time constant as function of symbol rate*/
	ui32AfcSlowControlTC = (1<<(ui32Log2DecFactor+2));
	ui8Status |= ATBM781xDVBCWrite(0x3bc,ui32AfcSlowControlTC);
	return ui8Status;

}

/****************************************************************************
Function:	ATBM781xDVBCChannelLockCheck
Parameters:   
	LockStatus :LockStatus returned by this pointer
Return: 	Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description:
Call this function to check Channel Lock status			 
******************************************************************************/
ATBM781x_STATUS   ATBM781xDVBCChannelLockCheck(uint8 *LockStatus)
{    
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;  
	uint32 i32Iter;
	uint32 u32DvbcStatus = 0;
	uint8 u8CurrentReceivingMode = 0;
	uint32 u32CheckTimes = 30;
	uint32 u32DvbcTimeOut = 800; /*Time out time unit is milliseconds*/
	uint32 u32DvbcTimingLockTime = 0;
	uint32 u32TimingLockTimeout = 200;
	uint32 u32SymbolTimeOut = 100;
	uint8  u8TimingLockedFlag = 0;
	uint8  u8SymbolDetected = 0, u8BwNumber = 0;
	status = ATBM781xRead(0x00, 0x13, &u8CurrentReceivingMode); /*0:no DVB-C, 1:DVB-C*/
    *LockStatus = 0;

	if((u8CurrentReceivingMode == 1)&&(status == ATBM781x_SUCCESSFUL)) /*DVB-C*/
	{
		u32CheckTimes = u32DvbcTimeOut/20;
		for(i32Iter = 0; i32Iter < u32CheckTimes; i32Iter++)
		{
			status |= ATBM781xRead(0x15, 0x4a, &u8BwNumber); 
			status |= ATBM781xDVBCRead(0x08, &u32DvbcStatus);       /*read DVB-C internal register */
			if(u32DvbcStatus&0x20) /*bit 5*/
			{                           
				*LockStatus = 1;   
				return status;           /*DVB-C FEC locked*/
			}    
			if((u32DvbcStatus&0x04) == 0x04) //bit 2
			{
				u8TimingLockedFlag = 1;
			}
			if(u8BwNumber > 0)
			{
				u8SymbolDetected = 1;
			}
			u32DvbcTimingLockTime = i32Iter*20;
			if(u32DvbcTimingLockTime >= u32SymbolTimeOut)
			{
				if(u8SymbolDetected == 0) //No symbol detected until timeout
				{                           
					*LockStatus = 0;	
					return status;        //DVB-C Symbol unlock, timeout.
				} 		
			}	
			if(u32DvbcTimingLockTime >= u32TimingLockTimeout) //DVB-C Timing lock time less than 150 Milliseconds
			{
				if(u8TimingLockedFlag == 0) //Timing lock failed
				{                           
					*LockStatus = 0;	
					return status;           //DVB-C Timing Sync unlock, timeout.
				} 		
			}    
			status |= ATBM781xSleep(20);
		}
	}
	return status; /*FEC not locked*/
}

/****************************************************************************
Function:	ATBM781xGetDVBCSymbolRate
Parameters:   ui32SymRate :return DVB-C symbol rate,unit is (K symbol/s)              
Return: 	Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal

Description:
Call this function to check and get DVB-C Symbol Rate, valid for DVB-C mode.			 
******************************************************************************/
ATBM781x_STATUS  ATBM781xGetDVBCSymbolRate(uint32 *ui32SymRate)
{
	ATBM781x_STATUS ui8Status  = ATBM781x_SUCCESSFUL;
	uint8 mode = 0;
	uint32 uTrlSampleRate = 0;
	int uTrlSampleRateError = 0;
	uint32 uDecLog2DecFactor = 0;
	int dProgrammedSymbRate = 0;
	int dOffset = 0;
	int symbol_rate = 0; 
	uint32 value1 = 1;
	int i32DVBCInternalOSCFreq = u32DVBCInternalOSCFreq;
	ui8Status = ATBM781xDVBCRead(0x218, &uDecLog2DecFactor);
	uTrlSampleRateError = uDecLog2DecFactor;
	ui8Status |= ATBM781xDVBCRead(0x210, &uTrlSampleRate);
	ui8Status |= ATBM781xDVBCRead(0x124, &uDecLog2DecFactor);
	ui8Status |= ATBM781xRead(0x00, 0x13,&mode);
	if((mode != 0x01)||(uTrlSampleRate == 0)) /*Not in DVB-C mode or I2C Error*/
	{
		return 0;
	}
	dProgrammedSymbRate = (int)((u32DVBCInternalOSCFreq*uTrlSampleRate+ ((1<<(uDecLog2DecFactor+1))*(1<<15)))/((value1<<(uDecLog2DecFactor+1))*(value1<<16)));
	if(dProgrammedSymbRate == 0)
	{
		dProgrammedSymbRate = dProgrammedSymbRate +1; 
	}
	dOffset = ((uTrlSampleRateError/137439)*i32DVBCInternalOSCFreq)/(dProgrammedSymbRate *(1<<uDecLog2DecFactor));	
	symbol_rate = dProgrammedSymbRate;
	if(dOffset>=0)
	{
		symbol_rate = symbol_rate + (symbol_rate*dOffset + 500000) /1000000;
	}
	else
	{
		symbol_rate = symbol_rate + (symbol_rate*dOffset - 500000) /1000000;
	}
	ATBM781xDebugPrint("dProgrammedSymbRate = %d, symbol rate = %d K\n", dProgrammedSymbRate, symbol_rate);
	*ui32SymRate = (uint32)symbol_rate;
	return ui8Status;
}
/****************************************************************************
Function:      ATBM781xGetDVBCCarrierOffset
Parameters:    i32COValue:return Carrier Offset Value by this pointer(KHz)
Return:        Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal

Description:
This function checks DVB-C signal carrier offset on field testing environment.
This function is not meant to be used by customers. It's a debug utility to be used by FAEs.
****************************************************************************/
ATBM781x_STATUS  ATBM781xGetDVBCCarrierOffset(int *i32COValue)
{  
	ATBM781x_STATUS ui8Status  = ATBM781x_SUCCESSFUL;
	int tmp = 0;
	int dtmp = 0;
	uint32 acq_status = 0,ui32Tmp = 0;
	int ItbCarFreqOffset_stat = 0,ItbCoarseFrequencyOffset = 0,ChannelCentreFrequency = 0;
	int DVBC_CarrierOffset = 0;
	/*-------Carrier frequency offset, reg ItbCarFreqOffset_stat---------------*/
	ui8Status = ATBM781xDVBCRead(0x3C0, &ui32Tmp); /*32bit signed*/
	tmp = ui32Tmp;
	/*ATBM781xDebugPrint("DVB-C carrier offset 0x3C0 = %d\n", tmp);*/
	dtmp = tmp;
	if(tmp<0)
		dtmp = -tmp; 
	ItbCarFreqOffset_stat = (dtmp/1000)*(gstATBM781xDvbConfig.DvbCTunerInput.u32SymbolRateK/10)/53687;/*khz*/
	if(tmp<0)
		ItbCarFreqOffset_stat = - ItbCarFreqOffset_stat;
	/*ItbCarFreqOffset_stat = 0;*/
	/*-----ChannelCentreFrequency , reg ItbFrequencyStatus------------------------*/
	ui8Status |= ATBM781xDVBCRead(0x11c, &ui32Tmp); /*27bit signed  */
	tmp = ui32Tmp;
	/*ATBM781xDebugPrint("DVB-C carrier offset 0x11C= %d\n",tmp);    */

	tmp = tmp&0x7ffffff;
	dtmp = tmp;
	if(tmp&(0x1<<26))
	{
		dtmp=-((0x7ffffff - dtmp) + 1);
	}
	if(dtmp<0)
	{
		dtmp = -dtmp;
		ChannelCentreFrequency = (dtmp/100)*(u32DVBCInternalOSCFreq/100)/13422; /*khz*/
		ChannelCentreFrequency = -ChannelCentreFrequency;
	}else
	{
		ChannelCentreFrequency = (dtmp/100)*(u32DVBCInternalOSCFreq/100)/13422; /*khz*/
	}
	/*------ItbCoarseFrequencyOffset--------------------------------------------*/
	ui8Status |= ATBM781xDVBCRead(0x118, &ui32Tmp); /*32bit signed  */
	tmp = ui32Tmp;
	dtmp = tmp;
	if (tmp<0)
	{
		dtmp = -tmp;
	}
	/*ATBM781xDebugPrint("DVB-C carrier offset 0x118  = %d\n", tmp);*/
	ItbCoarseFrequencyOffset = (dtmp/100)*(u32DVBCInternalOSCFreq/100)/13422; /*khz*/
	/*-----------  calculate overall_CFO -------------*/
	if(ChannelCentreFrequency<0)
	{
		DVBC_CarrierOffset =  (ChannelCentreFrequency  + ItbCoarseFrequencyOffset) + ItbCarFreqOffset_stat;
	}
	else
	{
		DVBC_CarrierOffset =  (ChannelCentreFrequency  - ItbCoarseFrequencyOffset) + ItbCarFreqOffset_stat;
	}
	ui8Status |= ATBM781xDVBCRead(0x3d0, &acq_status);
	if(acq_status&(0x20))
	{
		DVBC_CarrierOffset = DVBC_CarrierOffset;
	}
	else
	{
		DVBC_CarrierOffset = -DVBC_CarrierOffset;
	}
	ATBM781xDebugPrint("DVB-C signal Carrier Offset = %d KHz\n", DVBC_CarrierOffset);   

	*i32COValue = DVBC_CarrierOffset;
	return ui8Status;	
}

/****************************************************************************
Function:     ATBM781xGetDVBCQAM
Parameters:   ui8QAMIndex: QAM index will return by this pointer                    
Return:       Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal
Description:
Get DVB-C QAM index			
4: 16QAM
5: 32QAM
6: 64QAM
7: 128QAM
8: 256QAM
******************************************************************************/
ATBM781x_STATUS  ATBM781xGetDVBCQAM(uint8 *ui8QAMIndex)
{
	ATBM781x_STATUS ui8Status  = ATBM781x_SUCCESSFUL;
	uint32   ui32Tmp = 0;

	ui8Status = ATBM781xDVBCRead(0x3d0,&ui32Tmp);
	*ui8QAMIndex = (uint8)((ui32Tmp&0x7)+1); /*pow(2,tmp) means QAM		*/
	/*ATBM781xDebugPrint("DVB-C signal QAM index = %d \n", QAM_index);       */
	return ui8Status;
}
/****************************************************************************
Function:   ATBM781xGetDVBCSignalQuality
Parameters:   ui8DvbcSQI    :SQI value returned by this function                
Return:    DVB-C signal quality

Description:
Calculate  DVB-C signal quality, in range of 0-100			 
******************************************************************************/
ATBM781x_STATUS  ATBM781xGetDVBCSignalQuality(uint8 *ui8DvbcSQI)
{
	ATBM781x_STATUS ui8Status  = ATBM781x_SUCCESSFUL;
	uint32      ui32Tmp = 0, u32PER = 0;
	uint8 u8QamIndex = 0, u8TSLockFlag = 0, u8SQIValue = 0;
	int i32SNRx10dBRef = 291, i32SNRReceived = 0, i32CNrelx10 = 0;
	static int DVBCSNRx10dB[5] ={172,198, 228, 259, 291};

	*ui8DvbcSQI = 0;
	ui8Status = ATBM781xDVBCRead(0x3d0,&ui32Tmp);
	ui8Status |= ATBM781xGetDVBCPER(&u32PER);
	u8QamIndex = (uint8)((ui32Tmp&0x7)+1); //pow(2,tmp) means QAM		
	//ATBMDebugPrint("DVB-C signal QAM index = %d \n", QAM_index);    
	if((u8QamIndex >=4)&&(u8QamIndex <= 8))
	{
		u8QamIndex = u8QamIndex - 4;
		i32SNRx10dBRef = DVBCSNRx10dB[u8QamIndex];
	}
	ui8Status |= ATBM781xGetDVBCSNR(&i32SNRReceived);
	ui8Status |= ATBM781xTSLockFlag(&u8TSLockFlag);

	i32CNrelx10 = i32SNRReceived - i32SNRx10dBRef;   

	if(i32CNrelx10 < -30) //CNrel < -3 dB
	{
		u8SQIValue=  0; 
	}    
	else if(( i32CNrelx10 >= -30) && (i32CNrelx10 < 50)) //-3 dB ¡ÜCNrel < +5 dB
	{
		u8SQIValue = (uint8)(((30 + i32CNrelx10)*12)/10);	
		if((u8TSLockFlag != 1)&&(u8SQIValue >10)) //TS unlock
		{
			u8SQIValue = 10;
		}
	}
	else //CNrel ¡Ý+5 dB
	{
		if((u8TSLockFlag == 1)&&(u32PER ==0))
		{
			u8SQIValue = 100;;
		}		
	}

	if((i32CNrelx10 >=50)&&(u8TSLockFlag == 1)) // > +5dB
	{
		u8SQIValue = 100;
	}

	*ui8DvbcSQI = u8SQIValue;
	return ui8Status;
}

/****************************************************************************
Function:   ATBM781xGetDVBCSNR
Parameters:   i32SNRx10    :SNR * 10 value returned by this function                
Return:    DVBC signal-to-noise ratio in dB format

Description:
Calculate  DVB-C SNR, the SNR is dB unit scalar *10.			 
******************************************************************************/
ATBM781x_STATUS  ATBM781xGetDVBCSNR(int *i32SNRx10)
{
	ATBM781x_STATUS ui8Status  = ATBM781x_SUCCESSFUL;
	int    i32DVBCSNR;
	uint32  ui32Mer = 0, u32DvbcStatus = 0;
	int     i32Tmp1 = 0;
	uint32  ui32Tmp = 0;

	*i32SNRx10 = 0;
	i32DVBCSNR = 0;
	ui8Status = ATBM781xDVBCRead(0x3C4,&ui32Tmp);
	ui32Mer = ui32Tmp;
	ui8Status |= ATBM781xDVBCRead(0x3d0,&ui32Tmp);
	i32Tmp1 = ui32Tmp;
	ui32Mer    = ui32Mer&0xffff;
	i32Tmp1   = (i32Tmp1&0x7)+1; /*pow(2,i32Tmp1) means QAM		*/

	ui8Status |=  ATBM781xDVBCRead(0x08, &u32DvbcStatus);       /*read DVB-C internal register        */
	if(ui32Mer ==0)
	{    	
		ui32Mer = 1;
	}

	if((i32Tmp1==4) ||(i32Tmp1==6) ||(i32Tmp1==8))
	{
		/*if(i32Tmp!=0)*/
		{
			/* 			dbDVBCSNR = -20*log10(i32Tmp/65536.0)-0.8;//QAM=16,64,256*/
			i32DVBCSNR = -2*(ATBM781xLog10ConvertdBx10(ui32Mer)-ATBM781xLog10ConvertdBx10(65536))-10;
		}
	}
	else if( (i32Tmp1==5) ||(i32Tmp1==7))
	{
		/*if(i32Tmp !=0)*/
		{
			/* 			dbDVBCSNR = -20*log10(i32Tmp/65536.0)-4.0;//QAM=32,128*/
			i32DVBCSNR = -2*(ATBM781xLog10ConvertdBx10(ui32Mer)-ATBM781xLog10ConvertdBx10(65536))-40;

		}
	}
	else
	{
		i32DVBCSNR=0;

	}	
    if(u32DvbcStatus&0x24) //Signal lock or Timing Lock get value.
	{
       *i32SNRx10 = i32DVBCSNR;
	}
	else
	{
         *i32SNRx10  = 0;
	}
	/*  ATBM781xDebugPrint("DVBC SNR = %f dB\n",dbDVBCSNR);	*/
	//*i32SNRx10 = i32DVBCSNR * 10;
	return ui8Status;

}

/****************************************************************************
Function:     ATBM781xGetDVBCBER
Parameters:    *ui32BER                  
Return:       Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal
Description:
This function checks RS-BER, Attention RS-BER is not the TS output BER. the error bits
will be corrected by RS module. refer to ATBM781xGetDVBCPER
******************************************************************************/
ATBM781x_STATUS   ATBM781xGetDVBCBER(uint32 *ui32BER)
{
	uint8 ui8Tmp = 0;
	ATBM781x_STATUS ui8Status  = ATBM781x_SUCCESSFUL;
	uint16 ui16Tmp = 0,ui16WinLen = 0;
	uint32 u32DvbcStatus = 0;
	ATBM781x64Data i64Data,i64Data1;
    ui8Status |=  ATBM781xDVBCRead(0x08, &u32DvbcStatus);       /*read DVB-C internal register        */
	ui8Status |=  ATBM781xLatchOn();
	ui8Status |= ATBM781xRead(0x95,0x21,&ui8Tmp);
	ui16WinLen = ui8Tmp<<8;
	ui8Status |= ATBM781xRead(0x95,0x20,&ui8Tmp);
	ui16WinLen = ui16WinLen + ui8Tmp;

	ui8Status |= ATBM781xRead(0x95,0x23,&ui8Tmp);
	ui16Tmp = ui8Tmp<<8;
	ui8Status |= ATBM781xRead(0x95,0x22,&ui8Tmp);
	if(u32DvbcStatus&0x20) //Signal locked.
	{
		ui16Tmp = ui16Tmp + ui8Tmp;
	}
	else
	{
		ui16Tmp  = 0x8000;
	}
	ui8Status |= ATBM781xLatchOff();
	ATBM781x64Mult(&i64Data,ui16Tmp,1000000000);
	ATBM781x64Mult(&i64Data1,ui16WinLen,204*8);		
	*ui32BER = ATBM781x64Div(i64Data,i64Data1);
	return ui8Status;

}

/****************************************************************************
Function:     ATBM781xGetDVBCPER
Parameters:   *ui32PER                  
Return:       Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal
Description:
This function checks post RS-PER, can be used as output TS Packet error rate.
******************************************************************************/
ATBM781x_STATUS   ATBM781xGetDVBCPER(uint32 *ui32PER)
{
	uint8 ui8Tmp = 0;
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint16 ui16Tmp = 0,ui16WinLen = 0;
	ATBM781x64Data i64Data,i64Data1;

	ui8Status = ATBM781xLatchOn();
	ui8Status |= ATBM781xRead(0x95,0x21,&ui8Tmp);
	ui16WinLen = ui8Tmp<<8;
	ui8Status |= ATBM781xRead(0x95,0x20,&ui8Tmp);
	ui16WinLen = ui16WinLen + ui8Tmp;

	ui8Status |= ATBM781xRead(0x95,0x27,&ui8Tmp);
	ui16Tmp = ui8Tmp<<8;
	ui8Status |= ATBM781xRead(0x95,0x26,&ui8Tmp);
	ui16Tmp = ui16Tmp + ui8Tmp;
	ui8Status |= ATBM781xLatchOff();

	ATBM781x64Mult(&i64Data,ui16Tmp,1000000000);
	ATBM781x64Mult(&i64Data1,ui16WinLen,1);

	*ui32PER = ATBM781x64Div(i64Data,i64Data1);
	return ui8Status;


}
