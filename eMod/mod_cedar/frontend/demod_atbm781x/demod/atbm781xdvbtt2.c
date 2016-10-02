/***************************************************************************** 
Copyright 2012, AltoBeam Inc. All rights reserved.

File Name: Atbm781xDVBTT2.c

Version: 1.2.2
Released 2015-03-19

Description: Example programming interface for ATBM781x

Important struct:

gstATBM781xDvbConfig
All hardware dependent parameters should be customized according to system design          

Important functions:

ATBM781xPowerOnInit()
This function is the first function should be called. default DVB-T/T2 8MHz Bandwidth. 

ATBM781xSetDVBTxMode( uint8  ui8DvbTxBandwidthType)
This function sets the demodulator working in DVB-T/T2 auto mode.
The demodulator can receive DVB-T and DVB-T2 without discrimination on register setting.
For example, it can be also used in mode switching between DVB-T/T2 and DVB-C

ATBM781xDVBTxChannelLockCheck(uint8 *pu8LockStatus)
This function checks demodulator lock status until timeout.

ATBM781xTSLockFlag(uint8 *LockStatus);
This function checks the demodulator TS locking status.   
******************************************************************************/
//#include<stdio.h>
//#include "Atbm781x.h"
#include "drv_atbm_i.h"
static uint8 u8sDVBTxBandwidth = 8;

static uint8 atbmdvbtxCommon[] =
{	
	0x0d, 0x12, 0x40,
	0x0d, 0x13, 0xff,
	0x0d, 0x14, 0x40,
	0x0d, 0x15, 0xff,
	/*0x68, 0x42, 0x00,*/
	/*0x0e, 0x9f, 0x01,  //MLite Default 1*/
	/*0x0f, 0x34, 0xff,*/
	/*0x0f, 0x37, 0xff,*/
	/*0x0a, 0x72, 0x00,	*/
	/*Self Balance  */
	/*0x67, 0x25, 0x00,*/
	/*0x67, 0x2e, 0xa0,*/
	/*0x67, 0x2f, 0x0f,*/

	0x68, 0x42, 0x00,         
	/*0x88, 0x00, 0x03, //T/2 Auto*/
	/*0x17, 0xd8, 0x01, //TS watch dog thresh */
	0x10, 0xfb, 0x00, /*0x00 for PWM, 0x01 for PDM*/

	0x0a, 0x72, 0x00   /*CCI,ACI enable*/
};

static uint8 atbmdvbt2Setting[] =
{
	0x17, 0x9b, 0x00,
	0x17, 0x9d, 0x00,
	0x17, 0x9f, 0x00,
	0x17, 0xa1, 0x00
};
static uint8 atbmdvbtxTRegs[] =
{
	0x0f, 0x1c, 0x07, /*params_feq_sfi_det_th_in_dvbt                            */
	0x0f, 0x1d, 0x05, /*params_feq_sfi_det_th_out_dvbt  */
	0x0f, 0x1f, 0x14,/*params_feq_sfi_det_overflow_num_th_8k_dvbt     */
	0x0f, 0x22, 0x06,/*feq_sfi_det_num_out_dvbt*/
	/*0x0e, 0x39, 0x14,//th_channel_for_large_path_dy_dvbt                         */
	/*0x0e, 0x3d, 0x0a,//params_th_channel_for_normal_path_dy_dvbt                 */
	/*0x0e, 0x6e, 0x0c,//chan_search_alpha_short_dy_fo_8K_dvbt                     */
	/*0x0e, 0x71, 0xa6,//params_min_vale_floor_factor_8k_fo_dvbt                   */
	0x0f, 0x6a, 0x02,/*params_impulse_noise_factor_double_check_64qam_dvbt       */
	0x0f, 0x59, 0x0a,/*params_impulse_noise_power_th1_dvbt                       */
	0x0f, 0x65, 0x08 /*params_impulse_noise_th_small_factor_dvbt                 */
};

static uint8 atbmdvbtxMacRegs[] =
{
	0x24, 0x0D, 0x01,
	0x24, 0x0E, 0x01,
	0x24, 0x0F, 0x01,
	0x18, 0x65, 0x01,
	0x17, 0xC0, 0x00,
	0x17, 0xBF, 0x20,
	0x17, 0xC2, 0x00,
	0x17, 0xC1, 0x20,
	0x17, 0xC4, 0x00,
	0x17, 0xC3, 0x20,
	0x17, 0xC6, 0x00,
	0x17, 0xC5, 0x20,
	0x17, 0xDC, 0x00,
	0x17, 0xDB, 0x10,
	0x17, 0xDE, 0x00,
	0x17, 0xDD, 0x10,
	0x17, 0xE0, 0x00,
	0x17, 0xDF, 0x10,
	0x17, 0xE2, 0x00,
	0x17, 0xE1, 0x10,
	/*0x17, 0xBB, 0xF7,*/
	/*0x18, 0x55, 0x07,*/
	0x17, 0xf7, 0x00,
	/*0x18, 0x5e, 0x0f,*/
	0x18, 0x41, 0x09
};

static uint8 atbmdvbtx30Dot4M[] =
{      
	0x0a, 0xbf, 0x66,
	0x0a, 0xc0, 0x66,
	0x0a, 0xc1, 0x1e,
	0x0a, 0xc2, 0x79,
	0x0a, 0xc3, 0x0d,
	0x0a, 0xc4, 0x03,
	0x0a, 0xcf, 0x20,
	0x0a, 0xb8, 0x64,
	0x0a, 0x75, 0xc8,
	0x0a, 0x73, 0x12,
	0x0a, 0x74, 0x05,
	0x0a, 0x76, 0x10,
	0x0a, 0x78, 0xe0,
	0x0a, 0x79, 0xc8,
	0x0A, 0x88, 0x64,
	0x0A, 0x89, 0x00,
	0x0A, 0x8A, 0x96,
	0x0A, 0x8B, 0x00,
	0x0A, 0x8C, 0xc8,
	0x0A, 0x8D, 0x00,
	0x0A, 0x8E, 0xfa,
	0x0A, 0x8F, 0x00,
	0x0A, 0x90, 0x2c,
	0x0A, 0x91, 0x01,
	0x0A, 0x92, 0x5e,
	0x0A, 0x93, 0x01,
	0x0A, 0x94, 0x90,
	0x0A, 0x95, 0x01,
	0x0A, 0x96, 0xc2,
	0x0A, 0x97, 0x01,
	0x0A, 0x98, 0xf4,
	0x0A, 0x99, 0x01,
	0x0A, 0x9A, 0x26,
	0x0A, 0x9B, 0x02,
	0x0A, 0x9C, 0x58,
	0x0A, 0x9D, 0x02,
	0x0A, 0x9E, 0x8a,
	0x0A, 0x9F, 0x02,
	0x0A, 0xA0, 0xbc,
	0x0A, 0xA1, 0x02,
	0x0A, 0xA2, 0xee,
	0x0A, 0xA3, 0x32,
	0x0A, 0xA4, 0x20,
	0x0A, 0xA5, 0x03,
	0x0A, 0xA6, 0x52,
	0x0A, 0xA7, 0x03
};

/***************************default register value group for dvbt and dvbt2 *********************************************/
short i16DvbxBandWidthFilter[10][31] = {
	{-7  ,-9  ,-10 ,-9  ,-5  ,1   ,10  ,20  ,29  ,35  ,37  ,33  ,21  ,1   ,-24 ,-53 ,-80 ,-102,-112,-106,-80 ,-32 ,38  ,127 ,230 ,339 ,445 ,541 ,616 ,664 ,681 },/*1.7MHz 24*/
	{8   ,12  ,16  ,20  ,21  ,21  ,18  ,11  ,2   ,-11 ,-25 ,-41 ,-56 ,-69 ,-78 ,-81 ,-75 ,-61 ,-37 ,-2  ,43  ,97  ,158 ,223 ,289 ,352 ,410 ,459 ,497 ,520 ,528 },/*1.7MHz 30.4*/
	{-4  ,-7  ,-4  ,5   ,13  ,15  ,6   ,-12 ,-28 ,-28 ,-8  ,25  ,51  ,48  ,11  ,-46 ,-87 ,-80 ,-13 ,83  ,150 ,133 ,15  ,-158,-286,-259,-16 ,415 ,915 ,1313,1465},/*5MHz 24*/
	{10  ,0   ,-10 ,-18 ,-17 ,-7  ,10  ,27  ,36  ,30  ,8   ,-25 ,-54 ,-66 ,-51 ,-9  ,49  ,100 ,119 ,89  ,9   ,-99 ,-195,-235,-178,-10 ,257 ,574 ,874 ,1090,1168},/*5MHz 30.4*/
	{-2  ,5   ,8   ,1   ,-10 ,-16 ,-7  ,12  ,27  ,21  ,-9  ,-40 ,-43 ,-5  ,50  ,75  ,37  ,-47 ,-113,-93 ,21  ,152 ,184 ,51  ,-187,-350,-241,211 ,878 ,1473,1711},/*6MHz 24*/
	{-9  ,2   ,13  ,17  ,10  ,-6  ,-24 ,-31 ,-21 ,7   ,37  ,53  ,41  ,0   ,-52 ,-85 ,-76 ,-18 ,66  ,134 ,140 ,63  ,-78 ,-220,-279,-184,86  ,486 ,912 ,1237,1359},/*6MHz 30.4*/
	{6   ,-2  ,-9  ,-7  ,7   ,17  ,9   ,-14 ,-28 ,-12 ,25  ,44  ,15  ,-41 ,-65 ,-18 ,65  ,95  ,21  ,-102,-140,-23 ,163 ,218 ,25  ,-291,-399,-26 ,779 ,1607,1957},/*7MHz 24*/
	{9   ,-4  ,-15 ,-15 , -1 ,18  ,28  ,18  ,-10 ,-38 ,-43 ,-16 ,31  ,67  ,60  ,5   ,-68 ,-109,-76 ,24  ,134 ,173 ,90  ,-95 ,-275,-310,-98 ,352 ,911 ,1372,1550},/*7MHz 30.4*/
	{-5  ,-1  ,8   ,8   ,-5  ,-16 ,-7  ,17  ,24  ,-3  ,-35 ,-25 ,26  ,54  ,12  ,-61 ,-67 ,22  ,106 ,61  ,-88 ,-156,-17 ,199 ,201 ,-108,-417,-233,590 ,1594,2047},/*8MHz 24*/
	{0   ,0   ,0   ,0   ,0   ,-15 ,-17 ,3   ,27  ,31  ,4   ,-37 ,-53 ,-22 ,40  ,81  ,55  ,-31 ,-112,-110,-1  ,143 ,196 ,75  ,-170,-353,-263,187 ,871 ,1489,1737}/*8MHz 30.4*/
};
unsigned char ui8DvbxFilterMod[] = { 2 ,2  ,2  ,2  ,2  ,2  ,2 ,2,  1,  0};
/* ui16Alpha [] = {69,54,69	,54	,69	,54	,69	,54	,69	,54}; //using default setting PHY*/
int  i32DvbxInvFSADC [] = {87381,68985	,87381	,68985	,87381	,68985	,87381	,68985	,87381	,68985};

int  i32DvbxFSADC    [] = {1572864, 1992300, 1572864, 1992300, 1572864, 1992300, 1572864, 1992300, 1572864, 1992300}; /*U(22,16)*/

unsigned char ui8DvbxBWMode[] = {4,4,3,3,2,2,1,1,0,0};
/*unsigned int  i32DvbxRateRatio[] = {1745855027,2211416367,563714458,714038313,469762048,595031927,402653184,510027366,352321536,446273946};*/


/*Pre defined DVB-T signal parameters string, you can define any customized string on requirement */
static const char *gStrDvbTFFTMode[]         = { "2K", "8K", "RESERVED", "UNKNOWN"};
static const char *gStrDvbTGuardInterval[]   = { "1/32", "1/16", "1/8", "1/4", "UNKNOWN"};
static const char *gStrDvbTHierarchy[]       = { "NONE", "a=1", "a=2", "a=4",  "UNKNOWN"};
static const char *gStrDvbTConstellation[]   = { "QPSK", "16QAM", "64QAM",  "UNKNOWN"};
static const char *gStrDvbTCodeRate[]        = { "1/2", "2/3", "3/4", "5/6", "7/8", "RESERVED5", "RESERVED6", "UNKNOWN"};


/*Pre defined DVB-T2 signal parameters string, you can define any customized string on requirement */
static const char *gStrDvbT2FFTMode[]       = { "1K", "2K", "4K", "8K", "16K", "32K" , "RESERVED", "UNKNOWN"};
static const char *gStrDvbT2GuardInterval[] = { "1/32", "1/16", "1/8", "1/4", "1/128", "19/128", "19/256", "UNKNOWN"};
static const char *gStrDvbT2Version[]       = { "V1.11", "V1.21", "V1.31",  "UNKNOWN"};
static const char *gStrDvbT2CodeRate[]      = { "1/2", "3/5", "2/3", "3/4", "4/5", "5/6", "1/3", "2/5", "UNKNOWN"};
static const char *gStrDvbT2Constellation[] = { "QPSK", "16QAM", "64QAM", "256QAM","RESERVED4", "RESERVED5", \
"RESERVED6", "RESERVED7", "UNKNOWN"};
static const char *gStrDvbT2SisoMiso[]      = { "SISO", "MISO", "NON-T2",  "RESERVED3", "RESERVED4", "RESERVED5", \
"RESERVED6", "RESERVED7", "UNKNOWN"};


/*Merlin40 MLite set*/
ATBM781x_STATUS ATBM781xSetDVBT2BerWorkedRegs(void)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	ui8Status |=ATBM781xWrite(0x17, 0xcb, 0xff);
	ui8Status |= ATBM781xWrite(0x17, 0xfa, 0x00);
	/*ATBM781xWrite(0x17, 0xd9, 0xe1);  //BER ISSUE*/
	/*ATBM781xWrite(0x17, 0xf9, 0x01);  //default value*/
	ui8Status |= ATBM781xWrite(0x0a, 0xf0, 0x36);  /*default value*/
	return ui8Status;
}

/****************************************************************************
Function:    ATBM781xSetDVBTxMode
Parameters:  ui8DvbTxBandwidthType
Return:      Read  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description:
This function sets the demodulator working in DVB-T and DVB-T2 auto receiving mode
u32bandwidthKHz:
1700          5000          6000           7000          8000
****************************************************************************/
ATBM781x_STATUS  ATBM781xSetDVBTxMode(uint32   u32bandwidthKHz)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint8  u8ChipID = 0x00,ui8AnaReg = 0, ui8SyncdReg = 0;
	if(!gATBM781xPowerOnInited)
	{
		/*printf("Should call PowerOnInit before this function\n");*/
		return ATBM781x_SUCCESSFUL;
	}
	ui8Status |=ATBM781xRead(0x00, 0x00, &u8ChipID);

	gstATBM781xDvbConfig.DvbTxTunerInput.ui8BandwidthType = ATBM781x_DVBT_BANDWIDTH_8MHZ; /*default */

	if(u32bandwidthKHz == 8000)
	{
		gstATBM781xDvbConfig.DvbTxTunerInput.ui8BandwidthType = ATBM781x_DVBT_BANDWIDTH_8MHZ;
	}
	if(u32bandwidthKHz == 7000)
	{
		gstATBM781xDvbConfig.DvbTxTunerInput.ui8BandwidthType = ATBM781x_DVBT_BANDWIDTH_7MHZ;
	}
	if(u32bandwidthKHz == 6000)
	{
		gstATBM781xDvbConfig.DvbTxTunerInput.ui8BandwidthType = ATBM781x_DVBT_BANDWIDTH_6MHZ;
	}
	if(u32bandwidthKHz == 5000)
	{
		gstATBM781xDvbConfig.DvbTxTunerInput.ui8BandwidthType = ATBM781x_DVBT_BANDWIDTH_5MHZ;
	}
	if(u32bandwidthKHz == 1700)
	{
		gstATBM781xDvbConfig.DvbTxTunerInput.ui8BandwidthType = ATBM781x_DVBT_BANDWIDTH_1DOT7MHZ;
	}

	gstATBM781xDvbConfig.ui8ConfigDVBType = ATBM781x_DVBT_MODE; /*DVB-T/T2 auto detection.*/

	ui8Status |= ATBM781xWrite(0x00,0x03,1); /*Reset*/
	ui8Status |= ATBM781xSetSysClkClockDVBTx();
	ui8Status |= ATBM781xSetFreqAndRateRatio(gstATBM781xDvbConfig);
	ui8Status |= ATBM781xSetTSMode( gstATBM781xDvbConfig.stMPEGTSMode);
	ui8Status |= ATBM781xSetGpioPins();
	/*BER PER windows for L1 and common PLP*/
	if(0xa2 == u8ChipID)
	{	
		ui8Status |= ATBM781xWrite(0x17,0xbf,100);
		ui8Status |= ATBM781xWrite(0x17,0xc0,0);
		ui8Status |= ATBM781xWrite(0x17,0xc1,100);
		ui8Status |= ATBM781xWrite(0x17,0xc2,0);		
		ui8Status |= ATBM781xWrite(0x17,0xc3,0);
		ui8Status |= ATBM781xWrite(0x17,0xc4,1);
		ui8Status |= ATBM781xWrite(0x17,0xc5,100);
		ui8Status |= ATBM781xWrite(0x17,0xc6,0);
	}	
	if((0xa2 == u8ChipID)||(0xa4 == u8ChipID))
	{
		/*ui8Status |= ATBM781xWrite(0x00,0x37,0x05);//AGC Freeze GPIO5(reg_gpo6_0037)*/
	}

	if(gstATBM781xDvbConfig.DvbTxTunerInput.ui8IQSwapMode)
	{
		ui8Status |= ATBM781xSwapIQ(1);
	}else
	{
		ui8Status |= ATBM781xSwapIQ(0);
	}

	/*T BER PER */
	{
		ui8Status |= ATBM781xWrite(0x95,0x20,0x00);/*for count.*/
		ui8Status |= ATBM781xWrite(0x95,0x21,0x40);/*for count		*/
	}
	/*T2 BER, PER windows */
	{
		ui8Status |= ATBM781xWrite(0x18,0x16,0x00);/*count window*/
		ui8Status |= ATBM781xWrite(0x18,0x17,0x04);/*count window*/
		ui8Status |= ATBM781xWrite(0x18,0x18,0x00);/*count window*/
	}

	ui8Status |= ATBM781xSetBandWidthPara(gstATBM781xDvbConfig);

	ui8Status |= ATBM781xWriteRegArray(atbmdvbtxCommon, sizeof(atbmdvbtxCommon)/sizeof(uint8));

	ui8Status |= ATBM781xWriteRegArray(atbmdvbt2Setting, sizeof(atbmdvbt2Setting)/sizeof(uint8));

	if(gstATBM781xDvbConfig.ui32DemodClkKHz == 30400)
	{
		ui8Status |= ATBM781xWriteRegArray(atbmdvbtx30Dot4M, sizeof(atbmdvbtx30Dot4M)/sizeof(uint8));
		ui8Status |= ATBM781xWrite(0x0a,0xfb,0x00);
	}

	ui8Status |=ATBM781xWrite(0x24,0x03, 0x00);  /*Set auto PLP selection  mode.*/

	ui8Status |=ATBM781xWrite(0x10,0x2a, 0x00);  /**/

	ui8Status |=ATBM781xWrite(0x17,0xC7, 0x02);  /**/
	ui8Status |=ATBM781xWrite(0x17,0xC9, 0x02);  /**/
	ui8Status |=ATBM781xWrite(0x24,0x09, 0x3F);  /**/
	ui8Status |=ATBM781xWrite(0x17,0xd8, 0x1f);

	if(0xa2 == u8ChipID)
	{		
		ui8Status |=ATBM781xWrite(0x17,0xBB, 0xF1);	 /*f9->f1 */
		ui8Status |=ATBM781xWrite(0x17,0x49, 0x0F);  /*FF->f*/
		ui8Status |=ATBM781xWrite(0x17,0x47, 0xFF); /**/
	}

	ui8Status |=ATBM781xWrite(0x17,0x48, 0x00); /*ISSY error rapid reset*/


	if((0xa0 == u8ChipID)||(0xa2 == u8ChipID))
	{
		ui8Status |=ATBM781xWrite(0x17,0xF7, 0x00);
		ui8Status |=ATBM781xWrite(0x17,0xB5, 0xFF); /**/
		ui8Status |=ATBM781xWrite(0x17,0xB6, 0x4F); /**/
		ui8Status |=ATBM781xWrite(0x17,0x62, 0xff);
		ui8Status |=ATBM781xWrite(0x17,0x63, 0xff);
		ui8Status |=ATBM781xWrite(0x17,0xb3, 0x00);
		ui8Status |=ATBM781xWrite(0x17,0xb2, 0x0f);
		ui8Status |=ATBM781xWrite(0x17,0xb1, 0xff);
		ui8Status |=ATBM781xWrite(0x17,0xFA, 0x00); /**/
	}
	ui8Status |=ATBM781xWrite(0x17,0xb7, 0x00);  
	if(0xa4 == u8ChipID)	
	{
		ui8Status |=ATBM781xWrite(0x17,0xB5, 0x00); /**/
		ui8Status |=ATBM781xWrite(0x17,0xB6, 0x1b); /*tto`10%*/
		ui8Status |=ATBM781xWrite(0x17,0xb3, 0x00);
		ui8Status |=ATBM781xWrite(0x17,0xb2, 0x1b); /*f->4f,tto`10%*/
		ui8Status |=ATBM781xWrite(0x17,0xb1, 0x00);
	}

	/*chip id 0xa0*/
	if(0xa0 == u8ChipID)	
	{
		ui8Status |=ATBM781xWrite(0x0f, 0x34, 0xff); /*Only Merlin65 used.*/
		ui8Status |=ATBM781xWrite(0x0f, 0x37, 0xff);
		ui8Status |=ATBM781xWrite(0x0a, 0xf0, 0x45);/*Only Merlin65 used.*/
		/*Self Balance  */
		ui8Status |=ATBM781xWrite(0x67, 0x25, 0x00);
		ui8Status |=ATBM781xWrite(0x67, 0x2e, 0xa0);
		ui8Status |=ATBM781xWrite(0x67, 0x2f, 0x0f);/*Only Merlin65 used.*/
		/*Test DVB-T, Merlin 40 0x01*/
		ui8Status |=ATBM781xWrite(0x17, 0xf9, 0x00);
		
		/*ATBM781xWrite(0x01, 0x0c, 0x01);*/
	}
	ui8Status |=ATBM781xWrite(0x0f,0x72, 0x02);  /**/
	ui8Status |=ATBM781xWrite(0x88,0x0c, 0x08);  /**/
	ui8Status |=ATBM781xWrite(0x0f,0x73, 0x20);  /**/

	ui8Status |=ATBM781xWrite(0x17,0x53, 0x9e);  /*ISCR*/
	ui8Status |=ATBM781xWrite(0x28, 0x10, 0x00);
	ui8Status |=ATBM781xWrite(0x28, 0x11, 0x00);
	ui8Status |=ATBM781xWrite(0x28, 0x00, 0x40);
	ui8Status |=ATBM781xWrite(0x28, 0x01, 0x40);
	if(0xa4 == u8ChipID)
	{	
		ui8Status |=ATBM781xWrite(0x0f, 0x2f, 0x40);  
		/*ATBM781xSetDVBT2BerWorkedRegs();*/

		ui8Status |= ATBM781xWriteRegArray(atbmdvbtxMacRegs, sizeof(atbmdvbtxMacRegs)/sizeof(uint8));

		ui8Status |= ATBM781xWrite(0x17 ,0x47 ,0xf );
		ui8Status |= ATBM781xWrite(0x17 ,0x49 ,0x8 );
		ui8Status |= ATBM781xWrite(0x18 ,0x73 ,0xff);
		ui8Status |= ATBM781xWrite(0x18 ,0x75 ,0x01);
		ui8Status |= ATBM781xWrite(0x18 ,0x55 ,0x80);
		ui8Status |= ATBM781xWrite(0x17 ,0xbb ,0xf7);
		ui8Status |= ATBM781xWrite(0x18 ,0x5e ,0x5 );

		/*iscr_pll alpha*/
		ui8Status |= ATBM781xWrite(0x17, 0x4c, 0x08);
		ui8Status |= ATBM781xWrite(0x17, 0x4e, 0x20);
		ui8Status |= ATBM781xWrite(0x17, 0x56, 0x04);
		ui8Status |= ATBM781xWrite(0x17, 0x58, 0x10);
		ui8Status |= ATBM781xWrite(0x17, 0x44, 0x02);
		ui8Status |= ATBM781xWrite(0x17, 0x46, 0x08);
		/*Static Padding checking*/
		ui8Status |= ATBM781xWrite(0x17, 0x12, 0x01);

		/*add for dvb-t*/
		/*ATBM781xWrite(0x0f, 0x7f, 0x04);*/
		ui8Status |=ATBM781xWrite(0x0f, 0x8d, 0x02);
		ui8Status |=ATBM781xWrite(0x0f, 0x85, 0x80);
		ui8Status |=ATBM781xWrite(0x0f, 0x86, 0x7f);
		ui8Status |=ATBM781xWrite(0x0f, 0x87, 0xe0);
		ui8Status |=ATBM781xWrite(0x0f, 0x89, 0x07);

		/*add for dvb-tx	  */
		ui8Status |=ATBM781xWrite(0x0f, 0x7f, 0x02);	
		ui8Status |=ATBM781xWrite(0x0e, 0x9f, 0x00);
		/*dvb-t*/
		ui8Status |=ATBM781xWrite(0x0f, 0x80, 0x04);

		/*ui8Status |=  ATBM781xWrite(0x01, 0x0c, 0x01);	*/
		ui8Status |=  ATBM781xWrite(0x17, 0xfd, 0x01);	
		ui8Status |=  ATBM781xWrite(0x17, 0xd9, 0xc1);	
	}
	if((0xa2 == u8ChipID)||(0xa4 == u8ChipID))
	{
		ui8Status |=ATBM781xSetDVBT2BerWorkedRegs();
		ui8Status |=ATBM781xWrite(0x88, 0x00, 0x03); /*T T2*/
		ui8Status |=ATBM781xWrite(0x03,0x03, 0x01);  /**/
	}
	/* ATBM781xWrite(0x01,0x06, 0x00); */
	if(0xa2 == u8ChipID)	
	{
		ui8Status |=ATBM781xWrite(0x0e,0x9f, 0x01);
		ui8Status |=ATBM781xWrite(0x06,0x19, 0x20);  /**/
		ui8Status |=ATBM781xWrite(0x06,0x21, 0x7b);  /**/
		ui8Status |=ATBM781xWrite(0x06,0x21, 0x73);  /**/
		ui8Status |=ATBM781xSleep(10);
		ui8Status |= ATBM781xRead(0x06, 0x1A, &ui8AnaReg);
		ui8AnaReg = ui8AnaReg + 58; /*0x40 - 6*/
		ui8Status |= ATBM781xWrite(0x06, 0x19, ui8AnaReg);
	}
	ui8Status |= ATBM781xWriteRegArray(atbmdvbtxTRegs, sizeof(atbmdvbtxTRegs)/sizeof(uint8));
	ui8Status |= ATBM781xWrite(0x02, 0x0c, u8ATBM781xSoftwareVersion);  /*record SDK software version*/
	ui8Status |=  ATBM781xWrite(0x00, 0x2a, 0x03); /*shut diseqc clock*/
	ui8Status |=  ATBM781xWrite(0x67, 0x2F, 0x27); /*3.28 TDI*/
	ui8Status |= ATBM781xRead(0x17, 0x10, &ui8SyncdReg);
	ui8SyncdReg|=0x08;
	ui8Status |=  ATBM781xWrite(0x17, 0x10, ui8SyncdReg); /*6.11 Non-standard SYNCD*/
	ui8Status |= ATBM781xWrite(0x00, 0x04,1); /*config done*/

	if(0xa2 == u8ChipID)	
	{
		ui8Status |=ATBM781xSleep(10);
		ui8Status |=ATBM781xWrite(0x06,0x19, 0x20);  /**/
		ui8Status |=ATBM781xWrite(0x06,0x21, 0x7b);  /**/
		ui8Status |=ATBM781xWrite(0x06,0x21, 0x73);  /**/
		ui8Status |=ATBM781xSleep(10);
		ui8Status |= ATBM781xRead(0x06, 0x1A, &ui8AnaReg);
		ui8AnaReg = ui8AnaReg + 58; /*0x40 - 6*/
		ui8Status |= ATBM781xWrite(0x06, 0x19, ui8AnaReg);
		ui8Status |=  ATBM781xWrite(0x17, 0xdf, 0x1f);	
	}
	ui8Status |=  ATBM781xWrite(0x0a, 0x83, 0x50);		/*2k CCI*/
	ui8Status |=  ATBM781xWrite(0x00, 0x16, 0x00);		
	ui8Status |=  ATBM781xWrite(0x01, 0x0c, 0x01);	
	ui8Status |= ATBM781xWrite(0x00,0x05,1); /*reset dsp */
	ui8Status |= ATBM781xWrite(0x00,0x05,0); /*reset dsp*/
	return ui8Status;
}
/********************************************************************************
function    : ATBM781xSetFreqAndRateRatio
parameter   :stPara.ui32TunerIF:tuner if frequency ;  stPara.ui32DemodClk :demod clk
return      :Read  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description :Call this function to set demodulator's freq and rate ratio registers
*********************************************************************************/
ATBM781x_STATUS    ATBM781xSetFreqAndRateRatio(stATBM781xDvbConfig stPara)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint8  ui8FreqLifHigh = 0;
	uint32 ui32FreqLifLow = 0;
	ATBM781x64Data  i64Data,i64Data1,i64Rst;
	uint32 ui32TunerIFKHz = 24000;
	uint32 ui32ParamsIFFreq = 0;
	if((stPara.ui8ConfigDVBType == ATBM781x_DVBT_MODE)||(stPara.ui8ConfigDVBType == ATBM781x_DVBT2_MODE))
	{
		ui32TunerIFKHz = stPara.DvbTxTunerInput.u32IFFrequencyKHz;
	}

	if(stPara.ui8ConfigDVBType == ATBM781x_DVBC_MODE)
	{
		ui32TunerIFKHz = stPara.DvbCTunerInput.u32IFFrequencyKHz;
	}
	/* 	ui32TunerIFKHz = 5000;*/
	/*freq Calculate*/
	if (ui32TunerIFKHz < stPara.ui32DemodClkKHz)/*use equation dbTunerIF/dbDemodClk*/
	{
		ATBM781x64Mult(&i64Data,ATBM781xFixPow(2,30),ui32TunerIFKHz*64);
		i64Data1.ui32High = 0;
		i64Data1.ui32Low = stPara.ui32DemodClkKHz;
		i64Rst  =ATBM781x64DivReturn64(i64Data,i64Data1);
	}
	else/*use equation (dbTunerIF-dbDemodClk)/dbDemodClk*/
	{
		ATBM781x64Mult(&i64Data,ATBM781xFixPow(2,30),(ui32TunerIFKHz - stPara.ui32DemodClkKHz)*64);
		i64Data1.ui32High = 0;
		i64Data1.ui32Low = stPara.ui32DemodClkKHz;
		i64Rst  =ATBM781x64DivReturn64(i64Data,i64Data1);
	}
	ui32FreqLifLow = i64Rst.ui32Low;/*64bit system */
	ui8FreqLifHigh = (uint8)(i64Rst.ui32High);

	/*start write register*/
	ui8Status = ATBM781xWrite(0x0a,0x00,(uint8)(ui32FreqLifLow));
	ui8Status |= ATBM781xWrite(0x0a,0x01,(uint8)(ui32FreqLifLow>>8));
	ui8Status |= ATBM781xWrite(0x0a,0x02,(uint8)(ui32FreqLifLow>>16));
	ui8Status |= ATBM781xWrite(0x0a,0x03,(uint8)(ui32FreqLifLow>>24));

	/*ui8Status |= ATBM781xRead(0x0a,0x04,&ui8Tmp);*/
	ui8Status |=ATBM781xWrite(0x0a,0x04, ui8FreqLifHigh);

	ui32ParamsIFFreq = (ui32TunerIFKHz * 32768 + 250)/500;   /*round(IF_MHz*Pow(2,16)) //Add 2014.3.24*/
	ui8Status |= ATBM781xWrite(0x0a,0xbc,(uint8)(ui32ParamsIFFreq));
	ui8Status |= ATBM781xWrite(0x0a,0xbd,(uint8)(ui32ParamsIFFreq>>8));
	ui8Status |= ATBM781xWrite(0x0a,0xbe,(uint8)(ui32ParamsIFFreq>>16));
	return ui8Status;
}
/********************************************************************************
function    : ATBM781xSetBandWidthPara
parameter   :stPara.ui8DVBTxBandWidth
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description :according to stPara.ui8DVBTxBandWidth value .load the default register value
to the chip            
*********************************************************************************/
ATBM781x_STATUS   ATBM781xSetBandWidthPara(stATBM781xDvbConfig stPara)
{	
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;	
	uint8 ui8Tmp = 0, u8ChipID = 0;
	uint8 ui8RegTmp = 0; 
	uint8 ui8Iter = 0,ui8Numerator = 0,ui8Denominator = 0; 
	uint32 ui32RatioRate = 0; 
	ATBM781x64Data i64Data,i64Data1;
	ui8Status =ATBM781xRead(0x00, 0x00, &u8ChipID);
	if((stPara.ui8ConfigDVBType == ATBM781x_DVBT_MODE)||(stPara.ui8ConfigDVBType == ATBM781x_DVBT2_MODE))
	{
		ui8Iter = stPara.DvbTxTunerInput.ui8BandwidthType * 2 + (((int)stPara.ui32DemodClkKHz/1000) == 24?0:1);
		u8sDVBTxBandwidth = stPara.DvbTxTunerInput.ui8BandwidthType;

		if(stPara.DvbTxTunerInput.ui8BandwidthType != ATBM781x_DVBT_BANDWIDTH_8MHZ)  /*Default is for 8MHz Bandwidth.*/
		{		
			for (ui8Tmp=0; ui8Tmp<31; ui8Tmp++)
			{
				ui8Status |= ATBM781xWrite(0x0a,0x09+ui8Tmp*2,(uint8)i16DvbxBandWidthFilter[ui8Iter][ui8Tmp]);
				ui8Status |= ATBM781xWrite(0x0a,0x09+ui8Tmp*2+1,(uint8)(i16DvbxBandWidthFilter[ui8Iter][ui8Tmp]>>8));

			}
		}
		ui8Status |= ATBM781xWrite(0x0a,0xfb,ui8DvbxFilterMod[ui8Iter]);

		/* 		ui8Status |= ATBM781xWrite(0x0a,0x05,(uint8)i32DvbxRateRatio[ui8Iter]);*/
		/* 		ui8Status |= ATBM781xWrite(0x0a,0x06,(uint8)(i32DvbxRateRatio[ui8Iter]>>8));*/
		/* 		ui8Status |= ATBM781xWrite(0x0a,0x07,(uint8)(i32DvbxRateRatio[ui8Iter]>>16));*/
		/* 		ui8Status |= ATBM781xWrite(0x0a,0x08,(uint8)(i32DvbxRateRatio[ui8Iter]>>24));*/
		/*24MHz using default registers*/
		if(stPara.ui32DemodClkKHz == 30400)
		{
			ui8Status |= ATBM781xWrite(0x0a,0xc2,(uint8)i32DvbxInvFSADC[ui8Iter]);
			ui8Status |= ATBM781xWrite(0x0a,0xc3,(uint8)(i32DvbxInvFSADC[ui8Iter]>>8));
			ui8RegTmp = (uint8)(i32DvbxInvFSADC[ui8Iter]>>16);
			ui8RegTmp|= 0x02;
			ui8Status |= ATBM781xWrite(0x0a, 0xc4, ui8RegTmp);
		}
		ui8Status |= ATBM781xWrite(0x0a,0xbf,(uint8)i32DvbxFSADC[ui8Iter]);
		ui8Status |= ATBM781xWrite(0x0a,0xc0,(uint8)(i32DvbxFSADC[ui8Iter]>>8));
		ui8Status |= ATBM781xWrite(0x0a,0xc1,(uint8)(i32DvbxFSADC[ui8Iter]>>16));

		ui8Status |= ATBM781xWrite(0x0a,0xb9,ui8DvbxBWMode[ui8Iter]);
	}

	if(stPara.ui8ConfigDVBType == ATBM781x_DVBC_MODE)
	{
		ui8Iter = ATBM781x_DVBT_BANDWIDTH_8MHZ*2 + (((int)stPara.ui32DemodClkKHz/1000) == 24?0:1);	
		ui8Status |= ATBM781xWrite(0x0a,0xbf,(uint8)i32DvbxFSADC[ui8Iter]);
		ui8Status |= ATBM781xWrite(0x0a,0xc0,(uint8)(i32DvbxFSADC[ui8Iter]>>8));
		ui8Status |= ATBM781xWrite(0x0a,0xc1,(uint8)(i32DvbxFSADC[ui8Iter]>>16));
		/*24MHz using default registers*/
		if(stPara.ui32DemodClkKHz == 30400)
		{
			ui8Status |= ATBM781xWrite(0x0a,0xc2,(uint8)i32DvbxInvFSADC[ui8Iter]);
			ui8Status |= ATBM781xWrite(0x0a,0xc3,(uint8)(i32DvbxInvFSADC[ui8Iter]>>8));
			ui8RegTmp = (uint8)(i32DvbxInvFSADC[ui8Iter]>>16);
			ui8RegTmp|= 0x02;
			ui8Status |= ATBM781xWrite(0x0a, 0xc4, ui8RegTmp);
		}
		/*ui8Status |= ATBM781xWrite(0x0a,0x05,(uint8)i32DvbxRateRatio[ui8Iter]);*/
		/*ui8Status |= ATBM781xWrite(0x0a,0x06,(uint8)(i32DvbxRateRatio[ui8Iter]>>8));*/
		/*ui8Status |= ATBM781xWrite(0x0a,0x07,(uint8)(i32DvbxRateRatio[ui8Iter]>>16));*/
		/*ui8Status |= ATBM781xWrite(0x0a,0x08,(uint8)(i32DvbxRateRatio[ui8Iter]>>24));*/
		/* ATBM781xSetDvbBWSFI(stPara.ui32DemodClkKHz, i32InvFSADC[ui8Iter]);*/
	}
	if (u8sDVBTxBandwidth == ATBM781x_DVBT_BANDWIDTH_1DOT7MHZ)
	{
		ui8Numerator = 71;
		ui8Denominator = 131;

	}
	else if (u8sDVBTxBandwidth == ATBM781x_DVBT_BANDWIDTH_5MHZ)
	{
		ui8Numerator = 7;
		ui8Denominator = 40;
	}
	else if (u8sDVBTxBandwidth== ATBM781x_DVBT_BANDWIDTH_6MHZ)
	{
		ui8Numerator = 7;
		ui8Denominator = 48;
	}
	else if (u8sDVBTxBandwidth == ATBM781x_DVBT_BANDWIDTH_7MHZ)
	{
		ui8Numerator = 1;
		ui8Denominator = 8;
	}
	else if (u8sDVBTxBandwidth == ATBM781x_DVBT_BANDWIDTH_8MHZ)
	{
		ui8Numerator = 7;
		ui8Denominator = 64;
	}
	else
	{
		ui8Numerator = 7;
		ui8Denominator = 80;
	}
	/*Rate_ratio (0a 05-0a 08) the calculation formula is
	Rate_ratio = floor(FS_ADC/(2*BW)*2^28)   = FS_ADC/BW*2^27 = FS_ADC*ui8Numerator*2^27/ui8Denominator;
	BW is bandwidth£¬8M: 64/7e6; BW = 1/T
	*/

	ATBM781x64Mult(&i64Data,stPara.ui32DemodClkKHz*ui8Numerator,ATBM781xFixPow(2,27));
	ATBM781x64Mult(&i64Data1,ui8Denominator,1);

	i64Data = ATBM781x64DivReturn64(i64Data,i64Data1);/*1000;//clock is KHz*/
	ATBM781x64Mult(&i64Data1,1000,1);
	ui32RatioRate = ATBM781x64Div(i64Data,i64Data1);/*1000;//clock is KHz*/
	if((stPara.ui8ConfigDVBType == ATBM781x_DVBT_MODE)||(stPara.ui8ConfigDVBType == ATBM781x_DVBT2_MODE))
	{
		ui8Status |= ATBM781xWrite(0x0a,0x05,(uint8)ui32RatioRate);
		ui8Status |= ATBM781xWrite(0x0a,0x06,(uint8)(ui32RatioRate>>8));
		ui8Status |= ATBM781xWrite(0x0a,0x07,(uint8)(ui32RatioRate>>16));
		ui8Status |= ATBM781xWrite(0x0a,0x08,(uint8)(ui32RatioRate>>24));
	}
	/*ratio rate*/
	return ui8Status;
}
/********************************************************************************
function    : ATBM781xSetADC
parameter   :ui8Adc:1 
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description :Call this function to ADC mode.
*********************************************************************************/
ATBM781x_STATUS    ATBM781xSetADC(uint8 ui8Adc)
{
	return ATBM781xWrite(0x03,0x00,ui8Adc);
}

/********************************************************************************
function    : ATBM781xSwapIQ
parameter   :ui8IQ:1:not swap and 1 swap 
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description:Call this function to Set Swap demodulator's IQ or Not. 
*********************************************************************************/
ATBM781x_STATUS    ATBM781xSwapIQ(uint8 ui8IQ)
{
	return ATBM781xWrite(0x03,0x02,ui8IQ);
}


/********************************************************************************
function    :ATBM781xDVBTxModeDetected
parameter   :pu8DVBTxMode
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description:Call this function to check signal mode, DVB-T or DVB-T2 mode
*********************************************************************************/
ATBM781x_STATUS ATBM781xDVBTxModeDetected(uint8 *pu8DVBTxMode)
{
	uint8 u8SyncT2 = 0, u8SyncT = 0;

	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL; 
	*pu8DVBTxMode =ATBM781x_DVB_UNKNOWN;
	status =ATBM781xRead(0x88, 0x6D, &u8SyncT); 
	if((u8SyncT ==1)&&(status== ATBM781x_SUCCESSFUL))
	{
		*pu8DVBTxMode = ATBM781x_DVBT_MODE;
	}
	status |=ATBM781xRead(0x88, 0x6C, &u8SyncT2);

	if((u8SyncT2 ==1)&&(status== ATBM781x_SUCCESSFUL))
	{
		*pu8DVBTxMode = ATBM781x_DVBT2_MODE;
	}	
	return status;
}

/********************************************************************************
function    : ATBM781xSetDVBT2DisablePLPDecode
parameter   :N/A.
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal 
Description:Call this function to disable PLP decode, but receives signal parameters.
NOTE:only be used in DVB-Tx Scanning
*********************************************************************************/
ATBM781x_STATUS ATBM781xSetDVBT2DisablePLPDecode(void)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint8 u8tmp0, u8tmp1;
	status |=ATBM781xRead(0x0f, 0x70, &u8tmp0);
	status |=ATBM781xRead(0x17, 0xbb, &u8tmp1); 
	u8tmp0 |=0x08;
	u8tmp1 &=0xfe;
	status =ATBM781xWrite(0x0f, 0x70, u8tmp0); 
	status =ATBM781xWrite(0x17, 0xbb, u8tmp1); 
	status =ATBM781xWrite(0x00, 0x05, 0x01); 
	status =ATBM781xWrite(0x00, 0x05, 0x00); 
	return status;
}

/********************************************************************************
function    : ATBM781xSetDVBT2EnablePLPDecode
parameter   :N/A.
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal 
Description:Call this function to enable PLP decode.
NOTE:only be used in DVB-Tx Scanning
*********************************************************************************/
ATBM781x_STATUS ATBM781xSetDVBT2EnablePLPDecode(void)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint8 u8tmp0, u8tmp1;
	status |=ATBM781xRead(0x0f, 0x70, &u8tmp0);
	status |=ATBM781xRead(0x17, 0xbb, &u8tmp1); 
	u8tmp0 &=0xf7;
	u8tmp1 |=0x01;
	status =ATBM781xWrite(0x0f, 0x70, u8tmp0); 
	status =ATBM781xWrite(0x17, 0xbb, u8tmp1);
	status =ATBM781xWrite(0x00, 0x05, 0x01); 
	status =ATBM781xWrite(0x00, 0x05, 0x00); 
	return status;
}

/********************************************************************************
function    :ATBM781xDVBTxSigalParamsLockCheck
parameter   :pu8LockStatus
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description:Call this function to Check DVB-T or DVB-T2 signal

ATBM781xDVBTxSigalParamsLockCheck  locked indicate DVB-T or DVB-T2 signal is detected. 
it can be used after tuner tuned.
*********************************************************************************/
ATBM781x_STATUS ATBM781xDVBTxSigalParamsLockCheck(uint8 *pu8LockStatus)
{
	/*	
	88H	6CH	t2_det_done_reg
	88H	6DH	t_det_done_reg
	*/
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;

	uint8 u8SyncT = 0, u8SyncT2 = 0;
	uint8 u8L1PreCRCRight = 0, u8L1PostCRCRight = 0;

	uint8 u8DvbReceivingMode = ATBM781x_DVB_UNKNOWN;

	uint8 u8SyncFlag = 0, u8TSLockFlag = 0, u8PlpListready = 0, u8DvbtFecLock = 0;
	uint8 u8TPSDone = 0, u8TPSFlag = 0;

	uint8 u8Iter = 0;
	uint32 u32CheckTime = 1200;
	uint8  u8CheckCount = 40;   
	uint32 u32UnsyncTimeOut = 600;
	uint32 u32UnDecodedTimeOut = 900;       
	uint32 u32DelayIntervalms;
	uint32 u8MaxUnSyncCount, u8MaxUnDecodedCount; 

	u32DelayIntervalms    = u32CheckTime/u8CheckCount; 
	u8MaxUnSyncCount      = u32UnsyncTimeOut/u32DelayIntervalms;
	u8MaxUnDecodedCount = u32UnDecodedTimeOut/u32DelayIntervalms;

	*pu8LockStatus  = 0; /*Assuming not locked*/

	for(u8Iter =0; u8Iter< 40; u8Iter++)
	{
		/*Check Sync Status*/
		status |=ATBM781xRead(0x88, 0x6C, &u8SyncT2);
		status |=ATBM781xRead(0x88, 0x6D, &u8SyncT); 

		if(u8SyncT2 ==1)
		{
			u8DvbReceivingMode = ATBM781x_DVBT2_MODE;
			u8SyncFlag = 1;
		}
		if(u8SyncT ==1)
		{
			u8DvbReceivingMode = ATBM781x_DVBT_MODE;
			u8SyncFlag = 1;
		}
		if((u8Iter >= u8MaxUnSyncCount)&&(u8SyncFlag == 0)) /*UnSync Time out*/
		{
			*pu8LockStatus = 0;
			return status;
		}        

		/*Check only if DVB-T2 Sync detected*/
		if(u8DvbReceivingMode == ATBM781x_DVBT2_MODE) /*Sync State indicate DVB-T2 signal may be received*/
		{
			status |=ATBM781xRead(0x0B, 0x01, &u8L1PreCRCRight);
			status |=ATBM781xRead(0x0B, 0x02, &u8L1PostCRCRight);
			status |=ATBM781xRead(0x18, 0x2A, &u8TSLockFlag);
			status |=ATBM781xRead(0x24, 0x04, &u8PlpListready);

			if((u8L1PostCRCRight&0x01)&&((u8PlpListready&0x01) == 0x01)) /*Anyone CRC right indicate T2 signal can be decoded*/
			{
				if(status == ATBM781x_SUCCESSFUL)
					*pu8LockStatus = 1;
				return status;
			}
			if((u8Iter >= u8MaxUnDecodedCount)&&(u8L1PreCRCRight == 0)) /*UnDecoded Time out*/
			{
				*pu8LockStatus = 0;
				return status;
			}   		
		} 

		/*Check only if DVB-T Sync detected*/
		if(u8DvbReceivingMode == ATBM781x_DVBT_MODE) /*Sync State indicate DVB-T signal may be received*/
		{
			status |=ATBM781xRead(0x28, 0x2c, &u8TPSDone);
			/*status |=ATBM781xRead(0x18, 0x2a, &u8TSLockFlag);*/
			status |=ATBM781xRead(0x95, 0x37, &u8DvbtFecLock);
			if(u8TPSDone&0x01)
			{
				u8TPSFlag = 1;
			}
			if((u8Iter >= u8MaxUnDecodedCount)&&(u8TPSFlag == 0)) /*UnDecoded Time out to save scan time*/
			{
				*pu8LockStatus = 0;
				return ATBM781x_SUCCESSFUL;
			}   
			if(u8DvbtFecLock&0x01) /*TS locked or TPS done, DVB-T signal detected*/
			{
				if(status == ATBM781x_SUCCESSFUL)
					*pu8LockStatus = 1;
				return status;
			}
		}
		status |=ATBM781xSleep(u32DelayIntervalms);
	}
	return status;
}

/********************************************************************************
function    :ATBM781xDVBTxChannelLockCheck
parameter   :pu8LockStatus
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description:Call this function to Check DVB-T or DVB-T2 signal FEC(TS) lock state

DVB-T:
ATBM781xDVBTxChannelLockCheck locked indicate TPS decoded or TS locked.

DVB-T2:
ATBM781xDVBTxChannelLockCheck locked indicate PLP FEC locked or TS locked.
For MPLP, this function should be called after PLP id is set. 

MPLP Example:
1, ATBM781xDVBTxSigalParamsLockCheck,  Signal partially locked and T2 signal is detected.
2, ATBM781xGetSignalParamsT2
3, ATBM781xSetPLPForMultiPLPDVBT2
4, ATBM781xDVBTxChannelLockCheck,     Check FEC lock
*********************************************************************************/
ATBM781x_STATUS ATBM781xDVBTxChannelLockCheck(uint8 *pu8LockStatus)
{
	/*	
	88H	6CH	t2_det_done_reg
	88H	6DH	t_det_done_reg
	*/
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;

	uint8 u8SyncT = 0, u8SyncT2 = 0;
	uint8 u8L1PreCRCRight = 0, u8L1PostCRCRight = 0;

	uint8 u8DvbReceibingMode = ATBM781x_DVB_UNKNOWN;

	uint8 u8SyncFlag = 0, u8L1CRCFlag = 0, u8TSLockFlag = 0, u8DataPLPFecLock = 0, u8DvbtFecLock = 0;
	uint8 u8TPSDone = 0, u8TPSFlag = 0;

	uint8 u8Iter = 0;
	uint32 u32CheckTime = 3000;
	uint8  u8CheckCount = 60;   /*Should > 0*/
	uint32 u32UnsyncTimeOut = 500;
	uint32 u32UnDecodedDVBTTimeOut  = 700;   //TPS decoded
    uint32 u32UnDecodedDVBT2TimeOut = 800;   //DVB-T2 L1 right, add One T2 Frame.
    uint32 u32DVBTFECTimeOut = 2000;
	uint32 u32DelayIntervalms = 50;
	uint32 u8MaxUnSyncCount = 10, u8MaxUnDecodedCountT = 14, u8MaxUnDecodedCountT2 = 16, u8MaxFECCountDVBT = 40; 
	if(u8CheckCount!= 0)
	{	
		u32DelayIntervalms      = u32CheckTime/u8CheckCount; /*50 milliseconds, 3000/60 */
	}
	if(u32DelayIntervalms!=0)
	{	
		u8MaxUnSyncCount    = u32UnsyncTimeOut/u32DelayIntervalms;
		u8MaxUnDecodedCountT        = u32UnDecodedDVBTTimeOut/u32DelayIntervalms;        /*DVB-T  0.7s TPS decoded Timeout */
		u8MaxUnDecodedCountT2       = u32UnDecodedDVBT2TimeOut/u32DelayIntervalms;       /*DVB-T2 0.8s L1-Post decoded right timeout*/
		u8MaxFECCountDVBT           = u32DVBTFECTimeOut/u32DelayIntervalms;
	}
	*pu8LockStatus  = 0; /*Assuming not locked*/

	for(u8Iter =0; u8Iter< u8CheckCount; u8Iter++)
	{
		/*Check Sync Status*/
		status |=ATBM781xRead(0x88, 0x6C, &u8SyncT2);
		status |=ATBM781xRead(0x88, 0x6D, &u8SyncT); 

		if(u8SyncT2 ==1)
		{
			u8DvbReceibingMode = ATBM781x_DVBT2_MODE;
			u8SyncFlag = 1;
		}
		if(u8SyncT ==1)
		{
			u8DvbReceibingMode = ATBM781x_DVBT_MODE;
			u8SyncFlag = 1;
		}
		if((u8Iter >= u8MaxUnSyncCount)&&(u8SyncFlag == 0)) /*UnSync Time out*/
		{
			*pu8LockStatus = 0;
			return status;
		}        

		/*Check only if DVB-T2 Sync detected*/
		if(u8DvbReceibingMode == ATBM781x_DVBT2_MODE) /*Sync State indicate DVB-T2 signal may be received*/
		{
			status |=ATBM781xRead(0x0B, 0x01, &u8L1PreCRCRight);
			status |=ATBM781xRead(0x0B, 0x02, &u8L1PostCRCRight);
			status |=ATBM781xRead(0x18, 0x2A, &u8TSLockFlag);
			status |=ATBM781xRead(0x17, 0xEC, &u8DataPLPFecLock);

			if((u8L1PreCRCRight&0x01)||(u8L1PostCRCRight&0x01)) /*Anyone CRC right indicate T2 signal can be decoded*/
			{
				if(status == ATBM781x_SUCCESSFUL)
					u8L1CRCFlag = 1;
			}
			if((u8Iter >= u8MaxUnDecodedCountT2)&&(u8L1CRCFlag == 0)) /*T2 Signal's parameters decoding Time out*/
			{
				*pu8LockStatus = 0;
				return status;
			}   
			if((u8TSLockFlag&0x01)&&(u8L1PreCRCRight&0x01)) /*Add L1-PreCRC right, TS lock will not be the only indicator*/
			{
				if(status == ATBM781x_SUCCESSFUL)
					*pu8LockStatus = 1;
				return status;
			}	

			/*if(u8DataPLPFecLock&0x01) */ /*L1-PostCRC decoded right, T2 signal locked,PLP may be decoded*/
			/*{*/
			/*if(status == ATBM781x_SUCCESSFUL)*/
			/*	*pu8LockStatus = 1;*/
			/*	return status;*/
			/*	}	*/
		} 

		/*Check only if DVB-T Sync detected*/
		if(u8DvbReceibingMode == ATBM781x_DVBT_MODE) /*Sync State indicate DVB-T signal may be received*/
		{
			status |=ATBM781xRead(0x28, 0x2c, &u8TPSDone);
			status |=ATBM781xRead(0x95, 0x37, &u8DvbtFecLock);
			if(u8TPSDone&0x01)
			{
				u8TPSFlag = 1;
			}
			if((u8Iter >= u8MaxUnDecodedCountT)&&(u8TPSFlag == 0)) /*DVBT Signal's parameters decoding Time out to save scan time*/
			{
				*pu8LockStatus = 0;
				return ATBM781x_SUCCESSFUL;
			}   
			if(u8Iter >=  u8MaxFECCountDVBT)
			{
				*pu8LockStatus = 0;
				return ATBM781x_SUCCESSFUL;
			}
			if(u8DvbtFecLock&0x01) /*FEC locked, DVB-T signal detected*/
			{
				if(status == ATBM781x_SUCCESSFUL)
					*pu8LockStatus = 1;
				return status;
			}
		}
		status |=ATBM781xSleep(u32DelayIntervalms);
	}
	return status;
}

/********************************************************************************
function    :ATBM781xDVBTxChannelLockCheckForManual
parameter   :pu8LockStatus, 1--Locked;0--Unlocked
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description: Try to check lock state by calling 'ATBM781xDVBTxChannelLockCheck' some times until it's 
			locked or timeout, refer to ATBM781xDVBTxChannelLockCheck
*********************************************************************************/
ATBM781x_STATUS ATBM781xDVBTxChannelLockCheckForManual(uint8 *LockStatus) 
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint8 u8LockState = 0;
	uint8 u8Iter = 0;
	/*Max timeout is 2.4 seconds if no signal , 6 seconds if signal is very weak*/
	*LockStatus = 0;
	for(u8Iter=0; u8Iter<3; u8Iter++)
	{
		status = ATBM781xDVBTxChannelLockCheck(&u8LockState);
		if((u8LockState == 1)&&(status == ATBM781x_SUCCESSFUL))
		{
			*LockStatus = u8LockState;
			return status;
		}
	}
	return status;
}

/********************************************************************************
function    : ATBM781xDVBTxSignalIFAGC
parameter   :ui16CalcValue:if AGC return by this pointer 
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description:Call this function to get IF AGC value
*********************************************************************************/
ATBM781x_STATUS   ATBM781xDVBTxSignalIFAGC(uint16 *ui16CalcValue)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint8 ui8Pwm0 = 0, ui8Pwm1 = 0;

	ui8Status |= ATBM781xLatchOn();
	ui8Status |= ATBM781xRead(0x10,0x28,&ui8Pwm0);  
	ui8Status |= ATBM781xRead(0x10,0x29,&ui8Pwm1);
	ui8Status |= ATBM781xLatchOff();
	if (ui8Status)
	{
		*ui16CalcValue = 0;
		return ui8Status;
	}
	*ui16CalcValue = ((ui8Pwm1&0x03)<<8)|ui8Pwm0;
	return ui8Status;
}

/********************************************************************************
function    : ATBM781xDVBTxSignalQuality
parameter   :pu8SQI:SignalQuality return by this pointer 
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description:Call this function to get SignalQuality, DVB-T or DVB-T2
*********************************************************************************/
ATBM781x_STATUS ATBM781xDVBTxSignalQuality(uint8 *pu8SQI)	
{

	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL; 
	uint8 u8DVBTxMode = 0;
	status =  ATBM781xDVBTxModeDetected(&u8DVBTxMode);

	*pu8SQI = 0;
	if((u8DVBTxMode ==ATBM781x_DVBT_MODE)&&(status== ATBM781x_SUCCESSFUL))
	{
		status = ATBM781xGetDVBTSigalQuality(pu8SQI);
	}


	if((u8DVBTxMode ==ATBM781x_DVBT2_MODE)&&(status== ATBM781x_SUCCESSFUL))
	{
		status = ATBM781xGetDVBT2SignalQuality(pu8SQI);
	}
	return status;
}


/********************************************************************************
function    : ATBM781xGetDVBTxBER
parameter   :pu32PERx10E9:BER return by this pointer 
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description:Call this function to get BER,DVB-T or DVB-T2, this is not TS output BER, TS output error rate refer to PER functions.
*********************************************************************************/
ATBM781x_STATUS ATBM781xGetDVBTxBER(uint32 *pu32BERx10E9)	
{

	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL; 
	uint8 u8DVBTxMode = 0;
	status =  ATBM781xDVBTxModeDetected(&u8DVBTxMode);

	*pu32BERx10E9 = 1000000000;
	if((u8DVBTxMode ==ATBM781x_DVBT_MODE)&&(status== ATBM781x_SUCCESSFUL))
	{
		status =  ATBM781xGetDVBTBER(pu32BERx10E9);
	}


	if((u8DVBTxMode ==ATBM781x_DVBT2_MODE)&&(status== ATBM781x_SUCCESSFUL))
	{
		status =  ATBM781xGetDVBT2BER(pu32BERx10E9);
	}
	return status;

}

/********************************************************************************
function    : ATBM781xGetDVBTxPER
parameter   :pu32PERx10E9:PER return by this pointer 
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description:Call this function to get PER,DVB-T or DVB-T2, *scalar(10E9)
*********************************************************************************/
ATBM781x_STATUS ATBM781xGetDVBTxPER(uint32 *pu32PERx10E9)	
{

	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL; 
	uint8 u8DVBTxMode = 0;
	status =  ATBM781xDVBTxModeDetected(&u8DVBTxMode);

	*pu32PERx10E9 = 1000000000;

	if((u8DVBTxMode ==ATBM781x_DVBT_MODE)&&(status== ATBM781x_SUCCESSFUL))
	{
		status =  ATBM781xGetDVBTPER(pu32PERx10E9);
	}

	if((u8DVBTxMode ==ATBM781x_DVBT2_MODE)&&(status== ATBM781x_SUCCESSFUL))
	{
		status =  ATBM781xGetDVBT2PostBCHBlockER(pu32PERx10E9);
	}
	return status;

}

/********************************************************************************
function    : ATBM781xGetDVBTxSNR
parameter   :ui32SNRValue:DVBTx SNR value returned by this pointer unit is dB  *10 
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal 
Description:Call this function to Get Tx SNR result,Unit is dB in decimal integer format, *scalar(10).
*********************************************************************************/ 
ATBM781x_STATUS ATBM781xGetDVBTxSNR(uint32 *pu32SNRx10)	
{

	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL; 
	uint8 u8DVBTxMode = 0;
	status =  ATBM781xDVBTxModeDetected(&u8DVBTxMode);
	*pu32SNRx10 = 0;

	if((u8DVBTxMode ==ATBM781x_DVBT_MODE)&&(status== ATBM781x_SUCCESSFUL))
	{
		status =  ATBM781xGetDVBTSNR(pu32SNRx10);
	}


	if((u8DVBTxMode ==ATBM781x_DVBT2_MODE)&&(status== ATBM781x_SUCCESSFUL))
	{
		status =   ATBM781xGetDVBT2SNR(pu32SNRx10);
	}
	return status;
}

/********************************************************************************
function    : ATBM781xGetDVBTxCarrierOffset
parameter   :pi32Value:DVBTx CarrierOffset value returned by this pointer unit is Unit KHz 
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal 
Description:Call this function to get CarrierOffset result.
*********************************************************************************/ 
ATBM781x_STATUS ATBM781xGetDVBTxCarrierOffset(int *pi32Value)	
{

	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL; 
	uint8 u8DVBTxMode = 0;
	status =  ATBM781xDVBTxModeDetected(&u8DVBTxMode);

	*pi32Value = 0;
	if((u8DVBTxMode ==ATBM781x_DVBT_MODE)&&(status== ATBM781x_SUCCESSFUL))
	{
		status =  ATBM781xGetDVBTCFO(pi32Value);
	}


	if((u8DVBTxMode ==ATBM781x_DVBT2_MODE)&&(status== ATBM781x_SUCCESSFUL))
	{
		status =  ATBM781xGetDVBT2CFO(pi32Value);
	}
	return status;
}

/********************************************************************************
function    : ATBM781xGetDVBTxSampleOffsetPPM
parameter   :pi32Value:the value sample offset PPM is return by this pointer
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description:Call this function to calculate the PPM value of Sample frequency Offset
            and return this value,DVB-T or DVB-T2
*********************************************************************************/
ATBM781x_STATUS  ATBM781xGetDVBTxSampleOffsetPPM(int*pi32Value)
{

	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL; 
	uint8 u8DVBTxMode = 0;
	status =  ATBM781xDVBTxModeDetected(&u8DVBTxMode);
	*pi32Value = 0;

	if((u8DVBTxMode ==ATBM781x_DVBT_MODE)&&(status== ATBM781x_SUCCESSFUL))
	{
		status |= ATBM781xGetDVBTSampleOffsetPPM(pi32Value);
	}

	if((u8DVBTxMode ==ATBM781x_DVBT2_MODE)&&(status== ATBM781x_SUCCESSFUL))
	{
		status |=ATBM781xGetDVBT2SampleOffsetPPM(pi32Value);
	}
	return status;
}

/********************************************************************************
function    : ATBM781xSetPLPForMultiPLPDVBT2
parameter   :
	u8PLPid: PLP ID to receive
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description:Call this function to set PLP ID for multiPLP signal. 
			If to receive first PLP in multiPLP or single PLP signal, should use ATBM781xSetAutoPLPT2
*********************************************************************************/
ATBM781x_STATUS ATBM781xSetPLPForMultiPLPDVBT2(uint8 u8PLPid )
{ 
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	status |=ATBM781xWrite(0x24,0x02, u8PLPid);      
	status |=ATBM781xWrite(0x24,0x03, 0x01);      /* Enable PLP selection,*/	
	status |= ATBM781xWrite(0x00, 0x05, 0x01);
	status |= ATBM781xWrite(0x00, 0x05, 0x00);
	return status;
}



/********************************************************************************
function    : ATBM781xSetAutoPLPT2
parameter   :N/A
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description: Used for the default DVB-T2 single PLP working mode, or Receiving the first Data PLP in MPLP model. 
*********************************************************************************/
ATBM781x_STATUS ATBM781xSetAutoPLPT2(void)
{ 
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;	 

	status = ATBM781xWrite(0x24,0x03, 0x00);    /* Disable PLP selection, for single PLP or Multi-PLP, always receive first Data PLP */	
	status |= ATBM781xWrite(0x00, 0x05, 0x01);
	status |= ATBM781xWrite(0x00, 0x05, 0x00);
	if(status!=ATBM781x_SUCCESSFUL)
	{
		ATBM781xDebugPrint("Error in set auto PLP receiving mode\n");
	}
	return status;
}
/****************************************************************************
Function:	ATBM781xSetDVBT2BaseProfile
Parameters: No         
Return: 	Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal 

Description:
DVB-T2 Version >=1.31, T2 lite is support.
Some DVB-T2 signal may be Base and Lite Mixed signal.
Call this function to configure DVBT2 working in DVB-T2 Base profile mode.

In Channel scanning mode:
For T2-Base and Lite unknown and will receive both Base and Lite: ATBM781xSetDVBT2BaseAndLiteProfile
T2-Base only:                                                     ATBM781xSetDVBT2BaseProfile
T2-Lite only:                                                     ATBM781xSetDVBT2LiteProfile

In playing mode the Base or Lite is known:
T2-Base only:                                                  ATBM781xSetDVBT2BaseProfile
T2-Lite only:                                                  ATBM781xSetDVBT2LiteProfile
******************************************************************************/
ATBM781x_STATUS ATBM781xSetDVBT2BaseProfile(void)
{ 
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;	
	status =ATBM781xWrite(0x00, 0x05, 0x01); 
	status |= ATBM781xWrite(0x88,0x02, 0x08);   	
	status |= ATBM781xWrite(0x88,0x03, 0x08);   
	status |= ATBM781xWrite(0x88,0x04, 0x08);   
	status |= ATBM781xWrite(0x88,0x05, 0x08);   
	status |= ATBM781xWrite(0x88,0x0a, 0x02);   
	status |= ATBM781xWrite(0x88,0x06, 0x08);   /*Default*/
	status |= ATBM781xWrite(0x88,0x07, 0x1a);   /*Default*/
	status |= ATBM781xWrite(0x88,0x08, 0x2c);   /*Default*/
	status |= ATBM781xWrite(0x17,0xd8, 0x1f);
	status |= ATBM781xWrite(0x00, 0x05, 0x00); 	
	return status;
}
/****************************************************************************
Function:	ATBM781xSetDVBT2LiteProfile
Parameters: No         
Return: 	Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal 

Description:
DVB-T2 Version >=1.31, T2 lite is support.
Some DVB-T2 signal may be Base and Lite Mixed signal.
Call this function to configure DVBT2 working in DVB-T2 Lite profile mode.

In Channel scanning mode:
For T2-Base and Lite unknown and will receive both Base and Lite: ATBM781xSetDVBT2BaseAndLiteProfile
T2-Base only:                                                     ATBM781xSetDVBT2BaseProfile
T2-Lite only:                                                     ATBM781xSetDVBT2LiteProfile

In playing mode the Base or Lite is known:
T2-Base only:                                                  ATBM781xSetDVBT2BaseProfile
T2-Lite only:                                                  ATBM781xSetDVBT2LiteProfile
******************************************************************************/
ATBM781x_STATUS ATBM781xSetDVBT2LiteProfile(void)
{ 
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;	
	status =ATBM781xWrite(0x00, 0x05, 0x01); 

	status |= ATBM781xWrite(0x88,0x02, 0x23);     
	status |= ATBM781xWrite(0x88,0x03, 0x23);   
	status |= ATBM781xWrite(0x88,0x04, 0x23);   
	status |= ATBM781xWrite(0x88,0x05, 0x23);   
	status |= ATBM781xWrite(0x88,0x0a, 0x02);
	status |= ATBM781xWrite(0x88,0x06, 0x23);   /* ¼´ËÑË÷Ë³ÐòÎª011,100,010,000,001,¡­*/
	status |= ATBM781xWrite(0x88,0x07, 0x02);
	status |= ATBM781xWrite(0x88,0x08, 0x29);

	status |= ATBM781xWrite(0x17,0xd8, 0x3f);
	status |= ATBM781xWrite(0x00, 0x05, 0x00); 	
	return status;
}


/****************************************************************************
Function:	ATBM781xSetDVBT2BaseAndLiteProfile
Parameters: No         
Return: 	Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal 

Description:
DVB-T2 Version >=1.31, T2 lite is support.
Some DVB-T2 signal may be Base and Lite Mixed signal.
Call this function to configure DVBT2 working in DVB-T2 Base and Lite Auto Mode.

In Channel scanning mode:
For T2-Base and Lite unknown and will receive both Base and Lite: ATBM781xSetDVBT2BaseAndLiteProfile
T2-Base only:                                                 ATBM781xSetDVBT2BaseProfile
T2-Lite only:                                                 ATBM781xSetDVBT2LiteProfile

In playing mode the Base or Lite is known:
T2-Base only:                                                  ATBM781xSetDVBT2BaseProfile
T2-Lite only:                                                  ATBM781xSetDVBT2LiteProfile
******************************************************************************/
ATBM781x_STATUS ATBM781xSetDVBT2BaseAndLiteProfile(void)
{

	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;	
	status =ATBM781xWrite(0x00, 0x05, 0x01); 

	status |= ATBM781xWrite(0x88,0x02, 0x08);     
	status |= ATBM781xWrite(0x88,0x03, 0x23);   
	status |= ATBM781xWrite(0x88,0x04, 0x08);   
	status |= ATBM781xWrite(0x88,0x05, 0x08);   
	status |= ATBM781xWrite(0x88,0x0a, 0x04);
	status |= ATBM781xWrite(0x88,0x06, 0x08);   /*Default*/
	status |= ATBM781xWrite(0x88,0x07, 0x1a);   /*Default*/
	status |= ATBM781xWrite(0x88,0x08, 0x2c);   /*Default*/
	status |= ATBM781xWrite(0x17,0xd8, 0x3f);
	status |= ATBM781xWrite(0x00, 0x05, 0x00); 	
	return status;
}
/********************************************************************************
function    : ATBM781xGetDVBT2SNR
parameter   :ui32SNRValue:DVBT2 SNR value returned by this pointer unit is dB 
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal 
Description:Call this function to Get T2 SNR result, Unit is dB*10.
*********************************************************************************/
ATBM781x_STATUS   ATBM781xGetDVBT2SNR(uint32 *ui32SNRValuex10)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint8 ui8Value = 0;
	uint32 ui32RoSignalVar = 0,ui32RoNoiseVar = 0;
	ATBM781x64Data i64Data,i64Data1;

	/*Latch On*/
	ui8Status = ATBM781xLatchOn();

	ui8Status |= ATBM781xRead(0x11,0x5c,&ui8Value);
	ui32RoSignalVar = (ui8Value<<16);
	ui8Status |= ATBM781xRead(0x11,0x5b,&ui8Value);
	ui32RoSignalVar += (ui8Value<<8);
	ui8Status |= ATBM781xRead(0x11,0x5a,&ui8Value);
	ui32RoSignalVar += ui8Value;


	ui8Status |= ATBM781xRead(0x11,0x62,&ui8Value);
	ui32RoNoiseVar = (ui8Value<<16);
	ui8Status |= ATBM781xRead(0x11,0x61,&ui8Value);
	ui32RoNoiseVar += (ui8Value<<8);
	ui8Status |= ATBM781xRead(0x11,0x60,&ui8Value);
	ui32RoNoiseVar += ui8Value;

	/*Latch Off*/
	ui8Status |= ATBM781xLatchOff();
	if (ui8Status)
	{
		*ui32SNRValuex10 = 0;
		return ui8Status;
	}
	if (ui32RoNoiseVar == 0||ui32RoSignalVar == 0)
	{
		*ui32SNRValuex10 = 0;
		return ui8Status;
	}
	i64Data1.ui32High = 0;
	i64Data1.ui32Low = ui32RoNoiseVar;
	ATBM781x64Mult(&i64Data,ui32RoSignalVar,4);
	*ui32SNRValuex10 = ATBM781xLog10ConvertdBx10(ATBM781x64Div(i64Data,i64Data1));
	return ui8Status;
}


/****************************************************************************
Function:	ATBM781xGetDVBT2BER
Parameters:   ui32T2BER: return dvbt2 BER by this pointer,the return number  is the real 
BER value * 1e9.so ,if you want to get the correct BER value,you should 
use the return value/1e9                
Return: 	  Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal 
Description:
Call this function to get DVBT2 BER value, this is LDPC error rate, 
not TS output error rate.please refer to ATBM781xGetDVBT2PostBCHBlockER function.	 
******************************************************************************/
ATBM781x_STATUS  ATBM781xGetDVBT2BER(uint32 *ui32T2BER)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint8 ui8PlpFecType = 0,ui8CodeRate = 0;
	uint8 u8L1S1 = 0, u8LiteUsed = 0;
	uint8 ui8PWindow[3], ui8PBERCount[3];
	uint16  ui16PBCHLenTable[2][8] = {
		{7200,  9720,  10800, 11880, 12600, 13320, 5400, 6480}, /* Short FEC */
		{32400, 38880, 43200, 48600, 51840, 54000, 32400, 32400}  /* Normal FEC, the last two is not used */
	};
	uint16  ui16NBCHLen = 1;
	uint32 ui32BerCalculated = 0;
	uint32 ui32BlockWindow = 1, ui32BERCount = 1;
	ATBM781x64Data i64Data,i64Data1;

	/*Value:code rate
	000:1/2, 001:3/5, 010:2/3, 011:3/4, 100:4/5, 101:5/6
	*/
	ui8Status = ATBM781xRead(0x0C, 0x38,&ui8CodeRate); 

	/* ui8PlpFecType:0 16K, 1:64K*/
	ui8Status |= ATBM781xRead(0x0C, 0x2E,&ui8PlpFecType); 

	ui8Status |=ATBM781xRead(0x0B, 0x05,  &u8L1S1);
	if(((u8L1S1&0x07) == 0x03)||((u8L1S1&0x07) == 0x04))
	{
		u8LiteUsed = 1;
	}
	if((ui8PlpFecType < 2)&&(ui8CodeRate < 6)&&((u8L1S1&0x03)<= 0x01))
	{
		ui16NBCHLen = ui16PBCHLenTable[ui8PlpFecType][ui8CodeRate];
	}
	else if((ui8PlpFecType < 2)&&(ui8CodeRate < 8)&&(u8LiteUsed == 1))
	{
		ui16NBCHLen = ui16PBCHLenTable[ui8PlpFecType][ui8CodeRate];
	}
	else
	{
		*ui32T2BER = 1;
		return ui8Status;
	}

	ui8Status |= ATBM781xLatchOn();
	ui8Status |= ATBM781xRead(0x18, 0x18,&ui8PWindow[2]);
	ui8Status |= ATBM781xRead(0x18, 0x17,&ui8PWindow[1]);
	ui8Status |= ATBM781xRead(0x18, 0x16,&ui8PWindow[0]);

	ui8Status |= ATBM781xRead(0x18, 0x1f,&ui8PBERCount[0]);
	ui8Status |= ATBM781xRead(0x18, 0x20,&ui8PBERCount[1]);
	ui8Status |= ATBM781xRead(0x18, 0x21,&ui8PBERCount[2]);
	ui8Status |= ATBM781xLatchOff();
	if (ui8Status != 0)
	{
		*ui32T2BER = 0;
		return ui8Status;
	}
	ui32BlockWindow = (ui8PWindow[2]&0x0f)*256*256 + ui8PWindow[1]*256 + ui8PWindow[0];
	ui32BERCount = ui8PBERCount[2]*256*256 + ui8PBERCount[1]*256 + ui8PBERCount[0];

	if(ui32BlockWindow!= 0)
	{
		ATBM781x64Mult(&i64Data,ui32BERCount,1000000000);
		ATBM781x64Mult(&i64Data1,ui32BlockWindow,ui16NBCHLen);
		ui32BerCalculated = ATBM781x64Div(i64Data,i64Data1);

	}
	*ui32T2BER = ui32BerCalculated;
	return ui8Status;


}

/****************************************************************************
Function:	ATBM781xGetDVBT2PostBCHBlockER
Parameters:   ui32T2BlockER: return dvbt2 Post BCH block error rate by this pointer,
the return number  is the real number ui32T2BlockER value * 1e9.so ,if you want to get the correct
BlockER value,you should use the return value/1e9                
Return: 	  Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal 
Description:
Call this function to get DVBT2 BlockErrorRate value, can be used to evaluate TS output error rate.
******************************************************************************/
ATBM781x_STATUS  ATBM781xGetDVBT2PostBCHBlockER(uint32 *ui32T2BlockER)
{
	uint8 ui8Tmp = 0;
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint16 ui16Tmp = 0,ui16WinLen = 0;
	ATBM781x64Data i64Data,i64Data1;

	ui8Status = ATBM781xLatchOn();
	ui8Status |= ATBM781xRead(0x17,0xc4,&ui8Tmp);
	ui16WinLen = ui8Tmp<<8;
	ui8Status |= ATBM781xRead(0x17,0xc3,&ui8Tmp);
	ui16WinLen = ui16WinLen + ui8Tmp;

	ui8Status |= ATBM781xRead(0x17,0xd4,&ui8Tmp);
	ui16Tmp = ui8Tmp<<8;
	ui8Status |= ATBM781xRead(0x17,0xd3,&ui8Tmp);
	ui16Tmp = ui16Tmp + ui8Tmp;
	ui8Status |= ATBM781xLatchOff();
	ATBM781x64Mult(&i64Data,ui16Tmp,1000000000);
	ATBM781x64Mult(&i64Data1,ui16WinLen,1);
	*ui32T2BlockER = ATBM781x64Div(i64Data,i64Data1);
	return ui8Status;
}

/********************************************************************************
function    : ATBM781xGetDVBT2SampleOffsetPPM
parameter   :i32Value:the value sample offset PPM is return by this pointer
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description :Call this function to calculate the ppm value of sample frequency offset.
            Different Crystal used may have different offset value. recommended in range of [-70, 70]
*********************************************************************************/
ATBM781x_STATUS  ATBM781xGetDVBT2SampleOffsetPPM(int*i32Value)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint8 ui8Value = 0;
	char i8Signal = 1;
	int i32RoPhiAdj = 0;
	ATBM781x64Data i64Data,i64Data1;
	int i32OffsetValue = 0;
	unsigned int SampleRegs24M = 0;
	unsigned char u8TsLockFlag = 0;
	unsigned char u8SmpRegs[3];
	ui8Status = ATBM781xLatchOn();

	ui8Status |= ATBM781xRead(0x11,0x0d,&ui8Value);
	i32RoPhiAdj = ((ui8Value&0xf)<<16);
	ui8Status |= ATBM781xRead(0x11,0x0c,&ui8Value);
	i32RoPhiAdj += (ui8Value<<8);
	ui8Status |= ATBM781xRead(0x11,0x0b,&ui8Value);
	i32RoPhiAdj += ui8Value;

	ui8Status |= ATBM781xLatchOff();
	if (ui8Status)
	{
		*i32Value = 0;
		return ui8Status;
	}

	if (i32RoPhiAdj&0x80000)/*Negative Data*/
	{
		i8Signal = -1;
		i32RoPhiAdj = i32RoPhiAdj - 1048576;/*Convert it to 32 bit Value(it is a Negative Data)*/
		i32RoPhiAdj = i32RoPhiAdj*i8Signal;/*Convert it to Positive Data*/
	}
	/*i32RoPhiAdj *1.0e6 / pow(2.0, 30)*/

	ATBM781x64Mult(&i64Data,i32RoPhiAdj,15625);
	i64Data1.ui32High = 0;
	i64Data1.ui32Low = ATBM781xFixPow(2,24);
	*i32Value = ATBM781x64Div(i64Data,i64Data1)*i8Signal;
	ui8Status |= ATBM781xRead(0x18,0x2a,&u8TsLockFlag);
	i32OffsetValue = *i32Value ;
	if((u8TsLockFlag == 1)&&(i32OffsetValue < 500) &&(i32OffsetValue > -500))
	{
		ui8Status |= ATBM781xRead(0x0a,0x08,&ui8Value);
		SampleRegs24M = ((ui8Value&0xff)<<16);
		ui8Status |= ATBM781xRead(0x0a,0x07,&ui8Value);
		SampleRegs24M += (ui8Value<<8);
		ui8Status |= ATBM781xRead(0x0a,0x06,&ui8Value);
		SampleRegs24M += ui8Value;

		SampleRegs24M =  SampleRegs24M + i32OffsetValue*7/5;

		u8SmpRegs[0] = (uint8)(SampleRegs24M&0xff);
		u8SmpRegs[1] = (uint8)(SampleRegs24M>>8);
		u8SmpRegs[2] = (uint8)(SampleRegs24M>>16);
		ATBM781xDebugPrint("Registers Set for 24MHz Sample PPM calibration is 0x0a08=0x%x  0x0a07=0x%x 0x0a06=0x%x\n", u8SmpRegs[2], u8SmpRegs[1],u8SmpRegs[0]);
	}	
	return ui8Status;
	/* 	return (i32RoPhiAdj*15625/pow(2.0,24)); */
}
/********************************************************************************
function    :ATBM781xGetDVBT2CFO
parameter   :stPara,use the parameter  stPara.ui8DVBTxBandWidth. i32Value:T2 CFO value.Unit KHz
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal 
Description :Call this function to calculate the CFO values of DVB-T2.
*********************************************************************************/
ATBM781x_STATUS   ATBM781xGetDVBT2CFO(int*i32Value)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	char ui8DataA = 0;
	uint8 ui8Value = 0;
	uint8 ui8Numerator = 0,ui8Denominator = 0;
	char i8Signal = 1;
	int i32DataC = 0;
	int ui32CFO = 0,i32Rst = 0;
	ATBM781x64Data i64Data,i64Data1;

	ui8Status = ATBM781xLatchOn();
	ui8Status |= ATBM781xRead(0x11,0x0a,&ui8Value);
	ui32CFO = ((ui8Value&0xf)<<24);
	ui8Status |= ATBM781xRead(0x11,0x09,&ui8Value);
	ui32CFO += (ui8Value<<16);
	ui8Status |= ATBM781xRead(0x11,0x08,&ui8Value);
	ui32CFO += (ui8Value<<8);
	ui8Status |= ATBM781xRead(0x11,0x07,&ui8Value);
	ui32CFO += ui8Value;
	ui8Status |= ATBM781xLatchOff();

	if (ui8Status)
	{
		*i32Value = 0;
		return ui8Status;
	}
	if (ui32CFO&0x8000000)
	{
		ui32CFO = ui32CFO -268435456;/*ui32CFO - 2^28*/
		i8Signal = -1;
		ui32CFO = ui32CFO * i8Signal;/*first convert it to Positive data*/
	}

	/* 	ATBM781x64Mult(&i64Data,ui32CFO,1000);//KHz*/

	/*6487ED344    first should store the data to the Dividend   this data is PI*2^33 's Result*/
	i64Data1.ui32High = 6;
	i64Data1.ui32Low = 0x487ED345;


	if (u8sDVBTxBandwidth == ATBM781x_DVBT_BANDWIDTH_1DOT7MHZ)/*1.7 MHz Only support for T2 */
	{
		ATBM781x64Mult(&i64Data,ui32CFO,1000*131);/*KHz*/
		i32Rst = ATBM781x64Div(i64Data,i64Data1)/71;
	}
	else if (u8sDVBTxBandwidth == ATBM781x_DVBT_BANDWIDTH_5MHZ)/*6MHz*/
	{
		ATBM781x64Mult(&i64Data,ui32CFO,1000*40);/*KHz*/
		i32Rst = ATBM781x64Div(i64Data,i64Data1)/7;
	}
	else if (u8sDVBTxBandwidth == ATBM781x_DVBT_BANDWIDTH_6MHZ)/*6MHz*/
	{
		ATBM781x64Mult(&i64Data,ui32CFO,1000*48);/*KHz*/
		i32Rst = ATBM781x64Div(i64Data,i64Data1)/7;
	}
	else if (u8sDVBTxBandwidth== ATBM781x_DVBT_BANDWIDTH_7MHZ)/*7MHz*/
	{
		ATBM781x64Mult(&i64Data,ui32CFO,1000*8);/*KHz*/
		i32Rst = ATBM781x64Div(i64Data,i64Data1);
	}
	else if (u8sDVBTxBandwidth == ATBM781x_DVBT_BANDWIDTH_8MHZ)/*8MHz*/
	{
		ATBM781x64Mult(&i64Data,ui32CFO,1000*64);/*KHz*/
		i32Rst = ATBM781x64Div(i64Data,i64Data1)/7;
	}
	else/*other  (10MHZ)*/
	{
		ATBM781x64Mult(&i64Data,ui32CFO,1000*80);/*KHz*/
		i32Rst = ATBM781x64Div(i64Data,i64Data1)/7;
	}

	i32Rst = i32Rst*i8Signal;
	/*i8Signal = 1;*/
	ui8Status |= ATBM781xLatchOn();/**/
	ui8Status |= ATBM781xRead(0x88,0x88,&ui8Value);
	i32DataC = ((ui8Value& 0x3f)<<8);
	ui8Status |= ATBM781xRead(0x88,0x87,&ui8Value);
	i32DataC += ui8Value;
	ui8Status |= ATBM781xLatchOff();
	ui8Status |= ATBM781xRead(0x88,0x89,&ui8Value);
	ui8DataA  = ui8Value;
	if (ui8Status)
	{
		*i32Value = 0;
		return ui8Status;
	}
	if (i32DataC&0x2000)
	{
		i32DataC = i32DataC-16384;
		/*i8Signal = -1;*/
		/*i32DataC = i32DataC *i8Signal;*/
	}
	i32DataC = i32DataC* 1000/12868 + ui8DataA* 1000;
	if (u8sDVBTxBandwidth == ATBM781x_DVBT_BANDWIDTH_1DOT7MHZ)
	{
		ui8Numerator = 71;
		ui8Denominator = 131;

	}
	else if (u8sDVBTxBandwidth == ATBM781x_DVBT_BANDWIDTH_5MHZ)
	{
		ui8Numerator = 7;
		ui8Denominator = 40;
	}
	else if (u8sDVBTxBandwidth== ATBM781x_DVBT_BANDWIDTH_6MHZ)
	{
		ui8Numerator = 7;
		ui8Denominator = 48;
	}
	else if (u8sDVBTxBandwidth ==ATBM781x_DVBT_BANDWIDTH_7MHZ)
	{
		ui8Numerator = 1;
		ui8Denominator = 8;
	}
	else if (u8sDVBTxBandwidth ==ATBM781x_DVBT_BANDWIDTH_8MHZ)
	{
		ui8Numerator = 7;
		ui8Denominator = 64;
	}
	else
	{
		ui8Numerator = 7;
		ui8Denominator = 80;
	}
	i32DataC = (i32DataC *ui8Denominator)/(ui8Numerator*1024);
	*i32Value = i32Rst + i32DataC;
	return ui8Status;


}

/********************************************************************************
function    : ATBM781xGetDVBT2SignalQuality
parameter   :ui8TSQ:return the sq value by this pointer.
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal 
Description :Call this function to get DVB-T2 signal quality,value is in range of [0, 100].
*********************************************************************************/ 
ATBM781x_STATUS  ATBM781xGetDVBT2SignalQuality(uint8* pui8T2SQ)
{
	uint8 ui8CodeRate = 0,ui8PlpModu = 0;
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint32 ui32SNR = 0;
	uint32 ui32SqiValue = 0;
	uint32 ui32Ddvbt2BER = 0;
	uint32 ui32BerSqi = 1;
	uint32 ui32SnrX10 = 0, ui32SnrRel = 0; 
	uint8 u8L1S1 = 0, u8LiteUsed = 0;

	static  uint16 ui32PSnrNordigX10[4][8] = {
		/*   1/2,   3/5,   2/3,   3/4,    4/5,    5/6 ; Lite:1/3 2/5*/
		{35, 47, 56, 66, 72, 77, 10,  20},      /* QPSK */
		{87, 101, 114, 125, 133, 138, 40, 50}, /* 16-QAM */
		{130, 148, 162,  177, 187, 194, 130, 130}, /* 64-QAM */
		{170, 194, 208, 229, 243, 251, 170,170}, /* 256-QAM */
	};
	ui8Status |=ATBM781xRead(0x0B, 0x05,  &u8L1S1);
	if(((u8L1S1&0x07) == 0x03)||((u8L1S1&0x07) == 0x04))
	{
		u8LiteUsed = 1;
	}
	/*Value:code rate
	000:1/2, 001:3/5, 010:2/3, 011:3/4, 100:4/5, 101:5/6
	*/
	ui8Status = ATBM781xRead(0x0C, 0x38,&ui8CodeRate); 
	/*QPSK, 16QAM, 64QAM, 256QAM*/
	ui8Status |= ATBM781xRead(0x0C, 0x2F,&ui8PlpModu); 
	if((ui8CodeRate < 6)&&(ui8PlpModu < 4)&&(u8LiteUsed ==0))
	{
		ui32SnrRel = ui32PSnrNordigX10[ui8PlpModu][ui8CodeRate];
	}
	else if((ui8CodeRate < 8)&&(ui8PlpModu < 4)&&(u8LiteUsed ==1))
	{
		ui32SnrRel = ui32PSnrNordigX10[ui8PlpModu][ui8CodeRate]; /*For T2 Lite*/
	}
	else
	{
		*pui8T2SQ = 0;
		return ATBM781x_FAILED; /*error*/
	}
	ui8Status |= ATBM781xGetDVBT2SNR(&ui32SNR);/*snrÒÑ¾­*10*/
	ui32SnrX10 = (ui32SNR);

	if(ui32SnrX10>6)
	{
		ui32SnrX10 = ui32SnrX10 - 6; /*0.6dB calibration*/
	}
	if(ui32SnrX10 > ui32SnrRel + 30) /*SNR >3dB + C/Nrel, SQI = 100*/
	{
		ui32SqiValue = 100;
	}
	else if((ui32SnrX10  <= ui32SnrRel + 30) && (ui32SnrX10  >= ui32SnrRel - 30) ) 
	{  
		/*
		BER_SQI = 0             if BER>1.0E-4
		BER_SQI = (100/15)      if 1.0E-4 <= BER <= 1.0E-7
		BER_SQI = (100/6)       if BER<1.0E-7
		*/
		ui8Status |= ATBM781xGetDVBT2BER(&ui32Ddvbt2BER);
		/* 		 if( ui32Ddvbt2BER >0.000101)*/
		if( ui32Ddvbt2BER >= 101000)
		{
			ui32BerSqi = 0;
			ui32SqiValue = ui32BerSqi;
			*pui8T2SQ = (unsigned char)ui32SqiValue;
			return ui8Status;
		}
		/*       else if((ui32Ddvbt2BER < 0.0001)&&(ui32Ddvbt2BER >0.0000001))*/
		else if((ui32Ddvbt2BER < 101000)&&(ui32Ddvbt2BER >100))

		{
			/* 			 ui32BerSqi = 100.0/15;*/
			ui32BerSqi = 15;
		}
		else
		{
			/* 			 ui32BerSqi = 100.0/6;*/
			ui32BerSqi = 6;
		}
		/*SQI = (CR_received - CR_nordig + 3) * BER_SQI*/
		/* 		 ui32SqiValue = ((ui32SnrX10 - ui32SnrRel + 30.0)/10) * ui32BerSqi;*/
		if(ui32BerSqi!=0)
		{		
			ui32SqiValue = (((ui32SnrX10 - ui32SnrRel) + 30)*10) / ui32BerSqi;
		}
	}
	else
	{
		ui32SqiValue = 0;
	}
	if(ui32SqiValue< 5)
	{
		ui32SqiValue = 0;
	}

	if((ui32SqiValue<100)&&(ui32SqiValue >=10)&&(ui8CodeRate == 3))
	{
		ui32SqiValue = ui32SqiValue - ((110 - ui32SqiValue)/10);
	}
	if (ui8Status)
	{
		*pui8T2SQ = 0;
		return ui8Status;
	}
	*pui8T2SQ = (unsigned char)(ui32SqiValue);
	return ui8Status;
}


ATBM781x_STATUS ATBM781xSetDefaultParamsT2(ATBM781x_DVBT2_PARAMS *pDvbT2Params)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;	

	pDvbT2Params->Dvbt2BandwidthExt = ATBM781x_DVBT2_BANDWIDTH_EXT_UNKNOWN;
	pDvbT2Params->Dvbt2CodeRate      = ATBM781x_DVBT2_CODE_RATE_UNKNOWN;
	pDvbT2Params->Dvbt2Constellation = ATBM781x_DVBT2_QAM_UNKNOWN;
	pDvbT2Params->Dvbt2DataPLPType = ATBM781x_DVBT2_PLP_TYPE_UNKNOWN;
	pDvbT2Params->Dvbt2FecLengthType = ATBM781x_DVBT2_LDPC_LENGTH_UNKNOWN;

	pDvbT2Params->Dvbt2FFTMode          = ATBM781x_DVBT2_MODE_UNKNOWN;
	pDvbT2Params->Dvbt2GuardInterval  = ATBM781x_DVBT2_GI_UNKNOWN;
	pDvbT2Params->Dvbt2IssyType         = ATBM781x_DVBT2_ISSY_UNKNOWN;
	pDvbT2Params->Dvbt2PayloadType   = ATBM781x_DVBT2_PAYLOAD_UNKNOWN;
	pDvbT2Params->Dvbt2PilotPattern     = ATBM781x_DVBT2_PP_UNKNOWN;

	pDvbT2Params->Dvbt2SisoMiso         = ATBM781x_DVBT2_SISO_MISO_UNKNOWN;
	pDvbT2Params->Dvbt2StreamType   = ATBM781x_DVBT2_STREAM_MODE_UNKNOWN;
	pDvbT2Params->Dvbt2Version          =ATBM781x_DVBT2_VERSION_UNKNOWN;
	pDvbT2Params->Dvbt2BaseOrLiteType   = ATBM781x_DVBT2_BASE_LITE_UNKNOWN;
	pDvbT2Params->u32FefLength          = 0;
	pDvbT2Params->u8FefInterval          = 1;
	pDvbT2Params->u8FefType              = 0;
	pDvbT2Params->u8ActivePlpId         = 0;
	pDvbT2Params->u8ConstellationRotation =0;
	pDvbT2Params->u8DataPlpNumber =0;
	return status;	

}

/********************************************************************************
function    :ATBM781xGetSignalParamsT2
parameter   :pDvbTParams:pointer to DVB-T2 signal parameters.
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal 
Description :Call this function to Get DVB-T2 signal parameters.
*********************************************************************************/
ATBM781x_STATUS ATBM781xGetSignalParamsT2(ATBM781x_DVBT2_PARAMS *pDvbT2Params)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;	
	/*T2 System Parameters */
	uint8 u8FFTSize = 0, u8Ext = 0;
	uint8 u8GuardInterval = 0;
	uint8 u8PilotPattern = 0;
	uint8 u8SisoMiso = 0;
	uint8 u8T2Version = 0; 
	uint8 u8PreS1 = 0;
	uint8 u8ids[2];
	uint8 u8FefType = 0;
	uint8 u8Tmp[4];
	uint8   u8FefInterval = 0;


	/*PLP Parameters */
	uint8 u8DataPlpNumber = 0;              /*Number of Data PLP*/

	uint8 u8Iter = 0, u8TmpPlpID  = 0, u8plpIndex = 0; 

	uint8 u8PlpSet = 0, u8PlpValid = 0;
	uint8 u8Plpid = 0;  /* Current activated PLP ID */
	uint8 u8CR = 0, u8Modu = 0, u8Plptype = 0, u8Payloadtype = 0, u8Fectype = 0, u8Rotation = 0, u8Streammode = 0, u8Issy = 0, u8Issytype = 0;

	/*Set default Unknown State*/
	status = ATBM781xSetDefaultParamsT2(pDvbT2Params);

	/*T2 System Parameters */
	status |=ATBM781xRead(0x88, 0x63, &u8FFTSize);
	pDvbT2Params->Dvbt2FFTMode = (ATBM781x_DVBT2_FFT_MODE_TYPE)(u8FFTSize&0x07);

	status |=ATBM781xRead(0x0B, 0x04, &u8Ext);
	pDvbT2Params->Dvbt2BandwidthExt = (ATBM781x_DVBT2_BANDWIDTH_EXT_TYPE)(u8Ext&0x01);

	status |=ATBM781xRead(0x88, 0x64, &u8GuardInterval);
	pDvbT2Params->Dvbt2GuardInterval = (ATBM781x_DVBT2_GUARD_INTERVAL_TYPE)(u8GuardInterval&0x07);

	status |=ATBM781xRead(0x0B, 0x13,  &u8PilotPattern);
	pDvbT2Params->Dvbt2PilotPattern = (ATBM781x_DVBT2_PILOT_PATTERN_TYPE)(u8PilotPattern&0x0f);

	status |=ATBM781xRead(0x0B, 0x05,  &u8SisoMiso);
	pDvbT2Params->Dvbt2SisoMiso = (ATBM781x_DVBT2_SISO_MISO_TYPE)(u8SisoMiso&0x07);

	status |=ATBM781xRead(0x0B, 0x22,  &u8T2Version);
	if((u8T2Version&0x0f) < 3) /*0:V1.11,   1:v1.21,    2: v1.31 */
	{
		pDvbT2Params->Dvbt2Version = (ATBM781x_DVBT2_VERSION_TYPE)(u8T2Version&0x0f);
	}
	else
	{
		pDvbT2Params->Dvbt2Version = ATBM781x_DVBT2_VERSION_UNKNOWN;
	}

	status |=ATBM781xRead(0x0B, 0x05,  &u8PreS1);
	if((u8PreS1&0x07) <= 1)
	{
		pDvbT2Params->Dvbt2BaseOrLiteType = ATBM781x_DVBT2_BASE_PROFILE;
	}
	if(((u8PreS1&0x07) == 0x03)||((u8PreS1&0x07) == 0x04))
	{
		pDvbT2Params->Dvbt2BaseOrLiteType = ATBM781x_DVBT2_LITE_PROFILE;
	}
	status |= ATBM781xLatchOn();
	status |=ATBM781xRead(0x0B, 0x15,  &u8ids[0]);
	status |=ATBM781xRead(0x0B, 0x16,  &u8ids[1]);
	pDvbT2Params->Dvbt2CellId = (u8ids[1]<<8) + u8ids[0];

	status |=ATBM781xRead(0x0B, 0x17,  &u8ids[0]);
	status |=ATBM781xRead(0x0B, 0x18,  &u8ids[1]);
	pDvbT2Params->Dvbt2NetworkId = (u8ids[1]<<8) + u8ids[0];

	status |=ATBM781xRead(0x0B, 0x19,  &u8ids[0]);
	status |=ATBM781xRead(0x0B, 0x1a,  &u8ids[1]);
	pDvbT2Params->Dvbt2T2SystemId = (u8ids[1]<<8) + u8ids[0];

	status |=ATBM781xRead(0x0B, 0x72,  &u8FefType);
	status |=ATBM781xRead(0x0B, 0x6e,  &u8FefInterval);

	status |=ATBM781xRead(0x0B, 0x6f,  &u8Tmp[0]);
	status |=ATBM781xRead(0x0B, 0x70,  &u8Tmp[1]);
	status |=ATBM781xRead(0x0B, 0x71,  &u8Tmp[2]);
	
	pDvbT2Params->u8FefType = u8FefType&0x0F;
	pDvbT2Params->u32FefLength =  (u8Tmp[2]<<16) +  (u8Tmp[1]<<8) + u8Tmp[0];
	pDvbT2Params->u8FefInterval = u8FefInterval;


	/*PLP parameters */
	status |=ATBM781xRead(0x24, 0x04,  &u8PlpValid);
	if(u8PlpValid ==1)
	{
		status |=ATBM781xRead(0x24, 0x05,  &u8DataPlpNumber);
		pDvbT2Params->u8DataPlpNumber = u8DataPlpNumber;

		/*Read all available Data PLP_ID*/
		for(u8Iter = 0; u8Iter<u8DataPlpNumber; u8Iter++)
		{
			u8plpIndex = u8Iter;
			status |=ATBM781xRead(0x23, u8plpIndex,  &u8TmpPlpID);
			pDvbT2Params->u8DataPlpIdArray[u8Iter] = u8TmpPlpID;
		}

		/*
		Read Current active Data PLP ID	
		0x2403 ==1, PLP selected, 0x2402 is the valid PLP id. 
		0x2403 ==0, The default first Data PLP can be read from 0x2300
		*/
		status |=ATBM781xRead(0x24, 0x03,  &u8PlpSet);
		if((u8PlpSet&0x01) == 0x01)
		{
			status |=ATBM781xRead(0x24, 0x02,  &u8Plpid);
		}
		else
		{
			status |=ATBM781xRead(0x23, 0x00,  &u8Plpid);
		}
		pDvbT2Params->u8ActivePlpId = u8Plpid;
	}

	/*code rate:  000:1/2, 001:3/5, 010:2/3, 011:3/4, 100:4/5, 101:5/6  */
	status |=ATBM781xRead(0x0C, 0x38, &u8CR); 	  
	/*QPSK, 16QAM, 64QAM, 256QAM*/
	status |=ATBM781xRead(0x0C, 0x2F, &u8Modu);
	pDvbT2Params->Dvbt2CodeRate = (ATBM781x_DVBT2_CODE_RATE_TYPE)(u8CR&0x07);
	pDvbT2Params->Dvbt2Constellation= (ATBM781x_DVBT2_CONSTELLATION_TYPE)(u8Modu&0x07);

	status |=ATBM781xRead(0x0C, 0x30, &u8Plptype); /*TYPE1 PLP or TYPE2 PLP*/
	pDvbT2Params->Dvbt2DataPLPType = (ATBM781x_DVBT2_PLP_TYPE)(u8Plptype&0x07);
	status |=ATBM781xRead(0x0C, 0x3D, &u8Payloadtype);/*GSE, TS*/
	if((u8Payloadtype&0x1f) < 4)
	{
		pDvbT2Params->Dvbt2PayloadType = (ATBM781x_DVBT2_PLP_PAYLOAD_TYPE)(u8Payloadtype&0x01f);
	}
	else
	{
		pDvbT2Params->Dvbt2PayloadType = ATBM781x_DVBT2_PAYLOAD_UNKNOWN;
	}

	status |=ATBM781xRead(0x0C, 0x2E, &u8Fectype);
	pDvbT2Params->Dvbt2FecLengthType = (ATBM781x_DVBT2_FEC_LENGTH_TYPE)(u8Fectype&0x03);

	status |=ATBM781xRead(0x0C, 0x3E, &u8Rotation);
	pDvbT2Params->u8ConstellationRotation = u8Rotation&0x01;

	status |=ATBM781xRead(0x17, 0x7a, &u8Streammode);
	if(u8Streammode&0x01)
	{
		pDvbT2Params->Dvbt2StreamType= ATBM781x_DVBT2_HEM_MODE;
	}
	else
	{
		pDvbT2Params->Dvbt2StreamType = ATBM781x_DVBT2_NORMAL_MODE;
	}

	status |= ATBM781xRead(0x17, 0x84, &u8Issy);
	if(u8Issy&0x01)
	{
		status |=ATBM781xRead(0x17, 0x83, &u8Issytype);
		if(u8Issytype&0x01)
		{
			pDvbT2Params->Dvbt2IssyType = ATBM781x_DVBT2_ISSY_LONG;
		}else
		{
			pDvbT2Params->Dvbt2IssyType = ATBM781x_DVBT2_ISSY_SHORT;
		}
	}
	else
	{
		pDvbT2Params->Dvbt2IssyType = ATBM781x_DVBT2_ISSY_NO;
	}
	status |= ATBM781xLatchOff();
	return status;
}

ATBM781x_STATUS ATBM781xPrintSignalParamsT2(ATBM781x_DVBT2_PARAMS *pstDvbT2SignalParams)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;

	status = ATBM781xGetSignalParamsT2(pstDvbT2SignalParams);


	if (status == ATBM781x_SUCCESSFUL)
	{
		ATBM781xPrintStr ("FFT Mode:",           gStrDvbT2FFTMode[pstDvbT2SignalParams->Dvbt2FFTMode]);
		ATBM781xPrintStr ("GuardInterval:",    gStrDvbT2GuardInterval[pstDvbT2SignalParams->Dvbt2GuardInterval]);
		ATBM781xPrintStr ("DVBT2Version:",    gStrDvbT2Version[pstDvbT2SignalParams->Dvbt2Version]);		
		ATBM781xPrintStr ("SisoMiso:",            gStrDvbT2SisoMiso[pstDvbT2SignalParams->Dvbt2SisoMiso]);	
		ATBM781xPrintStr ("PLPConstellation:", gStrDvbT2Constellation[pstDvbT2SignalParams->Dvbt2Constellation]);
		ATBM781xPrintStr ("PLPCodeRate:",      gStrDvbT2CodeRate[pstDvbT2SignalParams->Dvbt2CodeRate]);
	}
	return status;
}

/*This function should be used in thread process environment. */
/*used for:*/
/*DNP is used but ISSY is not used Non-T2 complaint signal.*/
ATBM781x_STATUS ATBM781xDVBT2ThreadProcess(void)
{
	uint8 u8chip_id = 0,u8SyncT2 = 0, u8DataPLPFecLock = 0, u8SelfBalance = 0;
	uint8 u8Tmp[4];
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint8 u8NoissydnpOn = 0;

	status = ATBM781xRead(0x00,0x00,&u8chip_id);

	if((u8chip_id == 0xa2)||(u8chip_id == 0xa4))
	{
		status |=ATBM781xRead(0x88, 0x6C, &u8SyncT2); /*t2*/
		status |=ATBM781xRead(0x17, 0xEC, &u8DataPLPFecLock);
		if((u8SyncT2 == 1)&&((u8DataPLPFecLock&0x01) == 1))
		{
			status |=ATBM781xRead(0x17, 0x84,  &u8Tmp[0]);
			status |=ATBM781xRead(0x17, 0x87,  &u8Tmp[1]);

			if(((u8Tmp[0]&0x01) == 0)&&((u8Tmp[1]&0x01) == 1))
			{
				u8NoissydnpOn = 1;
			}

			status = ATBM781xRead(0x67,0x25,&u8SelfBalance);

			if((u8NoissydnpOn == 1)&&((u8SelfBalance&0x01) == 0x01))
			{		
				status|=ATBM781xWrite( 0x67, 0x25, 0x00); 				
			}
			if((u8NoissydnpOn == 0)&&((u8SelfBalance&0x01) == 0x00))
			{		
				status|=ATBM781xWrite( 0x67, 0x25, 0x01); 

			}
		}
	}
	return status;
}

/********************************************************************************
function    : ATBM781xSetHierarchyLpT
parameter   :N/A
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description :To receive LP Hierarchy of DVB-T signal. 
*********************************************************************************/
ATBM781x_STATUS ATBM781xSetHierarchyLpT(void )
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	ATBM781x_DVBT_PARAMS DvbTSignalParams;

	status = ATBM781xGetSignalParamsT(&DvbTSignalParams);
	/*
	If Hierarchy used ,LP profile stream can be found
	*/
	if((status == ATBM781x_SUCCESSFUL)&&(DvbTSignalParams.DvbtHierarchy != ATBM781x_DVBT_HIERARCHY_NON))
	{
		status|=ATBM781xWrite( 0x00, 0x05, 0x01);  /*Reset DSP*/
		status|=ATBM781xWrite( 0x28, 0x2b, 0x01); 
		status|=ATBM781xWrite( 0x00, 0x05, 0x00); 
	}
	if(status!=ATBM781x_SUCCESSFUL)
	{
		ATBM781xDebugPrint("Error in set HierarchyLpT\n");
	}
	return status;
}

/********************************************************************************
function    : ATBM781xSetDefaultHierarchyHpT
parameter   :N/A
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description :To receive HP Hierarchy of DVB-T signal. 
*********************************************************************************/
ATBM781x_STATUS ATBM781xSetDefaultHierarchyHpT(void)	 
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	/* if() //DVB-T Enable*/
	status  = ATBM781xWrite( 0x00, 0x05, 0x01);  /*Reset DSP*/
	status|=ATBM781xWrite( 0x28, 0x2b, 0x00);
	status|=ATBM781xWrite( 0x00, 0x05, 0x00); 

	if(status!=ATBM781x_SUCCESSFUL)
	{
		ATBM781xDebugPrint("Error in set HierarchyHpT\n"); 
	}
	return status;
}
/********************************************************************************
function    : ATBM781xGetDVBTSigalQuality
parameter   :ui8TSQ:return the sq value by this pointer.
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal 
Description :Call this function to get DVB-T signal quality, value is in range of [0, 100].
*********************************************************************************/ 
ATBM781x_STATUS  ATBM781xGetDVBTSigalQuality(uint8 *ui8TSQ)
{  

	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	/*00 QPSK,  1:16QAM,  2:64QAM,  3:Reserved*/
	uint8 ui8Constellation = 0,ui8Value = 0, u8TSLockFlag = 0; 
	/*Hierarchy information, */
	/*0:Non hierarchical, 1:¦Á= 1,  2:¦Á= 2,  3:¦Á= 4*/
	uint8 ui8HierarchyInfo = 0;
	uint8 ui8LpSelect = 0;
	/*3bits 000:1/2,    001:2/3,      010:3/4,  011: 5/6,  100: 7/8,  101: reserved*/
	uint8 ui8HPCodeRate = 0, ui8LPCodeRate = 0;
	uint8 ui8BerSqi = 0;
	int i32WinLength;
	int i32CNnordigx10 = 0;
	int i32CNreceviedx10 = 0;
	int i32CNrelx10 = 0;
	uint32 ui32Tmp = 0;

	uint32 ui32dPreRSBER = 0;
	ATBM781x64Data i64Data,i64Data1;



	/*CN of NorDig Profile 1 for Non Hierarchical signal*/
	static  uint32 NordigDVBTdBx10[3][5] = 
	{
		/* 1/2,   2/3,   3/4,   5/6,   7/8 */    
		{51, 69, 79, 89, 97},         /* QPSK */
		{108, 131, 146, 156, 160},    /* 16QAM */
		{165, 187, 202, 216, 225}     /* 64QAM */
	};

	/*CN of NorDig Profile 1 for Hierarchical HP signal*/
	static  uint32 NordigHierHpDVBTdBx10[3][2][5] = 
	{
		/* alpha = 1 */ 
		{  /* 1/2,   2/3,    3/4,    5/6,    7/8 */
			{91,   120,  136,  150,  166},    /* LP = 16QAM */
			{109,  141,  157,  194,  206}    /* LP = 64QAM */
		},/* alpha = 2 */ 
		{  /* 1/2,   2/3,    3/4,    5/6,    7/8 */
			{68,    91,  104,  119,  127},    /* LP = 16QAM */
			{85,   110,  128,  150,  160}    /* LP = 64QAM */
		},/* alpha = 4 */ 
		{  /* 1/2,   2/3,    3/4,    5/6,    7/8 */
			{58,    79,  91,   103,  121},    /* LP = 16QAM */
			{80,    93,  116,  130,  129}    /* LP = 64QAM */
		}
	};

	/*CN of NorDig Profile 1 for Hierarchical LP signal*/
	static  uint32 NordigHierLpDVBTdBx10[3][2][5] = 
	{
		/* alpha = 1 */ 
		{  /* 1/2,   2/3,    3/4,    5/6,    7/8 */
			{125,  143,  153,  163,  169},    /* 16QAM */
			{167,  191,  209,  225,  237}    /* 64QAM */
		}, /* alpha = 2 */ 
		{  /* 1/2,   2/3,    3/4,    5/6,    7/8 */
			{150,  172,  184,  191,  201},    /* 16QAM */
			{185,  212,  236,  247,  259}    /* 64QAM */
		}, /* alpha = 4 */ 
		{  /* 1/2,   2/3,    3/4,    5/6,    7/8 */
			{195,  214,  225,  237,  247},    /* 16QAM */
			{219,  242,  256,  269,  278}    /* 64QAM */
		}
	};

	/* 	*quality_per100 = 0; //default return value.*/

	ui8Status = ATBM781xRead(0x28,0x22,&ui8Constellation);
	ui8Status |= ATBM781xRead(0x28,0x23,&ui8HierarchyInfo);
	ui8Status |= ATBM781xRead(0x28,0x24,&ui8HPCodeRate);
	ui8Status |= ATBM781xRead(0x28,0x25,&ui8LPCodeRate);
	ui8Status |= ATBM781xRead(0x28,0x2b,&ui8LpSelect);


	ui8Status |= ATBM781xGetDVBTSNR(&ui32Tmp);

	i32CNreceviedx10 = (int)(ui32Tmp);/* (int)(10 * snr); returned value is SNRx10 */

	if((ui8Constellation > 2)||(ui8HPCodeRate > 4)||(ui8HierarchyInfo > 3))
	{
		return 0;
	}

	if(ui8HierarchyInfo == 0)  /*0:Non hierarchical*/
	{
		i32CNnordigx10 = NordigDVBTdBx10[ui8Constellation][ui8HPCodeRate];        
	}
	else /*>1:Hierarchical*/
	{
		if(ui8LpSelect == 0) /*HP*/
		{
			i32CNnordigx10 = NordigHierHpDVBTdBx10[ui8HierarchyInfo - 1][ui8Constellation - 1][ui8HPCodeRate];
		}
		else /*LP*/
		{
			i32CNnordigx10 = NordigHierLpDVBTdBx10[ui8HierarchyInfo - 1][ui8Constellation - 1][ui8LPCodeRate];
		}
	}

	ui8Status |= ATBM781xLatchOn();


	/* 		ui8Status = ATBM781xRead(0x11,0x0a,&ui8Value)&0xf;*/
	/* 		ui32CFO = (ui8Value<<24);*/
	/* 		ui8Status |= ATBM781xRead(0x11,0x09,&ui8Value);*/
	/* 		ui32CFO = (ui8Value<<16);*/
	ui8Status |= ATBM781xRead(0x95,0x21,&ui8Value);
	i32WinLength = (ui8Value<<8);
	ui8Status |= ATBM781xRead(0x95,0x20,&ui8Value);
	i32WinLength += ui8Value;


	ui8Status |= ATBM781xRead(0x95,0x23,&ui8Value);
	ui32Tmp = (ui8Value<<8);
	ui8Status |= ATBM781xRead(0x95,0x22,&ui8Value);
	ui32Tmp += ui8Value;

	ui8Status |= ATBM781xLatchOff();

	if (ui8Status)
	{
		*ui8TSQ = 0;
		return ui8Status;
	}

	/*ui32dPreRSBER = ui32Tmp*1.0/i32WinLength/204/8;/// double PreRS_BER*/
	ATBM781x64Mult(&i64Data,ui32Tmp,1000000000);
	ATBM781x64Mult(&i64Data1,i32WinLength,204*8);
	ui32dPreRSBER = ATBM781x64Div(i64Data,i64Data1);

	ui8Status |= ATBM781xRead(0x18, 0x2A, &u8TSLockFlag);	
	/* 			if(ui32dPreRSBER <1.0e-7)*/
	if((ui32dPreRSBER <100)&&(u8TSLockFlag&0x01))
	{
		ui8BerSqi = 100;
	}
	/* 			else if((ui32dPreRSBER < 1.0E-3)&&(ui32dPreRSBER >= 1.0E-7))*/
	else if((ui32dPreRSBER < 1000000)&&(ui32dPreRSBER >= 100))
	{
		/* 				ui8BerSqi = 2*ATBM781xLog10ConvertdB(1/ui32dPreRSBER) - 40;*/
		ui8BerSqi = (unsigned char)(2*ATBM781xLog10ConvertdB(1000000000/ui32dPreRSBER) - 40);
	}
	else
	{
		ui8BerSqi = 0;
	}

	/*Calculate signal quality*/
	i32CNrelx10 = i32CNreceviedx10 - i32CNnordigx10;   

	if(i32CNrelx10 < -70) /*CNrel < -7 dB*/
	{
		*ui8TSQ =  0; 
	}    
	else if(( i32CNrelx10 >= -70) && (i32CNrelx10 < 30)) /*-7 dB ¡ÜCNrel < +3 dB*/
	{
		/*SQI = (((C/Nrel -3)/10) + 1) * ui8BerSqi*/
		*ui8TSQ = (uint8)(((((i32CNrelx10 -30)) + 100) * ui8BerSqi) / 100);
	}
	else /*CNrel ¡Ý+3 dB*/
	{
		*ui8TSQ =  ui8BerSqi;
	}
	return ui8Status;
}

/*extern int      gTuner_Type;*/
/*extern void ATBM781xGetSi2176RssidBm(char *rssi_value);*/
/*extern void ATBM781xGetSi2178RssidBm(char *rssi_value);*/

ATBM781x_STATUS ATBM781xGetTxRSSIdBm(int *pi32RssidBm)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	/*unsigned  char ExtLNAGain = 0;*/
	/*ExtLNAGain = 0; //Fake value, should used correct external LNA gain value*/

	/*the Received signal strength RSSI can be read from tuner.*/
	*pi32RssidBm = -100;
	/*Ger Si2176 RSSI function*/
	/*
	if(gTuner_Type == TUNER_SI2176)
	{	
	char rssi_value;
	ExtLNAGain = 25;  Si2176 Tuner add external LNA
	ATBM781xGetSi2176RssidBm(&rssi_value);
	*pi32RssidBm = rssi_value - ExtLNAGain;
	}
	if(gTuner_Type == TUNER_SI2178)
	{	
	char rssi_value;
	ExtLNAGain = 19;  Si2176 Tuner add external LNA
	ATBM781xGetSi2178RssidBm(&rssi_value);
	*pi32RssidBm = rssi_value - ExtLNAGain;
	}
	*/
	return status;
}


/********************************************************************************
function    : ATBM781xGetDVBTxSigalStrength
parameter   :
	ui8TxSSI:pointer to DVB-Tx signal strength.
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal 
Description:Call this function to Get DVB-Tx signal strength.
            NOTE:this function may not work successfully without correct RSSI. 
			Should port RSSI function  from tuner.
*********************************************************************************/
ATBM781x_STATUS  ATBM781xGetDVBTxSigalStrength(uint8 *ui8TxSSI)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;	
	int i32Prec = 0, i32Prel = 0; /*Received RSSI */
	uint8 u8DVBTxMode = 0, u8Tmp = 0, u8Modu = 0, u8CR = 0, u8Alpha = 0, u8index = 0, u8SSI = 0;

	static int i32DvbtPref[] = 
	{ 
		-93,-91,-90,-89,-88, /*QPSK*/
		-87,-85,-84,-83,-82, /*16QAM*/
		-82,-80,-78,-77,-76 /*64QAM*/
	};

	static int i32Dvbt2Pref[] =
	{
		-96, -95, -94, -93, -92, -92, /*QPSK*/
		-91, -89, -88, -87,	-86, -86, /*16QAM*/
		-86, -85, -83, -82, -81, -80, /*64QAM*/
		-82, -80, -78, -76,	-75, -74  /*256QAM*/
	};

	i32Prel = -100;
	status = ATBM781xGetTxRSSIdBm(&i32Prec);

	status |=  ATBM781xDVBTxModeDetected(&u8DVBTxMode);


	if((u8DVBTxMode ==ATBM781x_DVBT_MODE)&&(status== ATBM781x_SUCCESSFUL))
	{
		status |=ATBM781xRead(0x28,0x22, &u8Tmp);/*modulation*/
		u8Modu = u8Tmp&0x03;

		status |=ATBM781xRead(0x28,0x24, &u8Tmp);/*HP code rate*/
		u8CR =  u8Tmp&0x07;

		status |=ATBM781xRead(0x28,0x23, &u8Tmp);/*alpha*/
		u8Alpha = u8Tmp&0x07;

		if((u8Alpha>0)&&(u8Alpha <4))
		{
			/*default HP profile, when Hierarchy used and LP selected, 0x282b = 1*/
			status |=ATBM781xRead(0x28,0x2b, &u8Tmp);/*DVB-T profile*/
			if((u8Tmp&0x01) == 0x01)
			{
				status |=ATBM781xRead(0x28,0x25, &u8Tmp);/*LP code rate*/
				u8CR    = u8Tmp&0x07;		
				u8Modu = 0;
			}
		}	
		u8index = u8Modu*5 + u8CR;
		if(u8index<15)
		{
			i32Prel = i32Prec - i32DvbtPref[u8index];
		}

	}

	if((u8DVBTxMode ==ATBM781x_DVBT2_MODE)&&(status== ATBM781x_SUCCESSFUL))
	{
		/*code rate:  000:1/2, 001:3/5, 010:2/3, 011:3/4, 100:4/5, 101:5/6  */
		status |=ATBM781xRead(0x0C, 0x38, &u8CR); 	  
		/*QPSK, 16QAM, 64QAM, 256QAM*/
		status |=ATBM781xRead(0x0C, 0x2F, &u8Modu);

		u8index = u8Modu*6 + u8CR;
		if(u8index<24)
		{
			i32Prel = i32Prec - i32Dvbt2Pref[u8index];
		}
	}

	/*
	SSI = 0 if Prel < -15dB
	SSI = (2/3) * (Prel + 15) if -15 dB ¡Ü Prel < 0dB
	SSI = 4 * Prel + 10 if 0 dB ¡Ü Prel < 20 dB
	SSI = (2/3) * (Prel - 20) + 90 if 20 dB ¡Ü Prel < 35 dB
	SSI = 100 if Prel ¡Ý 35 dB
	Prel = Prec - Pref
	*/

	if(i32Prel >= 35)
	{
		u8SSI = 100;
	}
	else if((i32Prel >= 20) && (i32Prel < 35))
	{
		u8SSI =  (uint8)(((i32Prel - 20) * 2) / 3 + 90);
	}
	else if((i32Prel >= 0)&&(i32Prel < 20))
	{
		u8SSI  = (uint8)(4 * i32Prel + 10);
	}
	else if((i32Prel >= -15)&&(i32Prel < 0))
	{
		u8SSI = (uint8)(((i32Prel + 15 ) * 2) / 3);
	}
	else
	{
		u8SSI = 0;
	}
	if(i32Prec< -90)
	{
		u8SSI =0;
	}
	*ui8TxSSI = u8SSI;

	return status;
}

/********************************************************************************
function    : ATBM781xGetSignalParamsT
parameter   :pDvbTParams:pointer to DVB-T signal parameters.
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal 
Description :Call this function to Get DVB-T signal parameters.
*********************************************************************************/
ATBM781x_STATUS ATBM781xGetSignalParamsT(ATBM781x_DVBT_PARAMS *pDvbTParams)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;	 
	uint8 u8TpsDone = 0, u8SyncT = 0;
	uint8 u8Tmp = 0;
	uint8 u8Alpha = 0; 

	/*Init parameters */
	pDvbTParams->DvbtFFTMode = ATBM781x_DVBT_MODE_UNKNOWN;
	pDvbTParams->DvbtGuardInterval = ATBM781x_DVBT_GI_UNKNOWN;
	pDvbTParams->DvbtHierarchy = ATBM781x_DVBT_HIERARCHY_UNKNOWN;
	pDvbTParams->DvbtConstellation = ATBM781x_DVBT_CONSTELLATION_UNKNOWN;
	pDvbTParams->DvbtHpCodeRate = ATBM781x_DVBT_CODERATE_UNKNOWN;
	pDvbTParams->DvbtLpCodeRate	= ATBM781x_DVBT_CODERATE_UNKNOWN;
	pDvbTParams->DvbtProfile = ATBM781x_DVBT_HP_PROFILE;

	status = ATBM781xRead(0x28,0x2c, &u8TpsDone);/*TPS done*/
	if(((u8TpsDone&0x01) == 0x01)&&(status ==ATBM781x_SUCCESSFUL)) /*TPS decoded */
	{
		status |=ATBM781xRead(0x28,0x27,  &u8Tmp);/*TPS FFT*/
		pDvbTParams->DvbtFFTMode = (ATBM781x_DVBT_FFT_MODE_TYPE)(u8Tmp&0x03);

		status |=ATBM781xRead(0x28,0x26, &u8Tmp);/*TPS GI*/
		pDvbTParams->DvbtGuardInterval = (ATBM781x_DVBT_GUARD_INTERVAL_TYPE)(u8Tmp&0x03);

		status |=ATBM781xRead(0x28,0x22, &u8Tmp);/*modulation*/
		pDvbTParams->DvbtConstellation = (ATBM781x_DVBT_CONSTELLATION_TYPE)(u8Tmp&0x03);

		status |=ATBM781xRead(0x28,0x23, &u8Tmp);/*alpha*/
		u8Alpha = u8Tmp&0x07;
		switch (u8Alpha)
		{
		case 0:
			pDvbTParams->DvbtHierarchy = ATBM781x_DVBT_HIERARCHY_NON;
			break;

		case 1:
			pDvbTParams->DvbtHierarchy = ATBM781x_DVBT_HIERARCHY_1;
			break;

		case 2:
			pDvbTParams->DvbtHierarchy = ATBM781x_DVBT_HIERARCHY_2;
			break;

		case 3:
			pDvbTParams->DvbtHierarchy = ATBM781x_DVBT_HIERARCHY_4;
			break;

		default:
			pDvbTParams->DvbtHierarchy = ATBM781x_DVBT_HIERARCHY_UNKNOWN;
			break;
		}		

		status |=ATBM781xRead(0x28,0x24, &u8Tmp);/*HP code rate*/
		pDvbTParams->DvbtHpCodeRate =  (ATBM781x_DVBT_CODE_RATE_TYPE)(u8Tmp&0x07);
		if((u8Alpha>0)&&(u8Alpha <4))
		{
			status |=ATBM781xRead(0x28,0x25, &u8Tmp);/*LP code rate*/
			pDvbTParams->DvbtLpCodeRate = (ATBM781x_DVBT_CODE_RATE_TYPE)(u8Tmp&0x07);
			/*default HP profile, when Hierarchy used and LP selected, 0x282b = 1*/
			status |=ATBM781xRead(0x28,0x2b, &u8Tmp);/*DVB-T profile*/
			if((u8Tmp&0x01) == 0x01)
			{
				pDvbTParams->DvbtProfile = ATBM781x_DVBT_LP_PROFILE;
			}
		}		
	}	
	else /*TPS not decoded, Check Sync state*/
	{		
		status |=ATBM781xRead(0x88, 0x6D, &u8SyncT); 	              
		if((u8SyncT&0x01)&&(status ==ATBM781x_SUCCESSFUL))
		{
			status |=ATBM781xRead(0x88,0x63, &u8Tmp);   /* Sync detected FFT mode */

			if((u8Tmp&0x07) == 0x01)
			{
				pDvbTParams->DvbtFFTMode = ATBM781x_DVBT_MODE_2K;
			}
			if((u8Tmp&0x07) == 0x03)
			{
				pDvbTParams->DvbtFFTMode = ATBM781x_DVBT_MODE_8K;
			}
			/* Sync detected Guard interval */
			status |=ATBM781xRead(0x88,0x64, &u8Tmp);
			if((u8Tmp&0x07)<0x04)
			{
				pDvbTParams->DvbtGuardInterval = (ATBM781x_DVBT_GUARD_INTERVAL_TYPE)(u8Tmp&0x07);
			}
		}
	}
	return status;
}
ATBM781x_STATUS ATBM781xPrintSignalParamsDVBTx(void)
{
	ATBM781x_DVB_T_T2_SIGNAL_PARAMS DvbTxParams;

	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL; 
	uint8 u8DVBTxMode = 0;
	status =  ATBM781xDVBTxModeDetected(&u8DVBTxMode);
	/*DVB-T detected*/
	if((u8DVBTxMode ==ATBM781x_DVBT_MODE)&&(status== ATBM781x_SUCCESSFUL))
	{
		DvbTxParams.DVBTxType = ATBM781x_DVBT_MODE;
		status |= ATBM781xPrintSignalParamsT(&DvbTxParams.DvbtSignalParams);

	}
	/*DVB-T2 detected*/
	if((u8DVBTxMode ==ATBM781x_DVBT2_MODE)&&(status== ATBM781x_SUCCESSFUL))
	{
		DvbTxParams.DVBTxType = ATBM781x_DVBT2_MODE;
		status |= ATBM781xPrintSignalParamsT2(&DvbTxParams.Dvbt2SignalParams);
	}
	return status;

}
ATBM781x_STATUS ATBM781xPrintSignalParamsT(ATBM781x_DVBT_PARAMS *pstDvbTSignalParams)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;

	status = ATBM781xGetSignalParamsT(pstDvbTSignalParams);

	if (status == ATBM781x_SUCCESSFUL) 
	{
		ATBM781xPrintStr ("FFT Mode:",      gStrDvbTFFTMode[pstDvbTSignalParams->DvbtFFTMode]);
		ATBM781xPrintStr ("GuardInterval:", gStrDvbTGuardInterval[pstDvbTSignalParams->DvbtGuardInterval]);
		ATBM781xPrintStr ("Constellation:", gStrDvbTConstellation[pstDvbTSignalParams->DvbtConstellation]);		
		ATBM781xPrintStr ("Hierarchy:",     gStrDvbTHierarchy[pstDvbTSignalParams->DvbtHierarchy]);		
		ATBM781xPrintStr ("HPCodeRate:",    gStrDvbTCodeRate[pstDvbTSignalParams->DvbtHpCodeRate]);
		if((pstDvbTSignalParams->DvbtHierarchy != ATBM781x_DVBT_HIERARCHY_NON)&&(pstDvbTSignalParams->DvbtHierarchy != ATBM781x_DVBT_HIERARCHY_UNKNOWN))
		{
			ATBM781xPrintStr ("LPCodeRate:",    gStrDvbTCodeRate[pstDvbTSignalParams->DvbtLpCodeRate]);
		}
	}
	return status;
}
/********************************************************************************
function    : ATBM781xGetDVBTSNR
parameter   :ui32SNRValue:DVBT2 SNR value returned by this pointer unit is dB  *10 
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal 
Description :Call this function to Get T2 SNR result, Unit is dB in decimal integer format.
*********************************************************************************/ 
ATBM781x_STATUS   ATBM781xGetDVBTSNR(uint32 *ui32SNRValuex10)
{
	uint8 ui8Value = 0;
	ATBM781x_STATUS ui8Status  = ATBM781x_SUCCESSFUL;
	uint32 ui32RoSignalVar = 0,ui32RoNoiseVar = 1;
	ATBM781x64Data i64Data,i64Data1;
	uint8 u8chipID = 0;
	ui8Status = ATBM781xRead(0x00,0x00,&u8chipID);
	/*return ATBM781xGetDVBT2SNR(ui32SNRValuex10);//atbm7812 SDK using same SNR as T2*/

	/*Latch On*/
	ui8Status |= ATBM781xLatchOn();

	ui8Status = ATBM781xRead(0x11,0xe3,&ui8Value);
	ui32RoNoiseVar = (ui8Value<<16);
	ui8Status |= ATBM781xRead(0x11,0xe2,&ui8Value);
	ui32RoNoiseVar += (ui8Value<<8);
	ui8Status |= ATBM781xRead(0x11,0xe1,&ui8Value);
	ui32RoNoiseVar += ui8Value;


	ui8Status |= ATBM781xRead(0x11,0xdd,&ui8Value);
	ui32RoSignalVar = (ui8Value<<16);
	ui8Status |= ATBM781xRead(0x11,0xdc,&ui8Value);
	ui32RoSignalVar += (ui8Value<<8);
	ui8Status |= ATBM781xRead(0x11,0xdb,&ui8Value);
	ui32RoSignalVar += ui8Value;

	/*Latch Off*/
	ui8Status |= ATBM781xLatchOff();
	if (ui8Status)
	{
		*ui32SNRValuex10 = 0;
		return ui8Status;
	}

	if (ui32RoNoiseVar == 0||ui32RoSignalVar == 0)
	{
		*ui32SNRValuex10 = 0;
		return ui8Status;
	}
	i64Data1.ui32High = 0;
	i64Data1.ui32Low = ui32RoNoiseVar;
	if(u8chipID == 0xa2)
	{
		ATBM781x64Mult(&i64Data,ui32RoSignalVar,4);
	}
	else if(u8chipID == 0xa4)
	{
		ATBM781x64Mult(&i64Data,ui32RoSignalVar,8);
	}
	else
	{
		ATBM781x64Mult(&i64Data,ui32RoSignalVar,8);
	}
	/*ATBM781x64Mult(&i64Data,ui32RoSignalVar,4);*/
	*ui32SNRValuex10 = ATBM781xLog10ConvertdBx10(ATBM781x64Div(i64Data,i64Data1));
	return ui8Status;

}


/****************************************************************************
Function:	ATBM781xGetDVBTBER
Parameters:   ui32BER: return DVB-T BER by this pointer,the return number  is the real 
BER value * 1e9. so ,if you want to get the correct BER value, you should 
use the return value/1e9                
Return: 	  Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal 
Description:
Call this function to get DVB-T BER value, this is FEC corrected error rate, is not TS output error.	 
******************************************************************************/
ATBM781x_STATUS   ATBM781xGetDVBTBER(uint32 *ui32BER)
{
	uint8 ui8Tmp = 0;
	ATBM781x_STATUS ui8Status  = ATBM781x_SUCCESSFUL;
	uint16 ui16Tmp = 0,ui16WinLen = 1;
	ATBM781x64Data i64Data,i64Data1;

	ui8Status = ATBM781xLatchOn();
	ui8Status |= ATBM781xRead(0x95,0x21,&ui8Tmp);
	ui16WinLen = ui8Tmp<<8;
	ui8Status |= ATBM781xRead(0x95,0x20,&ui8Tmp);
	ui16WinLen = ui16WinLen + ui8Tmp;

	ui8Status |= ATBM781xRead(0x95,0x23,&ui8Tmp);
	ui16Tmp = ui8Tmp<<8;
	ui8Status |= ATBM781xRead(0x95,0x22,&ui8Tmp);
	ui16Tmp = ui16Tmp + ui8Tmp;
	ui8Status |= ATBM781xLatchOff();
	ATBM781x64Mult(&i64Data,ui16Tmp,1000000000);
	ATBM781x64Mult(&i64Data1,ui16WinLen,204*8);
	*ui32BER = ATBM781x64Div(i64Data,i64Data1);
	return ui8Status;
}

/****************************************************************************
Function:	ATBM781xGetDVBTPER
Parameters:   ui32PER: return DVB-T PER by this pointer.
Return: 	  Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal 
Description:
Call this function to get DVB-T PER value, can be used to evaluate TS output error rate.		 
******************************************************************************/
ATBM781x_STATUS   ATBM781xGetDVBTPER(uint32 *ui32PER)
{
	uint8 ui8Tmp = 0;
	ATBM781x_STATUS ui8Status  = ATBM781x_SUCCESSFUL;
	uint16 ui16Tmp = 0,ui16WinLen = 1;
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
/********************************************************************************
function    : ATBM781xGetDVBTSampleOffsetPPM
parameter   :i32Value:the value sample offset PPM is return by this pointer
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description :Call this function to calculate the PPM value, test the sample frequency offset.
*********************************************************************************/
uint8  ATBM781xGetDVBTSampleOffsetPPM(int*i32Value)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint8 ui8Value = 0;
	char i8Signal = 1;
	int i32RoPhiAdj = 0;
	ATBM781x64Data i64Data,i64Data1;
	int i32OffsetValue = 0;
	unsigned int SampleRegs24M = 0;
	unsigned char u8TsLockFlag = 0;
	unsigned char u8SmpRegs[3];
	ui8Status = ATBM781xLatchOn();

	ui8Status |= ATBM781xRead(0x11,0x89,&ui8Value);
	i32RoPhiAdj = ((ui8Value&0xf)<<16);
	ui8Status |= ATBM781xRead(0x11,0x88,&ui8Value);
	i32RoPhiAdj += (ui8Value<<8);
	ui8Status |= ATBM781xRead(0x11,0x87,&ui8Value);
	i32RoPhiAdj += ui8Value;
	ui8Status |= ATBM781xLatchOff();
	if (ui8Status)
	{
		*i32Value = 0;
		return ui8Status;
	}
	if (i32RoPhiAdj&0x80000)/*Negative Data*/
	{
		i8Signal = -1;
		i32RoPhiAdj = i32RoPhiAdj - 1048576;/*Convert it to 32 bit Value(it is a Negative Data)*/
		i32RoPhiAdj = i32RoPhiAdj*i8Signal;/*Convert it to Positive Data*/
	}
	/*i32RoPhiAdj *1.0e6 / pow(2.0, 30)*/

	ATBM781x64Mult(&i64Data,i32RoPhiAdj,15625);
	i64Data1.ui32High = 0;
	i64Data1.ui32Low = ATBM781xFixPow(2,24);
	*i32Value = ATBM781x64Div(i64Data,i64Data1)*i8Signal;

	ui8Status |= ATBM781xRead(0x18,0x2a,&u8TsLockFlag);
	i32OffsetValue = *i32Value;
	if((u8TsLockFlag == 1)&&(i32OffsetValue < 500)&&(i32OffsetValue > -500))
	{
		ui8Status |= ATBM781xRead(0x0a,0x08,&ui8Value);
		SampleRegs24M = ((ui8Value&0xff)<<16);
		ui8Status |= ATBM781xRead(0x0a,0x07,&ui8Value);
		SampleRegs24M += (ui8Value<<8);
		ui8Status |= ATBM781xRead(0x0a,0x06,&ui8Value);
		SampleRegs24M += ui8Value;

		SampleRegs24M =  SampleRegs24M + i32OffsetValue*7/5;

		u8SmpRegs[0] = (uint8)(SampleRegs24M&0xff);
		u8SmpRegs[1] = (uint8)(SampleRegs24M>>8);
		u8SmpRegs[2] = (uint8)(SampleRegs24M>>16);

		ATBM781xDebugPrint("Registers Set for 24MHz Sample PPM calibration is 0x0a08=0x%x  0x0a07=0x%x 0x0a06=0x%x\n", u8SmpRegs[2], u8SmpRegs[1],u8SmpRegs[0]);
	}	
	return ui8Status;
	/* 	return (i32RoPhiAdj*15625/pow(2.0,24)); */
}
/********************************************************************************
function    : ATBM781xGetDVBTCFO
parameter   :stPara,use the parameter  stPara.ui8DVBTxBandWidth. i32Value:T2 CFO value.Unit KHz
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal 
Description :Call this function to calculate the CFO values of T2.
*********************************************************************************/
ATBM781x_STATUS  ATBM781xGetDVBTCFO(int*i32Value)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint8 ui8Value = 0;
	char i8Signal = 1;
	int ui32CFO = 0,i32Rst = 0;
	uint32 ui32Tmp = 0;
	ATBM781x64Data i64Data,i64Data1;
	/* 	double dbRrs;*/
	ui8Status = ATBM781xLatchOn();

	ui8Status |= ATBM781xRead(0x11,0x86,&ui8Value);
	ui32CFO = ((ui8Value)<<24);
	ui8Status |= ATBM781xRead(0x11,0x85,&ui8Value);
	ui32CFO += (ui8Value<<16);
	ui8Status |= ATBM781xRead(0x11,0x84,&ui8Value);
	ui32CFO += (ui8Value<<8);
	ui8Status |= ATBM781xRead(0x11,0x83,&ui8Value);
	ui32CFO += ui8Value;
	ui8Status |= ATBM781xRead(0x88,0x68,&ui8Value);
	ui8Status |= ATBM781xLatchOff();

	ui32Tmp= ui8Value;
	if (ui8Status)
	{
		*i32Value = 0;
		return ui8Status;
	}
	ui32Tmp = ui32Tmp&0xf;
	ui32CFO = (ui32CFO&0xfffffff) |(ui32Tmp<<28);

	if (ui32CFO&0x80000000)
	{
		/*ui32CFO = ui32CFO -0xffffffff-1;*/
		i8Signal = -1;
		ui32CFO = ui32CFO * i8Signal;/*first convert it to Positive data*/
	}

	/*ATBM781x64Mult(&i64Data,ui32CFO,1000);KHz*/
	/*6487ED344    first should store the data to the Dividend   this data is PI*2^33 's Result*/
	i64Data1.ui32High = 6;
	i64Data1.ui32Low = 0x487ED345;


	if (u8sDVBTxBandwidth == ATBM781x_DVBT_BANDWIDTH_1DOT7MHZ)/*1.7 MHz Only support for T2 */
	{
		ATBM781x64Mult(&i64Data,ui32CFO,1000*131);/*KHz*/
		i32Rst = ATBM781x64Div(i64Data,i64Data1)/71;
	}
	else if (u8sDVBTxBandwidth== ATBM781x_DVBT_BANDWIDTH_5MHZ)/*6MHz*/
	{
		ATBM781x64Mult(&i64Data,ui32CFO,1000*40);/*KHz*/
		i32Rst = ATBM781x64Div(i64Data,i64Data1)/7;
	}
	else if (u8sDVBTxBandwidth ==ATBM781x_DVBT_BANDWIDTH_6MHZ)/*6MHz*/
	{
		ATBM781x64Mult(&i64Data,ui32CFO,1000*48);/*KHz*/
		i32Rst = ATBM781x64Div(i64Data,i64Data1)/7;
	}
	else if (u8sDVBTxBandwidth ==ATBM781x_DVBT_BANDWIDTH_7MHZ)/*7MHz*/
	{
		ATBM781x64Mult(&i64Data,ui32CFO,1000*8);/*KHz*/
		i32Rst = ATBM781x64Div(i64Data,i64Data1);
	}
	else if (u8sDVBTxBandwidth == ATBM781x_DVBT_BANDWIDTH_8MHZ)/*8MHz*/
	{
		ATBM781x64Mult(&i64Data,ui32CFO,1000*64);/*KHz*/
		i32Rst = ATBM781x64Div(i64Data,i64Data1)/7;
	}
	else/*other  (10MHZ)*/
	{
		ATBM781x64Mult(&i64Data,ui32CFO,1000*80);/*KHz*/
		i32Rst = ATBM781x64Div(i64Data,i64Data1)/7;
	}
	*i32Value = i32Rst*i8Signal*-1;
	return ui8Status;
}


