/***************************************************************************** 
Copyright 2012, AltoBeam Inc. All rights reserved.

File Name: Atbm781xDVBSS2.c

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

ATBM781xSetDvbSxAutoMode() 
This function sets the demodulator working in DVB-S mode.

ATBM781xTSLockFlag(uint8 *LockStatus);
This function checks the demodulator TS locking status.   
******************************************************************************/
#include "Atbm781x.h"

ATBM781x_DVBSx_SIGNAL_SPECTRUM gATBM781xDVBSxLastSignalSpetrum = ATBM781x_DVBSx_SIGNAL_SPECTRUM_NORMAL;  //Used to record DVBSx signal's spectrum, normal or inverse. default as normal value
unsigned char gATBM781xDVBSxNormalSpectrumIqValue = 0; //Used to record Spectrum Normal IQ value, according to initialization parameters. should be be changed after set.
unsigned char MLiteSxRegs[] =
{
	0x00, 0x09, 0x00, 
	0x00, 0x16, 0x00, 
	0x08, 0x1c, 0x12, 
	0x08, 0x1d, 0x12, 
	0x09, 0x4e, 0x80, 
	0x09, 0x4f, 0x07, 
	0x09, 0x50, 0xff, 
	0x09, 0x51, 0xe8, 
	0x09, 0x52, 0xd2, 
	0x09, 0x53, 0xd2, 
	0x09, 0x54, 0x05, 
	0x09, 0x55, 0x3e, 
	0x09, 0x57, 0xff, 
	0x09, 0x58, 0x00, 
	0x09, 0x59, 0x40, 
	0x09, 0x5f, 0xff, 
	0x09, 0x60, 0x7f, 
	0x09, 0x61, 0xff, 
	0x09, 0x62, 0x7f, 
	0x09, 0x63, 0x01, 
	0x09, 0x64, 0x01, 
	0x09, 0x65, 0x00, 
	0x09, 0xb0, 0x01, 
	0x09, 0xc3, 0x88, 
	0x09, 0xc4, 0x00, 
	0x09, 0xc5, 0x00, 
	0x09, 0xc6, 0xef, 
	0x09, 0xc7, 0x1e, 
	0x09, 0xc8, 0x00, 
	0x09, 0xc9, 0x00, 
	0x09, 0xf0, 0x20, 
	0x09, 0xf1, 0x20, 
	0x09, 0xf6, 0x04, 
	0x09, 0xf7, 0x01, 
	0x0a, 0x72, 0x00, 
	0x0a, 0xf3, 0x01, 
	0x15, 0x3f, 0x00, 
	0x15, 0x4b, 0x33, 
	0x15, 0x4c, 0x01, 
	0x15, 0x80, 0x00, 
	0x15, 0x81, 0x00, 
	0x15, 0xa4, 0x00, 
	0x15, 0xb1, 0x30, 
	0x15, 0xcb, 0x0a, 
	0x17, 0xd9, 0xbf, 
	0x29, 0x17, 0x01, 
	0x88, 0x10, 0x5e, 
	0x88, 0x11, 0x02, 
	0x10, 0x04, 0xd0, 
	0x18, 0x76, 0xC0,   /*TS valid*/
	0x29, 0xb2, 0x08,
	0x29, 0xb3, 0x08,
	0x29, 0xb4, 0x08,
	0x29, 0xb5, 0x08,
	0x29, 0xb6, 0x08,
	0x29, 0xb7, 0x08,
	0x29, 0xb8, 0x08,
	0x29, 0xb9, 0x08,
	0x29, 0xba, 0x08,
	0x29, 0xbb, 0x08,
	0x29, 0xbc, 0x08,
	0x29, 0xbd, 0x08,
	0x29, 0xbe, 0x08,
	0x29, 0xbf, 0x08,
	0x29, 0xc0, 0x08,
	0x29, 0xc1, 0x08,
	0x29, 0xc2, 0x08,
	0x29, 0xc3, 0x08,
	0x29, 0xc4, 0x08,
	0x29, 0xc5, 0x08,
	0x29, 0xc6, 0x08,
	0x29, 0xc7, 0x08,
	0x29, 0xc8, 0x08,
	0x29, 0xc9, 0x08,
	0x29, 0xca, 0x08,
	0x29, 0xcb, 0x08,
	0x29, 0xcc, 0x08,
	0x29, 0xcd, 0x08,
	0x29, 0xce, 0x08,
	0x29, 0xcf, 0x08,
	0x29, 0xd0, 0x08,
	0x29, 0xd1, 0x08,
	0x29, 0xd2, 0x08,
	0x29, 0xd3, 0x08,
	0x29, 0xd4, 0x08,
	0x29, 0xd5, 0x08,
	0x29, 0xd6, 0x08,
	0x29, 0xd7, 0x08,
	0x29, 0xd8, 0x08,
	0x29, 0xd9, 0x08,
	0x29, 0xda, 0x08,
	0x29, 0xdb, 0x08,
	0x29, 0xdc, 0x08,
	0x29, 0xdd, 0x08,
	0x29, 0xde, 0x08,
	0x29, 0xdf, 0x08,
	0x29, 0xe0, 0x08,
	0x29, 0xe1, 0x08,
	0x29, 0xee, 0x08,
	0x29, 0xef, 0x08,
	0x29, 0xf0, 0x08,
	0x29, 0xf1, 0x08,
	0x29, 0xf2, 0x08,
	0x29, 0xf3, 0x08,
	0x29, 0xf4, 0x08,
	0x29, 0xf5, 0x08,
	0x29, 0xf6, 0x08,
	0x29, 0xf7, 0x08,
	0x29, 0xf8, 0x08,
	0x29, 0xf9, 0x08,
	0x0a, 0xf0, 0x16,
	0x0a, 0xf2, 0x03,
	0x15, 0xb8, 0x0c,
	0x15, 0xb9, 0x1a
};
/********************************************************************************
function    :ATBM781xSetDvbS2Mode
parameter   :ATBM781xSetDvbS2Mode set demodulator works on DVB-S2 mode only
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description:Call this function to set demodulator receives DVB-S2 signal
*********************************************************************************/
ATBM781x_STATUS  ATBM781xSetDvbS2Mode( )
{
	ATBM781x_STATUS  ui8Status  = ATBM781x_SUCCESSFUL;	
	uint8 ui8ChipId = 0, ui8ConfigDone = 0;
	uint8 ui8SxMode = 0;
	ui8Status = ATBM781xRead(0x00,0x00,&ui8ChipId);
	ui8Status = ATBM781xRead(0x00,0x04,&ui8ConfigDone);
	ui8Status = ATBM781xRead(0x00, 0x2b, &ui8SxMode);
	if((ui8ConfigDone == 0x01)&&((ui8SxMode&0x01) == 0x01))
	{
		ui8Status |= ATBM781xWrite(0x15,0xca,0x08); 
	}else
	{
		ui8Status |= ATBM781xSetDvbSxMode( );
		ui8Status |= ATBM781xWrite(0x15,0xca,0x08); 
	}
	ui8Status |= ATBM781xWrite(0x00,0x05,0x01); 
	ui8Status |= ATBM781xWrite(0x00,0x05,0x00); 
	return ui8Status;
}

/********************************************************************************
function    : ATBM781xSetDvbSMode
parameter   :ATBM781xSetDvbSMode set demodulator works on DVB-S mode only
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description
Call this function to set demodulator receives DVB-S signal
*********************************************************************************/
ATBM781x_STATUS  ATBM781xSetDvbSMode( )
{
	ATBM781x_STATUS  ui8Status = ATBM781x_SUCCESSFUL;	
	uint8 ui8ChipId = 0, ui8ConfigDone = 0;
	uint8 ui8SxMode = 0;
	/*ui8Status = ATBM781xWrite(0x00,0x03,0x01); */
	ui8Status = ATBM781xRead(0x00,0x00,&ui8ChipId);
	ui8Status = ATBM781xRead(0x00,0x04,&ui8ConfigDone);
	ui8Status = ATBM781xRead(0x00, 0x2b, &ui8SxMode);
	if((ui8ConfigDone == 0x01)&&((ui8SxMode&0x01) == 0x01))
	{
		ui8Status |= ATBM781xWrite(0x15,0xca,0x00); 
	}else
	{
		ui8Status |= ATBM781xSetDvbSxMode( );
		ui8Status |= ATBM781xWrite(0x15,0xca,0x00); 
	}
	ui8Status |= ATBM781xWrite(0x00,0x05,0x01); 
	ui8Status |= ATBM781xWrite(0x00,0x05,0x00); 
	return ui8Status;
}

/********************************************************************************
function    :ATBM781xSetDvbSxAutoMode
parameter   :ATBM781xSetDvbSMode set demodulator works on DVB-S/S3 auto mode
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description
Call this function to set demodulator receives DVB-S and DVB-S2 signal
*********************************************************************************/
ATBM781x_STATUS  ATBM781xSetDvbSxAutoMode( )
{
	ATBM781x_STATUS  ui8Status = ATBM781x_SUCCESSFUL;	
	uint8 ui8ChipId = 0, ui8ConfigDone = 0;
	uint8 ui8SxMode = 0;
	/*ui8Status = ATBM781xWrite(0x00,0x03,0x01); */
	ui8Status = ATBM781xRead(0x00,0x00,&ui8ChipId);
	ui8Status = ATBM781xRead(0x00,0x04,&ui8ConfigDone);
	ui8Status = ATBM781xRead(0x00, 0x2b, &ui8SxMode);
	if((ui8ConfigDone == 0x01)&&((ui8SxMode&0x01) == 0x01))
	{
		ui8Status |= ATBM781xWrite(0x15,0xca,0x02); 
	}else
	{
		ui8Status |= ATBM781xSetDvbSxMode( );
		ui8Status |= ATBM781xWrite(0x15,0xca,0x02); 
	}
	ui8Status |= ATBM781xSetDvbSxSpectrumAuto(); //Set IQ as Last detected signal spectrum, default normal spectrum.
	ui8Status |= ATBM781xWrite(0x00,0x05,0x01); 
	ui8Status |= ATBM781xWrite(0x00,0x05,0x00); 
	return ui8Status;
}
/********************************************************************************
function    :ATBM781xDvbSxSymbolRateReset
parameter   :N/A
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description
Call this function to reset the detected DVBSx symbol rate
*********************************************************************************/
ATBM781x_STATUS  ATBM781xDvbSxSymbolRateReset( )
{
	ATBM781x_STATUS  ui8Status = ATBM781x_SUCCESSFUL;	

	ui8Status  = ATBM781xWrite(0x09,0xfe,0x01); 
	ui8Status |= ATBM781xWrite(0x09,0xfe,0x00); 
	return ui8Status;
}

/********************************************************************************
function    : ATBM781xSetDvbSxMode
parameter   :ATBM781xSetDvbSMode set demodulator works on DVB-S/S2 auto detection mode 
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description
Call this function to set demodulator receives DVB-S/S2 signal
Demodulator detects and demodulates DVB-S or DVB-S2 signal automatically
*********************************************************************************/
ATBM781x_STATUS  ATBM781xSetDvbSxMode( )
{
	ATBM781x_STATUS  ui8Status  = ATBM781x_SUCCESSFUL;
	/*unsigned int u32Reg20c;*/
	uint8 ui8ChipId = 0, ui8Iter = 0;
	gATBM781xDvbSxInitFlag = 1;
	if(!gATBM781xPowerOnInited)
	{
		/*printf("Should call PowerOnInit before this function\n");*/
		return ATBM781x_SUCCESSFUL;
	}
	ui8Status = ATBM781xRead(0x00,0x00,&ui8ChipId);
	ui8Status |= ATBM781xWrite(0x00 ,0x3  ,0x01);       /*software reset  */


	/*ui8SymbolRateDetect = 0;*/
	gstATBM781xDvbConfig.ui8ConfigDVBType = ATBM781x_DVBS2_MODE;
	gstATBM781xDvbConfig.ui32DVBSSysClkKHz = 120000; /*120MHz*/
	ui8Status |= ATBM781xSetSysClkClockDVBSx();
	/*Set TS interface registers*/
	ui8Status |= ATBM781xSetTSMode(gstATBM781xDvbConfig.stMPEGTSMode);
	ui8Status |= ATBM781xSetGpioPins();
	/*LDPC*/
	for (ui8Iter = 0;ui8Iter<=22;ui8Iter++)
	{
		ui8Status |= ATBM781xWrite(0x16,0x52,0x00); 
		ui8Status |= ATBM781xWrite(0x16,0x50,ui8Iter); 
		ui8Status |= ATBM781xWrite(0x16,0x51,0x16); 
		if((ui8Iter == 4)&&(ui8ChipId != 0xa0))
		{
			ui8Status |= ATBM781xWrite(0x16,0x51,0x0e); 
		}
		if((ui8Iter == 5)&&(ui8ChipId != 0xa0))
		{
			ui8Status |= ATBM781xWrite(0x16,0x51,0x13); 
		}
		if((ui8Iter == 6)&&(ui8ChipId != 0xa0))
		{
			ui8Status |= ATBM781xWrite(0x16,0x51,0x12); 
		}
		if((ui8Iter == 8)&&(ui8ChipId != 0xa0))
		{
			ui8Status |= ATBM781xWrite(0x16,0x51,0x11); 
		}
		if((ui8Iter == 9)&&(ui8ChipId != 0xa0))
		{
			ui8Status |= ATBM781xWrite(0x16,0x51,0x15); 
		}
		ui8Status |= ATBM781xWrite(0x16,0x52,0x01); 
	}
	ui8Status |= ATBM781xWrite(0x16, 0x52, 0x00);
	ui8Status |= ATBM781xWrite(0x16, 0x66, 0x50); /*50->6c->0x50*/
	ui8Status |= ATBM781xWrite(0x16, 0x67, 0x55);
	/*15 b6 10*/
	/*15 b2 8*/
	ui8Status |=ATBM781xWrite(0x15 ,0xb3 ,0x17);    /**/
	ui8Status |=ATBM781xWrite(0x15 ,0xb2 ,0x08);    /**/
	ui8Status |=ATBM781xWrite(0x15 ,0xb6 ,0x10);    /**/
	/*end LDPC*/
	if(gstATBM781xDvbConfig.DvbSxTunerInput.ui8IQSwapMode)
	{
		ui8Status |= ATBM781xSwapIQ(1);
		gATBM781xDVBSxNormalSpectrumIqValue = 1; /*Equal to the first time setting value  for normal spectrum */
	}else
	{
		ui8Status |= ATBM781xSwapIQ(0);
		gATBM781xDVBSxNormalSpectrumIqValue = 0; /*equal to the first time setting value for normal spectrum */
	}
	if(gstATBM781xDvbConfig.DvbSxTunerInput.ui8IQAutoDetection)
	{
		ui8Status |= ATBM781xWrite(0x15,0xe0,0x01); 
	}else
	{
		ui8Status |= ATBM781xWrite(0x15,0xe0,0x00); 
	}
	ui8Status |= ATBM781xWrite(0x18,0x12,0x0); /*BER */
	ui8Status |= ATBM781xWrite(0x18,0x11,0x2); /*BER */
	ui8Status |= ATBM781xWrite(0x17,0xc3,0);
	ui8Status |= ATBM781xWrite(0x17,0xc4,2);    /*PER Window*/
	ui8Status |= ATBM781xWrite(0x10,0xfb,0x01);  /*  PWM	*/
	/*ui8Status |= ATBM781xWrite(0x00,0x21,0x03); //GPIO 1, have been set in ATBM781xSetGpioPins function */
	ui8Status |= ATBM781xWrite(0x10,0x2a,0x00);   
	ui8Status |= ATBM781xWrite(0x00,0x2b,0x01);    /*dvbs_mode_r*/
	ui8Status |= ATBM781xWrite(0x00,0x14,0x04);    /*dvbs_mode_r*/
	if(ui8ChipId == 0xa4)
	{		
		ui8Status |= ATBM781xWriteRegArray(MLiteSxRegs,sizeof(MLiteSxRegs)/sizeof(uint8));			
	}
	/*Set DVB-S/S2 internal registers*/
	ui8Status |= ATBM781xDVBSWrite(0x41C, 0x00000000);
	ui8Status |= ATBM781xDVBSWrite(0x004, 0x00000001);
	ui8Status |= ATBM781xDVBSWrite(0x004, 0x00000008);
	ui8Status |= ATBM781xDVBSWrite(0x100, 0x00000000);
	ui8Status |= ATBM781xDVBSWrite(0x104, 0x00000035);
	ui8Status |= ATBM781xDVBSWrite(0x108, 0x000000B3);
	ui8Status |= ATBM781xDVBSWrite(0x10C, 0x000000FA);
	ui8Status |= ATBM781xDVBSWrite(0x110, 0x00000002);
	ui8Status |= ATBM781xDVBSWrite(0x138, 0x00000001);
	ui8Status |= ATBM781xDVBSWrite(0x13C, 0x00000033);
	ui8Status |= ATBM781xDVBSWrite(0x144, 0x000000C0);
	ui8Status |= ATBM781xDVBSWrite(0x148, 0x00000006);
	ui8Status |= ATBM781xDVBSWrite(0x14C, 0x00000014);
	ui8Status |= ATBM781xDVBSWrite(0x150, 0x00000077);
	ui8Status |= ATBM781xDVBSWrite(0x154, 0x00000008);
	ui8Status |= ATBM781xDVBSWrite(0x158, 0x0000000C);
	ui8Status |= ATBM781xDVBSWrite(0x15C, 0x0000000C);
	ui8Status |= ATBM781xDVBSWrite(0x200, 0x000000FD);
	ui8Status |= ATBM781xDVBSWrite(0x204, 0x00000050);
	ui8Status |= ATBM781xDVBSWrite(0x208, 0x00000080);
	ui8Status |= ATBM781xDVBSWrite(0x20C, 0x0000015E);
	ui8Status |= ATBM781xDVBSWrite(0x210, 0x00008888);
	ui8Status |= ATBM781xDVBSWrite(0x214, 0x00000F01);
	ui8Status |= ATBM781xDVBSWrite(0x40C, 0x00002710);
	ui8Status |= ATBM781xDVBSWrite(0x454, 0x00000072);
	ui8Status |= ATBM781xDVBSWrite(0x524, 0x2000);  /*BER PER counter window*/
	/*Configure done*/
	ui8Status |= ATBM781xWrite(0x03,0x03,0x01); 
	ui8Status |= ATBM781xWrite(0x29,0x12,0x06); 
	ui8Status |= ATBM781xWrite(0x29,0x14,0x06); 
	ui8Status |= ATBM781xWrite(0x15,0xb1,0x2c); /*Lock Time related*/
	ui8Status |= ATBM781xWrite(0x09,0x52,0xb0); /*Symbol Detect thresh 5.20*/
	ui8Status |= ATBM781xWrite(0x09,0x53,0xb0);
	ui8Status |= ATBM781xWrite(0x09,0xb8,0x62);	
	ui8Status |= ATBM781xWrite(0x09,0xb7,0xaa); /*final value*/
	ui8Status |= ATBM781xWrite(0x09,0xb6,0x40);
	ui8Status |= ATBM781xWrite(0x09,0xf7,0x00); /*not remove DC*/
	ui8Status |= ATBM781xWrite(0x09,0xf6,0x00);
	ui8Status |= ATBM781xWrite(0x15,0xcb,0x01); /*a->1, only one coarse symbol*/
	/*All other registers should set before 0x010c!!!!!!!!! */
	ui8Status |= ATBM781xWrite(0x01, 0x0c, 0x01); /*before set 0x0620*/
	ui8Status |= ATBM781xWrite(0x06, 0x20, 0x22);
	ui8Status |= ATBM781xWrite(0x02, 0x0c, u8ATBM781xSoftwareVersion); /*record SDK software version*/
	ui8Status |= ATBM781xDVBSWrite(0x41C, 0x00000000); /*0x41c is the last register set*/
	ui8Status |= ATBM781xWrite(0x00,0x04,0x01); 
	ui8Status |= ATBM781xWrite(0x00,0x05,1); /*reset DSP */
	ui8Status |= ATBM781xWrite(0x00,0x05,0); /*reset DSP  	*/
	gATBM781xDvbSxInitFlag = 0;
	return ui8Status;
}

/********************************************************************************
function    :ATBM781xDVBSxModeDetected
parameter   :pu8DVBSxMode returns the demodulator's received signal mode: ATBM781x_DVBS_MODE or ATBM781x_DVBS2_MODE
return      :Operate Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description
Call this function to detect the working status of demodulator, DVB-S or DVB-S2 mode
can be checked if signal locked.
*********************************************************************************/
ATBM781x_STATUS ATBM781xDVBSxModeDetected(uint8 *pu8DVBSxMode)
{
	uint8 u8SyncS = 0, u8SyncS2 = 0;

	ATBM781x_STATUS status  = ATBM781x_SUCCESSFUL; 
	*pu8DVBSxMode =ATBM781x_DVB_UNKNOWN;
	status =ATBM781xRead(0x26, 0xa9, &u8SyncS); 
	status |=ATBM781xRead(0x26, 0xaa, &u8SyncS2);
	if((u8SyncS ==1)&&(status== ATBM781x_SUCCESSFUL)&&u8SyncS2 ==0)
	{
		*pu8DVBSxMode = ATBM781x_DVBS_MODE;
	}

	if((u8SyncS2 ==1)&&(status== ATBM781x_SUCCESSFUL)&&u8SyncS ==0)
	{
		*pu8DVBSxMode = ATBM781x_DVBS2_MODE;
	}	
	return status;
}
/*Internal function called by ATBM781xDVBSxChannelLockCheck and BlindScan function.*/
ATBM781x_STATUS ATBM781xDVBSxCoarseSymbolRate(uint32 *CoarseSymbolFakeDetectedK)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint8 ui8ArrayTmp[3];
	uint32 u32CoarseSymbolRateK =0;
	ui8Status |=ATBM781xRead(0x9,0x5c,&ui8ArrayTmp[2]);
	ui8Status |=ATBM781xRead(0x9,0x5b,&ui8ArrayTmp[1]);
	ui8Status |=ATBM781xRead(0x9,0x5a,&ui8ArrayTmp[0]);		
	u32CoarseSymbolRateK = (( ui8ArrayTmp[2]&0x1f)<<16);
	u32CoarseSymbolRateK += (ui8ArrayTmp[1]<<8);
	u32CoarseSymbolRateK += ui8ArrayTmp[0]; 
	/*u32CoarseSymbolRateK = ((u32CoarseSymbolRateK*1000)/32768;*/
	u32CoarseSymbolRateK = (u32CoarseSymbolRateK *125)/4096;
	*CoarseSymbolFakeDetectedK = u32CoarseSymbolRateK;
	return ui8Status;
}
/*Internal function called by ATBM781xDVBSxChannelLockCheck and BlindScan function.*/
ATBM781x_STATUS ATBM781xDVBSxBlindScanCheckSymbols(uint8 *SymbolFakeDetected)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint8 ui8Tmp = 0, ui8BWNum = 0, ui8ArrayTmp[3];
	uint32 ui32Reg = 0, ui32Reg2 = 0,u32CoarseSymbolRateM =0;
	uint32 u32Offset0 = 0, u32Offset1= 0;

	*SymbolFakeDetected = 0;
	ui8Status = ATBM781xLatchOn();
	ui8Status |= ATBM781xRead(0x15,0x4a,&ui8BWNum);

	ui8Status |= ATBM781xRead(0x15,0x43,&ui8Tmp);
	ui32Reg = (ui8Tmp<<8);
	ui8Status |= ATBM781xRead(0x15,0x42,&ui8Tmp);
	ui32Reg += ui8Tmp;

	ui32Reg = 1024*ui32Reg/1000;
	ui32Reg = (ui32Reg + 500)/1000; /*M symbol rate*/

	ui8Status |=ATBM781xRead(0x9,0x5c,&ui8ArrayTmp[2]);
	ui8Status |=ATBM781xRead(0x9,0x5b,&ui8ArrayTmp[1]);
	ui8Status |=ATBM781xRead(0x9,0x5a,&ui8ArrayTmp[0]);		
	u32CoarseSymbolRateM = (( ui8ArrayTmp[2]&0x1f)<<16);
	u32CoarseSymbolRateM += (ui8ArrayTmp[1]<<8);
	u32CoarseSymbolRateM += ui8ArrayTmp[0]; 
	u32CoarseSymbolRateM = (u32CoarseSymbolRateM + 16384)/32768;
	if((ui8BWNum&0x07) > 1)
	{
		ui8Status |= ATBM781xRead(0x15,0x45,&ui8Tmp);
		ui32Reg2 = (ui8Tmp<<8);
		ui8Status |= ATBM781xRead(0x15,0x44,&ui8Tmp);
		ui32Reg2 += ui8Tmp;
		ui32Reg2 = 1024*ui32Reg2/1000;
		ui32Reg2 = (ui32Reg2 + 500)/1000; /*M*/
	}

	ui8Status |= ATBM781xLatchOff();	
	if((ui8BWNum&0x07) > 1)
	{	      
		u32Offset0 = u32CoarseSymbolRateM > ui32Reg? (u32CoarseSymbolRateM - ui32Reg):(ui32Reg - u32CoarseSymbolRateM);
		u32Offset1 = u32CoarseSymbolRateM > ui32Reg2? (u32CoarseSymbolRateM - ui32Reg2):(ui32Reg2 - u32CoarseSymbolRateM);
		if((u32Offset0 >3)&&(u32Offset1 <3))
		{
			*SymbolFakeDetected = 1;
		}
	}
	else
	{
		*SymbolFakeDetected = 0;
	}
	return ui8Status; 
}

/********************************************************************************
Function    : ATBM781xDVBSxChannelLockCheck
Parameter : pu8DvbSxLockStatus 
Return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description
Repeat checking demodulator's lock status until timeout. 
this function can be used in DVB-S, DVB-S2 channel locking or channel scanning.
*********************************************************************************/
ATBM781x_STATUS    ATBM781xDVBSxChannelLockCheck(uint8 *pu8DvbSxLockStatus)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint32 u32Iter = 0;
	uint8 u8LockStatus = 0;
	uint32 u32UnsyncTimeOut = 150; /*spectrum auto detection*/
	/*uint32 u32SignalStatus;*/
	uint8 u8unlockedTime = 0;
	uint32 u32Reg = 0,u32CheckedTime= 0, u32TimingSyncTimeout = 0, u32TimingSyncCount = 0;
	/*This value will be changed according to symbol rate detected */
	uint32 u32UnDecodedTimeOut = 0;  
	uint32 u32DelayInterval = 50; 
	uint32 u32TotalCheckTimes = 0;
	uint32 u32SyncCheckTimes = 3;
	uint32 u32SybolrateK = 0;
	uint32 u32SybolrateUpdateK = 0;
	/*uint32 u32Control;*/
	uint8  u8SyncDone = 0;
	/*uint8  u8S2ModeSet = 0;*/
	uint8  u8SxAutoModeSet = 0; 
	uint8  u8S2fec = 0, u8Sfec = 0;
	uint8 u8FakeSignalChecked = 0;

	*pu8DvbSxLockStatus = 0;
	/*if(u32DelayInterval!= 0)*/
/*	{*/
	u32SyncCheckTimes = u32UnsyncTimeOut/u32DelayInterval;
	/*}*/
	for(u32Iter =0; u32Iter<u32SyncCheckTimes; u32Iter++)
	{      
		/* 		status = ATBM781xDVBSxTSLockFlag(&u8LockStatus);*/
		/* 		if((u8LockStatus == 1)&&(status == ATBM781x_SUCCESSFUL))*/
		/* 		{*/
		/* 			*pu8DvbSxLockStatus = 1;*/
		/* 			return status;*/
		/* 		}	*/
		status |= ATBM781xGetDVBSxSymbolRate(&u32SybolrateK);
		if((u32SybolrateK >980)&&(u32SybolrateK < 46000))
		{
			u8SyncDone = 1;
			break;
		}			

		status |= ATBM781xSleep(u32DelayInterval);
	}
	/*Check Symbol Detecting status.*/
	if(u8SyncDone != 1)
	{
		*pu8DvbSxLockStatus = 0;
		return status;
	}
	status |= ATBM781xTSLockFlag(&u8LockStatus);
	if((u8LockStatus == 1)&&(status == ATBM781x_SUCCESSFUL))
	{
		status |= ATBM781xDVBSRead(0x08,&u32Reg);
		status |= ATBM781xRead(0x17,0xec,&u8S2fec);
		if(u32Reg&0x20)
		{
			u8Sfec = 1;
		}
		if((u8Sfec == 1)||(u8S2fec&0x01))
		{
			*pu8DvbSxLockStatus = 1;
			status |= ATBM781xGetDvbSxSpectrumAuto();   /*Add to record Locked signal's spectrum*/
			return status;
		}			
	}    
	/*Set TS lock checking timeout value.*/
	if(u32SybolrateK > 980)
	{
		if(u32SybolrateK > 29000)
		{
			u32UnDecodedTimeOut = 1200;
			status |= ATBM781xRead(0x15, 0xca, &u8SxAutoModeSet);
			if(u8SxAutoModeSet == 0x00)
			{    
				u32UnDecodedTimeOut = 800;
			}
		}else if(u32SybolrateK > 20000)
		{
			u32UnDecodedTimeOut = 1500;		
		}
		else if(u32SybolrateK > 10000)
		{
			u32UnDecodedTimeOut = 1800;
		}
		else if(u32SybolrateK > 5000)
		{
			u32UnDecodedTimeOut = ((10000 - u32SybolrateK)/10 + 900)*2;
		}
		else if(u32SybolrateK > 2000)
		{
			u32UnDecodedTimeOut = ((5000 - u32SybolrateK)/5 + 1600)*2;
		}
		else
		{
			u32UnDecodedTimeOut =  (6000 - u32SybolrateK) + 400;
		}

	}
	/*Check TS lock status until timeout.*/
	status |= ATBM781xDVBSxBlindScanCheckSymbols(&u8FakeSignalChecked);
	if(u8FakeSignalChecked)
	{
		u32UnDecodedTimeOut = u32UnDecodedTimeOut*2; /*some times spectrum inverted*/
	}
	u32TotalCheckTimes = u32UnDecodedTimeOut/u32DelayInterval;
	for(u32Iter = 0; u32Iter<u32TotalCheckTimes; u32Iter++)
	{
		status |= ATBM781xTSLockFlag(&u8LockStatus);
		status |= ATBM781xGetDVBSxSymbolRate(&u32SybolrateUpdateK);
		/*status |= ATBM781xRead(0x17,0xec,&u8LockStatus);*/
		if((u8LockStatus == 1)&&(status == ATBM781x_SUCCESSFUL))
		{
			status |= ATBM781xDVBSRead(0x08,&u32Reg);
			status |= ATBM781xRead(0x17,0xec,&u8S2fec);
			if(u32Reg&0x20)
			{
				u8Sfec = 1;
			}
			if((u8Sfec == 1)||(u8S2fec&0x01))
			{
				*pu8DvbSxLockStatus = 1;
				status |=ATBM781xGetDvbSxSpectrumAuto();  /*Add to record Locked signal's spectrum*/
				return status;
			}			
		}    
		if(u32SybolrateUpdateK == 0) /*May be at reset time or symbol invalid*/
		{
			u8unlockedTime = u8unlockedTime +1;
			if(u8unlockedTime > 4)
			{
				*pu8DvbSxLockStatus = 0;
				return status;
			}		
		}
		if((u32SybolrateUpdateK > 0)&&(u32SybolrateUpdateK < 980))
		{		
			*pu8DvbSxLockStatus = 0;
			return status;					
		}
		status |=ATBM781xDVBSRead(0x08,&u32Reg);
		if(u32Reg&0x04)
		{
			u32CheckedTime = 0;
			u32TimingSyncCount = u32TimingSyncCount + 1;
		}
		else
		{
			u32CheckedTime = u32CheckedTime + u32DelayInterval;			
		}	
		u32TimingSyncTimeout = 600;
		if(u32SybolrateUpdateK <3000)
		{
			u32TimingSyncTimeout = 1000;
		}
		if(u32SybolrateUpdateK >10000) /*Symbol rate K*/
		{
			u32TimingSyncTimeout = 300; 
		}
		if((u32CheckedTime > u32TimingSyncTimeout)&&(!u8FakeSignalChecked)) /*200 or 500 milliseconds TimeSync unlocked after reset.*/
		{
			*pu8DvbSxLockStatus = 0;
			return status;
		}
		if((u32Iter*u32DelayInterval > 1500)&&(u32TimingSyncCount < u32Iter/2)&&(!u8FakeSignalChecked))
		{
			*pu8DvbSxLockStatus = 0;
			return status;
		}
		status |= ATBM781xSleep(u32DelayInterval);	   
	}
	return status;

}

ATBM781x_STATUS    ATBM781xDVBSxChannelLockCheckForAR(uint8 *pu8DvbSxLockStatus)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint32 u32Iter = 0;
	uint8 u8LockStatus = 0;
	uint32 u32SybolrateK = 0;
	uint8  u8SyncDone = 0;
	uint8  u8SxAutoModeSet = 0; 
	*pu8DvbSxLockStatus = 0;
	for(u32Iter =0; u32Iter<2; u32Iter++)
	{      
		status = ATBM781xGetDVBSxSymbolRate(&u32SybolrateK);
		if((u32SybolrateK >1000)&&(u32SybolrateK < 46000))
		{
			u8SyncDone = 1;
			break;
		}	
		status |=ATBM781xSleep(50);
	}
	if(u8SyncDone != 1)
	{
		*pu8DvbSxLockStatus = 0;
		return status;
	}
	if(u32SybolrateK <30000)
	{
		status |=ATBM781xDVBSxChannelLockCheck(&u8LockStatus);
	}
	else
	{
		status |= ATBM781xRead(0x15, 0xca, &u8SxAutoModeSet);
		if(u8SxAutoModeSet == 0x02)
		{     	
			status |=ATBM781xSetDvbSMode( );
			status |=ATBM781xDVBSxChannelLockCheck(&u8LockStatus);
			if(!u8LockStatus)
			{
				status |=ATBM781xSetDvbS2Mode( );
				status |=ATBM781xDVBSxChannelLockCheck(&u8LockStatus);
			}
			if(!u8LockStatus)
			{
				status |=ATBM781xSetDvbSxAutoMode();
			}
		}
		else
		{
			status |=ATBM781xDVBSxChannelLockCheck(&u8LockStatus);
			if(!u8LockStatus)
			{
				status |=ATBM781xSetDvbSxAutoMode();
			}
		}
	}
	status |=ATBM781xGetDvbSxSpectrumAuto(); /*Add to record Locked signal's spectrum*/
	*pu8DvbSxLockStatus = u8LockStatus;
	return status;
}

/********************************************************************************
Function    : ATBM781xDVBSxChannelLockCheckBlindScan
Parameter : pu8DvbSxLockStatus 
Return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description
Repeat checking demodulator's lock status until timeout. 
this function can be used in DVB-S, DVB-S2 channel scanning only.
*********************************************************************************/
ATBM781x_STATUS  ATBM781xDVBSxChannelLockCheckBlindScan(uint8 *pu8DvbSxLockStatus)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint32 u32Iter = 0;
	uint8 u8LockStatus = 0;
	uint32 u32UnsyncTimeOut = 150; /*spectrum auto detection*/
	/*uint32 u32SignalStatus;*/
	uint8 u8unlockedTime = 0;
	uint32 u32Reg = 0,u32CheckedTime= 0, u32TimingSyncTimeout = 0, u32TimingSyncCount = 0;
	/*This value will be changed according to symbol rate detected */
	uint32 u32UnDecodedTimeOut = 0;  
	uint32 u32DelayInterval = 50; 
	uint32 u32TotalCheckTimes = 0;
	uint32 u32SyncCheckTimes = 3;
	uint32 u32SybolrateK = 0;
	uint32 u32SybolrateUpdateK = 0;
	/*uint32 u32Control;*/
	uint8  u8SyncDone = 0;
	/*	uint8  u8S2ModeSet = 0; //First scan S mode then scan S2 mode when working in auto mode.*/
	uint8  u8SxAutoModeSet = 0;
	uint8  u8S2fec = 0, u8Sfec = 0;
	uint8  u8FakeSignalChecked = 0;

	*pu8DvbSxLockStatus = 0;
	/*if(u32DelayInterval!= 0)*/
	/*{*/
	u32SyncCheckTimes = u32UnsyncTimeOut/u32DelayInterval;
	/*}*/
	for(u32Iter =0; u32Iter<u32SyncCheckTimes; u32Iter++)
	{      
		/* 		status = ATBM781xDVBSxTSLockFlag(&u8LockStatus);*/
		/* 		if((u8LockStatus == 1)&&(status == ATBM781x_SUCCESSFUL))*/
		/* 		{*/
		/* 			*pu8DvbSxLockStatus = 1;*/
		/* 			return status;*/
		/* 		}	*/
		status = ATBM781xGetDVBSxSymbolRate(&u32SybolrateK);
		if((u32SybolrateK >980)&&(u32SybolrateK < 46000))
		{
			u8SyncDone = 1;
			break;
		}			

		status |=ATBM781xSleep(u32DelayInterval);
	}
	/*Check Symbol Detecting status.*/
	if(u8SyncDone != 1)
	{
		*pu8DvbSxLockStatus = 0;
		return status;
	}
	status |= ATBM781xTSLockFlag(&u8LockStatus);
	if((u8LockStatus == 1)&&(status == ATBM781x_SUCCESSFUL))
	{
		status |= ATBM781xDVBSRead(0x08,&u32Reg);
		status |= ATBM781xRead(0x17,0xec,&u8S2fec);
		if(u32Reg&0x20)
		{
			u8Sfec = 1;
		}
		if((u8Sfec == 1)||(u8S2fec&0x01))
		{
			status |=ATBM781xGetDvbSxSpectrumAuto(); 
			*pu8DvbSxLockStatus = 1;
			return status;
		}
	}
	/*Set TS lock checking timeout value.*/
	if(u32SybolrateK > 980)
	{
		if(u32SybolrateK > 29000)
		{
			u32UnDecodedTimeOut = 1200;
			status |= ATBM781xRead(0x15, 0xca, &u8SxAutoModeSet);
			if(u8SxAutoModeSet == 0x00)
			{    
				u32UnDecodedTimeOut = 800;
			}
		}else if(u32SybolrateK > 20000)
		{
			u32UnDecodedTimeOut = 1500;		
		}
		else if(u32SybolrateK > 10000)
		{
			u32UnDecodedTimeOut = 2000; //1800->2000
		}
		else if(u32SybolrateK > 5000)
		{
			u32UnDecodedTimeOut = ((10000 - u32SybolrateK)/10 + 900)*2 + 200;
		}
		else if(u32SybolrateK > 2000)
		{
			u32UnDecodedTimeOut = ((5000 - u32SybolrateK)/5 + 1800)*2;
		}
		else
		{
			u32UnDecodedTimeOut =  (6000 - u32SybolrateK) + 400;
		}
	}
	/*Check TS lock status until timeout.*/
	status |= ATBM781xDVBSxBlindScanCheckSymbols(&u8FakeSignalChecked);
	if(u8FakeSignalChecked)
	{
		u32UnDecodedTimeOut = (u32UnDecodedTimeOut*2); /*some times spectrum inverted*/
	}
	u32TotalCheckTimes = u32UnDecodedTimeOut/u32DelayInterval;
	for(u32Iter = 0; u32Iter<u32TotalCheckTimes; u32Iter++)
	{
		status |= ATBM781xTSLockFlag(&u8LockStatus);
		status |= ATBM781xGetDVBSxSymbolRate(&u32SybolrateUpdateK);
		/*status |= ATBM781xRead(0x17,0xec,&u8LockStatus);*/
		if((u8LockStatus == 1)&&(status == ATBM781x_SUCCESSFUL))
		{
			status |= ATBM781xDVBSRead(0x08,&u32Reg);
			status |= ATBM781xRead(0x17,0xec,&u8S2fec);
			if(u32Reg&0x20)
			{
				u8Sfec = 1;
			}
			if((u8Sfec == 1)||(u8S2fec&0x01))
			{
				*pu8DvbSxLockStatus = 1;
				status |=ATBM781xGetDvbSxSpectrumAuto(); /*Add to record Locked signal's spectrum*/
				return status;
			}			
		}    
		if(u32SybolrateUpdateK == 0) /*May be at reset time*/
		{
			u8unlockedTime = u8unlockedTime +1;
			if(u8unlockedTime > 4)
			{
				*pu8DvbSxLockStatus = 0;
				return status;
			}		
		}
		if((u32SybolrateUpdateK > 0)&&(u32SybolrateUpdateK < 980))
		{		
			*pu8DvbSxLockStatus = 0;
			return status;					
		}
		status |=ATBM781xDVBSRead(0x08,&u32Reg);
		if(u32Reg&0x04)
		{
			u32CheckedTime = 0;
			u32TimingSyncCount = u32TimingSyncCount + 1;
		}
		else
		{
			u32CheckedTime = u32CheckedTime + u32DelayInterval;			
		}	
		u32TimingSyncTimeout = 600;
		if(u32SybolrateUpdateK <3000)
		{
			u32TimingSyncTimeout = 1000;
		}
		if(u32SybolrateUpdateK >10000) /*Symbol rate K*/
		{
			u32TimingSyncTimeout = 300; 
		}
		if((u32CheckedTime > u32TimingSyncTimeout)&&(!u8FakeSignalChecked)) /*200 or 500 milliseconds TimeSync unlocked after reset.*/
		{
			*pu8DvbSxLockStatus = 0;
			return status;
		}
		if((u32Iter*u32DelayInterval > 1500)&&(u32TimingSyncCount < u32Iter/2)&&(!u8FakeSignalChecked))
		{
			*pu8DvbSxLockStatus = 0;
			return status;
		}
		status |=ATBM781xSleep(u32DelayInterval);	   
	}
	return status;

}



/********************************************************************************
Function    : ATBM781xDVBSxSNR
Parameter   : pi32SNRx10 
Return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description
Get DVB-Sx SNR,  Attention the returned SNR value is scaled by x10,
for example. when SNR = 11.6dB, the returned value is 116.
*********************************************************************************/
ATBM781x_STATUS ATBM781xDVBSxSNR(int32 *pi32SNRx10)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	/*uint32 u32Control;*/
	uint8  u8Smode = 0, u8S2mode = 0;
	uint8  u8DVBSMode = 0,u8DVBS2Mode = 0;
	*pi32SNRx10 = 0;

	/*status = ATBM781xDVBSRead(0x004,&u32Control);*/
	/*u32Control &= 0x0C;*/
	status |= ATBM781xRead(0x26, 0xa9, &u8Smode);
	status |= ATBM781xRead(0x26, 0xaa, &u8S2mode);
	if(((u8Smode&0x01) == 0x00)&&((u8S2mode&0x01) == 0x01)) /*DVBS2 mode*/
	{
		u8DVBS2Mode = 1;
	}
	if(((u8S2mode&0x01) == 0x00)&&((u8Smode&0x01) == 0x01)) /*DVBS2 mode*/
	{
		u8DVBSMode = 1;
	}
	if((u8DVBS2Mode == 1)&&(status==ATBM781x_SUCCESSFUL)) /*S2*/
	{

		status = ATBM781xGetDVBS2SNR(pi32SNRx10);
	}

	if((u8DVBSMode == 1)&&(status==ATBM781x_SUCCESSFUL))   /*S*/
	{
		status = ATBM781xGetDVBSSNR(pi32SNRx10);
	}
	return status;
}

/********************************************************************************
Function    : ATBM781xDVBSxSignalQuality
Parameter : pu8SQI
Return      : Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description
Get DVB-Sx evaluated signal quality, the returned signal quality is calculated  from SNR. 
signal quality range: 0 -> 100        
*********************************************************************************/
ATBM781x_STATUS    ATBM781xDVBSxSignalQuality(uint8 *pu8SQI)
{

	uint32 u32Reg = 0,u32Rxheader = 0;
	int32  i32SNRx10 = 0, i32SNRx10Plus = 0;
	uint8  u8ModCod = 0,u8CRindex = 0;
	int32  i32SNRRefer = 0;
	uint8  u8QualityValue = 0;
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint8  u8Smode = 0, u8S2mode = 0;
	uint8  u8DVBSMode = 0,u8DVBS2Mode = 0;

	uint8 DvbsSNR[5]      ={33,51,61,71,77};
	uint8 Dvbs2QpskSNR[8] ={11,24,33,41,47,52,63,65};
	uint8 Dvbs28pskSNR[6] ={55,65,81,94,100,110};	

	/*int Eb_NoX10 = 0;*/
	/*int i32DvbsCRmx100 = 176;*/
	/*int DvbsCRmx100[5] = {301,176,124,79,58};*/

	/*int Dvbs2QPSKCRmx100[8] = {301,176,124,79,58, 52,63,65};*/
	/*int Dvbs28PSKCRmx100[6] = {301,176,124,79,58, 50};*/

	/*int imp_margin_X10 = 8;*/
	/*int logmx100 = 301;*/

	status |= ATBM781xRead(0x26, 0xa9, &u8Smode);
	status |= ATBM781xRead(0x26, 0xaa, &u8S2mode);
	if(((u8Smode&0x01) == 0x00)&&((u8S2mode&0x01) == 0x01)) /*DVBS2 mode*/
	{
		u8DVBS2Mode = 1;
	}
	if(((u8S2mode&0x01) == 0x00)&&((u8Smode&0x01) == 0x01)) /*DVBS2 mode*/
	{
		u8DVBSMode = 1;
	}

	/*status = ATBM781xDVBSRead(0x004,&u32Control);*/
	/*u32Control &= 0x0C;*/

	*pu8SQI = 0;
	/*Set TS lock checking timeout value.*/
	if((u8DVBS2Mode == 1)&&(status == ATBM781x_SUCCESSFUL)) /*S2*/
	{
		status |=ATBM781xDVBSRead(0x43c, &u32Rxheader);

		u8ModCod =  (uint8)(u32Rxheader&0x1F);
		/*QPSK 4--11*/
		if((u8ModCod < 12)&&(u8ModCod>= 4))
		{
			u8CRindex = u8ModCod - 4;
			i32SNRRefer = Dvbs2QpskSNR[u8CRindex];
		}		
		/*8PSK 12-17*/
		if((u8ModCod>=12)&&(u8ModCod<18))
		{
			u8CRindex = u8ModCod - 12;
			i32SNRRefer = Dvbs28pskSNR[u8CRindex];
		}
	}

	if(u8DVBSMode == 1) /*DVB-S, QPSK*/
	{
		status |=ATBM781xDVBSRead(0x50c,&u32Reg);
		u32Reg &= 0x07;/*ACQ */
		if(u32Reg <= 4)
		{
			i32SNRRefer = DvbsSNR[u32Reg];
			/*i32DvbsCRmx100 = DvbsCRmx100[u32Reg];*/
		}
		/*logmx100 = 301;*/
	}  
	status = ATBM781xDVBSxSNR(&i32SNRx10);

	if((i32SNRx10 <100)&&(i32SNRx10 >50))
	{
		i32SNRx10 = i32SNRx10 - (100 - i32SNRx10)/7; /*Calibrate CN value.*/
	}



	/*   Eb_NoX10 = u32SNRx10*10 ¨C 10log(188/204) ¨C 10log(m) ¨C 10log(code_rate) + imp_margin_X10*/
	if(u8DVBSMode == 0x00) /*DVB-S, QPSK*/
	{
		/*	Eb_NoX10 = (i32DvbsCRmx100 + u32SNRx10*10 + imp_margin_X10 * 10 - 35 - logmx100)/10;*/
		/*	u32SNRx10 = Eb_NoX10;*/
	}
	/*Evaluation of signal quality*/
	if ((i32SNRx10 >= i32SNRRefer)&&(i32SNRRefer > 0))
	{
		i32SNRx10Plus = i32SNRx10 - i32SNRRefer;

		if(i32SNRx10Plus >= 100) /*10dB*/
			u8QualityValue = 100;
		else if(i32SNRx10Plus >= 50 )  /*5dB*/
			u8QualityValue = (uint8)(50 + (i32SNRx10Plus - 50));
		else if(i32SNRx10Plus >= 20)  /*signal>2dB*/
			u8QualityValue = (uint8)(20 + (i32SNRx10Plus - 20));
		else
			u8QualityValue = (uint8)(i32SNRx10Plus);			
	}
	else
	{
		/*	ui32SNRx10Minus = u32SNRRefer - u32SNRx10;*/
		/*if((ui32SNRx10Minus < 30)&&(u32SNRx10 > 0)) //<-3dB*/
		/*{*/
		/*	u8QualityValue = (30 -  ui32SNRx10Minus)*4/3;*/
		/*	}*/
		/*	else*/
		{
			u8QualityValue = 0;
		}		
	}

	if((u8QualityValue < 25)&&(u8QualityValue >= 2))
	{
		u8QualityValue = u8QualityValue - 2;
	}
	if((u8QualityValue <15 )&&(u8QualityValue > 3))
	{
		u8QualityValue = u8QualityValue - 3;
	}
	*pu8SQI = u8QualityValue;
	return status;
}

/********************************************************************************
Function    : ATBM781xDVBSxBER
Parameter : pui32BER
Return      : Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description
Get DVB-Sx BER
The returned BER value is scaled by *1.0E9.   
*********************************************************************************/
ATBM781x_STATUS  ATBM781xDVBSxBER(uint32 *pui32BER)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	/*uint32 u32Control;*/

	uint8  u8Smode = 0, u8S2mode = 0;
	uint8  u8DVBSMode = 0,u8DVBS2Mode = 0;
	status |= ATBM781xRead(0x26, 0xa9, &u8Smode);
	status |= ATBM781xRead(0x26, 0xaa, &u8S2mode);
	if(((u8Smode&0x01) == 0x00)&&((u8S2mode&0x01) == 0x01)) /*DVBS2 mode*/
	{
		u8DVBS2Mode = 1;
	}
	if(((u8S2mode&0x01) == 0x00)&&((u8Smode&0x01) == 0x01)) /*DVBS2 mode*/
	{
		u8DVBSMode = 1;
	}
	*pui32BER = 1000000000;
	/*status = ATBM781xDVBSRead(0x004,&u32Control);*/
	/*u32Control &= 0x0C;*/
	if((u8DVBS2Mode == 1)&&(status==ATBM781x_SUCCESSFUL)) /*S2*/
	{

		status = ATBM781xGetDVBS2BER(pui32BER);
	}

	if((u8DVBSMode == 1)&&(status==ATBM781x_SUCCESSFUL))   /*S*/
	{
		status =  ATBM781xGetDVBSBER(pui32BER);
	}
	return status;
}


/********************************************************************************
Function    : ATBM781xDVBSxPER
Parameter : pui32PER
Return      : Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description
Get DVB-Sx PER
The returned PER value is scaled by *1.0E9.   it indicates demodulator's output error rate.
*********************************************************************************/
ATBM781x_STATUS  ATBM781xDVBSxPER(uint32 *pui32PER)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	/*uint32 u32Control;*/

	uint8  u8Smode = 0, u8S2mode = 0;
	uint8  u8DVBSMode = 0,u8DVBS2Mode = 0;
	status |= ATBM781xRead(0x26, 0xa9, &u8Smode);
	status |= ATBM781xRead(0x26, 0xaa, &u8S2mode);
	if(((u8Smode&0x01) == 0x00)&&((u8S2mode&0x01) == 0x01)) /*DVBS2 mode*/
	{
		u8DVBS2Mode = 1;
	}
	if(((u8S2mode&0x01) == 0x00)&&((u8Smode&0x01) == 0x01)) /*DVBS2 mode*/
	{
		u8DVBSMode = 1;
	}
	*pui32PER = 1000000000;
	/*status = ATBM781xDVBSRead(0x004,&u32Control);*/
	/*u32Control &= 0x0C;*/
	if((u8DVBS2Mode == 1)&&(status==ATBM781x_SUCCESSFUL)) /*S2*/
	{

		status = ATBM781xGetDVBS2PostBCHPER(pui32PER);
	}

	if((u8DVBSMode == 1)&&(status==ATBM781x_SUCCESSFUL))   /*S*/
	{
		status =  ATBM781xGetDVBSUncorrectablePER(pui32PER);
	}
	return status;
}

/********************************************************************************
Function    : ATBM781xDVBSxSignalParams
Parameter : pstDVBSxParams
Return      : Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description
Get DVB-Sx signal parameters.

*********************************************************************************/
ATBM781x_STATUS  ATBM781xDVBSxSignalParams(ATBM781x_DVBSx_SIGNAL_PARAMS *pstDVBSxParams)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;

	uint32 u32Reg = 0,u32Rxheader = 0;
	uint32 ui32SymbolRateK = 0;
	uint8  u8ModCod = 0;
	uint8 u8ModuTmp = 0;
	uint8  u8Smode = 0, u8S2mode = 0;
	uint8  u8DVBSMode = 0, u8DVBS2Mode = 0, u8TSLockFlag = 0;
	status |= ATBM781xRead(0x26, 0xa9, &u8Smode);
	status |= ATBM781xRead(0x26, 0xaa, &u8S2mode);
	status |= ATBM781xRead(0x18, 0x2a, &u8TSLockFlag);
	if(!u8TSLockFlag)
	{
		status |=ATBM781xSleep(500);
		status |= ATBM781xRead(0x18, 0x2a, &u8TSLockFlag);
	}
	if((u8TSLockFlag == 0x01)&&(u8Smode == 0)&&(u8S2mode == 0)) /*Signal may not stable, wait 500ms read again*/
	{ 
		status |= ATBM781xRead(0x26, 0xa9, &u8Smode);
		status |= ATBM781xRead(0x26, 0xaa, &u8S2mode);
	}
	if(((u8Smode&0x01) == 0x00)&&((u8S2mode&0x01) == 0x01)) /*DVBS2 mode*/
	{
		u8DVBS2Mode = 1;
	}
	if(((u8S2mode&0x01) == 0x00)&&((u8Smode&0x01) == 0x01)) /*DVBS2 mode*/
	{
		u8DVBSMode = 1;
	}
	/*status = ATBM781xDVBSRead(0x004,&u32Control);*/
	/*u32Control &= 0x0C;*/

	/*Set default unknown*/
	pstDVBSxParams->DVBSxType = ATBM781x_DVB_UNKNOWN;
	pstDVBSxParams->DVBSxModu = ATBM781x_DVBSx_MODU_UNKNOWN;
	pstDVBSxParams->DVBSxSymbolRateK = 0;
	pstDVBSxParams->DVBS2CodeRate = ATBM781x_DVBS2_CODE_RATE_UNKNOWN;
	pstDVBSxParams->DVBSCodeRate = ATBM781x_DVBS_CODE_RATE_UNKNOWN;

	if((u8DVBS2Mode == 1)&&(status==ATBM781x_SUCCESSFUL)) /*S2*/
	{
		pstDVBSxParams->DVBSxType = ATBM781x_DVBS2_MODE;
		status |= ATBM781xDVBSRead(0x43c, &u32Rxheader);

		u8ModCod = (uint8)(u32Rxheader&0x1F);
		/*QPSK 4--11*/
		if((u8ModCod < 12)&&(u8ModCod>= 4))
		{			
			pstDVBSxParams->DVBSxModu = ATBM781x_DVBSx_MODU_QPSK;
			pstDVBSxParams->DVBS2CodeRate = (ATBM781x_DVBS2_CODE_RATE_TYPE)(u8ModCod);
			/*u32SNRRefer = Dvbs2QpskSNR[u8CRindex];*/
		}		
		/*8PSK 12-17*/
		if((u8ModCod>=12)&&(u8ModCod<18))
		{		
			pstDVBSxParams->DVBSxModu = ATBM781x_DVBSx_MODU_8PSK;
			switch(u8ModCod)
			{
			case 12:
				pstDVBSxParams->DVBS2CodeRate =  ATBM781x_DVBS2_CODE_RATE_3_5;
				break;
			case 13:
				pstDVBSxParams->DVBS2CodeRate =  ATBM781x_DVBS2_CODE_RATE_2_3;
				break;
			case 14:
				pstDVBSxParams->DVBS2CodeRate =  ATBM781x_DVBS2_CODE_RATE_3_4;
				break;
			case 15:
				pstDVBSxParams->DVBS2CodeRate =  ATBM781x_DVBS2_CODE_RATE_5_6;
				break;
			case 16:
				pstDVBSxParams->DVBS2CodeRate =  ATBM781x_DVBS2_CODE_RATE_8_9;
				break;
			case 17:
				pstDVBSxParams->DVBS2CodeRate =  ATBM781x_DVBS2_CODE_RATE_9_10;
				break;
			default:
				break;
			}
		}

		status |= ATBM781xGetDVBSxSymbolRate(&ui32SymbolRateK);
		if(status == ATBM781x_SUCCESSFUL)
		{
			pstDVBSxParams->DVBSxSymbolRateK = ui32SymbolRateK;
		}
	}


	if((u8DVBSMode == 1)&&(status==ATBM781x_SUCCESSFUL))   /*S*/
	{
		pstDVBSxParams->DVBSxType = ATBM781x_DVBS_MODE	;
		status |= ATBM781xDVBSRead(0x50c,&u32Reg);
		u8ModuTmp = (uint8)(u32Reg&0x08);
		if(u8ModuTmp == 0x00)
		{
			pstDVBSxParams->DVBSxModu = ATBM781x_DVBSx_MODU_QPSK;
		}
		u32Reg &= 0x07;/*ACQ */
		if(u32Reg <= 4)
		{
			pstDVBSxParams->DVBSCodeRate = (ATBM781x_DVBS_CODE_RATE_TYPE)(u32Reg);
		}

		status |= ATBM781xGetDVBSxSymbolRate(&ui32SymbolRateK);
		if(status==ATBM781x_SUCCESSFUL)
		{
			pstDVBSxParams->DVBSxSymbolRateK = ui32SymbolRateK;
		}

	}
	return status;
}

/****************************************************************************
Function:   ATBM781xGetDVBSxSymbolRate
Parameters:   pui32SymbolRateK:            
Return:   Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.

Description
Get DVBSx carrier offset  value.  Notice the unit is KSymbol per second.  

******************************************************************************/
ATBM781x_STATUS   ATBM781xGetDVBSxSymbolRate(uint32 *pui32SymbolRateK )
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint8 ui8Tmp = 0, ui8BWNum = 0, ui8ChipId = 0;
	uint32 ui32Reg = 0;

	ui8Status = ATBM781xLatchOn();

	ui8Status |= ATBM781xRead(0x15,0x4a,&ui8BWNum);
	ui8Status |= ATBM781xRead(0x00,0x00,&ui8ChipId);

	ui8Status |= ATBM781xRead(0x15,0x43,&ui8Tmp);
	ui32Reg = (ui8Tmp<<8);
	ui8Status |= ATBM781xRead(0x15,0x42,&ui8Tmp);
	ui32Reg += ui8Tmp;

	ui8Status |= ATBM781xLatchOff();
	if((ui8BWNum&0x07) > 0)
	{
		if(ui8ChipId == 0xa4)
		{
			*pui32SymbolRateK = 16*123*ui32Reg/1875;//result *1000 *123/120
		}
		else
		{
			*pui32SymbolRateK = 1024*ui32Reg/1000;//result *1000
		}
	}
	else
	{
		*pui32SymbolRateK = 0;
	}
	return ui8Status;
}
/****************************************************************************
Function:   ATBM781xGetDVBSxCarrierOffset
Parameters:   pi32CarrierOffsetKHz: DVBSx coarse Carrier Offset     
Return:   Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.

Description
Gt DVBS, DVBS2 Carrier Offset value,  Notice the unit is KHz.
The CFO is coarse CFO,  the coarse CFO can be detected earlier than LockedCFO.
If signal locked, ATBM781xGetDVBSxLockedCFO provides more accurate CFO.
******************************************************************************/
ATBM781x_STATUS    ATBM781xGetDVBSxCarrierOffset(int32 *pi32CarrierOffsetKHz)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	char i8Signal = 1;
	uint8 u8IQSet = 0;
	char i8IQInverse = -1;
	uint8 ui8Tmp = 0;
	short i16Tmp = 0;
	uint32 ui32Reg = 0;
	uint8  u8iqState = 0;
	ATBM781x64Data i64Data,i64Data1;

	ui8Status = ATBM781xLatchOn();
	ui8Status |= ATBM781xRead(0x15,0xe1,&u8iqState);
	ui8Status |= ATBM781xRead(0x03,0x02,&u8IQSet);
	ui8Status |= ATBM781xRead(0x09,0xc0,&ui8Tmp);/*bit[21:16]*/
	ui32Reg = ((ui8Tmp&0x3f)<<16);

	ui8Status |= ATBM781xRead(0x09,0xbf,&ui8Tmp);
	ui32Reg += (ui8Tmp<<8);

	ui8Status |= ATBM781xRead(0x09,0xbe,&ui8Tmp);
	ui32Reg += ui8Tmp;


	ui8Status |= ATBM781xRead(0x09,0xc2,&ui8Tmp);
	i16Tmp = ((ui8Tmp)<<8);

	ui8Status |= ATBM781xRead(0x09,0xc1,&ui8Tmp);
	i16Tmp = i16Tmp +ui8Tmp;

	ui8Status |= ATBM781xLatchOff();
	if(i16Tmp<0)
	{
		i8Signal = -1;
		i16Tmp = i16Tmp*i8Signal;
	}

	ATBM781x64Mult(&i64Data,ui32Reg, i16Tmp*1000);
	i64Data1.ui32High = 0;
	i64Data1.ui32Low = ATBM781xFixPow(2,30);
	if(u8iqState&0x01)
	{
		if(gATBM781xDVBSxNormalSpectrumIqValue == u8IQSet)
		{		
			i8Signal = i8Signal*i8IQInverse;
		}
	}
	else
	{
		if(gATBM781xDVBSxNormalSpectrumIqValue != u8IQSet)
		{		
			i8Signal = i8Signal*i8IQInverse;
		}
	}
	*pi32CarrierOffsetKHz =  ATBM781x64Div(i64Data,i64Data1)*i8Signal;

	return ui8Status;
}

/****************************************************************************
Function:   ATBM781xGetDVBSxLockedCFO
Parameters:   i32CFO
Return:   Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.

Description:Get DVB-Sx CFO value in KHz when demodulator signal is locked.
This is final CarrierOffset based on Coarse CFO and CFO bias.
Attention: the signal should be locked to get accurate CFO
******************************************************************************/
#if 1
ATBM781x_STATUS   ATBM781xGetDVBSxLockedCFO(int *i32LockedCFO)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL ;
	char  i8Signal = 1;
	uint8  u8iqState = 0, u8IQSet = 0;
	char i8IQInverse = -1;
	uint32 ui32Reg = 0,ui32Tmp = 0;
	int i32Value = 0, i32CFOBias = 0;
	ATBM781x64Data  i64Data,i64Data1;
	int32 i32CoarseCFO = 0;
	uint8 u8LockFlag = 0;

	ui8Status = ATBM781xDVBSRead(0x11c,&ui32Reg);/*27 s-bit*/
	ui32Reg = ui32Reg&0x7ffffff;

	i32Value= (int)ui32Reg;
	if (ui32Reg&0x4000000)
	{
		i8Signal = -1;
		i32Value =i32Value - 134217728;
		i32Value = i32Value*i8Signal;/*first convert to positive data*/
	}
	ui32Tmp = ATBM781xFixPow(2,27);
	ATBM781x64Mult(&i64Data,gstATBM781xDvbConfig.ui32DVBSSysClkKHz*1000,i32Value);
	ATBM781x64Mult(&i64Data1,ui32Tmp,1);
	ui8Status |= ATBM781xRead(0x15,0xe1,&u8iqState);
	ui8Status |= ATBM781xRead(0x03,0x02,&u8IQSet);
	if(u8iqState&0x01)
	{
		if(gATBM781xDVBSxNormalSpectrumIqValue == u8IQSet)
		{		
			i8Signal = i8Signal*i8IQInverse;
		}
	}
	else
	{
		if(gATBM781xDVBSxNormalSpectrumIqValue != u8IQSet)
		{		
			i8Signal = i8Signal*i8IQInverse;
		}
	}
	i32CFOBias = ATBM781x64Div(i64Data,i64Data1)*i8Signal;
	i32CFOBias= i32CFOBias/1000; /*Changed to KHz*/
	ui8Status |= ATBM781xGetDVBSxCarrierOffset(&i32CoarseCFO);
	ui8Status |=ATBM781xTSLockFlag(&u8LockFlag);
	if(u8LockFlag)
	{
		*i32LockedCFO = i32CoarseCFO - i32CFOBias;
	}else
	{
		*i32LockedCFO = i32CoarseCFO;
	}
	return ui8Status;
}
#endif
/********************************************************************************
Function    : ATBM781xGetDVBSSNR
Parameter : i32SNRx10
Return      : Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description
This is an internal function used by ATBM781xDVBSxSNR.

Get DVB-S SNR,  Attention the returned SNR value is scaled by x10,
for example. when SNR = 11.6dB, the returned value is 116.
*********************************************************************************/
ATBM781x_STATUS   ATBM781xGetDVBSSNR(int32 *i32SNRx10)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint32 ui32Reg = 0,ui32Tmp = 0;
	ui8Status = ATBM781xDVBSRead(0x498,&ui32Reg);/*16 u-bit*/
	ui32Reg = ui32Reg&0xffff;

	ui32Tmp = ATBM781xFixPow(2,16);

	/*for ui32Reg/ui32Tmp = 0;so we use  ui32Reg*1000/ui32Tmp
	result ,but the ATBM781xLog10ConvertdB calculate result is 10*log10(para),
	the last result mast sub 30  */
	if(ui32Reg != 0)
	{
		*i32SNRx10 = -2*(ATBM781xLog10ConvertdBx10(ui32Reg*1000/ui32Tmp)-300) -20;
	}
	else
	{
		*i32SNRx10 = 0;
	}
	return ui8Status;
}

/****************************************************************************
Function:   ATBM781xGetDVBSBER
Parameters:   ui32BER: RS corrected Bit error rate real number*1e9.                    
Return:   Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.

Description:
Calculate DVB-S RS bit error rate. Measure window is set by 0x524,  e.g 0x8000 MPEG packets. 
Here we read number of bit errors in the last measure window.
The following is BER count method:                     
1. Set 0x538 to 1(Ber active ==1)                     
2. ui32CorrectedBerBits  counter increments as error bits are detected until SampleSize uint8s are received.                          
3. 0x538 is automatically set to 0 by demodulator as SampleSize uint8s are received  (BerActive == 0).                     
4. Read ui32CorrectedBerBits from 0x530, which records error bits counted.

The returned BER value is scaled by *1.0E9. 
******************************************************************************/
ATBM781x_STATUS   ATBM781xGetDVBSBER(uint32 *ui32BER)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	/*      uint32 ui32Tmp;*/
	uint32 ui32WindoSize = 8192;
	uint32 ui32TotalBits = 0; /*SampleSize: 8192*204*8*/
	/*	int    i32EPara = 0;*/
	uint32 ui32Iter = 0;
	uint32 ui32BerStatus = 0;
	uint32 ui32CorrectedBerBits = 0;   
	ATBM781x64Data i64Data,i64Data1;


	ui8Status = ATBM781xDVBSRead(0x524,&ui32WindoSize);/*16bit*/
	ui32TotalBits = ui32WindoSize*204*8;

	while(ui32Iter < 10) /*TimeOut*/
	{
		ui8Status |= ATBM781xDVBSRead(0x538,&ui32BerStatus);/*2bit*/
		if((ui32BerStatus&0x01)!= 0)
		{
			/*ATBM781xDebugPrint("Ber counting not end, wait for count done\\n");        */
			ui8Status |=ATBM781xSleep(200);
		}else
		{
			break;
		}
		ui32Iter = ui32Iter + 1;
	}
	ui8Status |= ATBM781xDVBSRead(0x530,&ui32CorrectedBerBits);/*16bit*/
	ui32CorrectedBerBits = ui32CorrectedBerBits&0xffff;

	if(ui32CorrectedBerBits ==0)
	{
		*ui32BER = 0;
		/*          *i32pBerExponent = 0;           */
		/*ATBM781xDebugPrint("PreRsBer = %f E- %d\n",ui32BerCoeffient, *ui32pBerExponent);                */
		ui8Status |= ATBM781xDVBSWrite(0x538,0x1);  /*reset Ber calculate */
		return ui8Status;
	} 
	ATBM781x64Mult(&i64Data,ui32CorrectedBerBits,1000000000);
	i64Data1.ui32High = 0;
	i64Data1.ui32Low  = ui32TotalBits;

	/* 	 ui32CorrectedBerBits * 1e9/ui32TotalBits*/

	*ui32BER = ATBM781x64Div(i64Data,i64Data1);

	/* */
	/*      for (ui32Iter=0;ui32Iter<12;ui32Iter++)*/
	/*      {*/
	/*          ui32Tmp = ui32CorrectedBerBits/ui32TotalBits;*/
	/*          if (ui32Tmp)*/
	/*          {*/
	/*               ui32BerCoeffient = (ui32CorrectedBerBits*1000)/ui32TotalBits;*/
	/* 			  i32EPara -= 3;*/
	/*               break;*/
	/*          }*/
	/*          else*/
	/*          {*/
	/*               i32EPara -= 1;*/
	/*               ui32CorrectedBerBits *=10;*/
	/*          }*/
	/*      }    */
	/*      *i32pBerExponent  = i32EPara;*/
	/*      ATBM781xDebugPrint("PreRsBer = %f E- %d\n",ui32BerCoeffient, *ui32pBerExponent);  */

	ui8Status |= ATBM781xDVBSWrite(0x538,0x1); /*reset Ber calculate */
	return ui8Status;

}
/****************************************************************************
Function:   ATBM781xGetDVBSUncorrectablePER
Parameters:   ui32PER: RS corrected Bit error rate real number*1e9.                    
Return:   Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.

Description:
This function counts DVB-S data packet which contains uncorrected error and calculate packet error rates.
Measure window is set by  0x524, e.g 0x8000 MPEG packets. 
Here we read the counted number of uncorrected packets in the last measure window.
The following is count method:                     
1. Set 0x538 to 1(Ber active ==1)                     
2. ui32UnCorrectedPackets counter increments as uncorrected packets are detected until SampleSize uint8s are received.                          
3. 0x538 is automatically set to 0 by demodulator as SampleSize uint8s are received  (BerActive == 0).                     
4. Read Uncorrected Packets from 0x534, which records uncorrected packets counted.

The returned BER value is scaled by *1.0E9. 
******************************************************************************/
ATBM781x_STATUS    ATBM781xGetDVBSUncorrectablePER(uint32 *ui32PER )
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	/*      uint32 ui32Tmp;*/
	uint32 ui32TotalPkts = 32768; /*SampleSize: 0x8000 packets*/
	/*	uint32 ui32EPara = 0;*/
	/*      uint32 ui32Iter;*/
	uint32 ui32BerStatus = 0;
	uint32 ui32UnCorrectedPackets = 0, u32Reg = 0; 
	ATBM781x64Data i64Data,i64Data1;
  
	ui8Status |= ATBM781xDVBSRead(0x08,&u32Reg);

	ui8Status |= ATBM781xDVBSRead(0x524,&ui32TotalPkts);
	ui8Status |= ATBM781xDVBSRead(0x538,&ui32BerStatus);
	if(ui32BerStatus!= 0)
	{
		/*          ATBM781xDebugPrint("counting not end, wait for count done\n");        */
		ui8Status |=ATBM781xSleep(500);
	}

	ui8Status |= ATBM781xDVBSRead(0x534,&ui32UnCorrectedPackets);
	//ui32UnCorrectedPackets = ui32UnCorrectedPackets&0xffff;   
	if(u32Reg&0x20) //FEC lock
	{
		ui32UnCorrectedPackets = ui32UnCorrectedPackets&0xffff;
	}
	else
	{
		ui32UnCorrectedPackets = ui32TotalPkts;
	}
	if(ui32UnCorrectedPackets ==0)
	{
		*ui32PER = 0;
		/*          *i32pPktsExponent = 0;          */
		/*          ATBM781xDebugPrint("Pks Error = %f E- %d\n",ui32PktsCoeffient, *i32pPktsExponent);                 */
		ui8Status |= ATBM781xDVBSWrite(0x538,0x1);  /*reset calculator    */
		return ui8Status;
	}   
	ATBM781x64Mult(&i64Data,ui32UnCorrectedPackets,1000000000);
	i64Data1.ui32High = 0;
	i64Data1.ui32Low  = ui32TotalPkts;

	/* 	 ui32UnCorrectedPackets * 1e9/ui32TotalPkts*/

	*ui32PER = ATBM781x64Div(i64Data,i64Data1);


	/*      for (ui32Iter=0;ui32Iter<12;ui32Iter++)*/
	/*      {*/
	/*          ui32Tmp = ui32UnCorrectedPackets/ui32TotalPkts;*/
	/*          if (ui32Tmp)*/
	/*          {*/
	/*               ui32PktsCoeffient = (ui32UnCorrectedPackets*1000)/ui32TotalPkts;*/
	/* 			  ui32EPara -=3;*/
	/*               break;*/
	/*          }*/
	/*          else*/
	/*          {*/
	/*               ui32EPara -=1;*/
	/*               ui32UnCorrectedPackets *=10;*/
	/*          }*/
	/*      }    */
	/*      *i32pPktsExponent  = ui32EPara;*/
	/*      ATBM781xDebugPrint("Pks Error = %f E- %d\n",ui32PktsCoeffient, *i32pPktsExponent);   */

	ui8Status |= ATBM781xDVBSWrite(0x538,0x1); /*reset calculator*/
	return ui8Status;
}

/********************************************************************************
Function    : ATBM781xGetDVBS2SNR
Parameter : i32SNRx10
Return      : Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description
This is an internal function used by ATBM781xDVBSxSNR.

Get DVB-S2 SNR,  Attention the returned SNR value is scaled by x10,
for example. when SNR = 11.6dB, the returned value is 116.
*********************************************************************************/
ATBM781x_STATUS   ATBM781xGetDVBS2SNR(int32 *i32SNRx10)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint32 ui32Reg = 0,ui32Tmp = 0;
	ATBM781x64Data i64Data,i64Data1;

	ui8Status = ATBM781xDVBSRead(0x434,&ui32Reg);/*32 u-bit*/
	ui8Status = ATBM781xDVBSRead(0x430,&ui32Tmp);/*9 u-bit*/

	ui32Tmp = ui32Tmp&0x1ff;
	if ((ui32Tmp == 0)||(ui32Reg == 0))
	{
		*i32SNRx10 = 0;
		return ui8Status;
	}
	/*   ui32Tmp = ATBM781xFixPow(2,16);*/
	/*for ui32Reg/ui32Tmp = 0;so we use  ui32Reg*1000/ui32Tmp
	result ,but the ATBM781xLog10ConvertdB calculate result is 10*log10(para),
	the last result mast sub 30  */
	ATBM781x64Mult(&i64Data,ui32Reg,1000);
	i64Data1.ui32High = 0;
	i64Data1.ui32Low = ui32Tmp*512;

	*i32SNRx10 = ATBM781xLog10ConvertdBx10(ATBM781x64Div(i64Data,i64Data1)) - 300;
	return ui8Status;
}

/****************************************************************************
Function:   ATBM781xGetDVBS2BER
Parameters:   ui32S2BER: Error rate real number*1e9.                    
Return:   Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.

Description:
Get DVB-S2 BER value,
The returned BER value is scaled by *1.0E9.   This BER is not the TS output error
rate, some bit error may be corrected by BCH FEC decoder. refer to ATBM781xGetDVBS2PostBCHPER
******************************************************************************/
ATBM781x_STATUS  ATBM781xGetDVBS2BER(uint32 *ui32S2BER)
{
	uint8 ui8RXHeader = 0, ui8CodeRate = 0, ui8ModeCode = 0,ui8Status = ATBM781x_SUCCESSFUL;
	uint8 ui8PWindow[3], ui8PBERCount[3];
	static  uint16  ui16PBCHLenTable[23] = {      
		16200, 21600, 25920, 32400, 38880, 43200, 48600, 51840, 
		54000, 57600, 58320, 38800, 43200, 48600, 54000, 57600,
		58320, 43200, 48600, 51840, 54000, 57600, 58320
	};
	uint16 ui16NBCHLen = 1;
	uint32 ui32BerCalculated = 0;
	uint32 ui32BlockWindow = 0, ui32BERCount = 0;
	ATBM781x64Data i64Data,i64Data1;


	ui8Status = ATBM781xDVBSRead(0x43c,ui32S2BER);
	ui8RXHeader = (uint8)(*ui32S2BER);

	ui8ModeCode = ui8RXHeader&0x1F;
	if(ui8ModeCode < 24)
	{
		ui8CodeRate = ui8ModeCode - 1;
	}
	if(ui8CodeRate <23)
	{
		ui16NBCHLen = ui16PBCHLenTable[ui8CodeRate];
	}

	ui8Status |= ATBM781xLatchOn();
	ui8Status |= ATBM781xRead(0x18, 0x12,&ui8PWindow[2]);
	ui8Status |= ATBM781xRead(0x18, 0x11,&ui8PWindow[1]);
	ui8Status |= ATBM781xRead(0x18, 0x10,&ui8PWindow[0]);

	ui8Status |= ATBM781xRead(0x18, 0x1B,&ui8PBERCount[2]);
	ui8Status |= ATBM781xRead(0x18, 0x1A,&ui8PBERCount[1]);
	ui8Status |= ATBM781xRead(0x18, 0x19,&ui8PBERCount[0]);
	ui8Status |= ATBM781xLatchOff();

	if (ui8Status)
	{
		*ui32S2BER = 0;
		return ui8Status;
	}

	ui32BlockWindow = (ui8PWindow[2]&0x0f)*256*256 + ui8PWindow[1]*256 + ui8PWindow[0];
	ui32BERCount = ui8PBERCount[2]*256*256 + ui8PBERCount[1]*256 + ui8PBERCount[0];

	if(ui32BlockWindow!= 0)
	{
		ATBM781x64Mult(&i64Data,ui32BERCount,1000000000);
		ATBM781x64Mult(&i64Data1,ui32BlockWindow,ui16NBCHLen);


		/* 		ui32BerCalculated = ui32BERCount*1000000000/(ui32BlockWindow*ui16NBCHLen);*/
		ui32BerCalculated = ATBM781x64Div(i64Data,i64Data1);

	}
	*ui32S2BER = ui32BerCalculated;
	return ui8Status;

}

/****************************************************************************
Function:   ATBM781xGetDVBS2PostBCHPER
Parameters: ui32S2PER: RS corrected Bit error rate real number*1e9.                    
Return:   Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.

Description:
Get DVB-S2 BER value,
The returned BER value is scaled by *1.0E9.  the error rate is output block error
rate. can be used to evaluate TS error rate.
******************************************************************************/
ATBM781x_STATUS  ATBM781xGetDVBS2PostBCHPER(uint32 *ui32S2PER)
{
	uint8 ui8Tmp = 0;
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint16 ui16Tmp = 0,ui16WinLen = 0;
	ATBM781x64Data i64Data,i64Data1;

	ui8Status = ATBM781xLatchOn();
	ui8Status |= ATBM781xRead(0x17,0xc4,&ui8Tmp);
	ui16WinLen = ui8Tmp<<8;
	ui8Status |= ATBM781xRead(0x17,0xc3,&ui8Tmp);
	ui16WinLen =ui16WinLen + ui8Tmp;

	ui8Status |= ATBM781xRead(0x17,0xd4,&ui8Tmp);
	ui16Tmp = ui8Tmp<<8;
	ui8Status |= ATBM781xRead(0x17,0xd3,&ui8Tmp);
	ui16Tmp = ui16Tmp + ui8Tmp;
	if (ui16Tmp > ui16WinLen) //Not locked.
	{
		ui16Tmp = ui16WinLen; 
	}
	ui8Status |= ATBM781xLatchOff();
	ATBM781x64Mult(&i64Data,ui16Tmp,1000000000);
	ATBM781x64Mult(&i64Data1,ui16WinLen,1);
	*ui32S2PER = ATBM781x64Div(i64Data,i64Data1);
	return ui8Status;
}

//Internal function called by BlindScan function.
ATBM781x_STATUS  ATBM781xGetDVBS2RightCCIPower(uint32 *ui32SxRcciPwr)
{	
	uint32 u32Power = 0;
	uint8  ui8Power[3] ={0, 0, 0};
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;

	ui8Status = ATBM781xLatchOn();
	/*ui8Status |= ATBM781xRead(0x8a,0xa8,&ui8Power[2]);	*/
	/*ui8Status |= ATBM781xRead(0x8a,0xa7,&ui8Power[1]);*/
	ui8Status |= ATBM781xRead(0x8a,0xa6,&ui8Power[0]);
	ui8Status |= ATBM781xLatchOff();

	/*u32Power = (ui8Power[2]&0x07)*256*256 + ui8Power[1]*256 + ui8Power[0];*/
	u32Power = ui8Power[0];
	*ui32SxRcciPwr = u32Power;
	return ui8Status;
}

/****************************************************************************
Function:    ATBM781xGetDvbSxSpectrumAuto()
Parameters:  none
Return:      status 
Description: This function is used to check the last locked  DVBSx signal spectrum for demodulator
this function is used for blind scan or be used if signal's spectrum unkown.
****************************************************************************/
ATBM781x_STATUS ATBM781xGetDvbSxSpectrumAuto(void)  
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint8 u8ChipVersion = 0, ui8IQSwaped = 0, ui8IQValue = 0, u8LockStatus = 0, ui8SxMode = 0;
	status  = ATBM781xRead(0x00, 0x2b, &ui8SxMode);
	status |= ATBM781xRead(0x00, 0x00,&u8ChipVersion);
	status |= ATBM781xTSLockFlag(&u8LockStatus);
	if(((u8ChipVersion&0xff) == 0xa4)&&(u8LockStatus)&&((ui8SxMode&0x01) == 0x01))
	{
		status |= ATBM781xRead(0x03, 0x02,&ui8IQValue);
		status |= ATBM781xRead(0x15, 0xe1,&ui8IQSwaped);
		if(gATBM781xDVBSxNormalSpectrumIqValue == ui8IQValue) //Same as Init setting for Normal spectrum
		{
			if(ui8IQSwaped&0x01)
			{
				gATBM781xDVBSxLastSignalSpetrum = ATBM781x_DVBSx_SIGNAL_SPECTRUM_INVERSE;
			}else
			{
				gATBM781xDVBSxLastSignalSpetrum = ATBM781x_DVBSx_SIGNAL_SPECTRUM_NORMAL;
			}
		}
		else  //IQ set for Inverse spectrum
		{ 
			if(ui8IQSwaped&0x01)
			{
				gATBM781xDVBSxLastSignalSpetrum = ATBM781x_DVBSx_SIGNAL_SPECTRUM_NORMAL;
			}else
			{
				gATBM781xDVBSxLastSignalSpetrum = ATBM781x_DVBSx_SIGNAL_SPECTRUM_INVERSE;
			}
		}
		//*SpectrumInverse = gATBM781xDVBSxLastSignalSpetrumInverse;
	}	
	return status;
}

/****************************************************************************
Function:    ATBM781xSetDvbSxSpectrumAuto()
Parameters:  none
Return:      status 
Description: This function is used to set DVBSx IQ state for demodulator.            
here just set IQ state as the last locked signal indicates when working in playing 
TV or channel switching mode.		

Using Sample:

ATBM781xHoldDSP();
ATBM781xTunerI2CViaDemodOn();            //Tuner I2C via demodulator switch on
SetTunerFrequencyDvbSx( eg.1650MHZ)      //Tune DVBSx frequency
ATBM781xTunerI2CViaDemodOff();           //Tuner I2C via demodulator switch off			
ATBM781xSetDvbSxSpectrumManual();        //Set correct IQ state. ATBM781xSetDvbSxSpectrumAuto() can be used if no current spectrum known.
ATBM781xStartDSP();
****************************************************************************/
ATBM781x_STATUS ATBM781xSetDvbSxSpectrumAuto(void)  
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint8 u8ChipVersion = 0, ui8IQValue = 0, ui8SxMode =0;
    status  = ATBM781xRead(0x00, 0x2b, &ui8SxMode);
	status |= ATBM781xRead(0x00, 0x00,&u8ChipVersion);

	if(((u8ChipVersion&0xff) == 0xa4)&&((ui8SxMode&0x01) == 0x01))
	{
		status |= ATBM781xRead(0x03, 0x02,&ui8IQValue);
		if(gATBM781xDVBSxLastSignalSpetrum == ATBM781x_DVBSx_SIGNAL_SPECTRUM_INVERSE) //Signal Spectrum inverse
		{
			if(gATBM781xDVBSxNormalSpectrumIqValue == ui8IQValue) //Same as Init setting for Normal spectrum
			{			
				if((ui8IQValue&0x01) == 0x01) //Invert IQ
				{
					status |= ATBM781xWrite(0x03,0x02,0x00);
				}
				else
				{
					status |= ATBM781xWrite(0x03,0x02,0x01); //In fact set as previous value
				}
			}
			else
			{
				//  printf("Signal spectrum inverse and IQ set for inverse signal, do nothing\n");
			}
		}
		else  //Signal Spectrum Normal
		{
			if(gATBM781xDVBSxNormalSpectrumIqValue != ui8IQValue) //not for normal spectrum
			{			
				if((ui8IQValue&0x01) == 0x01)
				{
					status |= ATBM781xWrite(0x03,0x02,0x00);
				}
				else
				{
					status |= ATBM781xWrite(0x03,0x02,0x01);
				}
			}
			else
			{
				//printf("Normal spectrum and IQ set for Normal signal do nothing\n");
			}
		}
	}
	return status;
}

ATBM781x_STATUS ATBM781xGetDvbSxSpectrumManual(ATBM781x_DVBSx_SIGNAL_SPECTRUM  *pSignalSpectrum)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint8 u8ChipVersion = 0, ui8IQSwaped = 0, ui8IQValue = 0, u8LockStatus = 0, ui8SxMode = 0;
	status  = ATBM781xRead(0x00, 0x2b, &ui8SxMode);
	status |= ATBM781xRead(0x00, 0x00,&u8ChipVersion);
	status |= ATBM781xTSLockFlag(&u8LockStatus);
	*pSignalSpectrum = ATBM781x_DVBSx_SIGNAL_SPECTRUM_UNKNOWN;
	if(((u8ChipVersion&0xff) == 0xa4)&&(u8LockStatus)&&((ui8SxMode&0x01) == 0x01))
	{
		status |= ATBM781xRead(0x03, 0x02,&ui8IQValue);
		status |= ATBM781xRead(0x15, 0xe1,&ui8IQSwaped);
		
		if(gATBM781xDVBSxNormalSpectrumIqValue == ui8IQValue) //Same as Init setting for Normal spectrum
		{
			if(ui8IQSwaped&0x01)
			{
				gATBM781xDVBSxLastSignalSpetrum = ATBM781x_DVBSx_SIGNAL_SPECTRUM_INVERSE;
			}else
			{
				gATBM781xDVBSxLastSignalSpetrum = ATBM781x_DVBSx_SIGNAL_SPECTRUM_NORMAL;
			}
			*pSignalSpectrum = gATBM781xDVBSxLastSignalSpetrum;
		}
		else  //IQ set for Inverse spectrum
		{ 
			if(ui8IQSwaped&0x01)
			{
				gATBM781xDVBSxLastSignalSpetrum = ATBM781x_DVBSx_SIGNAL_SPECTRUM_NORMAL;
			}else
			{
				gATBM781xDVBSxLastSignalSpetrum = ATBM781x_DVBSx_SIGNAL_SPECTRUM_INVERSE;
			}
		}
		*pSignalSpectrum = gATBM781xDVBSxLastSignalSpetrum;		
	}		
	return status;
}

ATBM781x_STATUS ATBM781xSetDvbSxSpectrumManual(ATBM781x_DVBSx_SIGNAL_SPECTRUM  SignalSpectrum)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint8 u8ChipVersion = 0, ui8IQValue = 0, ui8SxMode =0;

	status  = ATBM781xRead(0x00, 0x2b, &ui8SxMode);
	status |= ATBM781xRead(0x00, 0x00,&u8ChipVersion);
	if(((u8ChipVersion&0xff) == 0xa4)&&((ui8SxMode&0x01) == 0x01))
	{
		status |= ATBM781xRead(0x03, 0x02,&ui8IQValue);

		if(SignalSpectrum == ATBM781x_DVBSx_SIGNAL_SPECTRUM_UNKNOWN)
		{
			SignalSpectrum = gATBM781xDVBSxLastSignalSpetrum;  //If spectrum unknown, just set as the last locked signal's spectrum
		}

		if(SignalSpectrum == ATBM781x_DVBSx_SIGNAL_SPECTRUM_INVERSE) //Signal Spectrum inverse
		{
			if(gATBM781xDVBSxNormalSpectrumIqValue == ui8IQValue) //Same as Init setting for Normal spectrum
			{			
				if((ui8IQValue&0x01) == 0x01) //Invert IQ
				{
					status |= ATBM781xWrite(0x03,0x02,0x00);
				}
				else
				{
					status |= ATBM781xWrite(0x03,0x02,0x01); //In fact set as previous value
				}
			}
			else
			{
				//printf("Signal spectrum inverse and IQ set for inverse signal, do nothing\n");
			}
		}
		if(SignalSpectrum == ATBM781x_DVBSx_SIGNAL_SPECTRUM_NORMAL) 	//Signal Spectrum Normal	
		{
			if(gATBM781xDVBSxNormalSpectrumIqValue != ui8IQValue) //not for normal spectrum
			{			
				if((ui8IQValue&0x01) == 0x01)
				{
					status |= ATBM781xWrite(0x03,0x02,0x00);
				}
				else
				{
					status |= ATBM781xWrite(0x03,0x02,0x01);
				}
			}
			else
			{
				//printf("Normal spectrum and IQ set for Normal signal do nothing\n");
			}
		}
	}
	return status;
}
ATBM781x_STATUS   ATBM781xCalibrateDVBSxSymbolRate(void)
{
	ATBM781x_STATUS ui8Status  = ATBM781x_SUCCESSFUL; 
	uint8  ui8Mode = ATBM781x_DVB_UNKNOWN;
	uint32 u32SybolrateK = 0, u32Rxheader;
	uint8  u8ChipVersion =0, u8ModCod = 0;
	uint8  ui8CrIndex = 0xff;

	ui8Status |= ATBM781xDVBSxModeDetected(&ui8Mode);
	if(ui8Mode != ATBM781x_DVBS2_MODE) 
	{
		return ui8Status;
	}	
	ui8Status |= ATBM781xRead(0x00, 0x00,&u8ChipVersion);
	if((u8ChipVersion&0xff) != 0xa4)
	{
		return ui8Status;
	}
	ui8Status |= ATBM781xGetDVBSxSymbolRate(&u32SybolrateK);
	ui8Status |= ATBM781xDVBSRead(0x43c, &u32Rxheader);
	u8ModCod =  (uint8)(u32Rxheader&0x1F);	
	switch(u8ModCod)
	{
	case 1:
		ui8CrIndex = 0;
		break;
	case 2:
		ui8CrIndex = 1;
		break;		
	case 3:
		ui8CrIndex = 2;
		break;
	case 4:
		ui8CrIndex = 3;
		break;
	case 5:
		ui8CrIndex = 4;
		break;
	case 6:
		ui8CrIndex = 5;
		break;
	case 7:
		ui8CrIndex = 6;
		break;
	case 8:
		ui8CrIndex = 7;
		break;
	case 9:
		ui8CrIndex = 8;
		break;
	case 10:
		ui8CrIndex = 9;
		break;
	case 11:
		ui8CrIndex = 10;
		break;
	case 12:
		ui8CrIndex = 4;
		break;
	case 13:
		ui8CrIndex = 5;
		break;
	case 14:
		ui8CrIndex = 6;
		break;
	case 15:
		ui8CrIndex = 8;
		break;
	case 16:
		ui8CrIndex = 9;
		break;
	case 17:
		ui8CrIndex = 10;
		break;
	default:
		break;
	}
	if(ui8CrIndex == 0xff) //CrIndex is not valid
	{
		return ui8Status;
	}

	if(u32SybolrateK > 5000) //Symbol rate >5M
	{	
		if(u8ModCod <12)// QPSK should iter more times
		{
			ui8Status |= ATBM781xWrite(0x16, 0x52, 0x00); 
			ui8Status |= ATBM781xWrite(0x16, 0x50, ui8CrIndex); 				
			ui8Status |= ATBM781xWrite(0x16, 0x51, 0x18);
			ui8Status |= ATBM781xWrite(0x16, 0x52, 0x01);
			ui8Status |= ATBM781xWrite(0x16, 0x52, 0x00);
			ui8Status |= ATBM781xWrite(0x16, 0x66, 0x78);	
		}
		else if(ui8CrIndex == 4) //For 8PSK 3_5
		{
			if(u32SybolrateK > 36000)
			{	
				ui8Status |=ATBM781xWrite(0x16, 0x52, 0x00); 
				ui8Status |=ATBM781xWrite(0x16, 0x50, 0x04);
				ui8Status |=ATBM781xWrite(0x16, 0x51, 0x10);
				ui8Status |=ATBM781xWrite(0x16, 0x52, 0x01);
				ui8Status |=ATBM781xWrite(0x16, 0x52, 0x00);
				ui8Status |=ATBM781xWrite(0x16, 0x66, 0x6d); 				
			}
			else if(u32SybolrateK > 30000)
			{
				ui8Status |=ATBM781xWrite(0x16, 0x52, 0x00); 
				ui8Status |=ATBM781xWrite(0x16, 0x50, 0x04);
				ui8Status |=ATBM781xWrite(0x16, 0x51, 0x14);
				ui8Status |=ATBM781xWrite(0x16, 0x52, 0x01);
				ui8Status |=ATBM781xWrite(0x16, 0x52, 0x00);
				ui8Status |=ATBM781xWrite(0x16, 0x66, 0x89); 				
			}
			else if(u32SybolrateK > 5000)
			{		
				ui8Status |=ATBM781xWrite(0x16, 0x52, 0x00); 
				ui8Status |=ATBM781xWrite(0x16, 0x50, 0x04);
				ui8Status |=ATBM781xWrite(0x16, 0x51, 0x18);
				ui8Status |=ATBM781xWrite(0x16, 0x52, 0x01);
				ui8Status |=ATBM781xWrite(0x16, 0x52, 0x00);
				ui8Status |=ATBM781xWrite(0x16, 0x66, 0xa0); 				
			}			
			else//Invalid symbol rate using default setting
			{	
				ui8Status |=ATBM781xWrite(0x16, 0x52, 0x00); 			
				ui8Status |=ATBM781xWrite(0x16, 0x50, 0x04);
				ui8Status |=ATBM781xWrite(0x16, 0x51, 0x3c);
				ui8Status |=ATBM781xWrite(0x16, 0x52, 0x01);
				ui8Status |=ATBM781xWrite(0x16, 0x52, 0x00);
				ui8Status |=ATBM781xWrite(0x16, 0x66, 0xff); 				
			}	
		} //Not 8psk 3_5 and not QPSK, using default iter times
		else
		{								
			ui8Status |= ATBM781xWrite(0x16,0x52,0x00); 
			ui8Status |= ATBM781xWrite(0x16,0x50,ui8CrIndex); 			
			if(ui8CrIndex == 5)
			{
				ui8Status |= ATBM781xWrite(0x16,0x51,0x13); 
			}
			else if(ui8CrIndex == 6)
			{
				ui8Status |= ATBM781xWrite(0x16,0x51,0x12); 
			}
			else if(ui8CrIndex == 8)
			{
				ui8Status |= ATBM781xWrite(0x16,0x51,0x11); 
			}
			else if(ui8CrIndex == 9)
			{
				ui8Status |= ATBM781xWrite(0x16,0x51,0x15); 
			}
			else
			{
				ui8Status |= ATBM781xWrite(0x16,0x51,0x16); 
			}
			ui8Status |= ATBM781xWrite(0x16, 0x52, 0x01);
			ui8Status |= ATBM781xWrite(0x16, 0x52, 0x00);				
			ui8Status |= ATBM781xWrite(0x16, 0x66, 0x50);	
		}
	}
	else //Symbol rate < 5M, all using large iter number
	{				
		ui8Status |= ATBM781xWrite(0x16, 0x52, 0x00); 
		ui8Status |= ATBM781xWrite(0x16, 0x50, ui8CrIndex); 				
		ui8Status |= ATBM781xWrite(0x16, 0x51, 0x32);
		ui8Status |= ATBM781xWrite(0x16, 0x52, 0x01);
		ui8Status |= ATBM781xWrite(0x16, 0x52, 0x00);
		ui8Status |= ATBM781xWrite(0x16, 0x66, 0xff);		
	}	
	return ui8Status;
}
